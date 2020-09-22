#pragma once

#include "PkbQueryEntityTypes.h"
#include <string>
#include <utility>
#include <vector>

typedef std::vector<PkbTables::LINE_NO> STRING_VECTOR;
typedef std::unordered_set<PkbTables::LINE_NO> STRING_SET;
typedef std::pair<std::vector<std::string>, std::vector<std::string>>
    STRING_PAIRS;

class PkbQueryInterface {
protected:
  // Derived tables using original tables from PkbTables
  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NO>
      prevLineTable; // invert of followTable
  KeysTable<PkbTables::LINE_NO, STRING_SET>
      childrenTable; // pseudo invert of parentTable
  KeysTable<PkbTables::StatementType, STRING_SET>
      invertStatementTypeTable; // pseudo invert of statementTypeTable
  KeysTable<PkbTables::PROC, STRING_SET>
      invertStatementProcTable; // pseudo invert of statementProcTable

  KeysTable<PkbTables::VAR_TABLE_INDEX, PkbTables::VAR> invertVarTable;
  KeysTable<PkbTables::PROC_TABLE_INDEX, PkbTables::PROC> invertProcTable;

  KeysTable<PkbTables::LINE_NO, STRING_SET> closeFollowTable;
  KeysTable<PkbTables::LINE_NO, STRING_SET> closeParentTable;
  KeysTable<PkbTables::LINE_NO, STRING_SET> closePrevLineTable;
  KeysTable<PkbTables::LINE_NO, STRING_SET> closeChildrenTable;

  KeysTable<PkbTables::LINE_NO, PkbTables::VAR_TABLE_INDEXES>
      usesTableTransited;
  KeysTable<PkbTables::VAR_TABLE_INDEX, std::unordered_set<PkbTables::LINE_NO>>
      invertUsesTable;
  KeysTable<PkbTables::LINE_NO, PkbTables::VAR_TABLE_INDEXES>
      modifiesTableTransited;
  KeysTable<PkbTables::VAR_TABLE_INDEX, std::unordered_set<PkbTables::LINE_NO>>
      invertModifiesTable;

  STRING_SET stmtTableIndexes;
  STRING_SET followTableIndexes;
  STRING_SET parentTableIndexes;
  STRING_SET prevLineTableIndexes;
  STRING_SET childrenTableIndexes;

  STRING_SET varNamesSet;
  STRING_SET procNamesSet;

public:
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
  virtual STRING_PAIRS match(Statement statement, Variable variable,
                             PatternSpec spec) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that are matched according to the type and RHS value specified in spec.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @param spec PatternSpec with a specified type and value.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual STRING_SET match(Statement statement, Underscore underscore,
                           PatternSpec spec) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that have the specified variable on the LHS and are matched according to
   *  the type and RHS value specified in spec.
   *  @param statement Statement with a specified type.
   *  @param varName String with a specified variable name.
   *  @param spec PatternSpec with a specified type and value.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual STRING_SET match(Statement statement, String varName,
                           PatternSpec spec) = 0;

  /**
   * Query API for normal select
   */

  /** @brief Retrieves all variables stored in Pkb.
   *  @param variable Empty Variable struct.
   *  @return Unordered_set of variables.
   */
  virtual STRING_SET select(Variable variable) = 0;

  /** @brief Retrieves all line numbers of statements of statement type stored
   *  in Pkb.
   *  @param statement Statement with a specified type.
   *  @return Unordered_set of statement line numbers.
   */
  virtual STRING_SET select(Statement statement) = 0;

  /** @brief Retrieves all procedures stored in Pkb.
   *  @param procedure Empty Procedure struct.
   *  @return Unordered_set of procedures.
   */
  virtual STRING_SET select(Procedure procedure) = 0;

  /** @brief Retrieves all constants stored in Pkb.
   *  @param constant Empty Constant struct.
   *  @return Unordered_set of constant.
   */
  virtual STRING_SET select(Constant constant) = 0;

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
  virtual STRING_SET follows(LineNumber line, Statement statement) = 0;

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
  virtual STRING_SET follows(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type that follows other statements of specified secondary statement type.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers
   */
  virtual STRING_PAIRS follows(Statement statement1, Statement statement2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that is followed by a line.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line number.
   */
  virtual STRING_SET follows(Statement statement, Underscore underscore) = 0;

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
  virtual STRING_SET follows(Underscore underscore, Statement statement) = 0;

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
  virtual STRING_SET followsStar(LineNumber line, Statement statement) = 0;

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
  virtual STRING_SET followsStar(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type that transitively follows other statements of specified secondary
   *  statement type.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers.
   */
  virtual STRING_PAIRS followsStar(Statement statement1,
                                   Statement statement2) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that is followed by a line.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual STRING_SET followsStar(Statement statement,
                                 Underscore underscore) = 0;

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
  virtual STRING_SET followsStar(Underscore underscore,
                                 Statement statement) = 0;

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
  virtual STRING_SET parent(LineNumber line, Statement statement) = 0;

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
  virtual STRING_SET parent(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type and line numbers of statements of specified secondary statement type
   *  where the primary statement line number contains the secondary statement
   *  line number.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers.
   */
  virtual STRING_PAIRS parent(Statement statement1, Statement statement2) = 0;

  /** @brief Retrieves line number of statement of specified statement type
   *  that has any line number contained within.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual STRING_SET parent(Statement statement, Underscore underscore) = 0;

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
  virtual STRING_SET parent(Underscore underscore, Statement statement) = 0;

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
  virtual STRING_SET parentStar(LineNumber line, Statement statement) = 0;

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
  virtual STRING_SET parentStar(Statement statement, LineNumber line) = 0;

  /** @brief Retrieves line numbers of statements of specified primary statement
   *  type and line numbers of statements of specified secondary statement type
   *  where the primary statement line number transitively contains the
   *  secondary statement line number.
   *  @param statement1 Statement with a specified type.
   *  @param statement2 Statement with a specified type.
   *  @return A pair of vectors of SIMPLE source line numbers.
   */
  virtual STRING_PAIRS parentStar(Statement statement1,
                                  Statement statement2) = 0;

  /** @brief Retrieves line number of statement of specified statement type
   *  that has any line number contained within.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual STRING_SET parentStar(Statement statement, Underscore underscore) = 0;

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
  virtual STRING_SET parentStar(Underscore underscore, Statement statement) = 0;

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
  virtual STRING_SET uses(LineNumber line, Variable variable) = 0;

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
  virtual STRING_SET uses(Statement statement, String varName) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  and the variables used in those statements.
   *  @param statement Statement with a specified type.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing line numbers and
   *  second vector containing variable names.
   */
  virtual STRING_PAIRS uses(Statement statement, Variable variable) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that uses any variables.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual STRING_SET uses(Statement statement, Underscore underscore) = 0;

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
  virtual STRING_SET uses(String procName, Variable variable) = 0;

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
  virtual STRING_SET uses(Procedure procedure, String varName) = 0;

  /** @brief Retrieves procedures and the variables used in those procedures.
   *  @param procedure Empty Procedure struct.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing procedure names and
   *  second vector containing variable names.
   */
  virtual STRING_PAIRS uses(Procedure procedure, Variable variable) = 0;

  /** @brief Retrieves procedures that uses any variables.
   *  @param procedure Empty Procedure struct.
   *  @param underscore Empty Underscore struct.
   *  @return A set of procedure names.
   */
  virtual STRING_SET uses(Procedure procedure, Underscore underscore) = 0;

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
  virtual STRING_SET modifies(LineNumber line, Variable variable) = 0;

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
  virtual STRING_SET modifies(Statement statement, String varName) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  and the variables modified in those statements.
   *  @param statement Statement with a specified type.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing line numbers and
   *  second vector containing variable names.
   */
  virtual STRING_PAIRS modifies(Statement statement, Variable variable) = 0;

  /** @brief Retrieves line numbers of statements of specified statement type
   *  that modifies any variables.
   *  @param statement Statement with a specified type.
   *  @param underscore Empty Underscore struct.
   *  @return A set of SIMPLE source line numbers.
   */
  virtual STRING_SET modifies(Statement statement, Underscore underscore) = 0;

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
  virtual STRING_SET modifies(String procName, Variable variable) = 0;

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
  virtual STRING_SET modifies(Procedure procedure, String varName) = 0;

  /** @brief Retrieves procedures and the variables modified in those
   *  procedures.
   *  @param procedure Empty Procedure struct.
   *  @param variable Empty Variable struct.
   *  @return A pair of vectors with first vector containing procedure names and
   *  second vector containing variable names.
   */
  virtual STRING_PAIRS modifies(Procedure procedure, Variable variable) = 0;

  /** @brief Retrieves procedures that modifies any variables.
   *  @param procedure Empty Procedure struct.
   *  @param underscore Empty Underscore struct.
   *  @return A set of procedure names.
   */
  virtual STRING_SET modifies(Procedure procedure, Underscore underscore) = 0;
};
