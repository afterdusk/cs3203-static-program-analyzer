#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <queue>
#include <sstream>

#include "ClauseDispatcher.h"
#include "DispatcherGraph.h"
#include "PqlEvaluator.h"

std::list<ClauseDispatcher *>
generateDispatchers(ParsedQuery &pq, PkbQueryInterface *queryHandler) {
  std::list<ClauseDispatcher *> dispatchers;
  for (auto &relationship : pq.relationships) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(relationship, queryHandler);
    dispatchers.push_back(dispatcher);
  }

  for (auto &pattern : pq.patterns) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromPattern(pattern, queryHandler);
    dispatchers.push_back(dispatcher);
  }

  for (auto &with : pq.withs) {
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromWith(with, pq.declarations, queryHandler);
    dispatchers.push_back(dispatcher);
  }
  return dispatchers;
}

void destroyDispatchers(std::list<ClauseDispatcher *> dispatchers) {
  for (auto &dispatcher : dispatchers) {
    delete dispatcher;
  }
}

void Pql::evaluate(ParsedQuery pq, PkbQueryInterface *queryHandler,
                   std::list<std::string> &result) {
  std::list<ClauseDispatcher *> dispatchers =
      generateDispatchers(pq, queryHandler);
  std::list<DispatcherGraph> graphs;
  std::unordered_set<SYMBOL> seen;

  // Instantiate priority queue of boolean clauses
  typedef std::pair<int, ClauseDispatcher *> PQ_DISPATCHER;
  std::priority_queue<PQ_DISPATCHER, std::vector<PQ_DISPATCHER>,
                      std::function<bool(PQ_DISPATCHER &, PQ_DISPATCHER &)>>
      booleanClauseQueue([](PQ_DISPATCHER &node1, PQ_DISPATCHER &node2) {
        return node1.first < node2.first;
      });

  // For each dispatcher, if dispatcher is:
  // - boolean returning clause: push into priority queue
  // - else:                     push into existing/new graph
  for (auto &dispatcher : dispatchers) {
    if (dispatcher->willReturnBoolean()) {
      booleanClauseQueue.push(PQ_DISPATCHER{
          dispatcher->dispatchPriority(),
          dispatcher,
      });
      continue;
    }

    // Find matched graphs (if any), and add dispatcher
    std::vector<std::list<DispatcherGraph>::iterator> matchedGraphs;
    for (std::list<DispatcherGraph>::iterator i = graphs.begin();
         i != graphs.end(); i++) {
      DispatcherGraph &graph = (*i);
      bool containsSymbol = false;
      for (auto &symbol : dispatcher->getSymbols()) {
        if (graph.contains(symbol)) {
          containsSymbol = true;
          break;
        }
      }
      if (containsSymbol) {
        matchedGraphs.push_back(i);
      }
    }

    switch (matchedGraphs.size()) {
    // Requirements specify that there will never be >2 way merge
    case 2:
      matchedGraphs[0]->merge(*matchedGraphs[1], dispatcher);
      graphs.erase(matchedGraphs[1]);
      break;
    case 1:
      matchedGraphs[0]->addDispatcher(dispatcher);
      break;
    case 0:
      DispatcherGraph graph;
      graph.addDispatcher(dispatcher);
      graphs.push_back(graph);
    }

    // Add symbols to symbols set
    for (auto &symbol : dispatcher->getSymbols()) {
      seen.insert(symbol);
    }
  }

  // Evaluate boolean clauses in order of priority
  while (!booleanClauseQueue.empty()) {
    ClauseDispatcher *dispatcher = booleanClauseQueue.top().second;
    booleanClauseQueue.pop();
    // Early termination if clause evaluates to false
    if (!dispatcher->booleanDispatch()) {
      if (pq.results.resultType == PqlResultType::Boolean) {
        result.push_back(FALSE_RESULT);
      }
      destroyDispatchers(dispatchers);
      return;
    }
  }

  // Identify symbols in result but are not seen in any of the clauses
  std::unordered_set<SYMBOL> seenSelected;
  std::vector<Element> unseenSelected;
  for (auto &element : pq.results.results) {
    std::optional<SYMBOL> attrSymbol =
        elementAttrToSymbol(pq.declarations[element.synonym], element);
    SYMBOL symbol = attrSymbol.value_or(element.synonym);
    if (setContains(seen, symbol)) {
      seenSelected.insert(symbol);
    } else {
      unseenSelected.push_back(element);
    }
    // If symbol is attribute, make sure synonym is not sliced away (if present)
    if (attrSymbol.has_value()) {
      seenSelected.insert(element.synonym);
    }
  }

  // Instantiate main evaluation table
  EvaluationTable table;

  // Instantiate priority queue of graphs
  typedef std::pair<int, DispatcherGraph *> PQ_GRAPH;
  std::priority_queue<PQ_GRAPH, std::vector<PQ_GRAPH>,
                      std::function<bool(PQ_GRAPH &, PQ_GRAPH &)>>
      graphQueue([](PQ_GRAPH &node1, PQ_GRAPH &node2) {
        return node1.first < node2.first;
      });

  // Push graphs into priority queue
  for (auto &graph : graphs) {
    graphQueue.push(PQ_GRAPH{graph.priority(), &graph});
  }

  // Evaluate graphs in order of priority
  while (!graphQueue.empty()) {
    DispatcherGraph *graph = graphQueue.top().second;
    graphQueue.pop();
    EvaluationTable incoming = graph->evaluate(seenSelected);

    // Early termination if table has no values
    if (!incoming.noSymbols() && incoming.rowCount() == 0) {
      if (pq.results.resultType == PqlResultType::Boolean) {
        result.push_back(FALSE_RESULT);
      }
      destroyDispatchers(dispatchers);
      return;
    }

    table.hashMerge(incoming);
  }

  // Resolve unseen selected symbols
  for (auto &element : unseenSelected) {
    TokenType type = pq.declarations[element.synonym];
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromElement(type, element, queryHandler);
    EvaluationTable clauseResult = dispatcher->resultDispatch();
    delete dispatcher;
    table.hashMerge(clauseResult);
  }

  // Return boolean result
  if (pq.results.resultType == PqlResultType::Boolean) {
    if (!table.noSymbols() && table.rowCount() == 0) {
      result.push_back(FALSE_RESULT);
    } else {
      result.push_back(TRUE_RESULT);
    }
    destroyDispatchers(dispatchers);
    return;
  }

  // Return non-boolean results
  std::vector<SYMBOL> selected;
  for (auto &element : pq.results.results) {
    SYMBOL symbol =
        elementAttrToSymbol(pq.declarations[element.synonym], element)
            .value_or(element.synonym);
    selected.push_back(symbol);
  }
  table.flatten(selected, result);
  destroyDispatchers(dispatchers);
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
  symbols = other.symbols;
  table = new TABLE;
  *table = *other.table;
}

EvaluationTable::~EvaluationTable() { delete table; }

EvaluationTable &EvaluationTable::operator=(const EvaluationTable &other) {
  if (this == &other)
    return *this;
  rows = other.rows;
  symbols = other.symbols;
  *table = *other.table;
  return *this;
}

bool EvaluationTable::operator==(EvaluationTable &other) {
  if (this->rows != other.rows || this->symbols != other.symbols) {
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
    symbols.insert(column.first);
    if (column.second.size() != rows) {
      throw "Invalid: Length mismatch between symbols";
    }
  }
  this->table = table;
};

void EvaluationTable::hashMerge(EvaluationTable &other) {
  // Terminate if other table is empty
  if (other.noSymbols()) {
    return;
  }

  // Add results and terminate early if this table is empty
  if (noSymbols()) {
    for (auto &otherColumn : *other.table) {
      (*table)[otherColumn.first] = otherColumn.second;
    }
    symbols = other.symbols;
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
    if (build->contains(probeColumn.first)) {
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
      for (auto &symbol : build->symbols) {
        (*newTable)[symbol].push_back((*(build->table))[symbol][buildIndex]);
      }
      for (auto &symbol : uncommonSymbols) {
        (*newTable)[symbol].push_back((*(probe->table))[symbol][probeIndex]);
      }
    }
  }

  // Mark all symbols in this batch of results as symbols
  for (auto &column : (*other.table)) {
    symbols.insert(column.first);
  }

  // Complete operation by replacing the values table
  delete table;
  table = newTable;
  rows = newRows;
}

void EvaluationTable::merge(EvaluationTable &other) {
  // Terminate if other table is empty
  if (other.noSymbols()) {
    return;
  }

  // Add results and terminate early if table is empty
  if (noSymbols()) {
    for (auto &otherColumn : *other.table) {
      (*table)[otherColumn.first] = otherColumn.second;
    }
    symbols = other.symbols;
    rows = other.rows;
    return;
  }

  // Separate symbols into seen and unseen
  std::list<SYMBOL> seenSymbols;
  std::list<SYMBOL> unseenSymbols;
  for (auto &otherColumn : *other.table) {
    if (contains(otherColumn.first)) {
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
      // Check if values of symbols columns match
      bool isMatch = true;
      for (auto &seenSymbol : seenSymbols) {
        std::vector<VALUE> seenColumn = (*table)[seenSymbol];
        if (seenColumn[tableIndex] != (*other.table)[seenSymbol][otherIndex]) {
          isMatch = false;
          break;
        }
      }
      // Push cross product into new table if symbols columns match
      if (isMatch) {
        newRows += 1;
        for (auto &symbol : symbols) {
          (*newTable)[symbol].push_back((*table)[symbol][tableIndex]);
        }
        for (auto &unseenSymbol : unseenSymbols) {
          (*newTable)[unseenSymbol].push_back(
              (*other.table)[unseenSymbol][otherIndex]);
        }
      }
    }
  }

  // Mark all unseen symbols as symbols
  for (auto &symbol : unseenSymbols) {
    symbols.insert(symbol);
  }

  // Complete operation by replacing the values table
  delete table;
  table = newTable;
  rows = newRows;
}

bool EvaluationTable::contains(SYMBOL symbol) {
  return setContains(symbols, symbol);
}

bool EvaluationTable::contains(std::vector<SYMBOL> checkSymbols) {
  for (auto &symbol : checkSymbols) {
    if (!contains(symbol))
      return false;
  }
  return true;
}

bool EvaluationTable::noSymbols() { return symbols.empty(); }

EvaluationTable
EvaluationTable::sliceSymbols(std::unordered_set<SYMBOL> symbolsWanted) {
  std::vector<SYMBOL> order;
  for (auto &symbol : symbolsWanted) {
    if (contains(symbol)) {
      order.push_back(symbol);
    }
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

void EvaluationTable::flatten(std::vector<SYMBOL> toSymbols,
                              std::list<VALUE> &result) {
  // Filter down to selected toSymbols to purge duplicate rows
  if (toSymbols.size() < symbols.size()) {
    std::unordered_set<SYMBOL> symbolSet(toSymbols.begin(), toSymbols.end());
    EvaluationTable filtered = sliceSymbols(symbolSet);
    filtered.flatten(toSymbols, result);
    return;
  }

  if (!contains(toSymbols)) {
    throw "Invalid: Unable to flatten, provided symbol not present in table";
  }

  for (int index = 0; index < rowCount(); index++) {
    std::stringstream rowStream;
    for (auto &symbol : toSymbols) {
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
    if (!contains(symbol)) {
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
