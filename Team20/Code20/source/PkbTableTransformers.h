#pragma once

#include "KeysTable.h"
#include "PkbTables.h"
#include <algorithm>
#include <stack>

class PkbTableTransformers {
private:
  /** @brief Auxiliary function of PkbTableTransformers::closeWarshall. For
  algorithm details, see PkbTableTransformers::closeWarshall.
  @param keysTable An associative container that contains key-values pairs with
  unique keys, and all keys and values have type `std::size_t`. Assume
  std::size_t is std::vector<T>::size_type.
  @return The reachability matrix of keysTable.
  */
  static std::vector<std::vector<bool>> closeWarshallReachabilityMatrix(
      KeysTable<std::size_t, std::unordered_set<std::size_t>> keysTable);

  /** @brief Auxiliary function of PkbTableTransformers::closeFlatten. For
  algorithm details, see PkbTableTransformers::closeFlatten.
  @param parent The parent for which all its descendants will be recursively
  unioned.
  @param parentChildrenTable An associative container that contains
  parent-children pairs with unique parents.
  @param mapCloseFlattened A reference to the flattened transitive closure of
  the associative container.
  */
  template <class T>
  static void
  closeFlattenAux(T parent,
                  KeysTable<T, std::unordered_set<T>> parentChildrenTable,
                  KeysTable<T, std::unordered_set<T>> &mapCloseFlattened);

public:
  /** @brief Composes keysTable with itself, once.
  Where `result` is the returned value, firstly copies keysTable.map to `result`
  by iterating through each key in keysTable.keys to get the value mapped by
  keysTable.map, to insert the key-value pair in `result`. Then, iterating
  through each key in keysTable.keys to get the key-values pair p1 in `result`,
  for each p1.value in p1.values, for each key-values pair p2 in `result` with
  p2.key equivalent to p1.value, concatenates the unordered_set of values
  `result[p1.key]` with p2.values.
  @param keysTable An associative container that contains key-values pairs with
  unique keys. There is a binary relation between keys and values.
  @return The composition of keysTable with itself.
  */
  template <class T>
  static KeysTable<T, std::unordered_set<T>>
  closeOnce(KeysTable<T, T> keysTable);

  /** @brief Composes two tables, where variantTable maps to a `std::variant`.
  @param variantTable Table that maps keys `T` to a `std::variant` of values `V`
  and intermediate values `U`.
  @param table Table that maps intermediate values `U` to values `V`.
  @return A table that maps keys `T` to values `V`, which is the composition of
  variantTable and table. If a key is mapped by variantTable to a value, then
  this is the value mapped to in the returned table. If a key is mapped by
  variantTable to an intermediate value, then the value mapped to from the
  intermediate value by table is the value mapped to in the returned table.
  */
  template <class T, class U, class V>
  static KeysTable<T, V> transit(KeysTable<T, std::variant<V, U>> variantTable,
                                 KeysTable<U, V> table);

  /** @brief Takes the transitive closure of keysTable. This uses the Warshall
  algorithm.
  @param keysTable An associative container that contains key-values pairs with
  unique keys, and all keys and values have type `std::size_t`. Assume
  std::size_t is std::vector<T>::size_type.
  @return The transitive closure of keysTable.
  */
  static KeysTable<std::size_t, std::unordered_set<std::size_t>> closeWarshall(
      KeysTable<std::size_t, std::unordered_set<std::size_t>> keysTable);

  /** @brief Inverts the keysTable.
  Where `result` is the returned value,
  iterates through each key in keysTable.keys to get the value mapped by
  keysTable.map, to insert the value-key pair in `result`.
  @param keysTable An associative container that contains key-value pairs with
  unique keys and unique values.
  @return The inverse of keysTable.
  */
  template <class Key, class T>
  static KeysTable<T, Key> invert(KeysTable<Key, T> keysTable);

  /** @brief Takes the transitive closure of keysTable. Assumes keysTable is an
  acyclic graph. This is just the composition of
  PkbTableTransformers::closeFlatten with PkbTableTransformers::closeOnce.
  @param keysTable An associative container that contains key-values pairs with
  unique keys. There is a binary relation between keys and values.
  @return The transitive closure of keysTable.
   */
  template <class T>
  static KeysTable<T, std::unordered_set<T>> close(KeysTable<T, T> keysTable);

  /** @brief Takes the flattened transitive closure of parentChildrenTable.
  Assumes parentChildrenTable is an acyclic graph. Where `result` is the
  returned value, "parent" is a key, "children" is the values
  parentChildrenTable maps the "parent" to, and whenever each "child" is also a
  "parent", the "children" of this "child" are "grandchildren" to the "parent",
  and "descendants" are all such recursively defined "children",
  "grandchildren", and so on. The algorithm should conceptually start by taking
  a parent with children but no grandchildren, and inserting the same parent
  with the same children into `result`. This should be repeated for all such
  parents. Then, the algorithm continues by taking a parent with children and
  grandchildren but no grand-grandchildren, and inserting the same parent with
  all descendants grouped together. This is done by, for each child,
  concatenating their children. This should be repeated for all such parents.
  Then, the algorthim continues by taking a parent with children and
  grandchildren and grand-grandchildren and grand-grand-grandchildren but no
  other descendants, and inserting the same parent with all descendants grouped
  together. This is done by, for each child, concatenating their descendants.
  This should be repeated for all such parents. And so on.
  @param parentChildrenTable An associative container that contains
  parent-children pairs with unique parents.
  @return The flattened transitive closure of parentChildrenTable.
  */
  template <class T>
  static KeysTable<T, std::unordered_set<T>>
  closeFlatten(KeysTable<T, std::unordered_set<T>> parentChildrenTable);

  /** @brief Inverts the keysTable when the keysTable is not invertible.
  Where `result` is the returned value,
  iterates through each key in keysTable.keys to get the value mapped by
  keysTable.map. For each key-value pair, if value is not already mapped in
  `result`, then inserts the pair {value, {key}}. Otherwise,
  inserts key into the unordered_set of keys `result[value]`.
  @param keysTable An associative container that contains key-value pairs with
  unique keys.
  @return The pseudoinverse of keysTable.
  */
  template <class Key, class T>
  static KeysTable<T, std::unordered_set<Key>>
  pseudoinvert(KeysTable<Key, T> keysTable);

  /** @brief Pseudoinverts the keysTable when the keysTable is not
  pseudoinvertible. The pseudoinverse of KeysTable<Key, std::unordered_set<T>>
  would be KeysTable<std::unordered_set<T>, std::unordered_set<Key>>, but
  std::unordered_set is unhashable. This function avoids this unhashable problem
  by directly constructing KeysTable<T, std::unordered_set<Key>>. The body of
  this function differs from the body of PkbTableTransformers::pseudoinvert by
  just one for-loop.
  @param keysTable An associative container that contains key-value pairs with
  unique keys.
  @return The pseudoinverse of keysTable, with flattened keys.
  */
  template <class Key, class T>
  static KeysTable<T, std::unordered_set<Key>>
  pseudoinvertFlattenKeys(KeysTable<Key, std::unordered_set<T>> keysTable);
};

template <class T>
KeysTable<T, std::unordered_set<T>>
PkbTableTransformers::closeOnce(KeysTable<T, T> keysTable) {
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
void PkbTableTransformers::closeFlattenAux(
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

template <class T, class U, class V>
KeysTable<T, V>
PkbTableTransformers::transit(KeysTable<T, std::variant<V, U>> variantTable,
                              KeysTable<U, V> table) {
  KeysTable<T, V> mapTransited;
  for (T key : variantTable.keys) {
    std::variant<V, U> variantTableValue = variantTable.map[key];
    V value;
    if (std::holds_alternative<U>(variantTableValue)) {
      value = table.map[std::get<U>(variantTableValue)];
    } else if (std::holds_alternative<V>(variantTableValue)) {
      value = std::get<V>(variantTableValue);
    }
    mapTransited.insert({key, value});
  }
  return mapTransited;
}

template <class Key, class T>
KeysTable<T, Key> PkbTableTransformers::invert(KeysTable<Key, T> keysTable) {
  KeysTable<T, Key> mapInverted;
  for (Key key : keysTable.keys) {
    T value = keysTable.map[key];
    mapInverted.insert({value, key});
  }
  return mapInverted;
}

template <class T>
KeysTable<T, std::unordered_set<T>>
PkbTableTransformers::close(KeysTable<T, T> keysTable) {
  return closeFlatten<T>(closeOnce<T>(keysTable));
}

template <class T>
KeysTable<T, std::unordered_set<T>> PkbTableTransformers::closeFlatten(
    KeysTable<T, std::unordered_set<T>> parentChildrenTable) {
  KeysTable<T, std::unordered_set<T>> mapCloseFlattened;
  for (T parent : parentChildrenTable.keys) {
    std::unordered_set<T> children = parentChildrenTable.map[parent];
    mapCloseFlattened.insert({parent, children});
  }
  KeysTable<T, std::unordered_set<T>> mapCloseFlattenedOld;
  // Don't know which parent are ancestors, so run auxiliary function on all
  // parents. All ancestors must be parents, so auxiliary function is run on
  // all ancestors, so the final result must contain all descendants.
  for (T parent : parentChildrenTable.keys) {
    closeFlattenAux(parent, parentChildrenTable, mapCloseFlattened);
  }
  return mapCloseFlattened;
}

template <class Key, class T>
KeysTable<T, std::unordered_set<Key>>
PkbTableTransformers::pseudoinvert(KeysTable<Key, T> keysTable) {
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
PkbTableTransformers::pseudoinvertFlattenKeys(
    KeysTable<Key, std::unordered_set<T>> keysTable) {
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
