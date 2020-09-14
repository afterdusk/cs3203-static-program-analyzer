#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Pkb.h"
#include "PkbQueryEntityTypes.h"
#include "TNode.h"

typedef std::vector<LINE_NO> STRING_VECTOR;
typedef std::unordered_set<LINE_NO> STRING_SET;
typedef std::pair<std::vector<std::string>, std::vector<std::string>>
    STRING_PAIRS;

class PkbQueryInterface {
private:
  Pkb pkb;
  Variable variable;
  Procedure procedure;
  Underscore underscore;

  // Tables from Pkb
  VAR_TABLE varTable;
  PROC_TABLE procTable;
  USES_TABLE usesTable;
  USES_PROC_TABLE usesProcTable;
  MODIFIES_TABLE modifiesTable;
  MODIFIES_PROC_TABLE modifiesProcTable;
  FOLLOW_TABLE followTable;
  PARENT_TABLE parentTable;
  STATEMENT_PROC_TABLE statementProcTable;
  STATEMENT_TYPE_TABLE statementTypeTable;
  ASSIGN_AST_TABLE assignAstTable;
  CONSTANT_TABLE constTable;

  // Derived tables using original tables from Pkb
  KeysTable<LINE_NO, LINE_NO> prevLineTable;    // invert of followTable
  KeysTable<LINE_NO, STRING_SET> childrenTable; // pseudo invert of parentTable
  KeysTable<StatementType, STRING_SET>
      invertStatementTypeTable; // pseudo invert of statementTypeTable
  KeysTable<PROC, STRING_SET>
      invertStatementProcTable; // pseudo invert of statementProcTable

  KeysTable<VAR_TABLE_INDEX, VAR> invertVarTable;
  KeysTable<PROC_TABLE_INDEX, PROC> invertProcTable;

  KeysTable<LINE_NO, STRING_SET> closeFollowTable;
  KeysTable<LINE_NO, STRING_SET> closeParentTable;
  KeysTable<LINE_NO, STRING_SET> closePrevLineTable;
  KeysTable<LINE_NO, STRING_SET> closeChildrenTable;

  KeysTable<LINE_NO, VAR_TABLE_INDEXES> usesTableTransited;
  KeysTable<VAR_TABLE_INDEX, std::unordered_set<LINE_NO>> invertUsesTable;
  KeysTable<LINE_NO, VAR_TABLE_INDEXES> modifiesTableTransited;
  KeysTable<VAR_TABLE_INDEX, std::unordered_set<LINE_NO>> invertModifiesTable;

  STRING_SET stmtTableIndexes;
  STRING_SET followTableIndexes;
  STRING_SET parentTableIndexes;
  STRING_SET prevLineTableIndexes;
  STRING_SET childrenTableIndexes;

  STRING_SET varNamesSet;
  STRING_SET procNamesSet;

public:
  PkbQueryInterface() {}
  PkbQueryInterface(Pkb pkb) {
    this->pkb = pkb;
    this->varTable = pkb.getVarTable();
    this->procTable = pkb.getProcTable();
    this->usesTable = pkb.getUsesTable();
    this->usesProcTable = pkb.getUsesProcTable();
    this->modifiesTable = pkb.getModifiesTable();
    this->modifiesProcTable = pkb.getModifiesProcTable();
    this->followTable = pkb.getFollowTable();
    this->parentTable = pkb.getParentTable();
    this->statementProcTable = pkb.getStatementProcTable();
    this->statementTypeTable = pkb.getStatementTypeTable();
    this->assignAstTable = pkb.getAssignAstTable();
    this->constTable = pkb.getConstantTable();

    this->prevLineTable = pkb.invert(pkb.getFollowTable());
    this->childrenTable = pkb.pseudoinvert(pkb.getParentTable());
    this->invertStatementTypeTable =
        pkb.pseudoinvert(pkb.getStatementTypeTable());
    this->invertStatementProcTable =
        pkb.pseudoinvert(pkb.getStatementProcTable());
    this->invertVarTable = pkb.invert(pkb.getVarTable());
    this->invertProcTable = pkb.invert(pkb.getProcTable());

    this->closeFollowTable = pkb.close(pkb.getFollowTable());
    this->closeParentTable = pkb.close(pkb.getParentTable());
    this->closePrevLineTable = pkb.close(prevLineTable);
    this->closeChildrenTable = pkb.closeFlatten(childrenTable);

    this->usesTableTransited =
        pkb.transit(pkb.getUsesTable(), pkb.getUsesProcTable());
    this->invertUsesTable = pkb.pseudoinvertFlattenKeys(usesTableTransited);
    this->modifiesTableTransited =
        pkb.transit(pkb.getModifiesTable(), pkb.getModifiesProcTable());
    this->invertModifiesTable =
        pkb.pseudoinvertFlattenKeys(modifiesTableTransited);

    this->stmtTableIndexes = STRING_SET(statementTypeTable.keys.begin(),
                                        statementTypeTable.keys.end());
    this->followTableIndexes =
        STRING_SET(followTable.keys.begin(), followTable.keys.end());
    this->parentTableIndexes =
        STRING_SET(parentTable.keys.begin(), parentTable.keys.end());
    this->prevLineTableIndexes =
        STRING_SET(prevLineTable.keys.begin(), prevLineTable.keys.end());
    this->childrenTableIndexes =
        STRING_SET(childrenTable.keys.begin(), childrenTable.keys.end());

    this->varNamesSet = STRING_SET(varTable.keys.begin(), varTable.keys.end());
    this->procNamesSet =
        STRING_SET(procTable.keys.begin(), procTable.keys.end());
  }

  /*
   * Query API for pattern
   */
  STRING_PAIRS match(Statement statement, Variable variable, PatternSpec spec);

  STRING_SET match(Statement statement, Underscore underscore,
                   PatternSpec spec);

  STRING_SET match(Statement statement, String varName, PatternSpec spec);

  /*
   * Query API for normal select
   */
  STRING_SET select(Variable variable);

  STRING_SET select(Statement statement);

  STRING_SET select(Procedure procedure);

  STRING_SET select(Constant constant);

  /*
   * Query API for follows
   */
  bool follows(LineNumber line1, LineNumber line2);

  STRING_SET follows(LineNumber line, Statement statement);

  bool follows(LineNumber line, Underscore underscore);

  STRING_SET follows(Statement statement, LineNumber line);

  STRING_PAIRS follows(Statement statement1, Statement statement2);

  STRING_SET follows(Statement statement, Underscore underscore);

  bool follows(Underscore underscore, LineNumber line);

  STRING_SET follows(Underscore underscore, Statement statement);

  bool follows(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for followsStar
   */
  bool followsStar(LineNumber line1, LineNumber line2);

  STRING_SET followsStar(LineNumber line, Statement statement);

  bool followsStar(LineNumber line, Underscore underscore);

  STRING_SET followsStar(Statement statement, LineNumber line);

  STRING_PAIRS followsStar(Statement statement1, Statement statement2);

  STRING_SET followsStar(Statement statement, Underscore underscore);

  bool followsStar(Underscore underscore, LineNumber line);

  STRING_SET followsStar(Underscore underscore, Statement statement);

  bool followsStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for parent
   */

  bool parent(LineNumber line1, LineNumber line2);

  STRING_SET parent(LineNumber line, Statement statement);

  bool parent(LineNumber line, Underscore underscore);

  STRING_SET parent(Statement statement, LineNumber line);

  STRING_PAIRS parent(Statement statement1, Statement statement2);

  STRING_SET parent(Statement statement, Underscore underscore);

  bool parent(Underscore underscore, LineNumber line);

  STRING_SET parent(Underscore underscore, Statement statement);

  bool parent(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for parentStar
   */

  bool parentStar(LineNumber line1, LineNumber line2);

  STRING_SET parentStar(LineNumber line, Statement statement);

  bool parentStar(LineNumber line, Underscore underscore);

  STRING_SET parentStar(Statement statement, LineNumber line);

  STRING_PAIRS parentStar(Statement statement1, Statement statement2);

  STRING_SET parentStar(Statement statement, Underscore underscore);

  bool parentStar(Underscore underscore, LineNumber line);

  STRING_SET parentStar(Underscore underscore, Statement statement);

  bool parentStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for uses
   */

  bool uses(LineNumber line, String variableName);

  STRING_SET uses(LineNumber line, Variable variable);

  bool uses(LineNumber line, Underscore underscore);

  STRING_SET uses(Statement statement, String variableName);

  STRING_PAIRS uses(Statement statement, Variable variable);

  STRING_SET uses(Statement statement, Underscore underscore);

  bool uses(String procedureName, String variableName);

  STRING_SET uses(String procedureName, Variable variable);

  bool uses(String procedureName, Underscore underscore);

  STRING_SET uses(Procedure procedure, String variableName);

  STRING_PAIRS uses(Procedure procedure, Variable variable);

  STRING_SET uses(Procedure procedure, Underscore underscore);

  /*
   * Query API for modifies
   */

  bool modifies(LineNumber line, String variableName);

  STRING_SET modifies(LineNumber line, Variable variable);

  bool modifies(LineNumber line, Underscore underscore);

  STRING_SET modifies(Statement statement, String variableName);

  STRING_PAIRS modifies(Statement statement, Variable variable);

  STRING_SET modifies(Statement, Underscore underscore);

  bool modifies(String procedureName, String variableName);

  STRING_SET modifies(String procedureName, Variable variable);

  bool modifies(String procedureName, Underscore underscore);

  STRING_SET modifies(Procedure, String variableName);

  STRING_PAIRS modifies(Procedure, Variable variable);

  STRING_SET modifies(Procedure, Underscore underscore);
};
