#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQL.h"

typedef std::string SYMBOL;
typedef std::string VALUE;
typedef std::unordered_map<SYMBOL, std::vector<VALUE>> TABLE;

/** @brief Represents the table of possible query results.
 *  As clauses are evaluated, values from the PkbTables are pushed into
 *  this table.
 */
class EvaluationTable {
private:
  std::unordered_set<SYMBOL> seen;
  TABLE *table;
  int rows;

public:
  EvaluationTable();

  EvaluationTable(const EvaluationTable &other);

  ~EvaluationTable();

  EvaluationTable &operator=(const EvaluationTable &other);

  bool operator==(EvaluationTable &other);

  /** @brief Creates a EvaluationTable with synonyms and their
   *  corresponding values.
   *
   *  e.g. {"a", "s"} = {{"1", "2"}, {"3", "4"}}
   *
   *  A EvaluationTable of this query can be instantiated as such:
   *
   *  EvaluationTable(new TABLE({
   *    {"a", {"1", "3"}},
   *    {"s", {"2", "4"}}
   *  }))
   */
  EvaluationTable(TABLE *values);

  /** @brief Merges in the values from another EvaluationTable.
   */
  void merge(EvaluationTable &other);

  /** @brief Returns whether the synonym is seen.
   */
  bool isSeen(SYMBOL synonym);

  /** @brief Returns whether the synonyms are seen.
   */
  bool areSeen(std::vector<SYMBOL> synonyms);

  /** @brief Returns whether the table is empty.
   *  NOTE: An empty table is one that has not seen
   *  any synonyms. A table with seen synonyms but no
   *  values for these synonyms is NOT empty.
   */
  bool empty();

  /** @brief Returns a new instance of an EvaluationTable
   *  containing only the synonyms passed in the vector.
   *  Throws an error if synonym is not present in table.
   */
  EvaluationTable slice(std::vector<SYMBOL> synonyms);

  /** @brief Flattens the values of the synonyms provided
   *  into a list of strings.
   */
  void flatten(std::vector<VALUE> synonyms, std::list<VALUE> &result);

  /** @brief Helper method that returns a crude "hash" of a row,
   *  created by concatenating the values of a row according to
   *  a provided order and delimiting by an illegal character.
   *  Can provide hash for subset of table's synonyms.
   */
  std::string rowHash(int index, std::vector<SYMBOL> order);

  /** @brief Returns the number of rows in the evaluation table.
   */
  int rowCount();

  /** @brief Debug/test method that retrieves the possible
   *  values of a synonym.
   */
  std::unordered_set<VALUE> select(SYMBOL synonym);
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
  EvaluationTable toEvaluationTable(STRING_SET &set);

  /** @brief Converts a STRING_PAIRS returned from the PkbTables to a
   *  ClauseResult.
   */
  EvaluationTable toEvaluationTable(STRING_PAIRS &vectorPair);

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
  virtual EvaluationTable resultDispatch();
};
