#pragma once

#include "PkbQueryEntityTypes.h"
#include <string>
#include <utility>
#include <vector>

typedef std::vector<PkbTables::LINE_NO> LINE_VECTOR;
typedef std::vector<std::string> NAME_VECTOR;
typedef std::unordered_set<PkbTables::LINE_NO> LINE_SET;
typedef std::unordered_set<std::string> NAME_SET;
typedef std::unordered_set<std::string> CONST_SET;
typedef std::pair<LINE_VECTOR, NAME_VECTOR> LINE_NAME_PAIRS;
typedef std::pair<LINE_VECTOR, LINE_VECTOR> LINE_LINE_PAIRS;
typedef std::pair<NAME_VECTOR, NAME_VECTOR> NAME_NAME_PAIRS;

class PkbQueryInterface {
protected:
  // Derived tables using original tables from PkbTables
  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NO>
      prevLineTable; // invert of followTable
  KeysTable<PkbTables::LINE_NO, LINE_SET>
      childrenTable; // pseudo invert of parentTable
  KeysTable<PkbTables::StatementType, LINE_SET>
      invertStatementTypeTable; // pseudo invert of statementTypeTable
  KeysTable<PkbTables::PROC, LINE_SET>
      invertStatementProcTable; // pseudo invert of statementProcTable
  KeysTable<PkbTables::CALL, PkbTables::PROCS>
      invertCallsTable; // pseudo invert of callsTable
  KeysTable<PkbTables::NEXT, PkbTables::LINE_NOS>
      invertNextsTable; // pseudo invert of nextsTable

  KeysTable<PkbTables::LINE_NO, LINE_SET> closeFollowTable;
  KeysTable<PkbTables::LINE_NO, LINE_SET> closeParentTable;
  KeysTable<PkbTables::LINE_NO, LINE_SET> closePrevLineTable;
  KeysTable<PkbTables::LINE_NO, LINE_SET> closeChildrenTable;
  KeysTable<PkbTables::PROC, PkbTables::CALLS> closeCallsTable;
  KeysTable<PkbTables::CALL, PkbTables::PROCS> closeInvertCallsTable;

  KeysTable<PkbTables::LINE_NO, PkbTables::VARS> usesTableTransited;
  KeysTable<PkbTables::VAR, PkbTables::LINE_NOS> invertUsesTable;
  KeysTable<PkbTables::LINE_NO, PkbTables::VARS> modifiesTableTransited;
  KeysTable<PkbTables::VAR, PkbTables::LINE_NOS> invertModifiesTable;

  LINE_SET stmtTableIndexes;
  LINE_SET followTableIndexes;
  LINE_SET parentTableIndexes;
  LINE_SET prevLineTableIndexes;
  LINE_SET childrenTableIndexes;
  LINE_SET nextTableIndexes;
  LINE_SET invertNextsTableIndexes;

  NAME_SET callsTableIndexesProcNames;
  NAME_SET invertCallsTableIndexesProcNames;

  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS> affectsBipTable;
  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS> invertAffectsBipTable;
  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS> closeAffectsBipTable;
  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS> closeInvertAffectsBipTable;
  bool areAllAffectsBipRelatedTablesDerived = false;
  bool areAllCloseAffectsBipRelatedTablesDerived = false;

  // cached tables
  KeysTable<PkbTables::LINE_NO, PkbTables::NEXTS> closeNextsTableCache;
  KeysTable<PkbTables::NEXT, PkbTables::LINE_NOS> closeInvertNextsTableCache;
  KeysTable<PkbTables::ASSIGNMENT, PkbTables::LINE_NOS> affectsTableCache;
  KeysTable<PkbTables::ASSIGNMENT, PkbTables::LINE_NOS> closeAffectsTableCache;
  KeysTable<PkbTables::ASSIGNMENT, PkbTables::LINE_NOS> invertAffectsTableCache;
  KeysTable<PkbTables::ASSIGNMENT, PkbTables::LINE_NOS>
      closeInvertAffectsTableCache;
  bool isCloseNextsTableCached = false;
  bool isCloseInvertNextsTableCached = false;
  bool isAffectsTableCached = false;
  bool isCloseAffectsTableCached = false;
  bool isInvertAffectsTableCached = false;
  bool isCloseInvertAffectsTableCached = false;

  /** @brief Retrieves line numbers of statements that are transitively next of
   * the input line number.
   *  @param lineNo A line number of a statement.
   *  @param lineNosVisited An empty unordered set.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET
  getTransitiveNextStatements(PkbTables::LINE_NO lineNo,
                              PkbTables::LINE_NOS lineNosVisited) = 0;

  /** @brief Retrieves line numbers of statements that are transitively previous
   * of the input line number.
   *  @param lineNo A line number of a statement.
   *  @param lineNosVisited An empty unordered set.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET
  getTransitivePrevStatements(PkbTables::LINE_NO lineNo,
                              PkbTables::LINE_NOS lineNosVisited) = 0;

  /** @brief Retrieves line numbers of assignment statements that are affected
   * by the assignment statement of input line number.
   *  @param lineNo A line number of a statement.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET getAffectedStatements(PkbTables::LINE_NO lineNo) = 0;

  /** @brief Auxiliary function of getAffectedStatements, that collects
   * then returns all assignment statements affected by modifiedVar.
   * @param modifiedVar Variable that affects collected assignment statements.
   * @param lineNo A line number of a statement, possibly affected by
   * modifiedVar.
   * @return A set of SIMPLE source line numbers of statements affected by
   * modifiedVar.
   */
  virtual LINE_SET getAffectedAux(PkbTables::VAR modifiedVar,
                                  PkbTables::LINE_NO lineNo,
                                  PkbTables::LINE_NOS lineNosVisited) = 0;

  /** @brief Retrieves line numbers of assignment statements that affects the
   * assignment statement of input line number.
   *  @param lineNo A line number of a statement.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET getAffectorStatements(PkbTables::LINE_NO lineNo) = 0;

  /** @brief Auxiliary function of getAffectorStatements, that collects
   * then returns all assignment statements that modifies the usedVar.
   * @param usedVar Variable that is used by the affected assignment statement.
   * @param lineNo A line number of a statement, possibly affects the assignment
   * statement using the usedVar.
   * @return A set of SIMPLE source line numbers of assignment statements that
   * modifies the usedVar.
   */
  virtual LINE_SET getAffectorAux(PkbTables::VAR usedVar,
                                  PkbTables::LINE_NO lineNo,
                                  PkbTables::LINE_NOS lineNosVisited) = 0;

  /** @brief Retrieves line numbers of assignment statements that are
   * transitively affected by the assignment statement of the input line
   * number.
   *  @param lineNo A line number of an assignment statement.
   *  @param lineNosVisited An empty unordered set.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET
  getTransitiveAffectedStatements(PkbTables::LINE_NO lineNo,
                                  PkbTables::LINE_NOS lineNosVisited) = 0;

  /** @brief Retrieves line numbers of assignment statements that transitively
   * affects the assignment statement of the input line number.
   *  @param lineNo A line number of an assignment statement.
   *  @param lineNosVisited An empty unordered set.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET
  getTransitiveAffectorStatements(PkbTables::LINE_NO lineNo,
                                  PkbTables::LINE_NOS lineNosVisited) = 0;

  /** @brief Checks whether there is a control flow path from the statement of
   * input line number to the end of its procedure where the input variable is
   * not modified.
   *  @param lineNo A line number of a statement.
   *  @param var The variable to be checked by the control flow path.
   *  @param lineNosVisited An empty unordered set.
   *  @return A boolean on whether there exists a control flow path where the
   * input line number is able to reach the end of its procedure without
   * modifying the input variable.
   */
  virtual bool checkReachLastStmtInProc(PkbTables::LINE_NO line,
                                        PkbTables::VAR var,
                                        PkbTables::LINE_NOS linesVisited) = 0;

  /** @brief Retrieves line numbers of assignment statements that are
   * affectedBip by the assignment statement of input line number.
   *  @param lineNo A line number of a statement.
   *  @param modifiedVar A variable modified by the assignment statement.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET getAffectedBipStatements(PkbTables::LINE_NO lineNo,
                                            PkbTables::VAR modifiedVar) = 0;

  /** @brief Auxiliary function of getAffectedBipStatements, that collects
   * then returns all assignment statements that uses the modifiedVar going
   * across procedures as well.
   * @param modifiedVar Variable that is modified by the initial assignment
   * statement.
   * @param lineNo A line number of a statement, that possibly uses the
   * modifiedVar.
   * @param lineNosVisited An empty unordered set.
   * @return A set of SIMPLE source line numbers of assignment statements that
   * uses the modifiedVar.
   */
  virtual LINE_SET getAffectedBipAux(PkbTables::VAR modifiedVar,
                                     PkbTables::LINE_NO lineNo,
                                     PkbTables::LINE_NOS lineNosVisited) = 0;

  /** @brief Derive affectsBip table through the use of getAffectedBipStatements
   * on every assignment statement in the SIMPLE source.
   *  @return The derived affectsBipTable mapping line numbers of assignments a
   * set of line numbers of assignment statements.
   */
  virtual KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS>
  deriveAffectsBipTable() = 0;

  /** @brief Derive all affectsBip related tables to be used in handling
   * affectsBip APIs.
   */
  virtual void deriveAllAffectsBipRelatedTables() = 0;

  /** @brief Derive all closeAffectsBip related tables to be used in handling
   * affectsBipStar APIs.
   */
  virtual void deriveAllCloseAffectsBipRelatedTables() = 0;

public:
  /** @brief Clears the cache tables used when nextStar, affects and
   *  affectsStar API calls are made.
   */
  virtual void clearCache() = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  along with the variable or procedure of the statement. For example if
   *  statement type is Print, then return pairs of Print statement line numbers
   *  matched with the variable printed. If statement type is Call, then return
   *  pairs of Call statement line numbers matched with procedure called. Only
   *  applicable for statement types: Print, Read and Call.
   *  @param statement Statement with a specified type.
   *  @return A pair of vectors with first vector containing line numbers and
   *  second vector containing variable or procedure names associated to the
   *  line numbers.
   */
  virtual LINE_NAME_PAIRS selectAttribute(Statement statement) = 0;

  /*
   * Query API for pattern
   */

  /** @brief Retrieves line numbers of statements of specified statement type
   *  and LHS variables associated with statements that are matched according
   *  to the type and RHS value specified in spec.
   *  @param statement Statement with a specified type.
   *  @param variable Empty Variable struct.
   *  @param spec PatternSpec with a specified type and value.
   *  @return A pair of vectors with first vector containing line numbers and
   *  second vector containing LHS variable names associated to the line
   *  numbers.
   */
  virtual LINE_NAME_PAIRS match(Statement statement, Variable variable,
                                PatternSpec spec) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that are matched according to the type and RHS value specified in spec.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @param spec PatternSpec with a specified type and value.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET match(Statement statement, Underscore underscore,
                         PatternSpec spec) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that have the specified variable on the LHS and are matched according to
   *  the type and RHS value specified in spec.
   *  @param statement Statement with a specified type.
   *  @param varName String with a specified variable name.
   *  @param spec PatternSpec with a specified type and value.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET match(Statement statement, String varName,
                         PatternSpec spec) = 0;

  /**
   * Query API for normal select
   */

  /** @brief Retrieves all variables stored in Pkb.
   *  @param variable Empty Variable struct.
   *  @return Unordered_set of variables.
   */
  virtual NAME_SET select(Variable variable) = 0;

  /** @brief Retrieves all line numbers of statements of statement type stored
   *  in Pkb.
   *  @param statement Statement with a specified type.
   *  @return Unordered_set of statement line numbers.
   */
  virtual LINE_SET select(Statement statement) = 0;

  /** @brief Retrieves all procedures stored in Pkb.
   *  @param procedure Empty Procedure struct.
   *  @return Unordered_set of procedures.
   */
  virtual NAME_SET select(Procedure procedure) = 0;

  /** @brief Retrieves all constants stored in Pkb.
   *  @param constant Empty Constant struct.
   *  @return Unordered_set of constant.
   */
  virtual CONST_SET select(Constant constant) = 0;

  /**
   * Query API for follows
   */

  /** @brief Checks whether specified line number is followed by another
   *  specified line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool follows(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line number of statement that follows the input line
   *  number and is the same type as input statement type.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET follows(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether a line has a following line.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool follows(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of statement that is followed by input line
   *  number.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET follows(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type that follows other statements of specified secondary statement type.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers
   */
  virtual LINE_LINE_PAIRS follows(Statement statement1,
                                  Statement statement2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that is followed by a line.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET follows(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether specified line number follows a line.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool follows(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that follows a line.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET follows(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether any line follows any line in the SIMPLE source.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool follows(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for followsStar
   */

  /** @brief Checks whether specified the line number is transitively followed
   *  by another specified line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool followsStar(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line numbers of statements that transitively follows the
   *  input line number and is the same type as input statement type.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET followsStar(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether a line has a following line.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool followsStar(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line numbers of statements that are transitively followed
   *  by input line number.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET followsStar(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type that transitively follows other statements of specified secondary
   *  statement type.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers.
   */
  virtual LINE_LINE_PAIRS followsStar(Statement statement1,
                                      Statement statement2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that is followed by a line.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET followsStar(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether the specified line number follows a line.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool followsStar(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that follows a line.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET followsStar(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether any line follows any line in the SIMPLE source.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool followsStar(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for parent
   */

  /** @brief Checks whether the second input line number is contained within the
   *  first input line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool parent(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that are contained within the input line number.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET parent(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether there is any line contained within the input line.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool parent(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of statement of specified statement type
   *  that has input line number contained within.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET parent(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type and line numbers of statements of specified secondary statement type
   *  where the primary statement line number contains the secondary statement
   *  line number.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers.
   */
  virtual LINE_LINE_PAIRS parent(Statement statement1,
                                 Statement statement2) = 0;

  /** @brief Retrieves line number of statement of specified statement type
   *  that has any line number contained within.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET parent(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether input line number is contained within any line
   *  number.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool parent(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers that are the same type as the specified
   *  statement type and are contained within any line number.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET parent(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether there is any line contained within any line in the
   *  SIMPLE source.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool parent(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for parentStar
   */

  /** @brief Checks whether the second input line number is transitively
   *  contained within the first input line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool parentStar(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that are transitively contained within the input line number.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET parentStar(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether there is any line contained within the input line.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool parentStar(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of statement of specified statement type
   *  that has input line number transitively contained within.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET parentStar(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type and line numbers of statements of specified secondary statement type
   *  where the primary statement line number transitively contains the
   *  secondary statement line number.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers.
   */
  virtual LINE_LINE_PAIRS parentStar(Statement statement1,
                                     Statement statement2) = 0;

  /** @brief Retrieves line number of statement of specified statement type
   *  that has any line number contained within.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET parentStar(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether input line number is contained within any line
   *  number.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool parentStar(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers that are the same type as the specified
   *  statement type and are contained within any line number.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET parentStar(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether there is any line contained within any line in the
   *  SIMPLE source.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool parentStar(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for uses
   */

  /** @brief Checks whether the input line uses the specified variable.
   *  @param line LineNumber of SIMPLE source.
   *  @param varName String with a specified variable name.
   *  @return boolean.
   */
  virtual bool uses(LineNumber line, String varName) = 0;

  /** @brief Retrieves variables that are used in the specified line.
   *  @param line LineNumber of SIMPLE source.
   *  @param variable Empty Variable struct.
   *  @return A set of variable names.
   */
  virtual NAME_SET uses(LineNumber line, Variable variable) = 0;

  /** @brief Checks whether the specified line uses any variables.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool uses(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that uses the specified variable.
   *  @param statement Statement with a specified type.
   *  @param varName String with a specified variable name.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET uses(Statement statement, String varName) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  and the variables used in those statements.
   *  @param statement Statement with a specified type.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing line numbers and
   *  second vector containing variable names.
   */
  virtual LINE_NAME_PAIRS uses(Statement statement, Variable variable) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that uses any variables.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET uses(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether the specified procedure uses the specified variable.
   *  @param procName String with a specified procedure name.
   *  @param varName String with a specified variable name.
   *  @return boolean.
   */
  virtual bool uses(String procName, String varName) = 0;

  /** @brief Retrieves variables that are used in the specified procedure.
   *  @param procName String with a specified procedure name.
   *  @param variable Empty Variable struct.
   *  @return A set of variable names.
   */
  virtual NAME_SET uses(String procName, Variable variable) = 0;

  /** @brief Checks whether the specified procedure uses any variables.
   *  @param procName String with a specified procedure name.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool uses(String procName, Underscore underscore) = 0;

  /** @brief Retrieves procedures that uses the specified variable.
   *  @param procedure Empty Procedure struct.
   *  @param varName String with a specified variable name.
   *  @return A set of procedure names.
   */
  virtual NAME_SET uses(Procedure procedure, String varName) = 0;

  /** @brief Retrieves procedures and the variables used in those procedures.
   *  @param procedure Empty Procedure struct.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing procedure names and
   *  second vector containing variable names.
   */
  virtual NAME_NAME_PAIRS uses(Procedure procedure, Variable variable) = 0;

  /** @brief Retrieves procedures that uses any variables.
   *  @param procedure Empty Procedure struct.
   *  @param underscore Empty Underscore struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET uses(Procedure procedure, Underscore underscore) = 0;

  /*
   * Query API for modifies
   */

  /** @brief Checks whether the input line modifies the specified variable.
   *  @param line LineNumber of SIMPLE source.
   *  @param varName String with a specified variable name.
   *  @return boolean.
   */
  virtual bool modifies(LineNumber line, String varName) = 0;

  /** @brief Retrieves variables that are modified in the specified line.
   *  @param line LineNumber of SIMPLE source.
   *  @param variable Empty Variable struct.
   *  @return A set of variable names.
   */
  virtual NAME_SET modifies(LineNumber line, Variable variable) = 0;

  /** @brief Checks whether the specified line modifies any variables.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool modifies(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that modifies the specified variable.
   *  @param statement Statement with a specified type.
   *  @param varName String with a specified variable name
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET modifies(Statement statement, String varName) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  and the variables modified in those statements.
   *  @param statement Statement with a specified type.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing line numbers and
   *  second vector containing variable names.
   */
  virtual LINE_NAME_PAIRS modifies(Statement statement, Variable variable) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that modifies any variables.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET modifies(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether the specified procedure modifies the specified
   *  variable.
   *  @param procName String with a specified procedure name.
   *  @param varName String with a specified variable name.
   *  @return boolean.
   */
  virtual bool modifies(String procName, String varName) = 0;

  /** @brief Retrieves variables that are modified in the specified procedure.
   *  @param procName String with a specified procedure name.
   *  @param variable Empty Variable struct.
   *  @return A set of variable names.
   */
  virtual NAME_SET modifies(String procName, Variable variable) = 0;

  /** @brief Checks whether the specified procedure modifies any variables.
   *  @param procName String with a specified procedure name.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool modifies(String procName, Underscore underscore) = 0;

  /** @brief Retrieves procedures that modifies the specified variable.
   *  @param procedure Empty Procedure struct.
   *  @param varName String with a specified variable name.
   *  @return A set of procedure names.
   */
  virtual NAME_SET modifies(Procedure procedure, String varName) = 0;

  /** @brief Retrieves procedures and the variables modified in those
   *  procedures.
   *  @param procedure Empty Procedure struct.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing procedure names and
   *  second vector containing variable names.
   */
  virtual NAME_NAME_PAIRS modifies(Procedure procedure, Variable variable) = 0;

  /** @brief Retrieves procedures that modifies any variables.
   *  @param procedure Empty Procedure struct.
   *  @param underscore Empty Underscore struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET modifies(Procedure procedure, Underscore underscore) = 0;

  /*
   * Query API for calls
   */

  /** @brief Checks whether a specified procedure directly calls another
   *  specified procedure.
   *  @param procedureName1 String with a specified procedure name.
   *  @param procedureName2 String with a specified procedure name.
   *  @return boolean.
   */
  virtual bool calls(String procedureName1, String procedureName2) = 0;

  /** @brief Retrieves all procedures directly called by a specified procedure.
   *  @param procedureName String with a specified procedure name.
   *  @param procedure Empty Procedure struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET calls(String procedureName, Procedure procedure) = 0;

  /** @brief Checks whether a specified procedure directly calls any procedure.
   *  @param procedureName String with a specified procedure name.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool calls(String procedureName, Underscore underscore) = 0;

  /** @brief Retrieves all procedures that directly calls a specified procedure.
   *  @param procedure Empty Procedure struct.
   *  @param procedureName String with a specified procedure name.
   *  @return A set of procedure names.
   */
  virtual NAME_SET calls(Procedure procedure, String procedureName) = 0;

  /** @brief Retrieves pairs of primary procedures and the secondary procedures
   *  that are directly called by the primary procedures.
   *  @param procedure1 Empty Procedure struct.
   *  @param procedure2 Empty Procedure struct.
   *  @return A pair of vectors with first vector containing primary procedures
   *  and second vector containing secondary procedures directly called by
   *  primary procedures.
   */
  virtual NAME_NAME_PAIRS calls(Procedure procedure1, Procedure procedure2) = 0;

  /** @brief Retrieves all procedures that directly calls any procedure.
   *  @param procedure Empty Procedure struct.
   *  @param underscore Empty Underscore struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET calls(Procedure procedure, Underscore underscore) = 0;

  /** @brief Checks whether a specified procedure is directly called by any
   *  procedure.
   *  @param underscore Empty Underscore struct.
   *  @param procedureName String with a specified procedure name.
   *  @return boolean.
   */
  virtual bool calls(Underscore underscore, String procedureName) = 0;

  /** @brief Retrieves all procedures that are directly called by any procedure.
   *  @param underscore Empty Underscore struct.
   *  @param procedure Empty Procedure struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET calls(Underscore underscore, Procedure procedure) = 0;

  /** @brief Checks whether any procedure directly calls any other procedure.
   *  @param underscore Empty Underscore struct.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool calls(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for callsStar
   */

  /** @brief Checks whether a specified procedure transitively calls another
   *  specified procedure.
   *  @param procedureName1 String with a specified procedure name.
   *  @param procedureName2 String with a specified procedure name.
   *  @return boolean.
   */
  virtual bool callsStar(String procedureName1, String procedureName2) = 0;

  /** @brief Retrieves all procedures transitively called by a specified
   *  procedure.
   *  @param procedureName String with a specified procedure name.
   *  @param procedure Empty Procedure struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET callsStar(String procedureName, Procedure procedure) = 0;

  /** @brief Checks whether a specified procedure calls any other procedure.
   *  @param procedureName String with a specified procedure name.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool callsStar(String procedureName, Underscore underscore) = 0;

  /** @brief Retrieves all procedures that transitively calls a specified
   *  procedure.
   *  @param procedure Empty Procedure struct.
   *  @param procedureName String with a specified procedure name.
   *  @return A set of procedure names.
   */
  virtual NAME_SET callsStar(Procedure procedure, String procedureName) = 0;

  /** @brief Retrieves pairs of primary procedures and the secondary procedures
   *  that are transitively called by the primary procedures.
   *  @param procedure1 Empty Procedure struct.
   *  @param procedure2 Empty Procedure struct.
   *  @return A pair of vectors with first vector containing primary procedures
   *  and second vector containing secondary procedures transitively called by
   *  primary procedures.
   */
  virtual NAME_NAME_PAIRS callsStar(Procedure procedure1,
                                    Procedure procedure2) = 0;

  /** @brief Retrieves all procedures that calls any procedure.
   *  @param procedure Empty Procedure struct.
   *  @param underscore Empty Underscore struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET callsStar(Procedure procedure, Underscore underscore) = 0;

  /** @brief Checks whether a specified procedure is called by any
   *  procedure.
   *  @param underscore Empty Underscore struct.
   *  @param procedureName String with a specified procedure name.
   *  @return boolean.
   */
  virtual bool callsStar(Underscore underscore, String procedureName) = 0;

  /** @brief Retrieves all procedures that are called by any other procedure.
   *  @param underscore Empty Underscore struct.
   *  @param procedure Empty Procedure struct.
   *  @return A set of procedure names.
   */
  virtual NAME_SET callsStar(Underscore underscore, Procedure procedure) = 0;

  /** @brief Checks whether any procedure calls any other procedure.
   *  @param underscore Empty Underscore struct.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool callsStar(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for next
   */

  /** @brief Checks whether the second input line number is next of the
   *  first input line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool next(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line number of statement that are next of the input line
   *  number and is the same type as input statement type.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET next(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether a specified line has a next line.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool next(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of statements where their next statement line
   *  number is the input line number.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET next(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves pairs of line numbers of statements of specified primary
   *  statement type where their next statements are of specified secondary
   *  statement type.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers
   */
  virtual LINE_LINE_PAIRS next(Statement statement1, Statement statement2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that has a next statement.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET next(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether statement of specified line number is the next of
   *  another statement.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool next(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that are next of a statement.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET next(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether there exists a statement that has a next statement
   * in the SIMPLE source.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool next(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for nextStar
   */

  /** @brief Checks whether statement of second specified line number is
   * transitively a next of the statement of first specified line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool nextStar(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line numbers of statements that are transitively next of
   * the statement of specified input line number and is the same type as input
   * statement type.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET nextStar(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether statement of specified line number has a next
   * statement.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool nextStar(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line numbers of statements that by transitivity have the
   * statement of specified line number as a next statement.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET nextStar(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves pairs of line numbers of statements of specified primary
   * statement type and statements of specified
   * secondary statement type where statements of specified secondary statement
   * type are transitively the next statements of the statements of primary
   * statement type.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers.
   */
  virtual LINE_LINE_PAIRS nextStar(Statement statement1,
                                   Statement statement2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that has a next statement.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual LINE_SET nextStar(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether statement of specified line number is the next of
   * another statement.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool nextStar(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that are next of a statement.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET nextStar(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether there exists a statement that has a next statement
   * in the SIMPLE source.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool nextStar(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for affects
   */

  /** @brief Checks whether the assignment statement on first input line number
   * affects the assignment statement on second input line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affects(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line number of assignment statements that are affected by
   * the assignment statement of input line number.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affects(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether the assignment statement of specified line number
   * affects any other assignment statements.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affects(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of assignment statements that affect the
   * assignment statement of input line number.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affects(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves pairs of line numbers of primary assignment statements
   * and other assignment statements affected by the primary assignment
   * statement.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers
   */
  virtual LINE_LINE_PAIRS affects(Statement statement1,
                                  Statement statement2) = 0;

  /** @brief Retrieves line numbers of assignment statements that affects other
   * assignment statements.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affects(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether assignment statement of specified line number is
   * affected by any assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affects(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of assignment statements that are affected
   * by other assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affects(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether there exists an assignment statement that affects
   * another assignment statement.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affects(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for affectsStar
   */

  /** @brief Checks whether the assignment on first input line number
   * transitively affects the assignment on second input line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affectsStar(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line number of assignment statements that are
   * transitively affected by the assignment statement of input line number.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsStar(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether the assignment statement of specified line number
   * affects any other assignment statements.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affectsStar(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of assignment statements that transitively
   * affect the assignment statement of input line number.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsStar(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves pairs of line numbers of primary assignment statements
   * and other assignment statements transitively affected by the primary
   * assignment statement.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers
   */
  virtual LINE_LINE_PAIRS affectsStar(Statement statement1,
                                      Statement statement2) = 0;

  /** @brief Retrieves line numbers of assignment statements that affects other
   * assignment statements.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsStar(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether assignment statement of specified line number is
   * affected by any assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affectsStar(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of assignment statements that are affected
   * by other assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsStar(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether there exists an assignment statement that affects
   * another assignment statement.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affectsStar(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for affectsBip
   */

  /** @brief Checks whether the assignment statement on first input line number
   * affectsBip the assignment statement on second input line number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affectsBip(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line number of assignment statements that are affectedBip
   * by the assignment statement of input line number.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBip(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether the assignment statement of specified line number
   * affectsBip any other assignment statements.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affectsBip(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of assignment statements that affectsBip the
   * assignment statement of input line number.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBip(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves pairs of line numbers of primary assignment statements
   * and other assignment statements affectedBip by the primary assignment
   * statement.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers
   */
  virtual LINE_LINE_PAIRS affectsBip(Statement statement1,
                                     Statement statement2) = 0;

  /** @brief Retrieves line numbers of assignment statements that affectsBip
   * other assignment statements.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBip(Statement statement, Underscore underscore) = 0;

  /** @brief Checks whether assignment statement of specified line number is
   * affectedBip by any assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affectsBip(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of assignment statements that are
   * affectedBip by other assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBip(Underscore underscore, Statement statement) = 0;

  /** @brief Checks whether there exists an assignment statement that affectsBip
   * another assignment statement.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affectsBip(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for affectsBipStar
   */

  /** @brief Checks whether the assignment statement on first input line number
   * transitively affectsBip the assignment statement on second input line
   * number.
   *  @param line1 LineNumber of SIMPLE source.
   *  @param line2 LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affectsBipStar(LineNumber line1, LineNumber line2) = 0;

  /** @brief Retrieves line number of assignment statements that are
   * transitively affectedBip by the assignment statement of input line number.
   *  @param line LineNumber of SIMPLE source.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBipStar(LineNumber line, Statement statement) = 0;

  /** @brief Checks whether the assignment statement of specified line number
   * affectsBip any other assignment statements.
   *  @param line LineNumber of SIMPLE source.
   *  @param underscore Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affectsBipStar(LineNumber line, Underscore underscore) = 0;

  /** @brief Retrieves line number of assignment statements that transitively
   * affectsBip the assignment statement of input line number.
   *  @param statement Statement with a specified type.
   *  @param line LineNumber of SIMPLE source.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBipStar(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves pairs of line numbers of primary assignment statements
   * and other assignment statements transitively affectedBip by the primary
   * assignment statement.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers
   */
  virtual LINE_LINE_PAIRS affectsBipStar(Statement statement1,
                                         Statement statement2) = 0;

  /** @brief Retrieves line numbers of assignment statements that affectsBip
   * other assignment statements.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBipStar(Statement statement,
                                  Underscore underscore) = 0;

  /** @brief Checks whether assignment statement of specified line number is
   * affectedBip by any assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param line LineNumber of SIMPLE source.
   *  @return boolean.
   */
  virtual bool affectsBipStar(Underscore underscore, LineNumber line) = 0;

  /** @brief Retrieves line numbers of assignment statements that are
   * affectedBip by other assignment statements.
   *  @param underscore Empty Underscore struct.
   *  @param statement Statement with a specified type.
   *  @return A set of SIMPLE source line number.
   */
  virtual LINE_SET affectsBipStar(Underscore underscore,
                                  Statement statement) = 0;

  /** @brief Checks whether there exists an assignment statement that affectsBip
   * another assignment statement.
   *  @param underscore1 Empty Underscore struct.
   *  @param underscore2 Empty Underscore struct.
   *  @return boolean.
   */
  virtual bool affectsBipStar(Underscore underscore1,
                              Underscore underscore2) = 0;
};
