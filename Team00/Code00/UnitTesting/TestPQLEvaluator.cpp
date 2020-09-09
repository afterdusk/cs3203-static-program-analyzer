#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestEvaluationTable){
  public :

      TEST_METHOD(TestBasicQuery){
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
ClauseResult s("s", {"1", "1", "2", "3"});
ClauseResult a("a", {"1", "2", "3", "3"});
table.add({s, a});

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
a = ClauseResult("a", {"2", "2", "3"});
ClauseResult v("v", {"w", "x", "z"});
table.add({a, v});

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

TEST_METHOD(TestOneSynonym) {
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
  ClauseResult p("p", {"its", "free", "real", "estate"});
  table.add({p});

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
  p = {"p", {"not", "free", "real", "estate"}};
  table.add({p});

  expectedResult = {"free", "real", "estate"};
  actualResult = table.select("p");
  Assert::IsTrue(expectedResult == actualResult);

  expectedRowCount = 3;
  actualRowCount = table.rowCount();
  Assert::IsTrue(expectedRowCount == actualRowCount);
} // namespace UnitTesting

TEST_METHOD(TestCommonSynonyms) {
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
  ClauseResult a("a", {"1", "1", "2", "3", "4"});
  ClauseResult v("v", {"x", "y", "y", "z", "z"});
  table.add({a, v});

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
  a = ClauseResult("a", {"100", "1", "2", "3"});
  v = ClauseResult("v", {"x", "y", "yoo", "z"});
  table.add({a, v});

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

TEST_METHOD(TestNoCommonSynonym) {
  std::vector<SYMBOL> synonyms = {"a", "a1", "v", "v1"};
  EvaluationTable table(synonyms);

  /* Clause 1: {a, v} = {{1, abc}, {2, def}}
     Two synonyms
     | a | a1 |  v  | v1 |
     |---|----|-----|----|
     | 1 |    | abc |    |
     | 2 |    | def |    |
   */
  ClauseResult a("a", {"1", "2"});
  ClauseResult v("v", {"abc", "def"});
  table.add({a, v});

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
  ClauseResult a1("a1", {"1", "2", "2"});
  ClauseResult v1("v1", {"x", "x", "y"});
  table.add({a1, v1});

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

TEST_METHOD(TestComplexQuery) {
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
  ClauseResult s("s", {"1", "1", "2", "3"});
  ClauseResult a("a", {"1", "2", "3", "3"});
  table.add({s, a});

  /* Clause 2: {a, v} = {{2, w}, {2, x}, {3, z}}
     Identical to basic query test
     | s | a | v | w | i | p |
     |---|---|---|---|---|---|
     | 1 | 2 | w |   |   |   |
     | 1 | 2 | x |   |   |   |
     | 2 | 3 | z |   |   |   |
     | 3 | 3 | z |   |   |   |
   */
  a = ClauseResult("a", {"2", "2", "3"});
  ClauseResult v("v", {"w", "x", "z"});
  table.add({a, v});

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
  ClauseResult w("w", {"2", "4"});
  ClauseResult i("i", {"5", "5"});
  table.add({w, i});

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
  v = ClauseResult("v", {"a", "w", "x", "c", "x"});
  ClauseResult p("p", {"never", "gonna", "give", "you", "up"});
  table.add({v, p});

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
