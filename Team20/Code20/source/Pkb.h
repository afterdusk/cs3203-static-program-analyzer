#pragma once

#include "PkbQueryInterface.h"
#include "PkbTableTransformers.h"
#include "PkbTables.h"

/** @brief The concrete class that implements both the Frontend - PKB interface,
as well as the PQL - PKB interface.
*/
class Pkb : public PkbTables, PkbQueryInterface {
protected:
  virtual LINE_SET
  getTransitiveNextStatements(PkbTables::LINE_NO lineNo,
                              PkbTables::LINE_NOS lineNosVisited);
  virtual LINE_SET
  getTransitivePrevStatements(PkbTables::LINE_NO lineNo,
                              PkbTables::LINE_NOS lineNosVisited);
  virtual LINE_SET getAffectedStatements(LINE_NO lineNo);
  virtual LINE_SET getAffectedAux(VAR modifiedVar, LINE_NO lineNo,
                                  LINE_NOS lineNosVisited);
  virtual LINE_SET getAffectorStatements(LINE_NO lineNo);
  virtual LINE_SET getAffectorAux(PkbTables::VAR usedVar,
                                  PkbTables::LINE_NO lineNo,
                                  PkbTables::LINE_NOS lineNosVisited);
  virtual LINE_SET getTransitiveAffectedStatements(LINE_NO lineNo,
                                                   LINE_NOS lineNosVisited);
  virtual LINE_SET getTransitiveAffectorStatements(LINE_NO lineNo,
                                                   LINE_NOS lineNosVisited);
  virtual bool checkReachLastStmtInProc(LINE_NO line, VAR var,
                                        LINE_NOS linesVisited);
  virtual LINE_SET getAffectedBipStatements(LINE_NO lineNo, VAR modifiedVar);
  virtual LINE_SET getAffectedBipAux(VAR modifiedVar, LINE_NO lineNo,
                                     LINE_NOS lineNosVisited);
  virtual KeysTable<LINE_NO, LINE_NOS> deriveAffectsBipTable();
  virtual void deriveAllAffectsBipRelatedTables();

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
  virtual const NEXTS_TABLE &getNextsTable() const;
  virtual const NEXT_BIPS_TABLE &getNextBipsTable() const;
  virtual const CFG_NODE_TABLE &getCfgNodeTable() const;
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
  virtual void addNext(LINE_NO lineNo, NEXT next);
  virtual void addNextBip(LINE_NO lineNo, NEXT_BIP nextBip);
  virtual void addCfgNode(LINE_NO lineNo, CFG_NODE cfgNode);
  virtual void deriveTables();

  /* PkbQueryInterface */

  virtual void clearCache();

  /*
   * API for PQL to handle attributes
   */
  virtual LINE_NAME_PAIRS selectAttribute(Statement statement);

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
  virtual NAME_SET select(Variable variable);
  virtual LINE_SET select(Statement statement);
  virtual NAME_SET select(Procedure procedure);
  virtual CONST_SET select(Constant constant);

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

  /*
   * Query API for next
   */
  virtual bool next(LineNumber line1, LineNumber line2);
  virtual LINE_SET next(LineNumber line, Statement statement);
  virtual bool next(LineNumber line, Underscore underscore);
  virtual LINE_SET next(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS next(Statement statement1, Statement statement2);
  virtual LINE_SET next(Statement statement, Underscore underscore);
  virtual bool next(Underscore underscore, LineNumber line);
  virtual LINE_SET next(Underscore underscore, Statement statement);
  virtual bool next(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for nextStar
   */
  virtual bool nextStar(LineNumber line1, LineNumber line2);
  virtual LINE_SET nextStar(LineNumber line, Statement statement);
  virtual bool nextStar(LineNumber line, Underscore underscore);
  virtual LINE_SET nextStar(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS nextStar(Statement statement1, Statement statement2);
  virtual LINE_SET nextStar(Statement statement, Underscore underscore);
  virtual bool nextStar(Underscore underscore, LineNumber line);
  virtual LINE_SET nextStar(Underscore underscore, Statement statement);
  virtual bool nextStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for affects
   */
  virtual bool affects(LineNumber line1, LineNumber line2);
  virtual LINE_SET affects(LineNumber line, Statement statement);
  virtual bool affects(LineNumber line, Underscore underscore);
  virtual LINE_SET affects(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS affects(Statement statement1, Statement statement2);
  virtual LINE_SET affects(Statement statement, Underscore underscore);
  virtual bool affects(Underscore underscore, LineNumber line);
  virtual LINE_SET affects(Underscore underscore, Statement statement);
  virtual bool affects(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for affectsStar
   */
  virtual bool affectsStar(LineNumber line1, LineNumber line2);
  virtual LINE_SET affectsStar(LineNumber line, Statement statement);
  virtual bool affectsStar(LineNumber line, Underscore underscore);
  virtual LINE_SET affectsStar(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS affectsStar(Statement statement1,
                                      Statement statement2);
  virtual LINE_SET affectsStar(Statement statement, Underscore underscore);
  virtual bool affectsStar(Underscore underscore, LineNumber line);
  virtual LINE_SET affectsStar(Underscore underscore, Statement statement);
  virtual bool affectsStar(Underscore underscore1, Underscore underscore2);

  /*
   * Query API for affectsBip
   */
  virtual bool affectsBip(LineNumber line1, LineNumber line2);
  virtual LINE_SET affectsBip(LineNumber line, Statement statement);
  virtual bool affectsBip(LineNumber line, Underscore underscore);
  virtual LINE_SET affectsBip(Statement statement, LineNumber line);
  virtual LINE_LINE_PAIRS affectsBip(Statement statement1,
                                     Statement statement2);
  virtual LINE_SET affectsBip(Statement statement, Underscore underscore);
  virtual bool affectsBip(Underscore underscore, LineNumber line);
  virtual LINE_SET affectsBip(Underscore underscore, Statement statement);
  virtual bool affectsBip(Underscore underscore1, Underscore underscore2);
};
