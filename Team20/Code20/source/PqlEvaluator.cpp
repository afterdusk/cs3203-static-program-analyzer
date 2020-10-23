#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>

#include "ClauseDispatcher.h"
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
