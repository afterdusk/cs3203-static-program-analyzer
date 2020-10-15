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
typedef std::string ROW_HASH;

constexpr auto TRUE_RESULT = "TRUE";
constexpr auto FALSE_RESULT = "FALSE";

/** @brief Local helper method for converting an element with an attribute
 *  to a symbol (string).
 *  @return an optional containing a symbol if element has an attribute, else
 *  an empty optional.
 */
std::optional<SYMBOL> elementAttrToSymbol(TokenType type, Element element);

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

  /** @brief Creates a EvaluationTable with symbols (either synonyms or
   *  synonym attributes) and their corresponding values.
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
  EvaluationTable(TABLE *table);

  /** @brief Merges in the values from another EvaluationTable.
   */
  void merge(EvaluationTable &other);

  /** @brief Returns whether the symbol is seen.
   */
  bool isSeen(SYMBOL symbol);

  /** @brief Returns whether the symbols are seen.
   */
  bool areSeen(std::vector<SYMBOL> symbols);

  /** @brief Returns whether the table is empty.
   *  NOTE: An empty table is one that has not seen
   *  any symbols. A table with seen symbols but no
   *  values for these symbols is NOT empty.
   */
  bool empty();

  /** @brief Returns a new instance of an EvaluationTable
   *  containing only the symbols passed in the vector.
   *  Throws an error if symbol is not present in table.
   */
  EvaluationTable slice(std::vector<SYMBOL> symbols);

  /** @brief Flattens the values of the elements provided
   *  into a list of strings.
   */
  void flatten(DECLARATIONS &declarations, TUPLE &selected,
               std::list<VALUE> &result);

  /** @brief Helper method that returns a crude "hash" of a row,
   *  created by concatenating the values of a row according to
   *  a provided order and delimiting by an illegal character.
   *  Can provide hash for subset of table's symbols.
   */
  ROW_HASH rowHash(int index, std::vector<SYMBOL> &order);

  /** @brief Returns the number of rows in the evaluation table.
   */
  int rowCount();

  /** @brief Debug/test method that retrieves the possible
   *  values of a symbol.
   */
  std::unordered_set<VALUE> select(SYMBOL symbol);
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
  std::vector<SYMBOL> symbols;
  std::vector<PKB_PARAM> pkbParameters;

  /** @brief Initializes a ClauseDispatcher class with the given
   *  PkbQueryInterface object, where the queries will be dispatched to.
   *  This constructor is protected and can only to be called by
   *  derived classes.
   */
  ClauseDispatcher(PkbQueryInterface *queryHandler);

  /** @brief Helper method that converts a PKB line number to string
   *  representation.
   */
  std::string toString(PkbTables::LINE_NO lineNumber);

  /** @brief Helper method that converts a line number from string
   *  representation to PKB's representation.
   */
  PkbTables::LINE_NO toLineNumber(std::string lineNumberStr);

  /** @brief Converts a PQLToken to the appropriate PkbTables_PARAM.
   *  Note that tokens representing symbols will have their
   *  symbols pushed into the symbols vector.
   */
  PKB_PARAM toParam(PqlToken token);

  /** @brief Converts a NAME_SET returned from the PkbTables to a
   *  EvaluationTable.
   */
  EvaluationTable toEvaluationTable(NAME_SET &set);

  /** @brief Converts a LINE_SET returned from the PkbTables to a
   *  EvaluationTable.
   */
  EvaluationTable toEvaluationTable(LINE_SET &set);

  /** @brief Converts a NAME_NAME_PAIRS returned from the PkbTables to a
   *  EvaluationTable.
   */
  EvaluationTable toEvaluationTable(NAME_NAME_PAIRS &pairs);

  /** @brief Converts a LINE_LINE_PAIRS returned from the PkbTables to a
   *  EvaluationTable.
   */
  EvaluationTable toEvaluationTable(LINE_LINE_PAIRS &pairs);

  /** @brief Converts a LINE_NAME_PAIRS returned from the PkbTables to a
   *  EvaluationTable.
   */
  EvaluationTable toEvaluationTable(LINE_NAME_PAIRS &pairs);

public:
  /** @brief Virtual destructor required to delete instances of
   *  derived classes through a pointer to the base ClauseDispatcher
   *  class.
   */
  virtual ~ClauseDispatcher(){};

  /** @brief Creates a ClauseDispatcher from an Element. This
   *  is used to query PkbTables for values of an entity/entity's attribute
   *  not involved in any such that, pattern or with clause.
   */
  static ClauseDispatcher *FromElement(TokenType type, Element element,
                                       PkbQueryInterface *queryHandler);

  /** @brief Creates a ClauseDispatcher from a ParsedRelationship.
   */
  static ClauseDispatcher *FromRelationship(ParsedRelationship pr,
                                            PkbQueryInterface *queryHandler);

  /** @brief Creates a ClauseDispatcher from a ParsedPattern.
   */
  static ClauseDispatcher *FromPattern(ParsedPattern pp,
                                       PkbQueryInterface *queryHandler);

  /** @brief Creates a ClauseDispatcher from a with clause (represented as
   * a pair of References).
   */
  static ClauseDispatcher *
  FromWith(std::pair<Reference, Reference> pw,
           std::unordered_map<std::string, TokenType> declarations,
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

  /** @brief Dispatches a query returning a EvaluationTable to the PkbTables.
   *  Should only be called when willReturnBoolean() is false. To be implemented
   *  by concrete subclass, else an error will be thrown.
   */
  virtual EvaluationTable resultDispatch();
};
