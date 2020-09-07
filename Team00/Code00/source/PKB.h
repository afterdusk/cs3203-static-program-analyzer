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

typedef std::string PROC;
typedef std::string VAR;
typedef uint64_t LINE_NO;
typedef uint64_t VAR_TABLE_INDEX;
typedef uint64_t PROC_TABLE_INDEX;
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
  /** @brief Inverts the keysTable.
  Where `result` is the returned value,
  iterates through each key in keysTable.keys to get the value mapped by
  keysTable.map, to insert the value-key pair in `result`.
  @param keysTable An associative container that contains key-value pairs with
  unique keys and unique values.
  @return The inverse of the associative container.
  */
  template <class Key, class T>
  KeysTable<T, Key> invert(KeysTable<Key, T> keysTable);

  /** @brief Takes the transitive closure of keysTable.
  Where `result` is the returned value,
  firstly copies keysTable.map to `result` by iterating through each
  key in keysTable.keys to get the value mapped by keysTable.map, to insert the
  key-value pair in `result`.
  Then, iterating through each key in keysTable.keys to get the key-values pair
  p1 in `result`, for each p1.value in p1.values, for each key-values pair p2 in
  `result` with p2.key equivalent to p1.value, concatenate the vector of values
  `result`[p1.key] with p2.values.
  @param keysTable An associative container that contains key-values pairs with
  unique keys. There is a binary relation between keys and values.
  @return The transitive closure of the associative container.
  */
  template <class Key, class T>
  KeysTable<Key, std::vector<T>> close(KeysTable<Key, T> keysTable);

  /** @brief Inverts the keysTable when the keysTable is not invertible.
  Where `result` is the returned value,
  iterates through each key in keysTable.keys to get the value mapped by
  keysTable.map. For each key-value pair, if value is not already mapped in
  `result`, then insert the pair {value, {key}}. Otherwise,
  push_back the vector of keys `result`[value] with key.
  @param An associative container that contains key-value pairs with unique
  keys.
  @return The pseudoinverse of the associative container.
  */
  template <class Key, class T>
  KeysTable<T, std::vector<Key>> pseudoinvert(KeysTable<Key, T> keysTable);

  /** @brief Get varTable.
  @return the varTable.
  */
  const VAR_TABLE &getVarTable() const;

  /** @brief Get procTable.
  @return the procTable.
  */
  const PROC_TABLE &getProcTable() const;

  /** @brief Get usesTable.
  @return the usesTable.
  */
  const USES_TABLE &getUsesTable() const;

  /** @brief Get usesProcTable.
  @return the usesProcTable.
  */
  const USES_PROC_TABLE &getUsesProcTable() const;

  /** @brief Get modifiesTable.
  @return the modifiesTable.
  */
  const MODIFIES_TABLE &getModifiesTable() const;

  /** @brief Get modifiesProcTable.
  @return the modifiesProcTable.
  */
  const MODIFIES_PROC_TABLE &getModifiesProcTable() const;

  /** @brief Get followTable.
  @return the followTable.
  */
  const FOLLOW_TABLE &getFollowTable() const;

  /** @brief Get parentTable.
  @return the parentTable.
  */
  const PARENT_TABLE &getParentTable() const;

  /** @brief Get statementProcTable.
  @return the statementProcTable.
  */
  const STATEMENT_PROC_TABLE &getStatementProcTable() const;

  /** @brief Get statementTypeTable.
  @return the statementTypeTable.
  */
  const STATEMENT_TYPE_TABLE &getStatementTypeTable() const;

  /** @brief Get assignAstTable.
  @return the assignAstTable.
  */
  const ASSIGN_AST_TABLE &getAssignAstTable() const;

  /** @brief Add variable to varTable.map.
  If variable exists in varTable.map, return its existing index.
  If variable does not exist in varTable.map, return index of added variable.
  @param var variable to be added to varTable.map.
  @return index of added variable.
  */
  VAR_TABLE_INDEX addVar(VAR var);

  /** @brief Add procedure to procTable.
  If procedure exists in procTable, return its existing index.
  If procedure does not exist in procTable, return index of added procedure.
  @param proc procedure to be added to procTable.
  @return index of added procedure.
  */
  PROC_TABLE_INDEX addProc(PROC proc);

  /** @brief Add uses to usesTable.
  @param lineNo line number of the SIMPLE code.
  @param uses uses to be added to usesTable.
  */
  void addUses(LINE_NO lineNo, USES uses);

  /** @brief Add varTableIndexes to usesProcTable.
  @param procTableIndex index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes varTableIndexes to be added to usesProcTable.
  */
  void addUsesProc(PROC_TABLE_INDEX procTableIndex,
                   VAR_TABLE_INDEXES varTableIndexes);

  /** @brief Add modifies to modifiesTable.
  @param lineNo line number of the SIMPLE code.
  @param modifies modifies to be added to modifiesTable.
  */
  void addModifies(LINE_NO lineNo, MODIFIES modifies);

  /** @brief Add varTableIndexes to modifiesProcTable.
  @param procTableIndex index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes varTableIndexes to be added to modifiesProcTable.
  */
  void addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                       VAR_TABLE_INDEXES varTableIndexes);

  /** @brief Add follow to followTable.map.
  @param lineNo line number of the SIMPLE code.
  @param follow follow to be added to followTable.map.
  */
  void addFollow(LINE_NO lineNo, FOLLOW follow);

  /** @brief Add parent to parentTable.map.
  @param child child to be added to parentTable.map.
  @param parent parent to be added to parentTable.map.
  */
  void addParent(CHILD child, PARENT parent);

  /** @brief Add statementProc to statementProcTable.
  @param lineNo line number of the SIMPLE code.
  @param statementProc statementProc to be added to statementProcTable.
  */
  void addStatementProc(LINE_NO lineNo, PROC statementProc);

  /** @brief Add statementType to statementTypeTable.
  @param lineNo line number of the SIMPLE code.
  @param statementType statementType to be added to statementTypeTable.
  */
  void addStatementType(LINE_NO lineNo, StatementType statementType);

  /** @brief Add ast to assignAstTable.
  @param lineNo line number of the SIMPLE code.
  @param ast ast to be added to assignAstTable.
  */
  void addAssignAst(LINE_NO lineNo, AST ast);
};

template <class Key, class T>
KeysTable<T, Key> PKB::invert(KeysTable<Key, T> keysTable) {
  KeysTable<T, Key> mapInverted;
  for (Key key : keysTable.keys) {
    T value = keysTable.map[key];
    mapInverted.insert({value, key});
  }
  return mapInverted;
}

template <class Key, class T>
KeysTable<Key, std::vector<T>> PKB::close(KeysTable<Key, T> keysTable) {
  KeysTable<Key, std::vector<T>> mapClosed;
  for (Key key : keysTable.keys) {
    T value = keysTable.map[key];
    mapClosed.insert({key, {value}});
  }
  for (Key key : keysTable.keys) {
    std::vector<T> values = mapClosed.map[key];
    for (T value : values) {
      auto p1 = {key, value};
      auto p2 = mapClosed.map.find(value);
      if (p2 != mapClosed.map.end()) {
        std::vector<T> v2 = p2->second;
        mapClosed.map[key].insert(mapClosed.map[key].end(), v2.begin(),
                                  v2.end());
      }
    }
  }
  return mapClosed;
}

template <class Key, class T>
bool KeysTable<Key, T>::insert(const KeysTable::value_type &value) {
  this->keys.push_back(std::get<const Key>(value));
  return std::get<bool>(this->map.insert(value));
}
template <class Key, class T>
typename std::unordered_map<Key, T>::size_type KeysTable<Key, T>::size() {
  return this->map.size();
}

template <class Key, class T>
KeysTable<T, std::vector<Key>> PKB::pseudoinvert(KeysTable<Key, T> keysTable) {
  KeysTable<T, std::vector<Key>> mapPseudoinverted;
  for (Key key : keysTable.keys) {
    T value = keysTable.map[key];
    auto p = mapPseudoinverted.map.find(value);
    if (p == mapPseudoinverted.map.end()) {
      mapPseudoinverted.insert({value, {key}});
    } else {
      mapPseudoinverted.map[value].push_back(key);
    }
  }
  return mapPseudoinverted;
}
