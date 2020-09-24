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
      ClauseResult &clauseResult = dispatcher->resultDispatch();
      delete dispatcher;
      table.add(clauseResult);
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
      ClauseResult &clauseResult = dispatcher->resultDispatch();
      delete dispatcher;
      table.add(clauseResult);
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
      ClauseResult &clauseResult = dispatcher->resultDispatch();
      delete dispatcher;
      // TODO: Optimize to reduce copying
      std::vector<VALUE> resultVector = clauseResult.valuesOf(selectedSynonym);
      std::copy(resultVector.begin(), resultVector.end(),
                std::back_inserter(result));
    }
  }
}

ClauseResult::ClauseResult(
    std::unordered_map<SYMBOL, std::vector<VALUE>> values) {
  if (values.size() == 0) {
    throw "Invalid: No results";
  }
  int length = (*values.begin()).second.size();
  for (auto &synonymValues : values) {
    if (synonymValues.second.size() != length) {
      throw "Invalid: Length mismatch between synonyms";
    }
  }
  this->values = values;
};

std::vector<SYMBOL> ClauseResult::synonyms() {
  std::vector<SYMBOL> result;
  for (auto &synonym : values) {
    result.push_back(synonym.first);
  }
  return result;
}

std::vector<VALUE> ClauseResult::valuesOf(SYMBOL synonym) {
  return values[synonym];
}

VALUE ClauseResult::valueAt(SYMBOL synonym, int i) {
  return values[synonym][i];
}

int ClauseResult::size() { return (*values.begin()).second.size(); }

bool ClauseResult::operator==(ClauseResult &other) {
  if (this->size() != other.size()) {
    return false;
  }
  std::set<std::vector<VALUE>> thisGroups;
  std::set<std::vector<VALUE>> otherGroups;

  for (int i = 0; i < this->size(); i++) {
    // A group is a row of values of each synonym
    std::vector<VALUE> thisGroup;
    std::vector<VALUE> otherGroup;
    for (auto &synonym : this->values) {
      thisGroup.push_back(synonym.second[i]);
      otherGroup.push_back(other.values[synonym.first][i]);
    }
    thisGroups.insert(thisGroup);
    otherGroups.insert(otherGroup);
  }

  return thisGroups == otherGroups;
}

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

ClauseResult ClauseDispatcher::toClauseResult(STRING_SET &set) {
  std::vector<VALUE> values;
  values.insert(values.end(), set.begin(), set.end());
  return ClauseResult({{synonyms[0], values}});
}

ClauseResult ClauseDispatcher::toClauseResult(STRING_PAIRS &vectorPair) {
  std::vector<VALUE> &first = vectorPair.first;
  std::vector<VALUE> &second = vectorPair.second;
  // Handle special case where synonyms are the same
  if (synonyms[0] == synonyms[1]) {
    std::vector<VALUE> commonValues;
    for (std::vector<int>::size_type i = 0; i < first.size(); i++) {
      if (first[i] == second[i]) {
        commonValues.push_back(first[i]);
      }
    }
    return ClauseResult({{synonyms[0], commonValues}});
  }
  return ClauseResult({{synonyms[0], first}, {synonyms[1], second}});
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

ClauseResult ClauseDispatcher::resultDispatch() {
  throw "Invalid: resultsDispatch not callable on this instance";
}

void EvaluationTable::add(ClauseResult &clauseResult) {
  // Add results and terminate early if table is empty
  if (empty()) {
    for (auto &clauseSynonym : clauseResult.synonyms()) {
      table[clauseSynonym] = clauseResult.valuesOf(clauseSynonym);
      seen.insert(clauseSynonym);
    }
    rows = clauseResult.size();
    return;
  }

  // Sort clause results into seen and unseen
  std::list<SYMBOL> seenClauseSynonyms;
  std::list<SYMBOL> unseenClauseSynonyms;
  for (auto &clauseSynonym : clauseResult.synonyms()) {
    if (isSeen(clauseSynonym)) {
      seenClauseSynonyms.push_back(clauseSynonym);
    } else {
      unseenClauseSynonyms.push_back(clauseSynonym);
    }
  }

  // Transient table that will store new values
  TABLE newTable;
  int newRows = 0;

  // Iterate over each result, each existing row in the table
  for (int clauseIndex = 0; clauseIndex < clauseResult.size(); clauseIndex++) {
    for (int valuesIndex = 0; valuesIndex < rowCount(); valuesIndex++) {
      // Check if values of seen columns match
      bool isMatch = true;
      for (auto &seenClauseSynonym : seenClauseSynonyms) {
        std::vector<VALUE> seenColumn = table[seenClauseSynonym];
        if (seenColumn[valuesIndex] !=
            clauseResult.valueAt(seenClauseSynonym, clauseIndex)) {
          isMatch = false;
          break;
        }
      }
      // Push cross product into new table if seen columns match
      if (isMatch) {
        newRows += 1;
        for (auto &synonym : seen) {
          newTable[synonym].push_back(table[synonym][valuesIndex]);
        }
        for (auto &unseenClauseSynonym : unseenClauseSynonyms) {
          newTable[unseenClauseSynonym].push_back(
              clauseResult.valueAt(unseenClauseSynonym, clauseIndex));
        }
      }
    }
  }

  // Mark all synonyms in this batch of results as seen
  for (auto &clauseSynonym : clauseResult.synonyms()) {
    seen.insert(clauseSynonym);
  }

  // Complete operation by replacing the values table
  table = newTable; // TODO: avoid copying here
  rows = newRows;
}

bool EvaluationTable::isSeen(SYMBOL synonym) {
  return seen.find(synonym) != seen.end();
}

bool EvaluationTable::empty() { return seen.empty(); }

std::unordered_set<VALUE> EvaluationTable::select(SYMBOL synonym) {
  std::vector<VALUE> &selected = table[synonym];
  return std::unordered_set<VALUE>(selected.begin(), selected.end());
}

std::vector<VALUE> EvaluationTable::selectColumn(SYMBOL synonym) {
  return table[synonym];
}

int EvaluationTable::rowCount() { return rows; }
