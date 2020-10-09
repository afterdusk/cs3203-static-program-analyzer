#pragma once

#include "PkbQueryInterface.h"
#include "PkbTableTransformers.h"
#include "PkbTables.h"

/** @brief The concrete class that implements both the Frontend - PKB interface,
as well as the PQL - PKB interface.
*/
class Pkb : public PkbTables, PkbQueryInterface {
public:
  /** @brief
  @return The Pkb class upcasted to PkbTables.
  */
  PkbTables *getTables();

  /** @brief
  @return The Pkb class upcasted to PkbQueryInterface.
  */
  PkbQueryInterface *getQueryInterface();

  /* PkbTables */
  virtual const VAR_TABLE &getVarTable() const;
  virtual const PROC_TABLE &getProcTable() const;
  virtual const USES_TABLE &getUsesTable() const;
  virtual const USES_PROC_TABLE &getUsesProcTable() const;
  virtual const MODIFIES_TABLE &getModifiesTable() const;
  virtual const MODIFIES_PROC_TABLE &getModifiesProcTable() const;
  virtual const FOLLOW_TABLE &getFollowTable() const;
  virtual const PARENT_TABLE &getParentTable() const;
  virtual const STATEMENT_PROC_TABLE &getStatementProcTable() const;
  virtual const STATEMENT_TYPE_TABLE &getStatementTypeTable() const;
  virtual const ASSIGN_AST_TABLE &getAssignAstTable() const;
  virtual const CONDITION_VARS_TABLE &getConditionVarsTable() const;
  virtual const CONSTANT_TABLE &getConstantTable() const;
  virtual const CALLS_TABLE &getCallsTable() const;
  virtual const NEXT_TABLE &getNextTable() const;
  virtual const NEXT_BIP_TABLE &getNextBipTable() const;
  virtual void addVar(VAR var);
  virtual void addProc(PROC proc);
  virtual void addUses(LINE_NO lineNo, USES uses);
  virtual void addUsesProc(PROC proc, VARS vars);
  virtual void addModifies(LINE_NO lineNo, MODIFIES modifies);
  virtual void addModifiesProc(PROC proc, VARS vars);
  virtual void addFollow(LINE_NO lineNo, FOLLOW follow);
  virtual void addParent(CHILD child, PARENT parent);
  virtual void addStatementProc(LINE_NO lineNo, PROC statementProc);
  virtual void addStatementType(LINE_NO lineNo, StatementType statementType);
  virtual void addAssignAst(LINE_NO lineNo, AST ast);
  virtual void addConditionVars(LINE_NO lineNo, VARS vars);
  virtual void addConstant(CONSTANT constant);
  virtual void addCall(PROC proc, CALL call);
  virtual void addNext(PkbTables::LINE_NO lineNo, PkbTables::NEXT next);
  virtual void addNextBip(PkbTables::LINE_NO lineNo,
                          PkbTables::NEXT_BIP nextBip);
  virtual void deriveTables();

  /* PkbQueryInterface */

  /*
   * API for PQL to handle attributes
   */
  virtual LINE_NAME_PAIRS getStmtLineAndName(Statement statement);

  /*
   * Query API for pattern
   */
  virtual LINE_NAME_PAIRS match(Statement statement, Variable variable,
                                PatternSpec spec);
  virtual LINE_SET match(Statement statement, Underscore underscore,
                         PatternSpec spec);
  virtual LINE_SET match(Statement statement, String varName, PatternSpec spec);

  /*
   * Query API for normal select
   */
  virtual PkbTables::VAR_TABLE select(Variable variable);
  virtual LINE_SET select(Statement statement);
  virtual PkbTables::PROC_TABLE select(Procedure procedure);
  virtual PkbTables::CONSTANT_TABLE select(Constant constant);

  /*
   * Query API for follows
   */
  virtual bool follows(LineNumber line1, LineNumber line2);
  virtual LINE_SET follows(LineNumber line, Statement statement);
  virtual bool follows(LineNumber line, Underscore underscore);
  virtual LINE_SET follows(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS follows(Statement statement1, Statement statement2);
  virtual LINE_SET follows(Statement statement, Underscore underscore);
  virtual bool follows(Underscore underscore, LineNumber line);
  virtual LINE_SET follows(Underscore underscore, Statement statement);
  virtual bool follows(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for followsStar
   */
  virtual bool followsStar(LineNumber line1, LineNumber line2);
  virtual LINE_SET followsStar(LineNumber line, Statement statement);
  virtual bool followsStar(LineNumber line, Underscore underscore);
  virtual LINE_SET followsStar(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS followsStar(Statement statement1,
                                      Statement statement2);
  virtual LINE_SET followsStar(Statement statement, Underscore underscore);
  virtual bool followsStar(Underscore underscore, LineNumber line);
  virtual LINE_SET followsStar(Underscore underscore, Statement statement);
  virtual bool followsStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for parent
   */
  virtual bool parent(LineNumber line1, LineNumber line2);
  virtual LINE_SET parent(LineNumber line, Statement statement);
  virtual bool parent(LineNumber line, Underscore underscore);
  virtual LINE_SET parent(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS parent(Statement statement1, Statement statement2);
  virtual LINE_SET parent(Statement statement, Underscore underscore);
  virtual bool parent(Underscore underscore, LineNumber line);
  virtual LINE_SET parent(Underscore underscore, Statement statement);
  virtual bool parent(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for parentStar
   */
  virtual bool parentStar(LineNumber line1, LineNumber line2);
  virtual LINE_SET parentStar(LineNumber line, Statement statement);
  virtual bool parentStar(LineNumber line, Underscore underscore);
  virtual LINE_SET parentStar(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS parentStar(Statement statement1,
                                     Statement statement2);
  virtual LINE_SET parentStar(Statement statement, Underscore underscore);
  virtual bool parentStar(Underscore underscore, LineNumber line);
  virtual LINE_SET parentStar(Underscore underscore, Statement statement);
  virtual bool parentStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for uses
   */
  virtual bool uses(LineNumber line, String variableName);
  virtual NAME_SET uses(LineNumber line, Variable variable);
  virtual bool uses(LineNumber line, Underscore underscore);
  virtual LINE_SET uses(Statement statement, String variableName);
  virtual LINE_NAME_PAIRS uses(Statement statement, Variable variable);
  virtual LINE_SET uses(Statement statement, Underscore underscore);
  virtual bool uses(String procedureName, String variableName);
  virtual NAME_SET uses(String procedureName, Variable variable);
  virtual bool uses(String procedureName, Underscore underscore);
  virtual NAME_SET uses(Procedure procedure, String variableName);
  virtual NAME_NAME_PAIRS uses(Procedure procedure, Variable variable);
  virtual NAME_SET uses(Procedure procedure, Underscore underscore);

  /*
   * Query API for modifies
   */
  virtual bool modifies(LineNumber line, String variableName);
  virtual NAME_SET modifies(LineNumber line, Variable variable);
  virtual bool modifies(LineNumber line, Underscore underscore);
  virtual LINE_SET modifies(Statement statement, String variableName);
  virtual LINE_NAME_PAIRS modifies(Statement statement, Variable variable);
  virtual LINE_SET modifies(Statement, Underscore underscore);
  virtual bool modifies(String procedureName, String variableName);
  virtual NAME_SET modifies(String procedureName, Variable variable);
  virtual bool modifies(String procedureName, Underscore underscore);
  virtual NAME_SET modifies(Procedure, String variableName);
  virtual NAME_NAME_PAIRS modifies(Procedure, Variable variable);
  virtual NAME_SET modifies(Procedure, Underscore underscore);

  /*
   * Query API for calls
   */
  virtual bool calls(String procedureName1, String procedureName2);
  virtual NAME_SET calls(String procedureName, Procedure procedure);
  virtual bool calls(String procedureName, Underscore underscore);
  virtual NAME_SET calls(Procedure procedure, String procedureName);
  virtual NAME_NAME_PAIRS calls(Procedure procedure1, Procedure procedure2);
  virtual NAME_SET calls(Procedure procedure, Underscore underscore);
  virtual bool calls(Underscore underscore, String procedureName);
  virtual NAME_SET calls(Underscore underscore, Procedure procedure);
  virtual bool calls(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for callsStar
   */
  virtual bool callsStar(String procedureName1, String procedureName2);
  virtual NAME_SET callsStar(String procedureName, Procedure procedure);
  virtual bool callsStar(String procedureName, Underscore underscore);
  virtual NAME_SET callsStar(Procedure procedure, String procedureName);
  virtual NAME_NAME_PAIRS callsStar(Procedure procedure1, Procedure procedure2);
  virtual NAME_SET callsStar(Procedure procedure, Underscore underscore);
  virtual bool callsStar(Underscore underscore, String procedureName);
  virtual NAME_SET callsStar(Underscore underscore, Procedure procedure);
  virtual bool callsStar(Underscore underscore1, Underscore underscore2);

  virtual PkbTables::AFFECTS affects(PkbTables::ASSIGNMENT assignment);
  virtual PkbTables::AFFECTS affectsAux(PkbTables::VAR modifiesVar,
                                        PkbTables::LINE_NO lineNo);
};
