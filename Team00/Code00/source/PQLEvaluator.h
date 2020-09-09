#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQL.h"

typedef std::string SYMBOL;
typedef std::string VALUE;

/** @brief Evaluates a parsed query object.
 *  This function serves as the entrypoint for the PQLEvaluator.
 */
std::unordered_set<VALUE> evaluateParsedQuery(ParsedQuery pq);

/** @brief Represents a query result returned by the PKB
 *  for a particular synonym.
 */
class ClauseResult {
private:
  SYMBOL synonym;
  std::vector<VALUE> values;

public:
  /** @brief Creates a ClauseResult with a synonym and the
   *  corresponding result values.
   *
   *  e.g. Query returns {"a", "s"} = {{"1", "2"}, {"3", "4"}}
   *
   *  The result is represented with two ClauseResult objects:
   *
   *  ClauseResult("a", {"1", "3"})
   *  ClauseResult("s", {"2", "4"})
   *
   *  Thus, note that the order of values matter.
   */
  ClauseResult(SYMBOL synonym, std::vector<VALUE> values);

  /** @brief Gets the synonym the result is for.
   */
  SYMBOL getSynonym();

  /** @brief Gets the ordered values in the result.
   */
  std::vector<VALUE> getValues();

  /** @brief Gets the value at the ith position.
   */
  VALUE getValueAt(int i);

  /** @brief Gets the total number of values.
   */
  int getLength();
};

typedef std::vector<ClauseResult> CLAUSE_RESULTS;

/** @brief Represents the table of possible query results.
 *  As clauses are evaluated, values from the PKB are pushed into
 *  this table.
 */
class EvaluationTable {
private:
  std::unordered_set<SYMBOL> synonyms;
  std::unordered_set<SYMBOL> seen;
  std::unordered_map<SYMBOL, std::vector<VALUE>> values;
  int rows;

  /** @brief Generates a map of synonym keys to empty value lists
   */
  std::unordered_map<SYMBOL, std::vector<VALUE>>
  generateValuesMap(std::unordered_set<SYMBOL> synonyms);

public:
  /** @brief Instantiate an EvaluationTable for a list of declared
   *  synonyms.
   */
  EvaluationTable(std::vector<SYMBOL> declaredSynonyms);

  /** @brief Adds the results of a PKB query to the EvaluationTable.
   */
  void add(CLAUSE_RESULTS clauseResults);

  /** @brief Retrieves the possible values of a synonym.
   */
  std::unordered_set<VALUE> select(SYMBOL synonym);

  /** @brief Retrieves the possible values of a synonym, in the
   *  order that is stored in the evaluation table.
   *  This function is primarily used for debugging and testing
   *  purposes. Duplicates are not filtered.
   */
  std::vector<VALUE> selectColumn(SYMBOL synonym);

  /** @brief Returns the number of rows in the evaluation table.
   */
  int rowCount();
};
