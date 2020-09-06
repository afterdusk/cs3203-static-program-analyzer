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

/** @brief An associative container that contains key-values pairs with unique
keys. There is a binary relation between keys and values. this->mapClosed is
undefined until this->close is called.
*/
template <class Key, class T>
class ClosableTable : public virtual KeysTable<Key, T> {
public:
  std::unordered_map<Key, std::vector<T>>
      mapClosed; /**< The transitive closure of the associative container. */

  /** @brief Defines this->mapClosed.
  Requires that this->keys kept track of all keys of key-value pairs inserted
  into this->map.

  Firstly copies this->map to this->mapClosed by iterating through each key in
  this->keys to get the value mapped by this->map, to insert the key-value pair
  in this->mapClosed.

  Then, iterating through each key in this->keys to get the key-values pair p1
  in this->mapClosed, for each p1.value in p1.values, for each key-values pair
  p2 in this->mapClosed with p2.key equivalent to p1.value, concatenate the
  vector of values this->mapClosed[p1.key] with p2.values.
  */
  void close();
};

/** @brief An associative container that contains key-value pairs with unique
keys. this->mapPseudoInverted is undefined until this->pseudoInvert is called.
*/
template <class Key, class T>
class PseudoInvertibleTable : public virtual KeysTable<Key, T> {
public:
  std::unordered_map<T, std::vector<Key>>
      mapPseudoInverted; /**< The pseudoinverse of the associative container. */

  /** @brief Defines this->mapPseudoInverted.
  Requires that this->keys kept track of all keys of key-value pairs inserted
  into this->map.

  Iterates through each key in this->keys to get the value mapped by this->map.
  For each key-value pair, if value is not already mapped in
  this->mapPseudoInverted, then insert the pair {value, {key}}. Otherwise,
  push_back the vector of keys this->mapPseudoInverted[value] with key.
  */
  void pseudoInvert();
};

/** @brief An associative container that is both invertible and closable.
 */
template <class Key, class T>
class InvertibleAndClosableTable : public InvertibleTable<Key, T>,
                                   public ClosableTable<Key, T> {};

/** @brief An associative container that is both pseudoinvertible and closable.
 */
template <class Key, class T>
class PseudoInvertibleAndClosableTable : public PseudoInvertibleTable<Key, T>,
                                         public ClosableTable<Key, T> {};

typedef std::string PROC;
typedef std::string VAR;
typedef uint64_t LINE_NO;
typedef uint64_t VAR_TABLE_INDEX;
typedef std::vector<PROC>::size_type PROC_TABLE_INDEX;
typedef std::unordered_set<VAR_TABLE_INDEX> VAR_TABLE_INDEXES;
typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> USES;
typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> MODIFIES;
typedef LINE_NO FOLLOW;
typedef std::vector<FOLLOW> FOLLOWS;
typedef LINE_NO PARENT;
typedef std::vector<PARENT> PARENTS;
typedef LINE_NO CHILD;
typedef std::vector<CHILD> CHILDREN;
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
typedef InvertibleAndClosableTable<LINE_NO, FOLLOW> FOLLOW_TABLE;
typedef PseudoInvertibleAndClosableTable<LINE_NO, PARENT> PARENT_TABLE;
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

  /** @brief Invert followTable.
   */
  void PKB::invertFollowTable();

  /** @brief Close followTable.
   */
  void closeFollowTable();

  /** @brief Pseudoinvert parentTable.
   */
  void PKB::pseudoInvertParentTable();

  /** @brief Close parentTable.
   */
  void closeParentTable();

  /** @brief Add variable to varTable.map.
  If variable exists in varTable.map, return its existing index.
  If variable does not exist in varTable.map, return index of added variable.
  @param var variable to be added to varTable.map.
  @return index of added variable.
  */
  VAR_TABLE_INDEX addVar(VAR var);

  /** @brief Get variable from varTable.map.
  @param varTableIndex index of the variable to return.
  @return the requested variable.
  */
  VAR getVar(VAR_TABLE_INDEX varTableIndex);

  /** @brief Get index of a variable from varTable.mapInverted.
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

  /** @brief Add follow to followTable.map.
  @param lineNo line number of the SIMPLE code.
  @param follow follow to be added to followTable.map.
  */
  void addFollow(LINE_NO lineNo, FOLLOW follow);

  /** @brief Get follow from followTable.map.
  @param lineNo line number of the SIMPLE code.
  @return the requested follow.
  */
  FOLLOW getFollow(LINE_NO lineNo);

  /** @brief Get line number from followTable.mapInverted.
  @param follow a follow.
  @return the requested line number of the SIMPLE code.
  */
  LINE_NO PKB::getFollowLineNo(FOLLOW follow);

  /** @brief Get a closed set of follow from followTable.mapClosed.
  @param lineNo line number of the SIMPLE code.
  @return the requested follow.
  */
  FOLLOWS getFollowStar(LINE_NO lineNo);

  /** @brief Add parent to parentTable.map.
  @param child child to be added to parentTable.map.
  @param parent parent to be added to parentTable.map.
  */
  void addParent(CHILD child, PARENT parent);

  /** @brief Get parent of a child from parentTable.map.
  @param child a child.
  @return the requested parent.
  */
  PARENT getParent(CHILD child);

  /** @brief Get children line numbers from parentTable.mapPseudoInverted.
  @param parent a parent.
  @return the requested children line numbers of the SIMPLE code.
  */
  CHILDREN PKB::getParentChildren(PARENT parent);

  /** @brief Get closed parents from parentTable.mapClosed.
  @param lineNo line number of the SIMPLE code.
  @return the requested closed parents.
  */
  PARENTS getParentStar(LINE_NO lineNo);

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
