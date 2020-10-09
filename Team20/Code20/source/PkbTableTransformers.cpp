#include "PkbTableTransformers.h"

KeysTable<std::size_t, std::unordered_set<std::size_t>>
PkbTableTransformers::closeWarshall(
    KeysTable<std::size_t, std::unordered_set<std::size_t>> keysTable) {
  // Convert reachability matrix into a KeysTable.
  std::vector<std::vector<bool>> reachability =
      closeWarshallReachabilityMatrix(keysTable);
  KeysTable<std::size_t, std::unordered_set<std::size_t>> closed;
  std::size_t n = reachability.size();
  for (std::size_t i = 0; i < n; ++i) {
    // For all destination nodes,
    for (std::size_t j = 0; j < n; ++j) {
      if (reachability[i][j]) {
        if (!closed.insert({i, {j}})) {
          // If key already mapped, then insert value into existing mapped
          // unordered_set.
          closed.map[i].insert(j);
        }
      }
    }
  }
  return closed;
}

std::vector<std::vector<bool>>
PkbTableTransformers::closeWarshallReachabilityMatrix(
    KeysTable<std::size_t, std::unordered_set<std::size_t>> keysTable) {

  // Maximum value in keysTable
  std::size_t n =
      *(std::max_element(keysTable.keys.begin(), keysTable.keys.end()));
  for (std::size_t key : keysTable.keys) {
    n = std::max(n, *(std::max_element(keysTable.map[key].begin(),
                                       keysTable.map[key].end())));
  }

  std::vector<std::vector<bool>> reachability(n + 1,
                                              std::vector<bool>(n + 1, false));
  for (std::size_t key : keysTable.keys) {
    std::unordered_set<std::size_t> values = keysTable.map[key];
    for (std::size_t value : values) {
      reachability[key][value] = true;
    }
  }

  // For all intermediate nodes,
  for (std::size_t k = 0; k < n; ++k) {
    // For all source nodes,
    for (std::size_t i = 0; i < n; ++i) {
      // For all destination nodes,
      for (std::size_t j = 0; j < n; ++j) {
        reachability[i][j] =
            reachability[i][j] || (reachability[i][k] && reachability[k][j]);
      }
    }
  }

  return reachability;
}
