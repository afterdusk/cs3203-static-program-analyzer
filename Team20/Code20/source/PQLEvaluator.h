#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQL.h"

typedef std::string SYMBOL;
typedef std::string VALUE;

/** @brief Represents a query result returned by the PkbTables
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
 *  to the PkbTables for evaluation.
 *  It serves as a bridge between the two components and
 *  encapsulates all dependencies on the PkbTables implementation
 *  and API.
 */
class ClauseDispatcher {
protected:
  typedef std::variant<Variable, Procedure, Constant, Underscore, LineNumber,
                       String, Statement, PatternSpec>
      PKB_PARAM;
  PkbQueryInterface *handler;
  std::vector<SYMBOL> synonyms;
  std::vector<PKB_PARAM> pkbParameters;

  /** @brief Initializes a ClauseDispatcher class with the given
   *  PkbQueryInterface object, where the queries will be dispatched to.
   *  This constructor is protected and can only to be called by
   *  derived classes.
   */
  ClauseDispatcher(PkbQueryInterface *queryHandler);

  /** @brief Converts a PQLToken to the appropriate PkbTables_PARAM.
   *  Note that tokens representing synonyms will have their
   *  symbols pushed into the synonyms vector.
   */
  PKB_PARAM toParam(PqlToken token);

  /** @brief Converts a STRING_SET returned from the PkbTables to a
   *  ClauseResult.
   */
  ClauseResult toClauseResult(STRING_SET &set);

  /** @brief Converts a STRING_PAIRS returned from the PkbTables to a
   *  ClauseResult.
   */
  ClauseResult toClauseResult(STRING_PAIRS &vectorPair);

public:
  /** @brief Virtual destructor required to delete instances of
   *  derived classes through a pointer to the base ClauseDispatcher
   *  class.
   */
  virtual ~ClauseDispatcher(){};

  /** @brief Creates a ClauseDispatcher from a token. This
   *  is used to query PkbTables for values of an entity not involved
   *  in any such that or pattern clause.
   */
  static ClauseDispatcher *FromToken(PqlToken token,
                                     PkbQueryInterface *queryHandler);

  /** @brief Creates a ClauseDispatcher from a ParsedRelationship.
   */
  static ClauseDispatcher *FromRelationship(ParsedRelationship pr,
                                            PkbQueryInterface *queryHandler);

  /** @brief Creates a ClauseDispatcher from a ParsedPattern.
   */
  static ClauseDispatcher *FromPattern(ParsedPattern pp,
                                       PkbQueryInterface *queryHandler);

  /** @brief Returns whether the clause will evaluate to a boolean
   *  result. For example, in the clause follows(1, 2).
   */
  bool willReturnBoolean();

  /** @brief Dispatches a query returning a boolean to the PkbTables. Should
   *  only be called when willReturnBoolean() is true. To be implemented by
   *  concrete subclass, else an error will be thrown.
   */
  virtual bool booleanDispatch();

  /** @brief Dispatches a query returning a ClauseResult to the PkbTables.
   *  Should only be called when willReturnBoolean() is false. To be implemented
   *  by concrete subclass, else an error will be thrown.
   */
  virtual ClauseResult resultDispatch();
};

/** @brief Represents the table of possible query results.
 *  As clauses are evaluated, values from the PkbTables are pushed into
 *  this table.
 */
class EvaluationTable {
private:
  typedef std::unordered_map<SYMBOL, std::vector<VALUE>> TABLE;
  std::unordered_set<SYMBOL> seen;
  TABLE table;
  int rows;

public:
  /** @brief Instantiate an empty EvaluationTable
   */
  EvaluationTable() : rows(0){};

  /** @brief Adds the results of a PkbTables query to the EvaluationTable.
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
