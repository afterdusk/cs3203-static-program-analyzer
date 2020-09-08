#pragma once

#include <string>
#include <utility>
#include <vector>

#include "PKB.h"
#include "PkbQueryResultAndEntityTypes.h"

typedef std::vector<LINE_NO> StmtNumberList;
typedef std::vector<std::string> StringList;
typedef std::pair<std::vector<LINE_NO>, std::vector<LINE_NO>> StmtNumberPairs;
typedef std::pair<std::vector<std::string>, std::vector<std::string>>
    StringPairs;
typedef std::pair<std::vector<LINE_NO>, std::vector<std::string>>
    StmtNumberAndStringPairs;

class PkbQueryInterface {
private:
  PKB pkb;
  Stmt stmt;
  Variable variable;
  Procedure procedure;
  Underscore underscore;

  // Tables from PKB
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

  // Derived tables using original tables from PKB
  KeysTable<LINE_NO, LINE_NO> prevLineTable; // invert of followTable
  KeysTable<LINE_NO, std::vector<LINE_NO>>
      childrenTable; // pseudo invert of parentTable
  KeysTable<StatementType, std::vector<LINE_NO>>
      invertStatementTypeTable; // pseudo invert of parentTable

  KeysTable<LINE_NO, std::vector<LINE_NO>> closeFollowTable;
  KeysTable<LINE_NO, std::vector<LINE_NO>> closeParentTable;
  KeysTable<LINE_NO, std::vector<LINE_NO>> closePrevLineTable;
  KeysTable<LINE_NO, std::vector<LINE_NO>> closeChildrenTable;

public:
  PkbQueryInterface() {}
  PkbQueryInterface(PKB pkb) {
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

    this->prevLineTable = pkb.invert(followTable);
    this->childrenTable = pkb.pseudoinvert(parentTable);
    this->invertStatementTypeTable = pkb.pseudoinvert(statementTypeTable);

    this->closeFollowTable = pkb.close(followTable);
    this->closeParentTable = pkb.close(parentTable);
    this->closePrevLineTable = pkb.close(prevLineTable);
    /*this->closeChildrenTable = pkb.close(childrenTable);*/
  }

  /*
   * Query API for pattern
   */
  // Result<StmtNumberAndStringPairs> match(Statement statement, Variable
  // variable,
  //                            PatternSpec spec);
  // Result<StmtNumberList> match(Statement statement, Underscore underscore,
  //                            PatternSpec spec);
  // Result<StmtNumberList> match(Statement statement, String varName,
  //                            PatternSpec spec);

  /*
   * Query API for normal select
   */
  // Result<StringList> select(Variable);
  // Result<StmtNumberList> select(Stmt);
  // Result<StmtNumberList> select(Statement);
  // Result<StringList> select(Procedure);

  /*
   * Query API for follows
   */
  Result<bool> follows(LineNumber line1, LineNumber line2);

  Result<StmtNumberList> follows(LineNumber line, Statement statement);

  Result<StmtNumberList> follows(LineNumber line, Stmt stmt);

  Result<bool> follows(LineNumber line, Underscore underscore);

  Result<StmtNumberList> follows(Statement statement, LineNumber line);

  Result<StmtNumberPairs> follows(Statement statement1, Statement statement2);

  Result<StmtNumberPairs> follows(Statement statement, Stmt stmt);

  Result<StmtNumberList> follows(Statement statement, Underscore underscore);

  Result<StmtNumberList> follows(Stmt stmt, LineNumber line);

  Result<StmtNumberPairs> follows(Stmt stmt, Statement statement);
  // Result<StmtNumberPairs> follows(Stmt stmt1, Stmt stmt2);
  // Result<StmtNumberList> follows(Stmt stmt, Underscore underscore)

  Result<bool> follows(Underscore underscore, LineNumber line);

  Result<StmtNumberList> follows(Underscore underscore, Statement statement);
  // Result<StmtNumberList> follows(Underscore underscore, Stmt stmt)
  // Result<bool> follows(Underscore underscore, Underscore underscore)

  /*
   * Query API for followsStar
   */
  Result<bool> followsStar(LineNumber line1, LineNumber line2);

  Result<StmtNumberList> followsStar(LineNumber line, Statement statement);

  Result<StmtNumberList> followsStar(LineNumber line, Stmt stmt);

  Result<bool> followsStar(LineNumber line, Underscore underscore);

  // Result<StmtNumberList> followsStar(Statement statement, LineNumber line);

  // Result<StmtNumberPairs> followsStar(Statement statement1, Statement
  // statement2) Result<StmtNumberPairs> followsStar(Statement statement, Stmt
  // stmt) Result<StmtNumberList> followsStar(Statement statement, Underscore
  // underscore)

  // Result<StmtNumberList> followsStar(Stmt stmt, LineNumber line);

  // Result<StmtNumberPairs> followsStar(Stmt stmt, Statement statement);
  // Result<StmtNumberPairs> followsStar(Stmt stmt1, Stmt stmt2);
  // Result<StmtNumberList> followsStar(Stmt stmt, Underscore underscore)

  Result<bool> followsStar(Underscore underscore, LineNumber line);
  // Result<StmtNumberList> followsStar(Underscore underscore, Statement
  // statement) Result<StmtNumberList> followsStar(Underscore underscore, Stmt
  // stmt) Result<bool> followsStar(Underscore underscore, Underscore
  // underscore)

  /*
   * Query API for parent
   */

  Result<bool> parent(LineNumber line1, LineNumber line2);

  Result<StmtNumberList> parent(LineNumber line, Statement statement);

  Result<StmtNumberList> parent(LineNumber line, Stmt stmt);

  Result<bool> parent(LineNumber line, Underscore underscore);

  Result<StmtNumberList> parent(Statement statement, LineNumber line);

  // Result<StmtNumberPairs> parent(Statement statement, Statement statement)
  // Result<StmtNumberPairs> parent(Statement statement, Stmt stmt)
  // Result<StmtNumberList> parent(Statement statement, Underscore underscore)

  Result<StmtNumberList> parent(Stmt stmt, LineNumber line);
  // Result<StmtNumberPairs> parent(Stmt stmt, Statement statement)
  // Result<StmtNumberPairs> parent(Stmt stmt, Stmt stmt)
  // Result<StmtNumberList> parent(Stmt stmt, Underscore underscore)

  Result<bool> parent(Underscore underscore, LineNumber line);
  // Result<StmtNumberList> parent(Underscore underscore, Statement statement)
  // Result<StmtNumberList> parent(Underscore underscore, Stmt stmt)
  // Result<bool> parent(Underscore underscore, Underscore underscore)

  /*
   * Query API for parentStar
   */

  Result<bool> parentStar(LineNumber line1, LineNumber line2);

  // Result<StmtNumberList> parentStar(LineNumber line, Statement statement);

  // Result<StmtNumberList> parentStar(LineNumber line, Stmt stmt);

  Result<bool> parentStar(LineNumber line, Underscore underscore);

  // Result<StmtNumberList> parentStar(Statement statement, LineNumber line);

  // Result<StmtNumberPairs> parentStar(Statement statement, Statement
  // statement) Result<StmtNumberPairs> parentStar(Statement statement, Stmt
  // stmt) Result<StmtNumberList> parentStar(Statement statement, Underscore
  // underscore)

  // Result<StmtNumberList> parentStar(Stmt stmt, LineNumber line)
  // Result<StmtNumberPairs> parentStar(Stmt stmt, Statement statement)
  // Result<StmtNumberPairs> parentStar(Stmt stmt, Stmt stmt)
  // Result<StmtNumberList> parentStar(Stmt stmt, Underscore underscore)

  Result<bool> parentStar(Underscore underscore, LineNumber line);
  // Result<StmtNumberList> parentStar(Underscore underscore, Statement
  // statement) Result<StmtNumberList> parentStar(Underscore underscore, Stmt
  // stmt) Result<bool> parentStar(Underscore underscore, Underscore underscore)

  /*
   * Query API for uses
   */

  // Result<bool> uses(LineNumber line, String variableName)
  // Result<StringList> uses(LineNumber line, Variable variable)
  // Result<bool> uses(LineNumber line, Underscore underscore)
  //
  // Result<StmtNumberList> uses(Stmt stmt, String variableName)
  // Result<StmtNumberAndStringPairs> uses(Stmt stmt, Variable variable)
  // Result<StmtNumberList> uses(Stmt, Underscore underscore)

  // Result<StmtNumberList> uses(Statement statement, String variableName)
  // Result<StmtNumberAndStringPairs> uses(Statement statement, Variable
  // variable) Result<StmtNumberList> uses(Statement statement, Underscore
  // underscore)
  //
  // Result<bool> uses(String procedureName, String variableName)
  // Result<StringList> uses(String procedureName, Variable variable)
  // Result<bool> (String procedureName, Underscore underscore)
  //
  // Result<StringList> uses(Procedure, String variableName)
  // Result<StringPairs> uses(Procedure, Variable variable)
  // Result<StringList> uses(Procedure, Underscore underscore)

  /*
   * Query API for modifies
   */

  // Result<bool> modifies(LineNumber line, String variableName)
  // Result<StringList> modifies(LineNumber line, Variable variable)
  // Result<bool> modifies(LineNumber line, Underscore underscore) same as^
  //
  // Result<StmtNumberList> modifies(Stmt stmt, String variableName)
  // Result<StmtNumberAndStringPairs> modifies(Stmt stmt, Variable variable)
  // Result<StmtNumberList> modifies(Stmt stmt, Underscore underscore)
  //
  // Result<StmtNumberList> modifies(Statement statement, String variableName)
  // Result<StmtNumberAndStringPairs> modifies(Statement statement, Variable
  // variable) Result<StmtNumberList> modifies(Statement, Underscore underscore)
  //
  // Result<bool> modifies(String procedureName, String variableName)
  // Result<StringList> modifies(String procedureName, Variable variable)
  // Result<bool> modifies(String procedureName, Underscore underscore)
  //
  // Result<StringList> modifies(Procedure, String variableName)
  // Result<StringPairs> modifies(Procedure, Variable variable)
  // Result<StringList> modifies(Procedure, Underscore underscore)
};
