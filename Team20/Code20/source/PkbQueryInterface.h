#pragma once

#include "PkbQueryEntityTypes.h"
#include <string>
#include <utility>
#include <vector>

typedef std::vector<PkbTables::LINE_NO> LINE_VECTOR;
typedef std::vector<std::string> NAME_VECTOR;
typedef std::unordered_set<PkbTables::LINE_NO> LINE_SET;
typedef std::unordered_set<std::string> NAME_SET;
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
      invertNextTable; // pseudo invert of nextTable

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
  LINE_SET invertNextTableIndexes;

  NAME_SET callsTableIndexesProcNames;
  NAME_SET invertCallsTableIndexesProcNames;

public:
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
  virtual LINE_NAME_PAIRS getStmtLineAndName(Statement statement) = 0;

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
  virtual PkbTables::VAR_TABLE select(Variable variable) = 0;

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
  virtual PkbTables::PROC_TABLE select(Procedure procedure) = 0;

  /** @brief Retrieves all constants stored in Pkb.
   *  @param constant Empty Constant struct.
   *  @return Unordered_set of constant.
   */
  virtual PkbTables::CONSTANT_TABLE select(Constant constant) = 0;

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

  /** @brief Defines the Affects relation for assignment.
  @param assignment An assignment statement.
  @return The statements affected by assignment.
  */
  virtual PkbTables::AFFECTS affects(PkbTables::ASSIGNMENT assignment) = 0;

  /** @brief Auxiliary function of PkbQueryInterface::affects, that collects
  then returns all statements affected by modifiesVar.
  @param modifiesVar Variable that affects collected statements.
  @param lineNo A statement, possibly affected by modifiesVar.
  @return The statements affected by modifiesVar.
  */
  virtual PkbTables::AFFECTS affectsAux(PkbTables::VAR modifiesVar,
                                        PkbTables::LINE_NO lineNo) = 0;
};
