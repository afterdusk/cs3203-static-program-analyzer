#pragma once

#include "KeysTable.h"
#include "TNode.h"
#include <set>
#include <string>
#include <unordered_set>
#include <variant>

/** @brief The Frontend - PKB interface. */
class PkbTables {
public:
  typedef std::string PROC;
  typedef std::unordered_set<PROC> PROCS;
  typedef std::string VAR;
  typedef std::unordered_set<VAR> VARS;
  typedef std::size_t LINE_NO;
  typedef std::unordered_set<LINE_NO> LINE_NOS;
  typedef std::variant<VARS, PROC> USES;
  typedef std::variant<VARS, PROC> MODIFIES;
  typedef LINE_NO FOLLOW;
  typedef std::unordered_set<FOLLOW> FOLLOWS;
  typedef LINE_NO PARENT;
  typedef std::unordered_set<PARENT> PARENTS;
  typedef LINE_NO CHILD;
  typedef std::unordered_set<CHILD> CHILDREN;
  enum class StatementType {
    Read,
    Print,
    Assign,
    Call,
    While,
    If,
  };
  typedef std::shared_ptr<TNode> AST;
  typedef std::string CONSTANT;
  typedef PROC CALL;
  typedef std::unordered_set<CALL> CALLS;
  typedef LINE_NO NEXT;
  typedef std::unordered_set<NEXT> NEXTS;
  enum class CallBranch {
    JumpTo,
    ReturnFrom,
  };
  typedef std::vector<LINE_NO> CALL_BRANCH_LABEL;
  typedef std::variant<NEXT, std::tuple<LINE_NO, CallBranch, CALL_BRANCH_LABEL>>
      NEXT_BIP;
  typedef std::set<NEXT_BIP> NEXT_BIPS;
  typedef LINE_NO ASSIGNMENT;
  typedef ASSIGNMENT AFFECT;
  typedef std::unordered_set<AFFECT> AFFECTS;

  typedef std::unordered_set<VAR> VAR_TABLE;
  typedef std::unordered_set<PROC> PROC_TABLE;
  typedef KeysTable<LINE_NO, USES> USES_TABLE;
  typedef KeysTable<PROC, VARS> USES_PROC_TABLE;
  typedef KeysTable<LINE_NO, MODIFIES> MODIFIES_TABLE;
  typedef KeysTable<PROC, VARS> MODIFIES_PROC_TABLE;
  typedef KeysTable<LINE_NO, FOLLOW> FOLLOW_TABLE;
  typedef KeysTable<LINE_NO, PARENT> PARENT_TABLE;
  typedef KeysTable<LINE_NO, PROC> STATEMENT_PROC_TABLE;
  typedef KeysTable<LINE_NO, StatementType> STATEMENT_TYPE_TABLE;
  typedef KeysTable<LINE_NO, AST> ASSIGN_AST_TABLE;
  typedef KeysTable<LINE_NO, VARS> CONDITION_VARS_TABLE;
  typedef std::unordered_set<CONSTANT> CONSTANT_TABLE;
  typedef KeysTable<PROC, CALLS> CALLS_TABLE;
  typedef KeysTable<LINE_NO, NEXTS> NEXTS_TABLE;
  typedef KeysTable<LINE_NO, NEXT_BIPS> NEXT_BIPS_TABLE;

  /** @brief Gets the varTable.
  @return The varTable.
  */
  virtual const VAR_TABLE &getVarTable() const = 0;

  /** @brief Gets the procTable.
  @return The procTable.
  */
  virtual const PROC_TABLE &getProcTable() const = 0;

  /** @brief Gets the usesTable.
  @return The usesTable.
  */
  virtual const USES_TABLE &getUsesTable() const = 0;

  /** @brief Gets the usesProcTable.
  @return The usesProcTable.
  */
  virtual const USES_PROC_TABLE &getUsesProcTable() const = 0;

  /** @brief Gets the modifiesTable.
  @return The modifiesTable.
  */
  virtual const MODIFIES_TABLE &getModifiesTable() const = 0;

  /** @brief Gets the modifiesProcTable.
  @return The modifiesProcTable.
  */
  virtual const MODIFIES_PROC_TABLE &getModifiesProcTable() const = 0;

  /** @brief Gets the followTable.
  @return The followTable.
  */
  virtual const FOLLOW_TABLE &getFollowTable() const = 0;

  /** @brief Gets the parentTable.
  @return The parentTable.
  */
  virtual const PARENT_TABLE &getParentTable() const = 0;

  /** @brief Gets the statementProcTable.
  @return The statementProcTable.
  */
  virtual const STATEMENT_PROC_TABLE &getStatementProcTable() const = 0;

  /** @brief Gets the statementTypeTable.
  @return The statementTypeTable.
  */
  virtual const STATEMENT_TYPE_TABLE &getStatementTypeTable() const = 0;

  /** @brief Gets the assignAstTable.
  @return The assignAstTable.
  */
  virtual const ASSIGN_AST_TABLE &getAssignAstTable() const = 0;

  /** @brief Gets the conditionVarsTable.
  @return The conditionVarsTable.
  */
  virtual const CONDITION_VARS_TABLE &getConditionVarsTable() const = 0;

  /** @brief Gets the constantTable.
  @return The constantTable.
  */
  virtual const CONSTANT_TABLE &getConstantTable() const = 0;

  /** @brief Gets the callsTable.
  @return The callsTable.
  */
  virtual const CALLS_TABLE &getCallsTable() const = 0;

  /** @brief Gets the nextsTable.
  @return The nextsTable.
  */
  virtual const NEXTS_TABLE &getNextsTable() const = 0;

  /** @brief Gets the nextBipsTable.
  @return The nextBipsTable.
  */
  virtual const NEXT_BIPS_TABLE &getNextBipsTable() const = 0;

  /** @brief Adds var to varTable if var is not in varTable.
  @param var Variable to be added to varTable.
  */
  virtual void addVar(VAR var) = 0;

  /** @brief Adds proc to procTable if proc is not in procTable.
  @param proc Procedure to be added to procTable.
  */
  virtual void addProc(PROC proc) = 0;

  /** @brief Adds {lineNo, uses} to usesTable if lineNo is not in usesTable.
  @param lineNo Line number of the SIMPLE code.
  @param uses Uses to be added to usesTable.
  */
  virtual void addUses(LINE_NO lineNo, USES uses) = 0;

  /** @brief Adds {proc, vars} to usesProcTable if
  proc is not in usesProcTable.
  @param proc Procedure to be added to usesProcTable.
  @param vars Variables to be added to usesProcTable.
  */
  virtual void addUsesProc(PROC proc, VARS vars) = 0;

  /** @brief Adds {lineNo, modifies} to modifiesTable if lineNo is not in
  modifiesTable.
  @param lineNo Line number of the SIMPLE code.
  @param modifies Modifies to be added to modifiesTable.
  */
  virtual void addModifies(LINE_NO lineNo, MODIFIES modifies) = 0;

  /** @brief Adds {proc, vars} to modifiesProcTable if
  proc is not in modifiesProcTable.
  @param proc Procedure to be added to modifiesProcTable.
  @param vars Variables to be added to modifiesProcTable.
  */
  virtual void addModifiesProc(PROC proc, VARS vars) = 0;

  /** @brief Adds {lineNo, follow} to followTable if lineNo is not in
  followTable.
  @param lineNo Line number of the SIMPLE code.
  @param follow Follow to be added to followTable.
  */
  virtual void addFollow(LINE_NO lineNo, FOLLOW follow) = 0;

  /** @brief Adds {child, parent} to parentTable if child is not in parentTable.
  @param child Child to be added to parentTable.
  @param parent Parent to be added to parentTable.
  */
  virtual void addParent(CHILD child, PARENT parent) = 0;

  /** @brief Adds {lineNo, statementProc} to statementProcTable if lineNo
  is not in statementProcTable.
  @param lineNo Line number of the SIMPLE code.
  @param statementProc StatementProc to be added to statementProcTable.
  */
  virtual void addStatementProc(LINE_NO lineNo, PROC statementProc) = 0;

  /** @brief Adds {lineNo, statementType} to statementTypeTable if lineNo
  is not in statementTypeTable.
  @param lineNo Line number of the SIMPLE code.
  @param statementType StatementType to be added to statementTypeTable.
  */
  virtual void addStatementType(LINE_NO lineNo,
                                StatementType statementType) = 0;

  /** @brief Adds {lineNo, ast} to assignAstTable if lineNo is not in
  assignAstTable.
  @param lineNo Line number of the SIMPLE code.
  @param ast Abstract syntax tree to be added to assignAstTable.
  */
  virtual void addAssignAst(LINE_NO lineNo, AST ast) = 0;

  /** @brief Adds {lineNo, vars} to conditionVarsTable if lineNo is
  not in conditionVarsTable.
  @param lineNo Line number of the SIMPLE code.
  @param vars Variables to be added to conditionVarsTable.
  */
  virtual void addConditionVars(LINE_NO lineNo, VARS vars) = 0;

  /** @brief Adds constant to constantTable if constant is not in constantTable.
  @param constant Constant to be added to constantTable.
  */
  virtual void addConstant(CONSTANT constant) = 0;

  /** @brief Adds call to callsTable.map.
  If callsTable.map does not map proc, then maps proc to a
  std::unordered_set with one element, call. Otherwise, calls
  callsTable.map[key]::insert on call.
  @param proc Procedure to be added to callsTable.
  @param call call to be added to callsTable.map.
  */
  virtual void addCall(PROC proc, CALL call) = 0;

  /** @brief Adds next to nextsTable.map.
  If nextsTable.map does not map lineNo, then maps lineNo to a
  std::unordered_set with one element, next. Otherwise, calls
  nextsTable.map[key]::insert on next.
  @param lineNo Line number of the SIMPLE code.
  @param next The next line number to be added to nextsTable.map.
  */
  virtual void addNext(PkbTables::LINE_NO lineNo, PkbTables::NEXT next) = 0;

  /** @brief Adds nextBip to nextBipsTable.map.
  If nextBipsTable.map does not map lineNo, then maps lineNo to a
  std::unordered_set with one element, nextBip. Otherwise, calls
  nextBipsTable.map[key]::insert on nextBip.
  @param lineNo Line number of the SIMPLE code.
  @param nextBip The nextBip to be added to nextBipsTable.map.
  */
  virtual void addNextBip(PkbTables::LINE_NO lineNo,
                          PkbTables::NEXT_BIP nextBip) = 0;

  /** @brief Creates derived tables. */
  virtual void deriveTables() = 0;

protected:
  VAR_TABLE varTable;            /**< A std::unordered_set of VAR. */
  PROC_TABLE procTable;          /**< A std::unordered_set of PROC. */
  USES_TABLE usesTable;          /**< A KeysTable mapping LINE_NO to USES. */
  USES_PROC_TABLE usesProcTable; /**< A KeysTable mapping PROC to
                                    VARS. */
  MODIFIES_TABLE modifiesTable; /**< A KeysTable mapping LINE_NO to MODIFIES. */
  MODIFIES_PROC_TABLE
  modifiesProcTable;        /**< A KeysTable mapping PROC to
                               VARS. */
  FOLLOW_TABLE followTable; /**< A KeysTable mapping LINE_NO to FOLLOW. */
  PARENT_TABLE parentTable; /**< A KeysTable mapping LINE_NO to PARENT. */
  STATEMENT_PROC_TABLE
  statementProcTable; /**< A KeysTable mapping LINE_NO to PROC. */
  STATEMENT_TYPE_TABLE
  statementTypeTable; /**< A KeysTable mapping LINE_NO to StatementType. */
  ASSIGN_AST_TABLE assignAstTable; /**< A KeysTable mapping LINE_NO to AST. */
  CONDITION_VARS_TABLE conditionVarsTable; /**< A KeysTable mapping LINE_NO
                                          to VARS. */
  CONSTANT_TABLE constantTable; /**< A std::unordered_set of CONSTANT. */
  CALLS_TABLE callsTable;       /**< A KeysTable mapping PROC to CALLS. */
  NEXTS_TABLE nextsTable;       /**< A KeysTable mapping LINE_NO to NEXTS. */
  NEXT_BIPS_TABLE
  nextBipsTable; /**< A KeysTable mapping LINE_NO to NEXT_BIPS. */
};
