#pragma once

#include "PkbQueryInterface.h"
#include "PkbTables.h"

class Pkb : public PkbTables, PkbQueryInterface {
public:
  PkbTables *getTables();
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
  virtual const CONSTANT_TABLE &getConstantTable() const;
  virtual const CALLS_TABLE &getCallsTable() const;
  virtual VAR_TABLE_INDEX addVar(VAR var);
  virtual PROC_TABLE_INDEX addProc(PROC proc);
  virtual void addUses(LINE_NO lineNo, USES uses);
  virtual void addUsesProc(PROC_TABLE_INDEX procTableIndex,
                           VAR_TABLE_INDEXES varTableIndexes);
  virtual void addModifies(LINE_NO lineNo, MODIFIES modifies);
  virtual void addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                               VAR_TABLE_INDEXES varTableIndexes);
  virtual void addFollow(LINE_NO lineNo, FOLLOW follow);
  virtual void addParent(CHILD child, PARENT parent);
  virtual void addStatementProc(LINE_NO lineNo, PROC statementProc);
  virtual void addStatementType(LINE_NO lineNo, StatementType statementType);
  virtual void addAssignAst(LINE_NO lineNo, AST ast);
  virtual void addConstant(CONSTANT constant);
  virtual void addCall(PROC_TABLE_INDEX pti, CALL call);
  virtual void deriveTables();

  /* PkbQueryInterface */
  /*
   * Query API for pattern
   */
  virtual STRING_PAIRS match(Statement statement, Variable variable,
                             PatternSpec spec);
  virtual STRING_SET match(Statement statement, Underscore underscore,
                           PatternSpec spec);
  virtual STRING_SET match(Statement statement, String varName,
                           PatternSpec spec);

  /*
   * Query API for normal select
   */
  virtual STRING_SET select(Variable variable);
  virtual STRING_SET select(Statement statement);
  virtual STRING_SET select(Procedure procedure);
  virtual STRING_SET select(Constant constant);

  /*
   * Query API for follows
   */
  virtual bool follows(LineNumber line1, LineNumber line2);
  virtual STRING_SET follows(LineNumber line, Statement statement);
  virtual bool follows(LineNumber line, Underscore underscore);
  virtual STRING_SET follows(Statement statement, LineNumber line);
  virtual STRING_PAIRS follows(Statement statement1, Statement statement2);
  virtual STRING_SET follows(Statement statement, Underscore underscore);
  virtual bool follows(Underscore underscore, LineNumber line);
  virtual STRING_SET follows(Underscore underscore, Statement statement);
  virtual bool follows(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for followsStar
   */
  virtual bool followsStar(LineNumber line1, LineNumber line2);
  virtual STRING_SET followsStar(LineNumber line, Statement statement);
  virtual bool followsStar(LineNumber line, Underscore underscore);
  virtual STRING_SET followsStar(Statement statement, LineNumber line);
  virtual STRING_PAIRS followsStar(Statement statement1, Statement statement2);
  virtual STRING_SET followsStar(Statement statement, Underscore underscore);
  virtual bool followsStar(Underscore underscore, LineNumber line);
  virtual STRING_SET followsStar(Underscore underscore, Statement statement);
  virtual bool followsStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for parent
   */
  virtual bool parent(LineNumber line1, LineNumber line2);
  virtual STRING_SET parent(LineNumber line, Statement statement);
  virtual bool parent(LineNumber line, Underscore underscore);
  virtual STRING_SET parent(Statement statement, LineNumber line);
  virtual STRING_PAIRS parent(Statement statement1, Statement statement2);
  virtual STRING_SET parent(Statement statement, Underscore underscore);
  virtual bool parent(Underscore underscore, LineNumber line);
  virtual STRING_SET parent(Underscore underscore, Statement statement);
  virtual bool parent(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for parentStar
   */
  virtual bool parentStar(LineNumber line1, LineNumber line2);
  virtual STRING_SET parentStar(LineNumber line, Statement statement);
  virtual bool parentStar(LineNumber line, Underscore underscore);
  virtual STRING_SET parentStar(Statement statement, LineNumber line);
  virtual STRING_PAIRS parentStar(Statement statement1, Statement statement2);
  virtual STRING_SET parentStar(Statement statement, Underscore underscore);
  virtual bool parentStar(Underscore underscore, LineNumber line);
  virtual STRING_SET parentStar(Underscore underscore, Statement statement);
  virtual bool parentStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for uses
   */
  virtual bool uses(LineNumber line, String variableName);
  virtual STRING_SET uses(LineNumber line, Variable variable);
  virtual bool uses(LineNumber line, Underscore underscore);
  virtual STRING_SET uses(Statement statement, String variableName);
  virtual STRING_PAIRS uses(Statement statement, Variable variable);
  virtual STRING_SET uses(Statement statement, Underscore underscore);
  virtual bool uses(String procedureName, String variableName);
  virtual STRING_SET uses(String procedureName, Variable variable);
  virtual bool uses(String procedureName, Underscore underscore);
  virtual STRING_SET uses(Procedure procedure, String variableName);
  virtual STRING_PAIRS uses(Procedure procedure, Variable variable);
  virtual STRING_SET uses(Procedure procedure, Underscore underscore);

  /*
   * Query API for modifies
   */
  virtual bool modifies(LineNumber line, String variableName);
  virtual STRING_SET modifies(LineNumber line, Variable variable);
  virtual bool modifies(LineNumber line, Underscore underscore);
  virtual STRING_SET modifies(Statement statement, String variableName);
  virtual STRING_PAIRS modifies(Statement statement, Variable variable);
  virtual STRING_SET modifies(Statement, Underscore underscore);
  virtual bool modifies(String procedureName, String variableName);
  virtual STRING_SET modifies(String procedureName, Variable variable);
  virtual bool modifies(String procedureName, Underscore underscore);
  virtual STRING_SET modifies(Procedure, String variableName);
  virtual STRING_PAIRS modifies(Procedure, Variable variable);
  virtual STRING_SET modifies(Procedure, Underscore underscore);
};
