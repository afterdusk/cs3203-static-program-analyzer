#pragma once

#include <unordered_map>
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
