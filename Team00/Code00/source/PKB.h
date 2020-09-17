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
typedef std::unordered_set<PROC> PROCS;
typedef std::string VAR;
typedef std::string LINE_NO;
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

class Pkb {
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
  CALLS_TABLE callsTable;

  /** @brief Auxiliary function of Pkb::closeFlatten. For algorithm details, see
  Pkb::closeFlatten.
  @param parent The parent for which all its descendants will be recursively
  unioned.
  @param parentChildrenTable An associative container that contains
  parent-children pairs with unique parents.
  @param mapCloseFlattened A reference to the flattened transitive closure of
  parentChildrenTable.
  */
  template <class T>
  void closeFlattenAux(T parent,
                       KeysTable<T, std::unordered_set<T>> parentChildrenTable,
                       KeysTable<T, std::unordered_set<T>> &mapCloseFlattened);

public:
  /** @brief Inverts the keysTable.
  Where `result` is the returned value,
  iterates through each key in keysTable.keys to get the value mapped by
  keysTable.map, to insert the value-key pair in `result`.
  @param keysTable An associative container that contains key-value pairs with
  unique keys and unique values.
  @return The inverse of keysTable.
  */
  template <class Key, class T>
  KeysTable<T, Key> invert(KeysTable<Key, T> keysTable);

  /** @brief Compose keysTable with itself, once.
  Where `result` is the returned value, firstly copies keysTable.map to `result`
  by iterating through each key in keysTable.keys to get the value mapped by
  keysTable.map, to insert the key-value pair in `result`. Then, iterating
  through each key in keysTable.keys to get the key-values pair p1 in `result`,
  for each p1.value in p1.values, for each key-values pair p2 in `result` with
  p2.key equivalent to p1.value, concatenate the unordered_set of values
  `result`[p1.key] with p2.values.
  @param keysTable An associative container that contains key-values pairs with
  unique keys. There is a binary relation between keys and values.
  @return The composition of keysTable with itself.
  */
  template <class T>
  KeysTable<T, std::unordered_set<T>> closeOnce(KeysTable<T, T> keysTable);

  /** @brief Takes the transitive closure of keysTable. This is just the
  composition of Pkb::closeFlatten with Pkb::closeOnce.
  @param keysTable An associative container that contains key-values pairs with
  unique keys. There is a binary relation between keys and values.
  @return The transitive closure of keysTable.
   */
  template <class T>
  KeysTable<T, std::unordered_set<T>> close(KeysTable<T, T> keysTable);

  /** @brief Takes the flattened transitive closure of parentChildrenTable.
  Where `result` is the returned value, "parent" is a key, "children" is the
  values parentChildrenTable maps the "parent" to, and whereever each
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
  @return The flattened transitive closure of parentChildrenTable.
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
  @param keysTable An associative container that contains key-value pairs with
  unique keys.
  @return The pseudoinverse of keysTable.
  */
  template <class Key, class T>
  KeysTable<T, std::unordered_set<Key>>
  pseudoinvert(KeysTable<Key, T> keysTable);

  /** @brief Pseudoinverts the keysTable when the keysTable is not
  pseudoinvertible. The pseudoinverse of KeysTable<Key, std::unordered_set<T>>
  would be KeysTable<std::unordered_set<T>, std::unordered_set<Key>>, but
  std::unordered_set is unhashable. This function avoids this unhashable problem
  by directly constructing KeysTable<T, std::unordered_set<Key>>. The body of
  this function differs from the body of Pkb::pseudoinvert by just one for-loop.
  @param keysTable An associative container that contains key-value pairs with
  unique keys.
  @return The pseudoinverse of keysTable, with flattened keys.
  */
  template <class Key, class T>
  KeysTable<T, std::unordered_set<Key>>
  pseudoinvertFlattenKeys(KeysTable<Key, std::unordered_set<T>> keysTable);

  /** @brief Compose two tables, each of specific type.
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

  /** @brief Get varTable.
  @return The varTable.
  */
  const VAR_TABLE &getVarTable() const;

  /** @brief Get procTable.
  @return The procTable.
  */
  const PROC_TABLE &getProcTable() const;

  /** @brief Get usesTable.
  @return The usesTable.
  */
  const USES_TABLE &getUsesTable() const;

  /** @brief Get usesProcTable.
  @return The usesProcTable.
  */
  const USES_PROC_TABLE &getUsesProcTable() const;

  /** @brief Get modifiesTable.
  @return The modifiesTable.
  */
  const MODIFIES_TABLE &getModifiesTable() const;

  /** @brief Get modifiesProcTable.
  @return The modifiesProcTable.
  */
  const MODIFIES_PROC_TABLE &getModifiesProcTable() const;

  /** @brief Get followTable.
  @return The followTable.
  */
  const FOLLOW_TABLE &getFollowTable() const;

  /** @brief Get parentTable.
  @return The parentTable.
  */
  const PARENT_TABLE &getParentTable() const;

  /** @brief Get statementProcTable.
  @return The statementProcTable.
  */
  const STATEMENT_PROC_TABLE &getStatementProcTable() const;

  /** @brief Get statementTypeTable.
  @return The statementTypeTable.
  */
  const STATEMENT_TYPE_TABLE &getStatementTypeTable() const;

  /** @brief Get assignAstTable.
  @return The assignAstTable.
  */
  const ASSIGN_AST_TABLE &getAssignAstTable() const;

  /** @brief Get constantTable.
  @return The constantTable.
  */
  const CONSTANT_TABLE &getConstantTable() const;

  /** @brief Get callsTable.
  @return the callsTable.
  */
  const CALLS_TABLE &getCallsTable() const;

  /** @brief Add var to varTable if var is not in varTable.
  @param var Variable to be added to varTable.
  @return If var exists in varTable, return its existing index. Otherwise,
  return index of the added var.
  */
  VAR_TABLE_INDEX addVar(VAR var);

  /** @brief Add proc to procTable if proc is not in procTable.
  @param proc Procedure to be added to procTable.
  @return If proc exists in procTable, return its existing index.  Otherwise,
  return index of added proc.
  */
  PROC_TABLE_INDEX addProc(PROC proc);

  /** @brief Add {lineNo, uses} to usesTable if lineNo is not in usesTable.
  @param lineNo Line number of the SIMPLE code.
  @param uses Uses to be added to usesTable.
  */
  void addUses(LINE_NO lineNo, USES uses);

  /** @brief Add {procTableIndex, varTableIndexes} to usesProcTable if
  procTableIndex is not in usesProcTable.
  @param procTableIndex Index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes Indexes of varTable to be added to usesProcTable.
  */
  void addUsesProc(PROC_TABLE_INDEX procTableIndex,
                   VAR_TABLE_INDEXES varTableIndexes);

  /** @brief Add {lineNo, modifies} to modifiesTable if lineNo is not in
  modifiesTable.
  @param lineNo Line number of the SIMPLE code.
  @param modifies Modifies to be added to modifiesTable.
  */
  void addModifies(LINE_NO lineNo, MODIFIES modifies);

  /** @brief Add {procTableIndex, varTableIndexes} to modifiesProcTable if
  procTableIndex is not in modifiesProcTable.
  @param procTableIndex Index mapped by PROC_TABLE to a PROC.
  @param varTableIndexes Indexes of varTable to be added to modifiesProcTable.
  */
  void addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                       VAR_TABLE_INDEXES varTableIndexes);

  /** @brief Add {lineNo, follow} to followTable if lineNo is not in
  followTable.
  @param lineNo Line number of the SIMPLE code.
  @param follow Follow to be added to followTable.
  */
  void addFollow(LINE_NO lineNo, FOLLOW follow);

  /** @brief Add {child, parent} to parentTable if child is not in parentTable.
  @param child Child to be added to parentTable.
  @param parent Parent to be added to parentTable.
  */
  void addParent(CHILD child, PARENT parent);

  /** @brief Add {lineNo, statementProc} to statementProcTable if lineNo
  is not in statementProcTable.
  @param lineNo Line number of the SIMPLE code.
  @param statementProc StatementProc to be added to statementProcTable.
  */
  void addStatementProc(LINE_NO lineNo, PROC statementProc);

  /** @brief Add {lineNo, statementType} to statementTypeTable if lineNo
  is not in statementTypeTable.
  @param lineNo Line number of the SIMPLE code.
  @param statementType StatementType to be added to statementTypeTable.
  */
  void addStatementType(LINE_NO lineNo, StatementType statementType);

  /** @brief Add {lineNo, ast} to assignAstTable if lineNo is not in
  assignAstTable.
  @param lineNo Line number of the SIMPLE code.
  @param ast Abstract syntax tree to be added to assignAstTable.
  */
  void addAssignAst(LINE_NO lineNo, AST ast);

  /** @brief Add constant to constantTable if constant is not in constantTable.
  @param constant Constant to be added to constantTable.
  */
  void addConstant(CONSTANT constant);

  /** @brief Add call to callsTable.map.
  If callsTable.map does not map `proc`, then maps `proc` to a
  std::unordered_set with one element `call`. Otherwise, calls
  callsTable.map[key]::insert on `call`.
  @param pti proc table index to be added to callsTable.
  @param call call to be added to callsTable.map.
  */
  void addCall(PROC_TABLE_INDEX pti, CALL call);
};

template <class Key, class T>
KeysTable<T, Key> Pkb::invert(KeysTable<Key, T> keysTable) {
  KeysTable<T, Key> mapInverted;
  for (Key key : keysTable.keys) {
    T value = keysTable.map[key];
    mapInverted.insert({value, key});
  }
  return mapInverted;
}

template <class T>
KeysTable<T, std::unordered_set<T>> Pkb::closeOnce(KeysTable<T, T> keysTable) {
  KeysTable<T, std::unordered_set<T>> mapClosed;
  for (T key : keysTable.keys) {
    T value = keysTable.map[key];
    mapClosed.insert({key, {value}});
  }
  for (T key : keysTable.keys) {
    std::unordered_set<T> values = mapClosed.map[key];
    for (T value : values) {
      // auto p1 = {key, value};
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
KeysTable<T, std::unordered_set<T>> Pkb::close(KeysTable<T, T> keysTable) {
  return Pkb::closeFlatten<T>(Pkb::closeOnce<T>(keysTable));
}

template <class T>
KeysTable<T, std::unordered_set<T>>
Pkb::closeFlatten(KeysTable<T, std::unordered_set<T>> parentChildrenTable) {
  KeysTable<T, std::unordered_set<T>> mapCloseFlattened;
  for (T parent : parentChildrenTable.keys) {
    std::unordered_set<T> children = parentChildrenTable.map[parent];
    mapCloseFlattened.insert({parent, children});
  }
  KeysTable<T, std::unordered_set<T>> mapCloseFlattenedOld;
  // Don't know which parent are ancestors, so run auxiliary function on all
  // parents. All ancestors must be parents, so auxiliary function is run on all
  // ancestors, so the final result must contain all descendants.
  for (T parent : parentChildrenTable.keys) {
    closeFlattenAux(parent, parentChildrenTable, mapCloseFlattened);
  }
  return mapCloseFlattened;
}

template <class T>
void Pkb::closeFlattenAux(
    T parent, KeysTable<T, std::unordered_set<T>> parentChildrenTable,
    KeysTable<T, std::unordered_set<T>> &mapCloseFlattened) {
  auto pair = parentChildrenTable.map.find(parent);
  if (pair != parentChildrenTable.map.end()) {
    // If parent has children.
    std::unordered_set<T> children = pair->second;
    for (T child : children) {
      // Recurse on child.
      closeFlattenAux(child, parentChildrenTable, mapCloseFlattened);
      // mapCloseFlattened now maps child to all descendants.
      // Update parent's descendants with child's descendants.
      mapCloseFlattened.map[parent].insert(mapCloseFlattened.map[child].begin(),
                                           mapCloseFlattened.map[child].end());
    }
  }
}

template <class Key, class T>
KeysTable<T, std::unordered_set<Key>>
Pkb::pseudoinvert(KeysTable<Key, T> keysTable) {
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
Pkb::pseudoinvertFlattenKeys(KeysTable<Key, std::unordered_set<T>> keysTable) {
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
