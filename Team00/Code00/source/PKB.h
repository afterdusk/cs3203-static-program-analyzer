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
typedef std::string LINE_NO;
typedef uint64_t VAR_TABLE_INDEX;
typedef uint64_t PROC_TABLE_INDEX;
typedef std::unordered_set<VAR_TABLE_INDEX> VAR_TABLE_INDEXES;
typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> USES;
typedef std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> MODIFIES;
typedef LINE_NO FOLLOW;
typedef std::unordered_set<FOLLOW> FOLLOWS;
typedef LINE_NO PARENT;
typedef std::unordered_set<PARENT> PARENTS;
typedef LINE_NO CHILD;
typedef std::unordered_set<CHILD> CHILDREN;
enum class StatementType {
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
typedef std::vector<CONSTANT> CONSTANT_TABLE;

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
  CONSTANT_TABLE constantTable;

  /** @brief Auxiliary function of PKB::closeFlatten. For algorithm details, see
  PKB::closeFlatten.
  @param parent The parent for which all its descendants will be recursively
  unioned.
  @param parentChildrenTable An associative container that contains
  parent-children pairs with unique parents.
  @param mapClosedFlattened A reference to the flattened transitive closure of
  the associative container.
  */
  template <class T>
  void closeFlattenAux(T parent,
                       KeysTable<T, std::unordered_set<T>> parentChildrenTable,
                       KeysTable<T, std::unordered_set<T>> &mapClosedFlattened);

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
  `result` with p2.key equivalent to p1.value, concatenate the unordered_set of
  values `result`[p1.key] with p2.values.
  @param keysTable An associative container that contains key-values pairs with
  unique keys. There is a binary relation between keys and values.
  @return The transitive closure of the associative container.
  */
  template <class T>
  KeysTable<T, std::unordered_set<T>> close(KeysTable<T, T> keysTable);

  /** @brief Takes the flattened transitive closure of parentChildrenTable.
  Where `result` is the returned value, "parent" is a key, "children" is the
  values the associative container maps the "parent" to, and whereever each
  "child" is also a "parent", the "children" of this "child" are "grandchildren"
  to the "parent", and "descendants" are all such recursively defined
  "children", "grandchildren", and so on. The algorithm should conceptually
  start by taking a parent with children but no grandchildren, and inserting the
  same parent with the same children into `result`. This should be repeated for
  all such parents. Then, take a parent with children and grandchildren but no
  grand-grandchildren, and inserting the same parent with all descendants
  grouped together. This is done by, for each child, concatenating their
  children. This should be repeated for all such parents. Then, take a parent
  with children and grandchildren and grand-grandchildren and
  grand-grand-grandchildren but no other descendants, and inserting the same
  parent with all descendants grouped together. This is done by, for each child,
  concatenating their descendants. This should be repeated for all such parents.
  And so on.
  @param parentChildrenTable An associative container that contains
  parent-children pairs with unique parents.
  @return The flattened transitive closure of the associative container.
  */
  template <class T>
  KeysTable<T, std::unordered_set<T>>
  closeFlatten(KeysTable<T, std::unordered_set<T>> parentChildrenTable);

  /** @brief Inverts the keysTable when the keysTable is not invertible.
  Where `result` is the returned value,
  iterates through each key in keysTable.keys to get the value mapped by
  keysTable.map. For each key-value pair, if value is not already mapped in
  `result`, then insert the pair {value, {key}}. Otherwise,
  insert key into the unordered_set of keys `result`[value].
  @param An associative container that contains key-value pairs with unique
  keys.
  @return The pseudoinverse of the associative container.
  */
  template <class Key, class T>
  KeysTable<T, std::unordered_set<Key>>
  pseudoinvert(KeysTable<Key, T> keysTable);

  /** @brief Pseudoinverts the keysTable when the keysTable is not
  pseudoinvertible. The pseudoinverse of KeysTable<Key, std::unordered_set<T>>
  would be KeysTable<std::unordered_set<T>, std::unordered_set<Key>>, but
  std::unordered_set is unhashable. This function avoids this unhashable problem
  by directly constructing KeysTable<T, std::unordered_set<Key>>. The body of
  this function differs from the body of PKB::pseudoinvert by just one for-loop.
  @param keysTable An associative container that contains key-value pairs with
  unique keys.
  @return The pseudoinverse of the associative container, with flattened keys.
  */
  template <class Key, class T>
  KeysTable<T, std::unordered_set<Key>>
  pseudoinvertFlattenKeys(KeysTable<Key, std::unordered_set<T>> keysTable);

  /** @brief Take the transitive relation of two tables, each of specific type.
  @param table table of type `KeysTable<LINE_NO,
  std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX>>`.
  @param procTable table of type `KeysTable<PROC_TABLE_INDEX,
  VAR_TABLE_INDEXES>`.
  @return a table of type `KeysTable<LINE_NO, VAR_TABLE_INDEXES>`.
  */
  KeysTable<LINE_NO, VAR_TABLE_INDEXES>
  transit(KeysTable<LINE_NO, std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX>>
              table,
          KeysTable<PROC_TABLE_INDEX, VAR_TABLE_INDEXES> procTable);

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

  /** @brief Get constantTable.
  @return the constantTable.
  */
  const CONSTANT_TABLE &getConstantTable() const;

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

  /** @brief Add constant to constantTable.
  @param constant constant to be added to constantTable.
  */
  void addConstant(CONSTANT constant);
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

template <class T>
KeysTable<T, std::unordered_set<T>> PKB::close(KeysTable<T, T> keysTable) {
  KeysTable<T, std::unordered_set<T>> mapClosed;
  for (T key : keysTable.keys) {
    T value = keysTable.map[key];
    mapClosed.insert({key, {value}});
  }
  for (T key : keysTable.keys) {
    std::unordered_set<T> values = mapClosed.map[key];
    for (T value : values) {
      auto p1 = {key, value};
      auto p2 = mapClosed.map.find(value);
      if (p2 != mapClosed.map.end()) {
        std::unordered_set<T> v2 = p2->second;
        mapClosed.map[key].insert(v2.begin(), v2.end());
      }
    }
  }
  return mapClosed;
}

template <class T>
KeysTable<T, std::unordered_set<T>>
PKB::closeFlatten(KeysTable<T, std::unordered_set<T>> parentChildrenTable) {
  KeysTable<T, std::unordered_set<T>> mapClosedFlattened;
  for (T parent : parentChildrenTable.keys) {
    std::unordered_set<T> children = parentChildrenTable.map[parent];
    mapClosedFlattened.insert({parent, children});
  }
  if (!parentChildrenTable.keys.empty()) {
    // Assume first key is common ancestor.
    closeFlattenAux(parentChildrenTable.keys[0], parentChildrenTable,
                    mapClosedFlattened);
  }
  return mapClosedFlattened;
}

template <class T>
void PKB::closeFlattenAux(
    T parent, KeysTable<T, std::unordered_set<T>> parentChildrenTable,
    KeysTable<T, std::unordered_set<T>> &mapClosedFlattened) {
  auto pair = parentChildrenTable.map.find(parent);
  if (pair != parentChildrenTable.map.end()) {
    // If parent has children.
    std::unordered_set<T> children = pair->second;
    for (T child : children) {
      // Recurse on child.
      closeFlattenAux(child, parentChildrenTable, mapClosedFlattened);
      // mapClosedFlattened now maps child to all descendants.
      // Update parent's descendants with child's descendants.
      mapClosedFlattened.map[parent].insert(
          mapClosedFlattened.map[child].begin(),
          mapClosedFlattened.map[child].end());
    }
  }
}

template <class Key, class T>
KeysTable<T, std::unordered_set<Key>>
PKB::pseudoinvert(KeysTable<Key, T> keysTable) {
  KeysTable<T, std::unordered_set<Key>> mapPseudoinverted;
  for (Key key : keysTable.keys) {
    T value = keysTable.map[key];
    auto p = mapPseudoinverted.map.find(value);
    if (p == mapPseudoinverted.map.end()) {
      mapPseudoinverted.insert({value, {key}});
    } else {
      mapPseudoinverted.map[value].insert(key);
    }
  }
  return mapPseudoinverted;
}

template <class Key, class T>
KeysTable<T, std::unordered_set<Key>>
PKB::pseudoinvertFlattenKeys(KeysTable<Key, std::unordered_set<T>> keysTable) {
  KeysTable<T, std::unordered_set<Key>> mapPseudoinvertedKeysFlattened;
  for (Key key : keysTable.keys) {
    std::unordered_set<T> values = keysTable.map[key];
    for (T value : values) {
      auto p = mapPseudoinvertedKeysFlattened.map.find(value);
      if (p == mapPseudoinvertedKeysFlattened.map.end()) {
        mapPseudoinvertedKeysFlattened.insert({value, {key}});
      } else {
        mapPseudoinvertedKeysFlattened.map[value].insert(key);
      }
    }
  }
  return mapPseudoinvertedKeysFlattened;
}

template <class Key, class T>
bool KeysTable<Key, T>::insert(const KeysTable::value_type &value) {
  bool insertionTookPlace = std::get<bool>(this->map.insert(value));
  if (insertionTookPlace) {
    this->keys.push_back(std::get<const Key>(value));
  }
  return insertionTookPlace;
}

template <class Key, class T>
typename std::unordered_map<Key, T>::size_type KeysTable<Key, T>::size() {
  return this->map.size();
}
