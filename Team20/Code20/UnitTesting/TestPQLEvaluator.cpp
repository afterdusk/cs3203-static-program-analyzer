#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPQLEvaluator){
  public :

      TEST_METHOD(TestEvaluationTable_BasicQuery){
          std::vector<SYMBOL> synonyms = {"s", "a", "v"};
EvaluationTable table(synonyms);

/* Clause 1: {s, a} = {{1, 1}, {1, 2}, {2, 3}, {3, 3}}
   Insertion into empty table
   | s | a | v |
   |---|---|---|
   | 1 | 1 |   |
   | 1 | 2 |   |
   | 2 | 3 |   |
   | 3 | 3 |   |
 */
ClauseResult firstClause({{"s", {"1", "1", "2", "3"}},
                          {"a", {"1", "2", "3", "3"}}});
table.add(firstClause);

std::unordered_set<VALUE> expectedResult = {"1", "2", "3"};
std::unordered_set<VALUE> actualResult = table.select("s");
Assert::IsTrue(expectedResult == actualResult);

expectedResult = {"1", "2", "3"};
actualResult = table.select("a");
Assert::IsTrue(expectedResult == actualResult);

int expectedRowCount = 4;
int actualRowCount = table.rowCount();
Assert::IsTrue(expectedRowCount == actualRowCount);

/* Clause 2: {a, v} = {{2, w}, {2, x}, {3, z}}
   Intersection operation (filtering + cross product)
   | s | a | v |
   |---|---|---|
   | 1 | 2 | w |
   | 1 | 2 | x |
   | 2 | 3 | z |
   | 3 | 3 | z |
 */
ClauseResult secondClause({{"a", {"2", "2", "3"}}, {"v", {"w", "x", "z"}}});
table.add(secondClause);

expectedResult = {"1", "2", "3"};
actualResult = table.select("s");
Assert::IsTrue(expectedResult == actualResult);

expectedResult = {"2", "3"};
actualResult = table.select("a");
Assert::IsTrue(expectedResult == actualResult);

expectedResult = {"w", "x", "z"};
actualResult = table.select("v");
Assert::IsTrue(expectedResult == actualResult);

expectedRowCount = 4;
actualRowCount = table.rowCount();
Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting

TEST_METHOD(TestEvaluationTable_OneSynonym) {
  std::vector<SYMBOL> synonyms = {"p"};
  EvaluationTable table(synonyms);

  /* Clause 1: p = {its, free, real, estate}
     One synonym
     |   p    |
     |--------|
     |  its   |
     |  free  |
     |  real  |
     | estate |
    */
  ClauseResult firstClause({{"p", {"its", "free", "real", "estate"}}});
  table.add(firstClause);

  std::unordered_set<VALUE> expectedResult = {"its", "free", "real", "estate"};
  std::unordered_set<VALUE> actualResult = table.select("p");
  Assert::IsTrue(expectedResult == actualResult);

  int expectedRowCount = 4;
  int actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);

  /* Clause 2: p = {free, real, estate}
     Same synonym - results should be filtered
     |   p    |
     |--------|
     |  free  |
     |  real  |
     | estate |
   */
  ClauseResult secondClause({{"p", {"not", "free", "real", "estate"}}});
  table.add(secondClause);

  expectedResult = {"free", "real", "estate"};
  actualResult = table.select("p");
  Assert::IsTrue(expectedResult == actualResult);

  expectedRowCount = 3;
  actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting

TEST_METHOD(TestEvaluationTable_CommonSynonyms) {
  std::vector<SYMBOL> synonyms = {"a", "v"};
  EvaluationTable table(synonyms);

  /* Clause 1: {a, v} = {{1, x}, {1, y}, {2, y}, {3, z}, {4, z}}
     Two synonyms
     | a | v |
     |---|---|
     | 1 | x |
     | 1 | y |
     | 2 | y |
     | 3 | z |
     | 4 | z |
    */
  ClauseResult firstClause(
      {{"a", {"1", "1", "2", "3", "4"}}, {"v", {"x", "y", "y", "z", "z"}}});
  table.add(firstClause);

  std::unordered_set<VALUE> expectedResult = {"1", "1", "2", "3", "4"};
  std::unordered_set<VALUE> actualResult = table.select("a");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"x", "y", "y", "z", "z"};
  actualResult = table.select("v");
  Assert::IsTrue(expectedResult == actualResult);

  int expectedRowCount = 5;
  int actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);

  /* Clause 2: {a, v} = {{100, x}, {1, y}, {2, yoo}, {3, z}}
     Both common synonyms - results should be filtered
     | a | v |
     |---|---|
     | 1 | y |
     | 3 | z |
   */
  ClauseResult secondClause(
      {{"a", {"100", "1", "2", "3"}}, {"v", {"x", "y", "yoo", "z"}}});
  table.add(secondClause);

  expectedResult = {"1", "3"};
  actualResult = table.select("a");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"y", "z"};
  actualResult = table.select("v");
  Assert::IsTrue(expectedResult == actualResult);

  expectedRowCount = 2;
  actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting

TEST_METHOD(TestEvaluationTable_NoCommonSynonym) {
  std::vector<SYMBOL> synonyms = {"a", "a1", "v", "v1"};
  EvaluationTable table(synonyms);

  /* Clause 1: {a, v} = {{1, abc}, {2, def}}
     Two synonyms
     | a | a1 |  v  | v1 |
     |---|----|-----|----|
     | 1 |    | abc |    |
     | 2 |    | def |    |
   */
  ClauseResult firstClause({{"a", {"1", "2"}}, {"v", {"abc", "def"}}});
  table.add(firstClause);

  std::unordered_set<VALUE> expectedResult = {"1", "2"};
  std::unordered_set<VALUE> actualResult = table.select("a");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"abc", "def"};
  actualResult = table.select("v");
  Assert::IsTrue(expectedResult == actualResult);

  int expectedRowCount = 2;
  int actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);

  /* Clause 2: {a1, v1} = {{1, x}, {2, x}, {2, y}}
     No common synonyms - outcome should be cross product
     | a | a1 |  v  | v1 |
     |---|----|-----|----|
     | 1 | 1  | abc | x  |
     | 2 | 2  | def | x  |
     | 1 | 2  | abc | y  |
     | 2 | 1  | def | x  |
     | 1 | 2  | abc | x  |
     | 2 | 2  | def | y  |
   */
  ClauseResult secondClause({{"a1", {"1", "2", "2"}}, {"v1", {"x", "x", "y"}}});
  table.add(secondClause);

  expectedResult = {"1", "2"};
  actualResult = table.select("a1");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"x", "y"};
  actualResult = table.select("v1");
  Assert::IsTrue(expectedResult == actualResult);

  expectedRowCount = 6;
  actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting

TEST_METHOD(TestEvaluationTable_EmptyResults) {
  std::vector<SYMBOL> synonyms = {"s", "a", "v"};
  EvaluationTable table(synonyms);

  /* Clause 1: {s, a} = {{1, 1}, {1, 2}}
     Identical to basic query test
     | s | a | v |
     |---|---|---|
     | 1 | 1 |   |
     | 1 | 2 |   |
   */
  ClauseResult firstClause({{"s", {"1", "1"}}, {"a", {"1", "2"}}});
  table.add(firstClause);

  /* Clause 2: {v} = {}
     Empty results
     | s | a | v |
     |---|---|---|
   */
  ClauseResult secondClause({{"v", std::vector<VALUE>()}});
  table.add(secondClause);

  std::unordered_set<VALUE> expectedResult = {};
  std::unordered_set<VALUE> actualResult = table.select("s");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {};
  actualResult = table.select("v");
  Assert::IsTrue(expectedResult == actualResult);

  int expectedRowCount = 0;
  int actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting

TEST_METHOD(TestEvaluationTable_ComplexQuery) {
  std::vector<SYMBOL> synonyms = {"s", "a", "v", "w", "i", "p"};
  EvaluationTable table(synonyms);

  /* Clause 1: {s, a} = {{1, 1}, {1, 2}, {2, 3}, {3, 3}}
     Identical to basic query test
     | s | a | v | w | i | p |
     |---|---|---|---|---|---|
     | 1 | 1 |   |   |   |   |
     | 1 | 2 |   |   |   |   |
     | 2 | 3 |   |   |   |   |
     | 3 | 3 |   |   |   |   |
   */
  ClauseResult firstClause(
      {{"s", {"1", "1", "2", "3"}}, {"a", {"1", "2", "3", "3"}}});
  table.add(firstClause);

  /* Clause 2: {a, v} = {{2, w}, {2, x}, {3, z}}
     Identical to basic query test
     | s | a | v | w | i | p |
     |---|---|---|---|---|---|
     | 1 | 2 | w |   |   |   |
     | 1 | 2 | x |   |   |   |
     | 2 | 3 | z |   |   |   |
     | 3 | 3 | z |   |   |   |
   */
  ClauseResult secondClause({{"a", {"2", "2", "3"}}, {"v", {"w", "x", "z"}}});
  table.add(secondClause);

  /* Clause 3: {w, i} = {{2, 5}, {4, 5}}
     No common synonyms
     | s | a | v | w | i | p |
     |---|---|---|---|---|---|
     | 1 | 2 | w | 2 | 5 |   |
     | 1 | 2 | x | 2 | 5 |   |
     | 2 | 3 | z | 2 | 5 |   |
     | 3 | 3 | z | 2 | 5 |   |
     | 1 | 2 | w | 4 | 5 |   |
     | 1 | 2 | x | 4 | 5 |   |
     | 2 | 3 | z | 4 | 5 |   |
     | 3 | 3 | z | 4 | 5 |   |
   */
  ClauseResult thirdClause({{"w", {"2", "4"}}, {"i", {"5", "5"}}});
  table.add(thirdClause);

  std::unordered_set<VALUE> expectedResult = {"2", "4"};
  std::unordered_set<VALUE> actualResult = table.select("w");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"5"};
  actualResult = table.select("i");
  Assert::IsTrue(expectedResult == actualResult);

  int expectedRowCount = 8;
  int actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);

  /* Clause 4: {v, p} = {{a, never}, {w, gonna}, {x, give}, {c, you}, {x, up}}
     One common one new
     | s | a | v | w | i |   p   |
     |---|---|---|---|---|-------|
     | 1 | 2 | w | 2 | 5 | gonna |
     | 1 | 2 | x | 2 | 5 |  give |
     | 1 | 2 | x | 2 | 5 |  up   |
     | 1 | 2 | w | 4 | 5 | gonna |
     | 1 | 2 | x | 4 | 5 |  give |
     | 1 | 2 | x | 4 | 5 |  up   |
   */
  ClauseResult fourthClause({{"v", {"a", "w", "x", "c", "x"}},
                             {"p", {"never", "gonna", "give", "you", "up"}}});
  table.add(fourthClause);

  expectedResult = {"1"};
  actualResult = table.select("s");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"2"};
  actualResult = table.select("a");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"w", "x"};
  actualResult = table.select("v");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"2", "4"};
  actualResult = table.select("w");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"5"};
  actualResult = table.select("i");
  Assert::IsTrue(expectedResult == actualResult);

  expectedResult = {"gonna", "give", "up"};
  actualResult = table.select("p");
  Assert::IsTrue(expectedResult == actualResult);

  expectedRowCount = 6;
  actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting
}
;
}
