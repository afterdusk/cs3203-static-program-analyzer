#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPqlEvaluator){
  public :

      TEST_METHOD(TestEvaluationTable_MergeBasicQuery){
          std::vector<SYMBOL> symbols = {"s", "a", "v"};
EvaluationTable table;

/* Clause 1: {s, a} = {{1, 1}, {1, 2}, {2, 3}, {3, 3}}
   Insertion into empty table
   | s | a | v |
   |---|---|---|
   | 1 | 1 |   |
   | 1 | 2 |   |
   | 2 | 3 |   |
   | 3 | 3 |   |
 */
EvaluationTable firstClause(new TABLE({{"s", {"1", "1", "2", "3"}},
                                       {"a", {"1", "2", "3", "3"}}}));
table.hashMerge(firstClause);

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
EvaluationTable
    secondClause(new TABLE({{"a", {"2", "2", "3"}}, {"v", {"w", "x", "z"}}}));
table.hashMerge(secondClause);

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

TEST_METHOD(TestEvaluationTable_MergeOneSynonym) {
  EvaluationTable table;

  /* Clause 1: p = {its, free, real, estate}
     One synonym
     |   p    |
     |--------|
     |  its   |
     |  free  |
     |  real  |
     | estate |
    */
  EvaluationTable firstClause(
      new TABLE({{"p", {"its", "free", "real", "estate"}}}));
  table.hashMerge(firstClause);

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
  EvaluationTable secondClause(
      new TABLE({{"p", {"not", "free", "real", "estate"}}}));
  table.hashMerge(secondClause);

  expectedResult = {"free", "real", "estate"};
  actualResult = table.select("p");
  Assert::IsTrue(expectedResult == actualResult);

  expectedRowCount = 3;
  actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting

TEST_METHOD(TestEvaluationTable_MergeEmpty) {
  EvaluationTable original(
      new TABLE({{"s", {"1", "1", "2", "3"}}, {"a", {"1", "2", "3", "3"}}}));
  EvaluationTable empty(new TABLE);

  EvaluationTable expected = original;
  EvaluationTable actual = original;
  actual.hashMerge(empty);

  Assert::IsTrue(expected == actual);
}

TEST_METHOD(TestEvaluationTable_MergeCommonSynonyms) {
  EvaluationTable table;

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
  EvaluationTable firstClause(new TABLE(
      {{"a", {"1", "1", "2", "3", "4"}}, {"v", {"x", "y", "y", "z", "z"}}}));
  table.hashMerge(firstClause);

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
  EvaluationTable secondClause(new TABLE(
      {{"a", {"100", "1", "2", "3"}}, {"v", {"x", "y", "yoo", "z"}}}));
  table.hashMerge(secondClause);

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

TEST_METHOD(TestEvaluationTable_MergeNoCommonSynonym) {
  EvaluationTable table;

  /* Clause 1: {a, v} = {{1, abc}, {2, def}}
     Two synonyms
     | a | a1 |  v  | v1 |
     |---|----|-----|----|
     | 1 |    | abc |    |
     | 2 |    | def |    |
   */
  EvaluationTable firstClause(
      new TABLE({{"a", {"1", "2"}}, {"v", {"abc", "def"}}}));
  table.hashMerge(firstClause);

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
  EvaluationTable secondClause(
      new TABLE({{"a1", {"1", "2", "2"}}, {"v1", {"x", "x", "y"}}}));
  table.hashMerge(secondClause);

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

TEST_METHOD(TestEvaluationTable_MergeEmptyResults) {
  EvaluationTable table;

  /* Clause 1: {s, a} = {{1, 1}, {1, 2}}
     Identical to basic query test
     | s | a | v |
     |---|---|---|
     | 1 | 1 |   |
     | 1 | 2 |   |
   */
  EvaluationTable firstClause(
      new TABLE({{"s", {"1", "1"}}, {"a", {"1", "2"}}}));
  table.hashMerge(firstClause);

  /* Clause 2: {v} = {}
     Empty results
     | s | a | v |
     |---|---|---|
   */
  EvaluationTable secondClause(new TABLE({{"v", std::vector<VALUE>()}}));
  table.hashMerge(secondClause);

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

TEST_METHOD(TestEvaluationTable_MergeComplexQuery) {
  EvaluationTable table;

  /* Clause 1: {s, a} = {{1, 1}, {1, 2}, {2, 3}, {3, 3}}
     Identical to basic query test
     | s | a | v | w | i | p |
     |---|---|---|---|---|---|
     | 1 | 1 |   |   |   |   |
     | 1 | 2 |   |   |   |   |
     | 2 | 3 |   |   |   |   |
     | 3 | 3 |   |   |   |   |
   */
  EvaluationTable firstClause(
      new TABLE({{"s", {"1", "1", "2", "3"}}, {"a", {"1", "2", "3", "3"}}}));
  table.hashMerge(firstClause);

  /* Clause 2: {a, v} = {{2, w}, {2, x}, {3, z}}
     Identical to basic query test
     | s | a | v | w | i | p |
     |---|---|---|---|---|---|
     | 1 | 2 | w |   |   |   |
     | 1 | 2 | x |   |   |   |
     | 2 | 3 | z |   |   |   |
     | 3 | 3 | z |   |   |   |
   */
  EvaluationTable secondClause(
      new TABLE({{"a", {"2", "2", "3"}}, {"v", {"w", "x", "z"}}}));
  table.hashMerge(secondClause);

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
  EvaluationTable thirdClause(
      new TABLE({{"w", {"2", "4"}}, {"i", {"5", "5"}}}));
  table.hashMerge(thirdClause);

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
  EvaluationTable fourthClause(
      new TABLE({{"v", {"a", "w", "x", "c", "x"}},
                 {"p", {"never", "gonna", "give", "you", "up"}}}));
  table.hashMerge(fourthClause);

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

TEST_METHOD(TestEvaluationTable_Slice) {
  /* Table to be sliced
     | s | a | v | w | i |   p   |
     |---|---|---|---|---|-------|
     | 1 | 2 | w | 2 | 5 | gonna |
     | 1 | 2 | x | 2 | 5 |  give |
     | 1 | 2 | x | 2 | 5 |  up   |
     | 1 | 2 | w | 4 | 5 | gonna |
     | 1 | 2 | x | 4 | 5 |  give |
     | 1 | 2 | x | 4 | 5 |  up   |
   */
  EvaluationTable table(
      new TABLE({{"s", {"1", "1", "1", "1", "1", "1"}},
                 {"a", {"2", "2", "2", "2", "2", "2"}},
                 {"v", {"w", "x", "x", "w", "x", "x"}},
                 {"w", {"2", "2", "2", "4", "4", "4"}},
                 {"i", {"5", "5", "5", "5", "5", "5"}},
                 {"p", {"gonna", "give", "up", "gonna", "give", "up"}}}));

  /* Single column
     | s |
     |---|
     | 1 |
   */
  EvaluationTable expected = EvaluationTable(new TABLE({{"s", {"1"}}}));
  EvaluationTable actual = table.slice({"s"});
  Assert::IsTrue(expected == actual);

  /* Two columns, one unique entry
     | s | a |
     |---|---|
     | 1 | 2 |
   */
  expected = EvaluationTable(new TABLE({{"s", {"1"}}, {"a", {"2"}}}));
  actual = table.slice({"s", "a"});
  Assert::IsTrue(expected == actual);

  /* Three columns, multiple unique entries
     | s | a | v |
     |---|---|---|
     | 1 | 2 | w |
     | 1 | 2 | x |
   */
  expected = EvaluationTable(
      new TABLE({{"s", {"1", "1"}}, {"a", {"2", "2"}}, {"v", {"w", "x"}}}));
  actual = table.slice({"s", "a", "v"});
  Assert::IsTrue(expected == actual);

  /* Three columns, multiple unique entries
     | s | v |   p   |
     |---|---|-------|
     | 1 | w | gonna |
     | 1 | x |  give |
     | 1 | x |  up   |
   */
  expected = EvaluationTable(new TABLE({{"s", {"1", "1", "1"}},
                                        {"v", {"w", "x", "x"}},
                                        {"p", {"gonna", "give", "up"}}}));
  actual = table.slice({"s", "v", "p"});
  Assert::IsTrue(expected == actual);

  // Invalid synonym in slice
  Assert::ExpectException<const char *>([&table] { table.slice({"a", "k"}); });
}

TEST_METHOD(TestEvaluationTable_Flatten) {
  /* Single column
     | s |
     |---|
     | 1 |
     | 2 |
     | 3 |
     | 4 |
   */
  EvaluationTable table(new TABLE({{"s", {"1", "2", "3", "4"}}}));
  std::list<VALUE> expected = {"1", "2", "3", "4"};
  std::list<VALUE> actual;
  table.flatten({"s"}, actual);
  expected.sort();
  actual.sort();
  Assert::IsTrue(expected == actual);

  /* Multiple columns
     | s | a | v | w | i |   p   |
     |---|---|---|---|---|-------|
     | 1 | 2 | w | 2 | 5 | gonna |
     | 1 | 2 | x | 2 | 5 |  give |
     | 1 | 2 | x | 2 | 5 |  up   |
     | 1 | 2 | w | 4 | 5 | gonna |
     | 1 | 2 | x | 4 | 5 |  give |
     | 1 | 2 | x | 4 | 5 |  up   |
   */
  table = EvaluationTable(
      new TABLE({{"s", {"1", "1", "1", "1", "1", "1"}},
                 {"a", {"2", "2", "2", "2", "2", "2"}},
                 {"v", {"w", "x", "x", "w", "x", "x"}},
                 {"w", {"2", "2", "2", "4", "4", "4"}},
                 {"i", {"5", "5", "5", "5", "5", "5"}},
                 {"p", {"gonna", "give", "up", "gonna", "give", "up"}}}));
  expected = {"gonna 5 1 2 w 2", "give 5 1 2 x 2", "up 5 1 2 x 2",
              "gonna 5 1 2 w 4", "give 5 1 2 x 4", "up 5 1 2 x 4"};
  actual.clear();
  table.flatten({"p", "i", "s", "a", "v", "w"}, actual);
  expected.sort();
  actual.sort();
  Assert::IsTrue(expected == actual);

  /* Multiple columns, but only subset of synonyms included
   | s | a | v | w | i |   p   |     | s | v |   p   |
   |---|---|---|---|---|-------|     |---|---|-------|
   | 1 | 2 | w | 2 | 5 | gonna |     | 1 | w | gonna |
   | 1 | 2 | x | 2 | 5 |  give | --> | 1 | x |  give |
   | 1 | 2 | x | 2 | 5 |  up   |     | 1 | x |  up   |
   | 1 | 2 | w | 4 | 5 | gonna |
   | 1 | 2 | x | 4 | 5 |  give |
   | 1 | 2 | x | 4 | 5 |  up   |
 */
  table = EvaluationTable(
      new TABLE({{"s", {"1", "1", "1", "1", "1", "1"}},
                 {"a", {"2", "2", "2", "2", "2", "2"}},
                 {"v", {"w", "x", "x", "w", "x", "x"}},
                 {"w", {"2", "2", "2", "4", "4", "4"}},
                 {"i", {"5", "5", "5", "5", "5", "5"}},
                 {"p", {"gonna", "give", "up", "gonna", "give", "up"}}}));
  expected = {"gonna 1 w", "give 1 x", "up 1 x"};
  actual.clear();
  table.flatten({"p", "s", "v"}, actual);
  expected.sort();
  actual.sort();
  Assert::IsTrue(expected == actual);

  // Invalid synonym in provided order
  Assert::ExpectException<const char *>([&table] { table.slice({"a", "k"}); });
}
}
;
}
