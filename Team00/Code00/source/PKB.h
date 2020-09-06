#pragma once

#include "TNode.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

/** @brief An associative container that contains key-value pairs with unique
keys. Inserting into KeysTable inserts into this->map, and calls
this->keys::push_back.
*/
template <class Key, class T> class KeysTable {
public:
  typedef std::pair<const Key, T>
      value_type;                 /**< Overloaded this->map::value_type. */
  std::unordered_map<Key, T> map; /**< The associative container. */
  std::vector<Key> keys;          /**< The keys of this->map. */
  bool insert(const value_type &value); /**< Overloaded this->map::insert. Also
                                      calls this->keys::push_back on the key. */
  typename std::unordered_map<Key, T>::size_type
  size(); /**< Overloaded this->map::size. */
};

/** @brief An associative container that contains key-value pairs with unique
keys and unique values. this->mapInverted is undefined until this->invert is
called.
*/
template <class Key, class T>
class InvertibleTable : public virtual KeysTable<Key, T> {
public:
  std::unordered_map<T, Key>
      mapInverted; /**< The inverse of the associative container. */

  /** @brief Defines this->mapInverted.
  Requires that this->keys kept track of all keys of key-value pairs inserted
  into this->map.

  Iterates through each key in this->keys to get the value mapped by this->map,
  to insert the value-key pair in this->mapInverted.
  */
  void invert();
};

typedef std::string PROC;
typedef std::string VAR;
typedef uint64_t LINE_NO;
typedef uint64_t VAR_TABLE_INDEX;
typedef std::vector<PROC>::size_type PROC_TABLE_INDEX;
typedef std::unordered_set<VAR_TABLE_INDEX> VAR_TABLE_INDEXES;
typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> USES;
typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> MODIFIES;
typedef LINE_NO FOLLOW;
typedef LINE_NO PARENT;
typedef LINE_NO CHILD;
enum class StatementType {
  READ,
  PRINT,
  ASSIGN,
  CALL,
  WHILE,
  IF,
};
typedef TNode AST;

typedef InvertibleTable<VAR, VAR_TABLE_INDEX> VAR_TABLE;
typedef std::vector<PROC> PROC_TABLE;
typedef std::unordered_map<LINE_NO, USES> USES_TABLE;
typedef std::unordered_map<PROC_TABLE_INDEX, VAR_TABLE_INDEXES> USES_PROC_TABLE;
typedef std::unordered_map<LINE_NO, MODIFIES> MODIFIES_TABLE;
typedef std::unordered_map<PROC_TABLE_INDEX, VAR_TABLE_INDEXES>
    MODIFIES_PROC_TABLE;
typedef std::unordered_map<LINE_NO, FOLLOW> FOLLOW_TABLE;
typedef std::unordered_map<LINE_NO, PARENT> PARENT_TABLE;
typedef std::unordered_map<LINE_NO, PROC> STATEMENT_PROC_TABLE;
typedef std::unordered_map<LINE_NO, PROC> STATEMENT_PROC_TABLE;
typedef std::unordered_map<LINE_NO, StatementType> STATEMENT_TYPE_TABLE;
typedef std::unordered_map<LINE_NO, AST> ASSIGN_AST_TABLE;

class PKB {
private:
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

public:
  /** @brief Invert varTable.
   */
  void invertVarTable();

  /** @brief Add variable to varTable.
  If variable exists in varTable, return its existing index.
  If variable does not exist in varTable, return index of added variable.
  @param var variable to be added to varTable.
  @return index of added variable.
  */
  VAR_TABLE_INDEX addVar(VAR var);

  /** @brief Get variable from varTable.
  @param varTableIndex index of the variable to return.
  @return the requested variable.
  */
  VAR getVar(VAR_TABLE_INDEX varTableIndex);

  /** @brief Get index of a variable from varTable.
  @param var the variable to return.
  @return the requested index of the variable.
  */
  VAR_TABLE_INDEX getVarTableIndex(VAR var);

  /** @brief Add procedure to procTable, return index of added procedure.
  @param proc procedure to be added to procTable.
  @return index of added procedure.
  */
  PROC_TABLE_INDEX addProc(PROC proc);

  /** @brief Get procedure from procTable.
  @param index index of the procedure to return.
  @return the requested procedure.
  */
  PROC getProc(PROC_TABLE_INDEX index);

  /** @brief Add uses to usesTable.
  @param lineNo line number of the SIMPLE code.
  @param uses uses to be added to usesTable.
  */
  void addUses(LINE_NO lineNo, USES uses);

  /** @brief Get uses from usesTable.
  The API user wants to get a return value of type VAR_TABLE_INDEXES.
  Returned value is of type USES, which is either of type VAR_TABLE_INDEXES, or
  of type PROC_TABLE_INDEX. If returned value is of type PROC_TABLE_INDEX, then
  the API user should call the API `VAR_TABLE_INDEXES
  getUsesProc(PROC_TABLE_INDEX)`.
  @param lineNo line number of the SIMPLE code.
  @return the requested uses.
  */
  USES getUses(LINE_NO lineNo);

  /** @brief Add varTableIndexes to usesProcTable.
  @param procTableIndex index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes varTableIndexes to be added to usesProcTable.
  */
  void addUsesProc(PROC_TABLE_INDEX procTableIndex,
                   VAR_TABLE_INDEXES varTableIndexes);

  /** @brief Get varTableIndexes from usesProcTable.
  @param procTableIndex index mapped by PROC_TABLE to a PROC.
  @return varTableIndexes the requested varTableIndexes.
  */
  VAR_TABLE_INDEXES getUsesProc(PROC_TABLE_INDEX procTableIndex);

  /** @brief Add modifies to modifiesTable.
  @param lineNo line number of the SIMPLE code.
  @param modifies modifies to be added to modifiesTable.
  */
  void addModifies(LINE_NO lineNo, MODIFIES modifies);

  /** @brief Get modifies from modifiesTable.
  The API user wants to get a return value of type VAR_TABLE_INDEXES.
  Returned value is of type MODIFIES, which is either of type VAR_TABLE_INDEXES,
  or of type PROC_TABLE_INDEX. If returned value is of type PROC_TABLE_INDEX,
  then the API user should call the API `VAR_TABLE_INDEXES
  getModifiesProc(PROC_TABLE_INDEX)`.
  @param lineNo line number of the SIMPLE code.
  @return the requested modifies.
  */
  MODIFIES getModifies(LINE_NO lineNo);

  /** @brief Add varTableIndexes to modifiesProcTable.
  @param procTableIndex index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes varTableIndexes to be added to modifiesProcTable.
  */
  void addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                       VAR_TABLE_INDEXES varTableIndexes);

  /** @brief Get varTableIndexes from modifiesProcTable.
  @param procTableIndex index mapped by PROC_TABLE to a PROC.
  @return the requested varTableIndexes.
  */
  VAR_TABLE_INDEXES getModifiesProc(PROC_TABLE_INDEX procTableIndex);

  /** @brief Add follow to followTable.
  @param lineNo line number of the SIMPLE code.
  @param follow follow to be added to followTable.
  */
  void addFollow(LINE_NO lineNo, FOLLOW follow);

  /** @brief Get follow from followTable.
  @param lineNo line number of the SIMPLE code.
  @return the requested follow.
  */
  FOLLOW getFollow(LINE_NO lineNo);

  /** @brief Add parent to parentTable.
  @param child child to be added to parentTable.
  @param parent parent to be added to parentTable.
  */
  void addParent(CHILD child, PARENT parent);

  /** @brief Get parent of a child from parentTable.
  @param child a child.
  @return the requested parent.
  */
  PARENT getParent(CHILD child);

  /** @brief Add statementProc to statementProcTable.
  @param lineNo line number of the SIMPLE code.
  @param statementProc statementProc to be added to statementProcTable.
  */
  void addStatementProc(LINE_NO lineNo, PROC statementProc);

  /** @brief Get statementProc from statementProcTable.
  @param lineNo line number of the SIMPLE code.
  @return the requested statementProc.
  */
  PROC getStatementProc(LINE_NO lineNo);

  /** @brief Add statementType to statementTypeTable.
  @param lineNo line number of the SIMPLE code.
  @param statementType statementType to be added to statementTypeTable.
  */
  void addStatementType(LINE_NO lineNo, StatementType statementType);

  /** @brief Get statementType from statementTypeTable.
  @param lineNo line number of the SIMPLE code.
  @return the requested statementType.
  */
  StatementType getStatementType(LINE_NO lineNo);

  /** @brief Add ast to assignAstTable.
  @param lineNo line number of the SIMPLE code.
  @param ast ast to be added to assignAstTable.
  */
  void addAssignAst(LINE_NO lineNo, AST ast);

  /** @brief Get ast from assignAstTable.
  @param lineNo line number of the SIMPLE code.
  @return the requested ast.
  */
  AST getAssignAst(LINE_NO lineNo);
};
