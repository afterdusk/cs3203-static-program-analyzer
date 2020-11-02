#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQL.h"

typedef std::string SYMBOL;
typedef std::string VALUE;
typedef std::unordered_map<SYMBOL, std::vector<VALUE>> TABLE;
typedef std::string ROW_HASH;

constexpr auto TRUE_RESULT = "TRUE";
constexpr auto FALSE_RESULT = "FALSE";
constexpr auto RESULT_DELIMITER = " ";
constexpr auto HASH_DELIMITER = "@"; // must be illegal char in SIMPLE grammar

/** @brief Local helper method for converting an element with an attribute
 *  to a symbol (string).
 *  @return an optional containing a symbol if element has an attribute, else
 *  an empty optional.
 */
std::optional<SYMBOL> elementAttrToSymbol(TokenType type, Element element);

/** @brief Represents the table of possible query results.
 *  As clauses are evaluated, values from the PkbTables are pushed into
 *  this table.
 */
class EvaluationTable {
private:
  std::unordered_set<SYMBOL> symbols;
  TABLE *table;
  int rows;

public:
  EvaluationTable();

  EvaluationTable(const EvaluationTable &other);

  ~EvaluationTable();

  EvaluationTable &operator=(const EvaluationTable &other);

  bool operator==(EvaluationTable &other);

  /** @brief Creates a EvaluationTable with symbols (either synonyms or
   *  synonym attributes) and their corresponding values.
   *
   *  e.g. {"a", "s"} = {{"1", "2"}, {"3", "4"}}
   *
   *  A EvaluationTable of this query can be instantiated as such:
   *
   *  EvaluationTable(new TABLE({
   *    {"a", {"1", "3"}},
   *    {"s", {"2", "4"}}
   *  }))
   */
  EvaluationTable(TABLE *table);

  /** @brief Merges in the values from another EvaluationTable using
   *  the classic hash join algorithm.
   */
  void hashMerge(EvaluationTable &other);

  /** @brief Merges in the values from another EvaluationTable using
   *  a nested loop join algorithm.
   */
  void merge(EvaluationTable &other);

  /** @brief Returns whether the table contains symbol.
   */
  bool contains(SYMBOL symbol);

  /** @brief Returns whether the symbols are seen.
   */
  bool contains(std::vector<SYMBOL> checkSymbols);

  /** @brief Returns whether the table has no symbols.
   */
  bool noSymbols();

  /** @brief Returns a new instance of an EvaluationTable
   *  containing only the symbols passed in the set.
   *  Ignores symbols in set that are not present in table.
   */
  EvaluationTable sliceSymbols(std::unordered_set<SYMBOL> symbolsWanted);

  /** @brief Flattens the list of symbols provided into a list of
   *  strings. Duplicate results are removed and an error is thrown
   *  if any of the provided symbols do not exist in the table.
   */
  void flatten(std::vector<SYMBOL> toSymbols, std::list<VALUE> &result);

  /** @brief Helper method that returns a crude "hash" of a row,
   *  created by concatenating the values of a row according to
   *  a provided order and delimiting by an illegal character.
   *  Can provide hash for subset of table's symbols.
   */
  ROW_HASH rowHash(int index, std::vector<SYMBOL> &order);

  /** @brief Returns the number of rows in the evaluation table.
   */
  int rowCount();

  /** @brief Debug/test method that retrieves the possible
   *  values of a symbol.
   */
  std::unordered_set<VALUE> select(SYMBOL symbol);
};
