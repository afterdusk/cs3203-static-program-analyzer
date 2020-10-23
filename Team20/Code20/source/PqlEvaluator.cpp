#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>

#include "Dispatchers.h"
#include "PqlEvaluator.h"

void Pql::evaluate(ParsedQuery pq, PkbQueryInterface *queryHandler,
                   std::list<std::string> &result) {
  // Instantiate evaluation table
  EvaluationTable table;

  // Fetch values for with clauses and push to table
  for (auto &with : pq.withs) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromWith(with, pq.declarations, queryHandler);
    EvaluationTable clauseResult = dispatcher->resultDispatch();
    delete dispatcher;
    table.hashMerge(clauseResult);
  }

  // Do the same for such that clauses
  for (auto &relationship : pq.relationships) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(relationship, queryHandler);
    if (dispatcher->willReturnBoolean()) {
      // Early termination if clause evaluates to false
      if (!dispatcher->booleanDispatch()) {
        delete dispatcher;
        if (pq.results.resultType == PqlResultType::Boolean) {
          result.push_back(FALSE_RESULT);
        }
        return;
      }
    } else {
      EvaluationTable clauseResult = dispatcher->resultDispatch();
      delete dispatcher;
      table.hashMerge(clauseResult);
    }
  }

  // Do the same for pattern clauses
  for (auto &pattern : pq.patterns) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromPattern(pattern, queryHandler);
    EvaluationTable clauseResult = dispatcher->resultDispatch();
    delete dispatcher;
    table.hashMerge(clauseResult);
  }

  // Early termination if table contains symbols, but has no values
  if (!table.empty() && table.rowCount() == 0) {
    if (pq.results.resultType == PqlResultType::Boolean) {
      result.push_back(FALSE_RESULT);
    }
    return;
  }

  // Identify symbols not present in EvaluationTable
  std::unordered_set<SYMBOL> seenSelected;
  std::vector<Element> unseenSelected;
  for (auto &element : pq.results.results) {
    std::optional<SYMBOL> attrSymbol =
        elementAttrToSymbol(pq.declarations[element.synonym], element);
    SYMBOL symbol = attrSymbol.value_or(element.synonym);
    if (table.isSeen(symbol)) {
      seenSelected.insert(symbol);
    } else {
      unseenSelected.push_back(element);
    }
    // If symbol is attribute, make sure synonym is not sliced away (if present)
    if (attrSymbol.has_value() && table.isSeen(element.synonym)) {
      seenSelected.insert(element.synonym);
    }
  }

  // Resolve unseen selected symbols
  EvaluationTable filtered = table.slice(seenSelected);
  for (auto &element : unseenSelected) {
    TokenType type = pq.declarations[element.synonym];
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromElement(type, element, queryHandler);
    EvaluationTable clauseResult = dispatcher->resultDispatch();
    delete dispatcher;
    filtered.hashMerge(clauseResult);
  }

  if (pq.results.resultType == PqlResultType::Boolean) {
    if (!filtered.empty() && filtered.rowCount() == 0) {
      result.push_back(FALSE_RESULT);
    } else {
      result.push_back(TRUE_RESULT);
    }
    return;
  }

  std::vector<SYMBOL> selected;
  for (auto &element : pq.results.results) {
    SYMBOL symbol =
        elementAttrToSymbol(pq.declarations[element.synonym], element)
            .value_or(element.synonym);
    selected.push_back(symbol);
  }
  filtered.flatten(selected, result);
}

std::optional<SYMBOL> elementAttrToSymbol(TokenType type, Element element) {
  std::stringstream stream;
  stream << element.synonym;
  switch (element.refType) {
  case AttributeRefType::PROCNAME:
    if (type == TokenType::CALL) {
      stream << "."
             << "procName";
      break;
    }
    [[fallthrough]];
  case AttributeRefType::VARNAME:
    if (type == TokenType::PRINT || type == TokenType::READ) {
      stream << "."
             << "varName";
      break;
    }
    [[fallthrough]];
  default:
    return {};
  }
  return stream.str();
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
  std::unordered_set<std::string> thisGroups;
  std::unordered_set<std::string> otherGroups;

  std::vector<SYMBOL> order;
  for (auto &column : *table) {
    order.push_back(column.first);
  }

  for (int index = 0; index < this->rows; index++) {
    thisGroups.insert(rowHash(index, order));
    otherGroups.insert(other.rowHash(index, order));
  }

  return thisGroups == otherGroups;
}

EvaluationTable::EvaluationTable(TABLE *table) {
  rows = table->size() == 0 ? 0 : (*table->begin()).second.size();
  for (auto &column : *table) {
    seen.insert(column.first);
    if (column.second.size() != rows) {
      throw "Invalid: Length mismatch between symbols";
    }
  }
  this->table = table;
};

void EvaluationTable::hashMerge(EvaluationTable &other) {
  // Terminate if other table is empty
  if (other.empty()) {
    return;
  }

  // Add results and terminate early if this table is empty
  if (empty()) {
    for (auto &otherColumn : *other.table) {
      (*table)[otherColumn.first] = otherColumn.second;
      seen.insert(otherColumn.first);
    }
    rows = other.rows;
    return;
  }

  // Set build side to be the smaller table
  EvaluationTable *build;
  EvaluationTable *probe;
  if (rowCount() < other.rowCount()) {
    build = this;
    probe = &other;
  } else {
    build = &other;
    probe = this;
  }

  // Separate probe side symbols into common and uncommon
  std::vector<SYMBOL> commonSymbols;
  std::vector<SYMBOL> uncommonSymbols;
  for (auto &probeColumn : *(probe->table)) {
    if (build->isSeen(probeColumn.first)) {
      commonSymbols.push_back(probeColumn.first);
    } else {
      uncommonSymbols.push_back(probeColumn.first);
    }
  }

  // Prepare hash representation of build side
  std::unordered_map<ROW_HASH, std::vector<int>> buildHashes;
  for (int buildIndex = 0; buildIndex < build->rowCount(); buildIndex++) {
    ROW_HASH rowhash = build->rowHash(buildIndex, commonSymbols);
    buildHashes[rowhash].push_back(buildIndex);
  }

  // Transient table that will store new values
  TABLE *newTable = new TABLE;
  int newRows = 0;

  // Iterate over probe side and push hash matches
  for (int probeIndex = 0; probeIndex < probe->rowCount(); probeIndex++) {
    ROW_HASH rowhash = probe->rowHash(probeIndex, commonSymbols);
    for (auto &buildIndex : buildHashes[rowhash]) {
      newRows += 1;
      for (auto &symbol : build->seen) {
        (*newTable)[symbol].push_back((*(build->table))[symbol][buildIndex]);
      }
      for (auto &symbol : uncommonSymbols) {
        (*newTable)[symbol].push_back((*(probe->table))[symbol][probeIndex]);
      }
    }
  }

  // Mark all symbols in this batch of results as seen
  for (auto &column : (*other.table)) {
    seen.insert(column.first);
  }

  // Complete operation by replacing the values table
  delete table;
  table = newTable;
  rows = newRows;
}

void EvaluationTable::merge(EvaluationTable &other) {
  // Terminate if other table is empty
  if (other.empty()) {
    return;
  }

  // Add results and terminate early if table is empty
  if (empty()) {
    for (auto &otherColumn : *other.table) {
      (*table)[otherColumn.first] = otherColumn.second;
      seen.insert(otherColumn.first);
    }
    rows = other.rows;
    return;
  }

  // Separate symbols into seen and unseen
  std::list<SYMBOL> seenSymbols;
  std::list<SYMBOL> unseenSymbols;
  for (auto &otherColumn : *other.table) {
    if (isSeen(otherColumn.first)) {
      seenSymbols.push_back(otherColumn.first);
    } else {
      unseenSymbols.push_back(otherColumn.first);
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
      for (auto &seenSymbol : seenSymbols) {
        std::vector<VALUE> seenColumn = (*table)[seenSymbol];
        if (seenColumn[tableIndex] != (*other.table)[seenSymbol][otherIndex]) {
          isMatch = false;
          break;
        }
      }
      // Push cross product into new table if seen columns match
      if (isMatch) {
        newRows += 1;
        for (auto &symbol : seen) {
          (*newTable)[symbol].push_back((*table)[symbol][tableIndex]);
        }
        for (auto &unseenSymbol : unseenSymbols) {
          (*newTable)[unseenSymbol].push_back(
              (*other.table)[unseenSymbol][otherIndex]);
        }
      }
    }
  }

  // Mark all unseen symbols as seen
  for (auto &symbol : unseenSymbols) {
    seen.insert(symbol);
  }

  // Complete operation by replacing the values table
  delete table;
  table = newTable;
  rows = newRows;
}

bool EvaluationTable::isSeen(SYMBOL symbol) {
  return seen.find(symbol) != seen.end();
}

bool EvaluationTable::areSeen(std::vector<SYMBOL> symbols) {
  for (auto &symbol : symbols) {
    if (!isSeen(symbol))
      return false;
  }
  return true;
}

bool EvaluationTable::empty() { return seen.empty(); }

EvaluationTable EvaluationTable::slice(std::unordered_set<SYMBOL> symbols) {
  std::vector<SYMBOL> order;
  order.insert(order.end(), symbols.begin(), symbols.end());

  if (!areSeen(order)) {
    throw "Invalid: Unable to select symbols not present in table";
  }

  TABLE *newTable = new TABLE;
  std::unordered_set<VALUE> added;
  for (int index = 0; index < rowCount(); index++) {
    ROW_HASH rowhashStr = rowHash(index, order);
    if (added.find(rowhashStr) != added.end()) {
      continue;
    }

    added.insert(rowhashStr);
    for (auto &symbol : order) {
      (*newTable)[symbol].push_back((*table)[symbol][index]);
    }
  }
  return EvaluationTable(newTable);
}

void EvaluationTable::flatten(std::vector<SYMBOL> symbols,
                              std::list<VALUE> &result) {
  // Filter down to selected symbols to purge duplicate rows
  if (symbols.size() < seen.size()) {
    std::unordered_set<SYMBOL> symbolSet(symbols.begin(), symbols.end());
    EvaluationTable filtered = slice(symbolSet);
    filtered.flatten(symbols, result);
    return;
  }

  for (int index = 0; index < rowCount(); index++) {
    std::stringstream rowStream;
    for (auto &symbol : symbols) {
      rowStream << (*table)[symbol][index] << RESULT_DELIMITER;
    }
    std::string rowString = rowStream.str();
    rowString.erase(rowString.find_last_not_of(RESULT_DELIMITER) + 1);
    result.push_back(rowString);
  }
}

ROW_HASH EvaluationTable::rowHash(int index, std::vector<SYMBOL> &order) {
  std::stringstream stream;
  for (auto &symbol : order) {
    if (!isSeen(symbol)) {
      throw "Invalid: Order provided contains invalid symbol";
    }
    stream << (*table)[symbol][index] << HASH_DELIMITER;
  }
  return stream.str();
}

int EvaluationTable::rowCount() { return rows; }

std::unordered_set<VALUE> EvaluationTable::select(SYMBOL symbol) {
  std::vector<VALUE> &selected = (*table)[symbol];
  return std::unordered_set<VALUE>(selected.begin(), selected.end());
}

ClauseDispatcher::ClauseDispatcher(PkbQueryInterface *queryHandler) {
  handler = queryHandler;
}

std::string ClauseDispatcher::toString(PkbTables::LINE_NO lineNumber) {
  std::stringstream stream;
  stream << lineNumber;
  return stream.str();
}

PkbTables::LINE_NO ClauseDispatcher::toLineNumber(std::string lineNumberStr) {
  PkbTables::LINE_NO lineNumber;
  std::stringstream stream(lineNumberStr);
  stream >> lineNumber;
  return lineNumber;
}

ClauseDispatcher *ClauseDispatcher::FromElement(TokenType type, Element element,
                                                PkbQueryInterface *handler) {
  std::optional<SYMBOL> maybeSymbol = elementAttrToSymbol(type, element);
  if (maybeSymbol.has_value()) {
    return new SelectAttributeDispatcher(type, element, handler);
  }
  return new SelectDispatcher(type, element.synonym, handler);
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
  case TokenType::CALL:
    return new CallsDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::CALL_T:
    return new CallsStarDispatcher(pr.firstArgument, pr.secondArgument,
                                   handler);
  case TokenType::NEXT:
    return new NextDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::NEXT_T:
    return new NextStarDispatcher(pr.firstArgument, pr.secondArgument, handler);
  default:
    throw "Invalid: Relationship not implemented";
  }
}

ClauseDispatcher *ClauseDispatcher::FromPattern(ParsedPattern pp,
                                                PkbQueryInterface *handler) {
  return new PatternDispatcher(pp, handler);
}

ClauseDispatcher *ClauseDispatcher::FromWith(
    std::pair<Reference, Reference> pw,
    std::unordered_map<std::string, TokenType> declarations,
    PkbQueryInterface *queryHandler) {
  if (pw.first.referenceType == ReferenceType::ELEMENT &&
      pw.second.referenceType == ReferenceType::ELEMENT) {
    TokenType firstType = declarations[pw.first.element.synonym];
    TokenType secondType = declarations[pw.second.element.synonym];
    return new WithElementPairDispatcher(firstType, pw.first.element,
                                         secondType, pw.second.element,
                                         queryHandler);
  }
  if (pw.first.referenceType == ReferenceType::ELEMENT &&
      pw.second.referenceType == ReferenceType::RAW_VALUE) {
    TokenType type = declarations[pw.first.element.synonym];
    return new WithElementRawPairDispatcher(type, pw.first.element,
                                            pw.second.pqlToken, queryHandler);
  }
  if (pw.first.referenceType == ReferenceType::RAW_VALUE &&
      pw.second.referenceType == ReferenceType::ELEMENT) {
    TokenType type = declarations[pw.second.element.synonym];
    return new WithElementRawPairDispatcher(type, pw.second.element,
                                            pw.first.pqlToken, queryHandler);
  }
  throw "Invalid: With comparison not implemented";
}

ClauseDispatcher::PKB_PARAM ClauseDispatcher::toParam(PqlToken token) {
  switch (token.type) {
  case TokenType::VARIABLE:
    symbols.push_back(token.value);
    return Variable{};
  case TokenType::PROCEDURE:
    symbols.push_back(token.value);
    return Procedure{};
  case TokenType::CONSTANT:
    symbols.push_back(token.value);
    return Constant{};
  case TokenType::UNDERSCORE:
    return Underscore{};
  case TokenType::NUMBER:
    return LineNumber{toLineNumber(token.value)};
  case TokenType::STRING: {
    String s;
    s.name = token.value;
    return s;
  }
  case TokenType::PROG_LINE:
  case TokenType::STMT: {
    symbols.push_back(token.value);
    Statement s;
    return s;
  }
  case TokenType::READ: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Read;
    return s;
  }
  case TokenType::PRINT: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Print;
    return s;
  }
  case TokenType::ASSIGN: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Assign;
    return s;
  }
  case TokenType::CALL: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Call;
    return s;
  }
  case TokenType::WHILE: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::While;
    return s;
  }
  case TokenType::IF: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::If;
    return s;
  }
  default:
    throw "Invalid: No PkbTables param for token";
  }
}

EvaluationTable ClauseDispatcher::toEvaluationTable(NAME_SET &set) {
  std::vector<VALUE> &column = std::vector<VALUE>();
  column.insert(column.end(), set.begin(), set.end());
  return EvaluationTable(new TABLE({{symbols[0], column}}));
}

EvaluationTable ClauseDispatcher::toEvaluationTable(LINE_SET &set) {
  std::vector<VALUE> &column = std::vector<VALUE>();
  for (PkbTables::LINE_NO lineNumber : set) {
    column.push_back(toString(lineNumber));
  }
  return EvaluationTable(new TABLE({{symbols[0], column}}));
}

EvaluationTable ClauseDispatcher::toEvaluationTable(NAME_NAME_PAIRS &pairs) {
  std::vector<VALUE> &first = pairs.first;
  std::vector<VALUE> &second = pairs.second;

  // Handle special case where symbols are the same
  if (symbols[0] == symbols[1]) {
    std::vector<VALUE> &column = std::vector<VALUE>();
    for (std::vector<int>::size_type i = 0; i < first.size(); i++) {
      if (first[i] == second[i]) {
        column.push_back(first[i]);
      }
    }
    return EvaluationTable(new TABLE({{symbols[0], column}}));
  }

  return EvaluationTable(
      new TABLE({{symbols[0], first}, {symbols[1], second}}));
}

EvaluationTable ClauseDispatcher::toEvaluationTable(LINE_LINE_PAIRS &pairs) {
  std::vector<VALUE> first;
  std::vector<VALUE> second;

  for (auto &value : pairs.first) {
    first.push_back(toString(value));
  }
  for (auto &value : pairs.second) {
    second.push_back(toString(value));
  }

  NAME_NAME_PAIRS nameNamePairs = {first, second};
  return toEvaluationTable(nameNamePairs);
}

EvaluationTable ClauseDispatcher::toEvaluationTable(LINE_NAME_PAIRS &pairs) {
  std::vector<VALUE> first;

  for (auto &value : pairs.first) {
    first.push_back(toString(value));
  }

  NAME_NAME_PAIRS nameNamePairs = {first, pairs.second};
  return toEvaluationTable(nameNamePairs);
}

std::vector<SYMBOL> ClauseDispatcher::getSymbols() { return symbols; }

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
