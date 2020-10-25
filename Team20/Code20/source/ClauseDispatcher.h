#pragma once

#include "PqlEvaluator.h"

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
  int cachedPriority = -1;

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

  /** @return The symbols in the clause
   */
  std::vector<SYMBOL> getSymbols();

  /** @return Whether the clause will evaluate to a boolean
   *  result. For example, in the clause follows(1, 2).
   */
  virtual bool willReturnBoolean();

  /** @brief Assigns a priority value to the clause represented by the
   *  dispatcher. Priority is assigned based on static approximates of
   *  execution time and number of results.
   *  @return A non-negative priority value (lower is higher priority)
   */
  virtual int dispatchPriority();

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
