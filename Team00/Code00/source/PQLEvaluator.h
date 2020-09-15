#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQL.h"

typedef std::string SYMBOL;
typedef std::string VALUE;

/** @brief Represents a query result returned by the Pkb
 *  for a particular clause.
 */
class ClauseResult {
private:
  std::unordered_map<SYMBOL, std::vector<VALUE>> values;

public:
  /** @brief Creates a ClauseResult with synonyms and their
   *  corresponding result values.
   *
   *  e.g. Query returns {"a", "s"} = {{"1", "2"}, {"3", "4"}}
   *
   *  A ClauseResult of this query can be instantiated as such:
   *
   *  ClauseResult({
   *    {"a", {"1", "3"}},
   *    {"s", {"2", "4"}}
   *  })
   */
  ClauseResult(std::unordered_map<SYMBOL, std::vector<VALUE>> values);

  /** @brief Gets the synonym the result is for.
   */
  std::vector<SYMBOL> synonyms();

  /** @brief Gets the values for a particular synonym.
   */
  std::vector<VALUE> valuesOf(SYMBOL synonym);

  /** @brief Gets the value at the ith position for a specific
   *  synonym.
   */
  VALUE valueAt(SYMBOL synonym, int i);

  /** @brief Gets the number of results.
   */
  int size();

  /** @brief Compares a ClauseResult with another.
   */
  bool operator==(ClauseResult &other);
};

/** @brief This class represents a PQL clause dispatchable
 *  to the Pkb for evaluation.
 *  It serves as a bridge between the two components and
 *  encapsulates all dependencies on the Pkb implementation
 *  and API.
 */
class ClauseDispatcher {
private:
  typedef std::variant<Variable, Procedure, Constant, Underscore, LineNumber,
                       String, Statement, PatternSpec>
      PKB_PARAM;
  PkbQueryInterface &handler;
  std::optional<TokenType> maybeRelationship;
  std::vector<SYMBOL> synonyms;
  std::vector<PKB_PARAM> pkbParameters;

  /** @brief Converts a PQLToken to the appropriate Pkb_PARAM.
   *  Note that tokens representing synonyms will have their
   *  symbols pushed into the synonyms vector.
   */
  PKB_PARAM toParam(PqlToken token);

  /** @brief Converts a STRING_SET returned from the Pkb to a
   *  ClauseResult.
   */
  ClauseResult toClauseResult(STRING_SET &set);

  /** @brief Converts a STRING_VECTOR returned from the Pkb to a
   *  ClauseResult.
   */
  ClauseResult toClauseResult(STRING_VECTOR &vector);

  /** @brief Converts a STRING_PAIRS returned from the Pkb to a
   *  ClauseResult.
   */
  ClauseResult toClauseResult(STRING_PAIRS &vectorPair);

public:
  /** @brief Creates a ClauseDispatcher from a token. This
   *  is used to query Pkb for values of an entity not involved
   *  in any such that or pattern clause.
   */
  ClauseDispatcher(PqlToken token, PkbQueryInterface &queryHandler);

  /** @brief Creates a ClauseDispatcher from a ParsedRelationship.
   */
  ClauseDispatcher(ParsedRelationship parsedRelationship,
                   PkbQueryInterface &queryHandler);

  /** @brief Creates a ClauseDispatcher from a ParsedPattern.
   */
  ClauseDispatcher(ParsedPattern parsedPattern,
                   PkbQueryInterface &queryHandler);

  /** @brief Returns whether the clause will evaluate to a boolean
   *  result. For example, in the clause follows(1, 2).
   */
  bool willReturnBoolean();

  /** @brief Dispatches a query returning a boolean to the Pkb. Should
   *  only be called when willReturnBoolean() is true.
   */
  bool booleanDispatch();

  /** @brief Dispatches a query returning a ClauseResult to the Pkb.
   *  Should only be called when willReturnBoolean() is false.
   */
  ClauseResult resultDispatch();
};

/** @brief Represents the table of possible query results.
 *  As clauses are evaluated, values from the Pkb are pushed into
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
  EvaluationTable(std::vector<SYMBOL> declared);

  /** @brief Adds the results of a Pkb query to the EvaluationTable.
   */
  void add(ClauseResult &clauseResult);

  /** @brief Returns whether the synonym is seen.
   */
  bool isSeen(SYMBOL synonym);

  /** @brief Returns whether the table is empty.
   *  NOTE: An empty table is one that has not seen
   *  any synonyms. A table with seen synonyms but no
   *  values for these synonyms is NOT empty.
   */
  bool empty();

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
