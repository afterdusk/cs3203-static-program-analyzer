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
      int weight = countCommonSymbols(dispatcher, associatedDispatcher) * 2;
      adjacencyList.at(dispatcher)
          .insert(Edge{
              associatedDispatcher,
              weight,
          });

      adjacencyList.at(associatedDispatcher)
          .insert(Edge{
              dispatcher,
              weight,
          });
    }
    symbols.at(symbol).insert(dispatcher);
  }
  totalPriority += dispatcher->dispatchPriority();
}

int DispatcherGraph::countCommonSymbols(ClauseDispatcher *first,
                                        ClauseDispatcher *second) {
  int count = 0;
  for (auto &firstSymbol : first->getSymbols()) {
    for (auto &secondSymbol : second->getSymbols()) {
      count = firstSymbol == secondSymbol ? count + 1 : count;
    }
  }
  return count;
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
  totalPriority += otherGraph.totalPriority;
  addDispatcher(dispatcher);
}

bool DispatcherGraph::contains(SYMBOL symbol) const {
  return mapContains(symbols, symbol);
}

bool DispatcherGraph::contains(ClauseDispatcher *clauseDispatcher) const {
  return mapContains(adjacencyList, clauseDispatcher);
}

int DispatcherGraph::priority() { return totalPriority; }

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
  ClauseDispatcher *lightest = NULL;
  for (const auto &[clause, edges] : adjacencyList) {
    if (lightest == NULL ||
        clause->dispatchPriority() < lightest->dispatchPriority()) {
      lightest = clause;
    }
  }
  pq.push(PQ_NODE{
      lightest->dispatchPriority(),
      lightest,
  });

  // 2. Iterate through priority queue
  while (!pq.empty()) {
    // do not use reference, as top of queue changes after popping
    const auto topValue = pq.top();
    pq.pop();
    ClauseDispatcher *dispatcher = topValue.second;
    if (setContains(evaluated, dispatcher)) {
      continue;
    }
    table.hashMerge(dispatcher->resultDispatch());
    if (table.rowCount() == 0) {
      return table;
    }
    for (const auto &e : adjacencyList.at(dispatcher)) {
      int pqWeight = e.weight + e.neighbour->dispatchPriority();
      pq.push(PQ_NODE{pqWeight, e.neighbour});
    }
    evaluated.insert(dispatcher);
  }
  return table;
};

bool DispatcherGraph::operator==(const DispatcherGraph &other) const {
  return adjacencyList == other.adjacencyList && symbols == other.symbols;
}
