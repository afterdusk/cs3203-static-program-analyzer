#include <iostream>
#include <list>
#include <set>

#include "Dispatchers.h"
#include "PqlEvaluator.h"

void Pql::evaluate(ParsedQuery pq, PkbQueryInterface *queryHandler,
                   std::list<std::string> &result) {
  // Instantiate evaluation table
  EvaluationTable table;

  // Fetch values for relationship clauses from PkbTables and push to table
  for (auto &relationship : pq.relationships) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(relationship, queryHandler);
    if (dispatcher->willReturnBoolean()) {
      // Early termination if clause evaluates to false
      if (!dispatcher->booleanDispatch()) {
        delete dispatcher;
        return;
      }
    } else {
      EvaluationTable &clauseResult = dispatcher->resultDispatch();
      delete dispatcher;
      table.merge(clauseResult);
    }
  }

  // Do the same for pattern clauses
  for (auto &pattern : pq.patterns) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromPattern(pattern, queryHandler);
    if (dispatcher->willReturnBoolean()) {
      // Early termination if clause evaluates to false
      if (!dispatcher->booleanDispatch()) {
        delete dispatcher;
        return;
      }
    } else {
      EvaluationTable &clauseResult = dispatcher->resultDispatch();
      delete dispatcher;
      table.merge(clauseResult);
    }
  }

  // Select values from table if contained in table, else fetch from PKB
  SYMBOL selectedSynonym = pq.results[0];
  if (table.isSeen(selectedSynonym)) {
    std::unordered_set<std::string> selected = table.select(selectedSynonym);
    result.insert(result.end(), selected.begin(), selected.end());
  } else {
    if (table.empty() || table.rowCount() != 0) {
      TokenType type = pq.declarations[selectedSynonym];
      PqlToken token = PqlToken{type, selectedSynonym};
      ClauseDispatcher *dispatcher =
          ClauseDispatcher::FromToken(token, queryHandler);
      EvaluationTable &clauseResult = dispatcher->resultDispatch();
      delete dispatcher;
      // TODO: Optimize to reduce copying
      std::vector<VALUE> resultVector = clauseResult.valuesOf(selectedSynonym);
      std::copy(resultVector.begin(), resultVector.end(),
                std::back_inserter(result));
    }
  }
}

EvaluationTable::EvaluationTable() : rows(0) { table = new TABLE; }

EvaluationTable::EvaluationTable(const EvaluationTable &other) {
  rows = other.rows;
  seen = other.seen;
  table = new TABLE;
  *table = *other.table;
}

EvaluationTable::~EvaluationTable() { delete table; }

EvaluationTable &EvaluationTable::operator=(const EvaluationTable &other) {
  if (this == &other)
    return *this;
  rows = other.rows;
  seen = other.seen;
  *table = *other.table;
  return *this;
}

bool EvaluationTable::operator==(EvaluationTable &other) {
  if (this->rows != other.rows || this->seen != other.seen) {
    return false;
  }
  std::set<std::vector<VALUE>> thisGroups;
  std::set<std::vector<VALUE>> otherGroups;

  for (int i = 0; i < this->rows; i++) {
    // A group is a row of values of each synonym
    std::vector<VALUE> thisGroup;
    std::vector<VALUE> otherGroup;
    for (auto &synonym : *table) {
      thisGroup.push_back(synonym.second[i]);
      otherGroup.push_back((*other.table)[synonym.first][i]);
    }
    thisGroups.insert(thisGroup);
    otherGroups.insert(otherGroup);
  }

  return thisGroups == otherGroups;
}

EvaluationTable::EvaluationTable(TABLE *values) {
  if (values->size() == 0) {
    throw "Invalid: No results";
  }
  rows = (*values->begin()).second.size();
  for (auto &synonymValues : *values) {
    seen.insert(synonymValues.first);
    if (synonymValues.second.size() != rows) {
      throw "Invalid: Length mismatch between synonyms";
    }
  }
  table = values;
};

void EvaluationTable::merge(EvaluationTable &other) {
  // Add results and terminate early if table is empty
  if (empty()) {
    for (auto &otherColumn : *other.table) {
      (*table)[otherColumn.first] = otherColumn.second;
      seen.insert(otherColumn.first);
    }
    rows = other.rows;
    return;
  }

  // Sort clause results into seen and unseen
  std::list<SYMBOL> seenClauseSynonyms;
  std::list<SYMBOL> unseenClauseSynonyms;
  for (auto &otherColumn : *other.table) {
    if (isSeen(otherColumn.first)) {
      seenClauseSynonyms.push_back(otherColumn.first);
    } else {
      unseenClauseSynonyms.push_back(otherColumn.first);
    }
  }

  // Transient table that will store new values
  TABLE *newTable = new TABLE;
  int newRows = 0;

  // Iterate over each result, each existing row in the table
  for (int otherIndex = 0; otherIndex < other.rowCount(); otherIndex++) {
    for (int tableIndex = 0; tableIndex < rowCount(); tableIndex++) {
      // Check if values of seen columns match
      bool isMatch = true;
      for (auto &seenClauseSynonym : seenClauseSynonyms) {
        std::vector<VALUE> seenColumn = (*table)[seenClauseSynonym];
        if (seenColumn[tableIndex] !=
            (*other.table)[seenClauseSynonym][otherIndex]) {
          isMatch = false;
          break;
        }
      }
      // Push cross product into new table if seen columns match
      if (isMatch) {
        newRows += 1;
        for (auto &synonym : seen) {
          (*newTable)[synonym].push_back((*table)[synonym][tableIndex]);
        }
        for (auto &unseenClauseSynonym : unseenClauseSynonyms) {
          (*newTable)[unseenClauseSynonym].push_back(
              (*other.table)[unseenClauseSynonym][otherIndex]);
        }
      }
    }
  }

  // Mark all synonyms in this batch of results as seen
  for (auto &otherColumn : *other.table) {
    seen.insert(otherColumn.first);
  }

  // Complete operation by replacing the values table
  table = newTable;
  rows = newRows;
}

bool EvaluationTable::isSeen(SYMBOL synonym) {
  return seen.find(synonym) != seen.end();
}

bool EvaluationTable::empty() { return seen.empty(); }

std::unordered_set<VALUE> EvaluationTable::select(SYMBOL synonym) {
  std::vector<VALUE> &selected = (*table)[synonym];
  return std::unordered_set<VALUE>(selected.begin(), selected.end());
}

// TODO: Remove this when selecting tuples is properly implemented
std::vector<VALUE> EvaluationTable::valuesOf(SYMBOL synonym) {
  return (*table)[synonym];
}

int EvaluationTable::rowCount() { return rows; }

ClauseDispatcher::ClauseDispatcher(PkbQueryInterface *queryHandler) {
  handler = queryHandler;
}

ClauseDispatcher *ClauseDispatcher::FromToken(PqlToken token,
                                              PkbQueryInterface *handler) {
  return new SelectDispatcher(token, handler);
}

ClauseDispatcher *
ClauseDispatcher::FromRelationship(ParsedRelationship pr,
                                   PkbQueryInterface *handler) {
  switch (pr.relationship) {
  case TokenType::FOLLOWS:
    return new FollowsDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::FOLLOWS_T:
    return new FollowsStarDispatcher(pr.firstArgument, pr.secondArgument,
                                     handler);
  case TokenType::PARENT:
    return new ParentDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::PARENT_T:
    return new ParentStarDispatcher(pr.firstArgument, pr.secondArgument,
                                    handler);
  case TokenType::USES:
    return new UsesDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::MODIFIES:
    return new ModifiesDispatcher(pr.firstArgument, pr.secondArgument, handler);
  default:
    throw "Invalid: Relationship not implemented";
  }
}

ClauseDispatcher *ClauseDispatcher::FromPattern(ParsedPattern pp,
                                                PkbQueryInterface *handler) {
  return new PatternDispatcher(pp, handler);
}

ClauseDispatcher::PKB_PARAM ClauseDispatcher::toParam(PqlToken token) {
  switch (token.type) {
  case TokenType::VARIABLE:
    synonyms.push_back(token.value);
    return Variable{};
  case TokenType::PROCEDURE:
    synonyms.push_back(token.value);
    return Procedure{};
  case TokenType::CONSTANT:
    synonyms.push_back(token.value);
    return Constant{};
  case TokenType::UNDERSCORE:
    return Underscore{};
  case TokenType::NUMBER:
    return LineNumber{token.value};
  case TokenType::STRING: {
    String s;
    s.name = token.value;
    return s;
  }
  case TokenType::STMT: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::NONE;
    return s;
  }
  case TokenType::READ: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::READ;
    return s;
  }
  case TokenType::PRINT: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::PRINT;
    return s;
  }
  case TokenType::ASSIGN: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::ASSIGN;
    return s;
  }
  case TokenType::CALL: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::CALL;
    return s;
  }
  case TokenType::WHILE: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::WHILE;
    return s;
  }
  case TokenType::IF: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::IF;
    return s;
  }
  default:
    throw "Invalid: No PkbTables param for token";
  }
}

EvaluationTable ClauseDispatcher::toEvaluationTable(STRING_SET &set) {
  std::vector<VALUE> &column = std::vector<VALUE>();
  column.insert(column.end(), set.begin(), set.end());
  return EvaluationTable(new TABLE({{synonyms[0], column}}));
}

EvaluationTable ClauseDispatcher::toEvaluationTable(STRING_PAIRS &vectorPair) {
  std::vector<VALUE> &first = vectorPair.first;
  std::vector<VALUE> &second = vectorPair.second;

  // Handle special case where synonyms are the same
  if (synonyms[0] == synonyms[1]) {
    std::vector<VALUE> &column = std::vector<VALUE>();
    for (std::vector<int>::size_type i = 0; i < first.size(); i++) {
      if (first[i] == second[i]) {
        column.push_back(first[i]);
      }
    }
    return EvaluationTable(new TABLE({{synonyms[0], column}}));
  }
  return EvaluationTable(
      new TABLE({{synonyms[0], first}, {synonyms[1], second}}));
}

bool ClauseDispatcher::willReturnBoolean() {
  for (auto &param : pkbParameters) {
    if (std::holds_alternative<Statement>(param) ||
        std::holds_alternative<Procedure>(param) ||
        std::holds_alternative<Variable>(param)) {
      return false;
    }
  }
  return true;
}

bool ClauseDispatcher::booleanDispatch() {
  throw "Invalid: booleanDispatch not callable on this instance";
}

EvaluationTable ClauseDispatcher::resultDispatch() {
  throw "Invalid: resultsDispatch not callable on this instance";
}
