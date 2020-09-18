#pragma once

#include <string>
#include <utility>
#include <vector>

#include "PkbQueryEntityTypes.h"
#include "PkbTables.h"
#include "TNode.h"

typedef std::vector<PkbTables::LINE_NO> STRING_VECTOR;
typedef std::unordered_set<PkbTables::LINE_NO> STRING_SET;
typedef std::pair<std::vector<std::string>, std::vector<std::string>>
    STRING_PAIRS;

class PkbQueryInterface {
private:
  PkbTables pkbTables;
  Variable variable;
  Procedure procedure;
  Underscore underscore;

  // Tables from PkbTables
  PkbTables::VAR_TABLE varTable;
  PkbTables::PROC_TABLE procTable;
  PkbTables::USES_TABLE usesTable;
  PkbTables::USES_PROC_TABLE usesProcTable;
  PkbTables::MODIFIES_TABLE modifiesTable;
  PkbTables::MODIFIES_PROC_TABLE modifiesProcTable;
  PkbTables::FOLLOW_TABLE followTable;
  PkbTables::PARENT_TABLE parentTable;
  PkbTables::STATEMENT_PROC_TABLE statementProcTable;
  PkbTables::STATEMENT_TYPE_TABLE statementTypeTable;
  PkbTables::ASSIGN_AST_TABLE assignAstTable;
  PkbTables::CONSTANT_TABLE constTable;

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
  PkbQueryInterface() {}
  PkbQueryInterface(PkbTables pkbTables) {
    this->pkbTables = pkbTables;
    this->varTable = pkbTables.getVarTable();
    this->procTable = pkbTables.getProcTable();
    this->usesTable = pkbTables.getUsesTable();
    this->usesProcTable = pkbTables.getUsesProcTable();
    this->modifiesTable = pkbTables.getModifiesTable();
    this->modifiesProcTable = pkbTables.getModifiesProcTable();
    this->followTable = pkbTables.getFollowTable();
    this->parentTable = pkbTables.getParentTable();
    this->statementProcTable = pkbTables.getStatementProcTable();
    this->statementTypeTable = pkbTables.getStatementTypeTable();
    this->assignAstTable = pkbTables.getAssignAstTable();
    this->constTable = pkbTables.getConstantTable();

    this->prevLineTable = pkbTables.invert(pkbTables.getFollowTable());
    this->childrenTable = pkbTables.pseudoinvert(pkbTables.getParentTable());
    this->invertStatementTypeTable =
        pkbTables.pseudoinvert(pkbTables.getStatementTypeTable());
    this->invertStatementProcTable =
        pkbTables.pseudoinvert(pkbTables.getStatementProcTable());
    this->invertVarTable = pkbTables.invert(pkbTables.getVarTable());
    this->invertProcTable = pkbTables.invert(pkbTables.getProcTable());

    this->closeFollowTable = pkbTables.close(pkbTables.getFollowTable());
    this->closeParentTable = pkbTables.close(pkbTables.getParentTable());
    this->closePrevLineTable = pkbTables.close(prevLineTable);
    this->closeChildrenTable = pkbTables.closeFlatten(childrenTable);

    this->usesTableTransited = pkbTables.transit(pkbTables.getUsesTable(),
                                                 pkbTables.getUsesProcTable());
    this->invertUsesTable =
        pkbTables.pseudoinvertFlattenKeys(usesTableTransited);
    this->modifiesTableTransited = pkbTables.transit(
        pkbTables.getModifiesTable(), pkbTables.getModifiesProcTable());
    this->invertModifiesTable =
        pkbTables.pseudoinvertFlattenKeys(modifiesTableTransited);

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
