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
  virtual STRING_PAIRS match(Statement statement, Variable variable,
                             PatternSpec spec) = 0;

  virtual STRING_SET match(Statement statement, Underscore underscore,
                           PatternSpec spec) = 0;

  virtual STRING_SET match(Statement statement, String varName,
                           PatternSpec spec) = 0;

  /*
   * Query API for normal select
   */
  virtual STRING_SET select(Variable variable) = 0;

  virtual STRING_SET select(Statement statement) = 0;

  virtual STRING_SET select(Procedure procedure) = 0;

  virtual STRING_SET select(Constant constant) = 0;

  /*
   * Query API for follows
   */
  virtual bool follows(LineNumber line1, LineNumber line2) = 0;

  virtual STRING_SET follows(LineNumber line, Statement statement) = 0;

  virtual bool follows(LineNumber line, Underscore underscore) = 0;

  virtual STRING_SET follows(Statement statement, LineNumber line) = 0;

  virtual STRING_PAIRS follows(Statement statement1, Statement statement2) = 0;

  virtual STRING_SET follows(Statement statement, Underscore underscore) = 0;

  virtual bool follows(Underscore underscore, LineNumber line) = 0;

  virtual STRING_SET follows(Underscore underscore, Statement statement) = 0;

  virtual bool follows(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for followsStar
   */
  virtual bool followsStar(LineNumber line1, LineNumber line2) = 0;

  virtual STRING_SET followsStar(LineNumber line, Statement statement) = 0;

  virtual bool followsStar(LineNumber line, Underscore underscore) = 0;

  virtual STRING_SET followsStar(Statement statement, LineNumber line) = 0;

  virtual STRING_PAIRS followsStar(Statement statement1,
                                   Statement statement2) = 0;

  virtual STRING_SET followsStar(Statement statement,
                                 Underscore underscore) = 0;

  virtual bool followsStar(Underscore underscore, LineNumber line) = 0;

  virtual STRING_SET followsStar(Underscore underscore,
                                 Statement statement) = 0;

  virtual bool followsStar(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for parent
   */

  virtual bool parent(LineNumber line1, LineNumber line2) = 0;

  virtual STRING_SET parent(LineNumber line, Statement statement) = 0;

  virtual bool parent(LineNumber line, Underscore underscore) = 0;

  virtual STRING_SET parent(Statement statement, LineNumber line) = 0;

  virtual STRING_PAIRS parent(Statement statement1, Statement statement2) = 0;

  virtual STRING_SET parent(Statement statement, Underscore underscore) = 0;

  virtual bool parent(Underscore underscore, LineNumber line) = 0;

  virtual STRING_SET parent(Underscore underscore, Statement statement) = 0;

  virtual bool parent(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for parentStar
   */

  virtual bool parentStar(LineNumber line1, LineNumber line2) = 0;

  virtual STRING_SET parentStar(LineNumber line, Statement statement) = 0;

  virtual bool parentStar(LineNumber line, Underscore underscore) = 0;

  virtual STRING_SET parentStar(Statement statement, LineNumber line) = 0;

  virtual STRING_PAIRS parentStar(Statement statement1,
                                  Statement statement2) = 0;

  virtual STRING_SET parentStar(Statement statement, Underscore underscore) = 0;

  virtual bool parentStar(Underscore underscore, LineNumber line) = 0;

  virtual STRING_SET parentStar(Underscore underscore, Statement statement) = 0;

  virtual bool parentStar(Underscore underscore1, Underscore underscore2) = 0;

  /*
   * Query API for uses
   */

  virtual bool uses(LineNumber line, String variableName) = 0;

  virtual STRING_SET uses(LineNumber line, Variable variable) = 0;

  virtual bool uses(LineNumber line, Underscore underscore) = 0;

  virtual STRING_SET uses(Statement statement, String variableName) = 0;

  virtual STRING_PAIRS uses(Statement statement, Variable variable) = 0;

  virtual STRING_SET uses(Statement statement, Underscore underscore) = 0;

  virtual bool uses(String procedureName, String variableName) = 0;

  virtual STRING_SET uses(String procedureName, Variable variable) = 0;

  virtual bool uses(String procedureName, Underscore underscore) = 0;

  virtual STRING_SET uses(Procedure procedure, String variableName) = 0;

  virtual STRING_PAIRS uses(Procedure procedure, Variable variable) = 0;

  virtual STRING_SET uses(Procedure procedure, Underscore underscore) = 0;

  /*
   * Query API for modifies
   */

  virtual bool modifies(LineNumber line, String variableName) = 0;

  virtual STRING_SET modifies(LineNumber line, Variable variable) = 0;

  virtual bool modifies(LineNumber line, Underscore underscore) = 0;

  virtual STRING_SET modifies(Statement statement, String variableName) = 0;

  virtual STRING_PAIRS modifies(Statement statement, Variable variable) = 0;

  virtual STRING_SET modifies(Statement, Underscore underscore) = 0;

  virtual bool modifies(String procedureName, String variableName) = 0;

  virtual STRING_SET modifies(String procedureName, Variable variable) = 0;

  virtual bool modifies(String procedureName, Underscore underscore) = 0;

  virtual STRING_SET modifies(Procedure, String variableName) = 0;

  virtual STRING_PAIRS modifies(Procedure, Variable variable) = 0;

  virtual STRING_SET modifies(Procedure, Underscore underscore) = 0;
};
