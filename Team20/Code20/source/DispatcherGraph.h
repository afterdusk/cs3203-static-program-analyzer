#pragma once
#include "Dispatchers.h"
#include "Pql.h"

/** @brief `DispatcherGraph` encapsulates the relationship between
 * `ClauseDispatcher`s. In this graph, a `ClauseDispatcher` is represented as a
 * node, and it forms an edge with every other `ClauseDispatcher` that shares a
 * common symbol. Each graph is a strongly connected graph, which means there
 * exists a path from a `ClauseDispatcher` to any other `ClauseDispatcher`.
 */
struct DispatcherGraph {
public:
  struct Edge {
    ClauseDispatcher *neighbour;
    int weight;
    bool operator==(const Edge &other) const;
  };

  /** @brief Add dispatcher adds a clause dispatcher to the graph. Throws an
   * exception if clause dispatcher is not the first clause dispatcher and it
   * does not share a common symbol with any other clause dispatcher in the
   * graph.
   */
  void addDispatcher(ClauseDispatcher *dispatcher);

  /** @brief Merges with another `DispatcherGraph`, and adds the
   * `ClauseDispatchers` to the current graph in-place. Throws an exception if
   * there are common symbols between the two graphs. Throws an error if the
   * connecting dispatcher does not contain any common symbol with any of the
   * other graphs.
   *  @param otherGraph otherGraph other graph which does not have common
   * symbols with this graph.
   *  @param dispatcher dispatcher that connected the two disjoint graphs.
   */
  void merge(DispatcherGraph &otherGraph, ClauseDispatcher *dispatcher);

  /** @brief Returns true if symbol is contained in the current graph.
   */
  bool contains(SYMBOL) const;

  /** @brief Returns true if clause dispatcher is contained in the current
   * graph.
   */
  bool contains(ClauseDispatcher *clauseDispatcher) const;

  /** @brief Gets the sum of priority values of dispatchers contained in the
   * current graph.
   *  @return priority value
   */
  int priority();

  /** @brief Evaluates the `ClauseDispatcher`s in an efficient manner.
   * Clauses are evaluated based on their estimated ease of evaluation and their
   * common synonyms with clauses that have already already evaluated.
   *  @param selectedSymbols symbols that are selected in the query
   *  @return evaluation table that is the result from evaluating the clauses.
   */
  EvaluationTable evaluate(std::unordered_set<SYMBOL> selectedSymbols);

  bool DispatcherGraph::operator==(const DispatcherGraph &other) const;

private:
  typedef std::unordered_set<ClauseDispatcher *> CLAUSE_DISPATCHER_SET;
  typedef std::unordered_map<SYMBOL, CLAUSE_DISPATCHER_SET> SYMBOLS_MAP;
  typedef std::unordered_set<Edge> EDGES;
  typedef std::unordered_map<ClauseDispatcher *, EDGES> ADJACACENCY_LIST;
  typedef std::unordered_set<SYMBOL> SYMBOL_SET;
  ADJACACENCY_LIST adjacencyList;
  SYMBOLS_MAP symbolsToClauseDispatchersMap;
  SYMBOL_SET symbols;
  int totalPriority = 0;

  int countCommonSymbols(ClauseDispatcher *first, ClauseDispatcher *second);
  void ensureSymbolsNotContainedInOtherGraph(
      const DispatcherGraph *otherGraph) const;
  void ensureHasOneCommonSymbolWith(ClauseDispatcher *dispatcher) const;
};

namespace std {
template <> struct hash<DispatcherGraph::Edge> {
  size_t operator()(const DispatcherGraph::Edge &obj) const {
    return hash<ClauseDispatcher *>()(obj.neighbour);
  }
};
} // namespace std
