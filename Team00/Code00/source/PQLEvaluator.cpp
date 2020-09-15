#include <iostream>
#include <list>
#include <set>

#include "PQLEvaluator.h"

// TODO: Accept the output string as a parameter, then populate it with results
std::list<std::string> PQL::evaluate(ParsedQuery pq, Pkb &pkb) {
  // Instantiate query handler and evaluation table
  std::vector<SYMBOL> synonyms;
  for (auto &synonym : pq.declaration_clause) {
    synonyms.push_back(synonym.first);
  }
  EvaluationTable table(synonyms);
  PkbQueryInterface queryHandler(pkb);

  // Fetch values for relationship clauses from Pkb and push to table
  for (auto &relationship : pq.relationship_clauses) {
    ClauseDispatcher dispatcher(relationship, queryHandler);
    // Early termination if clause evaluates to false
    if (dispatcher.willReturnBoolean()) {
      if (!dispatcher.booleanDispatch()) {
        return {};
      }
    }
    ClauseResult &clauseResult = dispatcher.resultDispatch();
    table.add(clauseResult);
  }

  // Do the same for pattern clauses
  for (auto &pattern : pq.pattern_clauses) {
    ClauseDispatcher dispatcher(pattern, queryHandler);
    // Early termination if clause evaluates to false
    if (dispatcher.willReturnBoolean()) {
      if (!dispatcher.booleanDispatch()) {
        return {};
      }
    }
    ClauseResult &clauseResult = dispatcher.resultDispatch();
    table.add(clauseResult);
  }

  // Select values from table if contained in table, else fetch from PKB
  std::list<std::string> result;
  SYMBOL selectedSynonym = pq.result_clause[0];
  if (table.isSeen(selectedSynonym)) {
    std::unordered_set<std::string> selected = table.select(selectedSynonym);
    result.insert(result.end(), selected.begin(), selected.end());
  } else {
    if (!table.empty() && table.rowCount() == 0) {
      result = {};
    } else {
      TokenType type = pq.declaration_clause[selectedSynonym];
      PqlToken token = PqlToken{type, selectedSynonym};
      ClauseDispatcher dispatcher(token, queryHandler);
      ClauseResult &clauseResult = dispatcher.resultDispatch();
      // TODO: Optimize to reduce copying
      std::vector<VALUE> resultVector = clauseResult.valuesOf(selectedSynonym);
      std::copy(resultVector.begin(), resultVector.end(),
                std::back_inserter(result));
    }
  }
  return result;
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

ClauseDispatcher::ClauseDispatcher(PqlToken token, PkbQueryInterface &handler)
    : handler(handler) {
  pkbParameters.push_back(toParam(token));
}

ClauseDispatcher::ClauseDispatcher(ParsedRelationship pr,
                                   PkbQueryInterface &handler)
    : handler(handler) {
  maybeRelationship = pr.relationship;
  pkbParameters.push_back(toParam(pr.first_argument));
  pkbParameters.push_back(toParam(pr.second_argument));
}

ClauseDispatcher::ClauseDispatcher(ParsedPattern pp, PkbQueryInterface &handler)
    : handler(handler) {
  maybeRelationship = TokenType::MATCH;
  pkbParameters.push_back(toParam(pp.synonym));
  pkbParameters.push_back(toParam(pp.lhs));
  pkbParameters.push_back(pp.rhs);
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
    s.type = StatementType::NONE;
    return s;
  }
  case TokenType::READ: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = StatementType::READ;
    return s;
  }
  case TokenType::PRINT: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = StatementType::PRINT;
    return s;
  }
  case TokenType::ASSIGN: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = StatementType::ASSIGN;
    return s;
  }
  case TokenType::CALL: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = StatementType::CALL;
    return s;
  }
  case TokenType::WHILE: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = StatementType::WHILE;
    return s;
  }
  case TokenType::IF: {
    synonyms.push_back(token.value);
    Statement s;
    s.type = StatementType::IF;
    return s;
  }
  default:
    throw "Invalid: No Pkb param for token";
  }
}

bool ClauseDispatcher::booleanDispatch() {
  switch (maybeRelationship.value()) {
  case TokenType::FOLLOWS: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.follows(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.follows(*first, *second);
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.follows(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.follows(*first, *second);
      }
    }
    throw "Invalid: Parameters provided do not return boolean";
  }
  case TokenType::FOLLOWS_T: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.followsStar(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.followsStar(*first, *second);
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.followsStar(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.followsStar(*first, *second);
      }
    }
    throw "Invalid: Parameters provided do not return boolean";
  }
  case TokenType::PARENT: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.parent(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.parent(*first, *second);
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.parent(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.parent(*first, *second);
      }
    }
    throw "Invalid: Parameters provided do not return boolean";
  }
  case TokenType::PARENT_T: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.parentStar(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.parentStar(*first, *second);
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return handler.parentStar(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.parentStar(*first, *second);
      }
    }
    throw "Invalid: Parameters provided do not return boolean";
  }
  case TokenType::USES: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return handler.uses(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.uses(*first, *second);
      }
    }
    if (String *first = std::get_if<String>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return handler.uses(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.uses(*first, *second);
      }
    }
    throw "Invalid: Parameters provided do not return boolean";
  }
  case TokenType::MODIFIES: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return handler.modifies(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.modifies(*first, *second);
      }
    }
    if (String *first = std::get_if<String>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return handler.modifies(*first, *second);
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return handler.modifies(*first, *second);
      }
    }
    throw "Invalid: Parameters provided do not return boolean";
  }
  default:
    throw "Invalid: Relationship not implemented";
  }
}

ClauseResult ClauseDispatcher::resultDispatch() {
  // No relationship - a "select" operation on all values of an entity
  if (!maybeRelationship.has_value()) {
    if (Statement *entity = std::get_if<Statement>(&pkbParameters[0])) {
      return toClauseResult(handler.select(*entity));
    }
    if (Procedure *entity = std::get_if<Procedure>(&pkbParameters[0])) {
      return toClauseResult(handler.select(*entity));
    }
    if (Variable *entity = std::get_if<Variable>(&pkbParameters[0])) {
      return toClauseResult(handler.select(*entity));
    }
    if (Constant *entity = std::get_if<Constant>(&pkbParameters[0])) {
      return toClauseResult(handler.select(*entity));
    }
  }

  switch (maybeRelationship.value()) {
  case TokenType::MATCH: {
    if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        PatternSpec third = std::get<PatternSpec>(pkbParameters[2]);
        return toClauseResult((handler.match(*first, *second, third)));
      }
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        PatternSpec third = std::get<PatternSpec>(pkbParameters[2]);
        return toClauseResult((handler.match(*first, *second, third)));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        PatternSpec third = std::get<PatternSpec>(pkbParameters[2]);
        return toClauseResult((handler.match(*first, *second, third)));
      }
    }
    throw "Invalid: Parameters provided do not return ClauseResult";
  }
  case TokenType::FOLLOWS: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.follows(*first, *second));
      }
    }
    if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return toClauseResult(handler.follows(*first, *second));
      }
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.follows(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.follows(*first, *second));
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.follows(*first, *second));
      }
    }
    throw "Invalid: Parameters provided do not return ClauseResult";
  }
  case TokenType::FOLLOWS_T: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.followsStar(*first, *second));
      }
    }
    if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return toClauseResult(handler.followsStar(*first, *second));
      }
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.followsStar(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.followsStar(*first, *second));
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.followsStar(*first, *second));
      }
    }
    throw "Invalid: Parameters provided do not return ClauseResult";
  }
  case TokenType::PARENT: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.parent(*first, *second));
      }
    }
    if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return toClauseResult(handler.parent(*first, *second));
      }
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.parent(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.parent(*first, *second));
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.parent(*first, *second));
      }
    }
    throw "Invalid: Parameters provided do not return ClauseResult";
  }
  case TokenType::PARENT_T: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.parentStar(*first, *second));
      }
    }
    if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
      if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
        return toClauseResult(handler.parentStar(*first, *second));
      }
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.parentStar(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.parentStar(*first, *second));
      }
    }
    if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
      if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
        return toClauseResult(handler.parentStar(*first, *second));
      }
    }
    throw "Invalid: Parameters provided do not return ClauseResult";
  }
  case TokenType::USES: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
    }
    if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
    }
    if (String *first = std::get_if<String>(&pkbParameters[0])) {
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
    }
    if (Procedure *first = std::get_if<Procedure>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.uses(*first, *second));
      }
    }
    throw "Invalid: Parameters provided do not return ClauseResult";
  }
  case TokenType::MODIFIES: {
    if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
    }
    if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
    }
    if (String *first = std::get_if<String>(&pkbParameters[0])) {
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
    }
    if (Procedure *first = std::get_if<Procedure>(&pkbParameters[0])) {
      if (String *second = std::get_if<String>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
      if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
      if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
        return toClauseResult(handler.modifies(*first, *second));
      }
    }
    throw "Invalid: Parameters provided do not return ClauseResult";
  }
  default:
    throw "Invalid: Relationship not implemented";
  }
}

ClauseResult ClauseDispatcher::toClauseResult(STRING_SET &set) {
  std::vector<VALUE> values;
  values.insert(values.end(), set.begin(), set.end());
  return ClauseResult({{synonyms[0], values}});
}

ClauseResult ClauseDispatcher::toClauseResult(STRING_VECTOR &vector) {
  return ClauseResult({{synonyms[0], vector}});
}

ClauseResult ClauseDispatcher::toClauseResult(STRING_PAIRS &vectorPair) {
  return ClauseResult(
      {{synonyms[0], vectorPair.first}, {synonyms[1], vectorPair.second}});
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

EvaluationTable::EvaluationTable(std::vector<SYMBOL> declared) {
  if (declared.size() == 0) {
    throw "Invalid: Cannot create an EvaluationTable with no synonyms";
  }

  synonyms = std::unordered_set<SYMBOL>(declared.begin(), declared.end());
  values = generateValuesMap(synonyms);
};

std::unordered_map<SYMBOL, std::vector<VALUE>>
EvaluationTable::generateValuesMap(std::unordered_set<SYMBOL> synonyms) {
  std::unordered_map<SYMBOL, std::vector<VALUE>> newValues;
  for (auto &synonym : synonyms) {
    newValues.insert({synonym, {}});
  }
  return newValues;
}

void EvaluationTable::add(ClauseResult &clauseResult) {
  // Validate clause result synonyms
  for (auto &clauseSynonym : clauseResult.synonyms()) {
    if (synonyms.find(clauseSynonym) == synonyms.end()) {
      throw "Invalid: Clause result synonym not found in EvaluationTable";
    }
  }

  // Add results and terminate early if table is empty
  if (empty()) {
    for (auto &clauseSynonym : clauseResult.synonyms()) {
      values[clauseSynonym] = clauseResult.valuesOf(clauseSynonym);
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
  std::unordered_map<SYMBOL, std::vector<VALUE>> newValues =
      generateValuesMap(synonyms);
  int newRows = 0;

  // Iterate over each result, each existing row in the table
  for (int clauseIndex = 0; clauseIndex < clauseResult.size(); clauseIndex++) {
    for (int valuesIndex = 0; valuesIndex < rowCount(); valuesIndex++) {
      // Check if values of seen columns match
      bool isMatch = true;
      for (auto &seenClauseSynonym : seenClauseSynonyms) {
        std::vector<VALUE> seenColumn = values[seenClauseSynonym];
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
          newValues[synonym].push_back(values[synonym][valuesIndex]);
        }
        for (auto &unseenClauseSynonym : unseenClauseSynonyms) {
          newValues[unseenClauseSynonym].push_back(
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
  values = newValues;
  rows = newRows;
}

bool EvaluationTable::isSeen(SYMBOL synonym) {
  return seen.find(synonym) != seen.end();
}

bool EvaluationTable::empty() { return seen.empty(); }

std::unordered_set<VALUE> EvaluationTable::select(SYMBOL synonym) {
  std::vector<VALUE> &selected = values[synonym];
  return std::unordered_set<VALUE>(selected.begin(), selected.end());
}

std::vector<VALUE> EvaluationTable::selectColumn(SYMBOL synonym) {
  return values[synonym];
}

int EvaluationTable::rowCount() { return rows; }
