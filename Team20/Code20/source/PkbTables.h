#pragma once

#include "KeysTable.h"
#include "TNode.h"
#include <string>
#include <unordered_set>
#include <variant>

/** @brief The Frontend - PKB interface. */
class PkbTables {
public:
  typedef std::string PROC;
  typedef std::string VAR;
  typedef std::string LINE_NO;
  typedef std::unordered_set<LINE_NO> LINE_NOS;
  typedef uint64_t VAR_TABLE_INDEX;
  typedef std::unordered_set<VAR_TABLE_INDEX> VAR_TABLE_INDEXES;
  typedef uint64_t PROC_TABLE_INDEX;
  typedef std::unordered_set<PROC_TABLE_INDEX> PROC_TABLE_INDEXES;
  typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> USES;
  typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> MODIFIES;
  typedef LINE_NO FOLLOW;
  typedef std::unordered_set<FOLLOW> FOLLOWS;
  typedef LINE_NO PARENT;
  typedef std::unordered_set<PARENT> PARENTS;
  typedef LINE_NO CHILD;
  typedef std::unordered_set<CHILD> CHILDREN;
  enum class StatementType {
    NOTSET, // default constructor value
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
    NONE,
  };
  typedef TNode AST;
  typedef std::string CONSTANT;
  typedef PROC_TABLE_INDEX CALL;
  typedef std::unordered_set<CALL> CALLS;

  typedef KeysTable<VAR, VAR_TABLE_INDEX> VAR_TABLE;
  typedef KeysTable<PROC, PROC_TABLE_INDEX> PROC_TABLE;
  typedef KeysTable<LINE_NO, USES> USES_TABLE;
  typedef KeysTable<PROC_TABLE_INDEX, VAR_TABLE_INDEXES> USES_PROC_TABLE;
  typedef KeysTable<LINE_NO, MODIFIES> MODIFIES_TABLE;
  typedef KeysTable<PROC_TABLE_INDEX, VAR_TABLE_INDEXES> MODIFIES_PROC_TABLE;
  typedef KeysTable<LINE_NO, FOLLOW> FOLLOW_TABLE;
  typedef KeysTable<LINE_NO, PARENT> PARENT_TABLE;
  typedef KeysTable<LINE_NO, PROC> STATEMENT_PROC_TABLE;
  typedef KeysTable<LINE_NO, StatementType> STATEMENT_TYPE_TABLE;
  typedef KeysTable<LINE_NO, AST> ASSIGN_AST_TABLE;
  typedef std::unordered_set<CONSTANT> CONSTANT_TABLE;
  typedef KeysTable<PROC_TABLE_INDEX, CALLS> CALLS_TABLE;

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

  /** @brief Gets the constantTable.
  @return The constantTable.
  */
  virtual const CONSTANT_TABLE &getConstantTable() const = 0;

  /** @brief Gets the callsTable.
  @return the callsTable.
  */
  virtual const CALLS_TABLE &getCallsTable() const = 0;

  /** @brief Adds var to varTable if var is not in varTable.
  @param var Variable to be added to varTable.
  @return If var exists in varTable, return its existing index. Otherwise,
  return index of the added var.
  */
  virtual VAR_TABLE_INDEX addVar(VAR var) = 0;

  /** @brief Adds proc to procTable if proc is not in procTable.
  @param proc Procedure to be added to procTable.
  @return If proc exists in procTable, return its existing index.  Otherwise,
  return index of added proc.
  */
  virtual PROC_TABLE_INDEX addProc(PROC proc) = 0;

  /** @brief Adds {lineNo, uses} to usesTable if lineNo is not in usesTable.
  @param lineNo Line number of the SIMPLE code.
  @param uses Uses to be added to usesTable.
  */
  virtual void addUses(LINE_NO lineNo, USES uses) = 0;

  /** @brief Adds {procTableIndex, varTableIndexes} to usesProcTable if
  procTableIndex is not in usesProcTable.
  @param procTableIndex Index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes Indexes of varTable to be added to usesProcTable.
  */
  virtual void addUsesProc(PROC_TABLE_INDEX procTableIndex,
                           VAR_TABLE_INDEXES varTableIndexes) = 0;

  /** @brief Adds {lineNo, modifies} to modifiesTable if lineNo is not in
  modifiesTable.
  @param lineNo Line number of the SIMPLE code.
  @param modifies Modifies to be added to modifiesTable.
  */
  virtual void addModifies(LINE_NO lineNo, MODIFIES modifies) = 0;

  /** @brief Adds {procTableIndex, varTableIndexes} to modifiesProcTable if
  procTableIndex is not in modifiesProcTable.
  @param procTableIndex Index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes Indexes of varTable to be added to modifiesProcTable.
  */
  virtual void addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                               VAR_TABLE_INDEXES varTableIndexes) = 0;

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

  /** @brief Adds constant to constantTable if constant is not in constantTable.
  @param constant Constant to be added to constantTable.
  */
  virtual void addConstant(CONSTANT constant) = 0;

  /** @brief Adds call to callsTable.map.
  If callsTable.map does not map `proc`, then maps `proc` to a
  std::unordered_set with one element `call`. Otherwise, calls
  callsTable.map[key]::insert on `call`.
  @param pti proc table index to be added to callsTable.
  @param call call to be added to callsTable.map.
  */
  virtual void addCall(PROC_TABLE_INDEX pti, CALL call) = 0;

  /** @brief Creates derived tables. */
  virtual void deriveTables() = 0;

  /** @brief Composes two tables, each of specific type.
  @param table Table of type `KeysTable<LINE_NO, std::variant<VAR_TABLE_INDEXES,
  PROC_TABLE_INDEX>>`.
  @param procTable Table of type `KeysTable<PROC_TABLE_INDEX,
  VAR_TABLE_INDEXES>`.
  @return A table of type `KeysTable<LINE_NO, VAR_TABLE_INDEXES>`.
  */
  KeysTable<LINE_NO, VAR_TABLE_INDEXES>
  transit(KeysTable<LINE_NO, std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX>>
              table,
          KeysTable<PROC_TABLE_INDEX, VAR_TABLE_INDEXES> procTable);

protected:
  VAR_TABLE varTable;   /**< A KeysTable mapping VAR to VAR_TABLE_INDEX. */
  PROC_TABLE procTable; /**< A KeysTable mapping proc to PROC_TABLE_INDEX. */
  USES_TABLE usesTable; /**< A KeysTable mapping LINE_NO to USES. */
  USES_PROC_TABLE usesProcTable; /**< A KeysTable mapping PROC_TABLE_INDEX to
                                    VAR_TABLE_INDEXES. */
  MODIFIES_TABLE modifiesTable; /**< A KeysTable mapping LINE_NO to MODIFIES. */
  MODIFIES_PROC_TABLE
  modifiesProcTable;        /**< A KeysTable mapping PROC_TABLE_INDEX to
                               VAR_TABLE_INDEXES. */
  FOLLOW_TABLE followTable; /**< A KeysTable mapping LINE_NO to FOLLOW. */
  PARENT_TABLE parentTable; /**< A KeysTable mapping LINE_NO to PARENT. */
  STATEMENT_PROC_TABLE
  statementProcTable; /**< A KeysTable mapping LINE_NO to PROC. */
  STATEMENT_TYPE_TABLE
  statementTypeTable; /**< A KeysTable mapping LINE_NO to StatementType. */
  ASSIGN_AST_TABLE assignAstTable; /**< A KeysTable mapping LINE_NO to AST. */
  CONSTANT_TABLE constantTable;    /**< A std::unordered_set of CONSTANT. */
  CALLS_TABLE callsTable; /**< A KeysTable mapping PROC_TABLE_INDEX to CALLS. */
};
