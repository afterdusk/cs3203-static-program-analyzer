#include "DispatcherGraph.h"
#include <assert.h>
#include <functional>
#include <iostream>
#include <queue>

bool DispatcherGraph::Edge::operator==(
    const DispatcherGraph::Edge &other) const {
  return neighbour == other.neighbour && weight == other.weight;
}

void DispatcherGraph::addDispatcher(ClauseDispatcher *dispatcher) {
  if (contains(dispatcher)) {
    throw "Error: dispatcher already exists in graph";
  }
  if (!symbols.empty()) {
    ensureHasOneCommonSymbolWith(dispatcher);
  }
  adjacencyList[dispatcher] = EDGES();
  for (const auto &symbol : dispatcher->getSymbols()) {
    if (!contains(symbol)) {
      symbols[symbol] = CLAUSE_DISPATCHER_SET();
    }
    for (const auto &associatedDispatcher : symbols[symbol]) {
      // TODO: get weight
      int weight = 0;
      adjacencyList.at(dispatcher)
          .insert(Edge{
              associatedDispatcher,
              // TODO: GET WEIGHT
              weight,
          });

      adjacencyList.at(associatedDispatcher)
          .insert(Edge{
              dispatcher,
              // TODO: GET WEIGHT
              weight,
          });
    }
    symbols.at(symbol).insert(dispatcher);
  }
}

void DispatcherGraph::ensureSymbolsNotContainedInOtherGraph(
    const DispatcherGraph *otherGraph) const {
  for (const auto &symbolPair : symbols) {
    if (otherGraph->contains(symbolPair.first)) {
      throw "ERROR: There exists common symbol between the two graphs";
    }
  }
}

void DispatcherGraph::ensureHasOneCommonSymbolWith(
    ClauseDispatcher *dispatcher) const {
  for (const auto &symbol : dispatcher->getSymbols()) {
    if (mapContains(symbols, symbol)) {
      return;
    }
  }
  throw "ERROR: There exists no common symbol between graph and dispatcher";
}

void DispatcherGraph::merge(DispatcherGraph &otherGraph,
                            ClauseDispatcher *dispatcher) {
  ensureSymbolsNotContainedInOtherGraph(&otherGraph);
  otherGraph.ensureSymbolsNotContainedInOtherGraph(this);
  ensureHasOneCommonSymbolWith(dispatcher);
  otherGraph.ensureHasOneCommonSymbolWith(dispatcher);

  for (const auto &[clause, edges] : otherGraph.adjacencyList) {
    adjacencyList[clause].insert(edges.begin(), edges.end());
  }
  for (const auto &[symbol, clauses] : otherGraph.symbols) {
    symbols[symbol].insert(clauses.begin(), clauses.end());
  }
  addDispatcher(dispatcher);
}

bool DispatcherGraph::contains(SYMBOL symbol) const {
  return mapContains(symbols, symbol);
}

bool DispatcherGraph::contains(ClauseDispatcher *clauseDispatcher) const {
  return mapContains(adjacencyList, clauseDispatcher);
}

void DispatcherGraph::deallocatePointers() {
  for (const auto &symbolDispatchersPair : symbols) {
    const auto pointers = symbolDispatchersPair.second;
    for (auto it : pointers) {
      delete it;
    }
  }
}

EvaluationTable DispatcherGraph::evaluate() {
  typedef std::pair<int, ClauseDispatcher *> PQ_NODE;
  auto compare = [](PQ_NODE &node1, PQ_NODE &node2) {
    return node1.first < node2.first;
  };
  std::priority_queue<PQ_NODE, std::vector<PQ_NODE>,
                      std::function<bool(PQ_NODE &, PQ_NODE &)>>
      pq(compare);

  std::unordered_set<ClauseDispatcher *> evaluated;
  EvaluationTable table;

  // 1. Add starting node
  for (const auto &nodeEdgesPair : adjacencyList) {
    // TODO: Add node with smallest weight
    const auto &node = nodeEdgesPair.first;
    pq.push(PQ_NODE{
        0,
        node,
    });
    break;
  }

  // 2. Iterate through priority queue
  while (!pq.empty()) {
    const auto &topValue = pq.top();
    pq.pop();
    ClauseDispatcher *dispatcher = topValue.second;
    if (setContains(evaluated, dispatcher)) {
      continue;
    }
    table.hashMerge(dispatcher->resultDispatch());
    if (table.rowCount() == 0) {
      deallocatePointers();
      return table;
    }
    for (const auto &e : adjacencyList.at(dispatcher)) {
      // TODO: Add neighbour's weight
      int pqWeight = e.weight;
      pq.push(PQ_NODE{pqWeight, e.neighbour});
    }
  }
  deallocatePointers();
  return table;
};

bool DispatcherGraph::operator==(const DispatcherGraph &other) const {
  return adjacencyList == other.adjacencyList && symbols == other.symbols;
}
