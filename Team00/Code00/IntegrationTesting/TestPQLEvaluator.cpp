#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPQLEvaluator) {
public:
  SetUpTests setUpTests;

  TEST_METHOD_INITIALIZE(SetUpPkbTables) {
    setUpTests = SetUpTests::SetUpTests();
  }

  TEST_METHOD(TestClauseDispatcher_WillReturnBoolean_True) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::NUMBER, "1"},
                                PqlToken{TokenType::NUMBER, "2"}};
    ClauseDispatcher firstDispatcher(first, setUpTests.pkbQueryInterface);
    Assert::IsTrue(firstDispatcher.willReturnBoolean());

    ParsedRelationship second = {TokenType::FOLLOWS_T,
                                 PqlToken{TokenType::NUMBER, "1"},
                                 PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher secondDispatcher(second, setUpTests.pkbQueryInterface);
    Assert::IsTrue(secondDispatcher.willReturnBoolean());

    ParsedRelationship third = {TokenType::PARENT,
                                PqlToken{TokenType::UNDERSCORE},
                                PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher thirdDispatcher(second, setUpTests.pkbQueryInterface);
    Assert::IsTrue(thirdDispatcher.willReturnBoolean());

    ParsedRelationship fourth = {TokenType::USES,
                                 PqlToken{TokenType::STRING, "proc"},
                                 PqlToken{TokenType::STRING, "x"}};
    ClauseDispatcher fourthDispatcher(fourth, setUpTests.pkbQueryInterface);
    Assert::IsTrue(fourthDispatcher.willReturnBoolean());
  }

  TEST_METHOD(TestClauseDispatcher_WillReturnBoolean_False) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::NUMBER, "1"},
                                PqlToken{TokenType::STMT, "s"}};
    ClauseDispatcher firstDispatcher(first, setUpTests.pkbQueryInterface);
    Assert::IsTrue(!firstDispatcher.willReturnBoolean());

    ParsedRelationship second = {TokenType::FOLLOWS_T,
                                 PqlToken{TokenType::CALL, "c"},
                                 PqlToken{TokenType::ASSIGN, "a"}};
    ClauseDispatcher secondDispatcher(second, setUpTests.pkbQueryInterface);
    Assert::IsTrue(!secondDispatcher.willReturnBoolean());

    ParsedRelationship third = {TokenType::USES,
                                PqlToken{TokenType::PROCEDURE, "p"},
                                PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher thirdDispatcher(second, setUpTests.pkbQueryInterface);
    Assert::IsTrue(!thirdDispatcher.willReturnBoolean());

    // Query without a relationship
    TokenType fourth = TokenType::CALL;
    ClauseDispatcher fourthDispatcher(fourth, setUpTests.pkbQueryInterface);
    Assert::IsTrue(!fourthDispatcher.willReturnBoolean());
  }

  TEST_METHOD(TestClauseDispatcher_BooleanDispatchFollows) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::NUMBER, "1"},
                                PqlToken{TokenType::NUMBER, "2"}};
    ClauseDispatcher firstDispatcher(first, setUpTests.pkbQueryInterface);
    bool expected = true;
    bool actual = firstDispatcher.booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [&firstDispatcher] { firstDispatcher.resultDispatch(); });

    ParsedRelationship second = {TokenType::FOLLOWS,
                                 PqlToken{TokenType::NUMBER, "1"},
                                 PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher secondDispatcher(second, setUpTests.pkbQueryInterface);
    expected = true;
    actual = secondDispatcher.booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [&secondDispatcher] { secondDispatcher.resultDispatch(); });

    ParsedRelationship third = {TokenType::FOLLOWS,
                                PqlToken{TokenType::UNDERSCORE},
                                PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher thirdDispatcher(third, setUpTests.pkbQueryInterface);
    expected = true;
    actual = thirdDispatcher.booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [&thirdDispatcher] { thirdDispatcher.resultDispatch(); });

    ParsedRelationship fourth = {TokenType::FOLLOWS,
                                 PqlToken{TokenType::NUMBER, "3"},
                                 PqlToken{TokenType::NUMBER, "4"}};
    ClauseDispatcher fourthDispatcher(fourth, setUpTests.pkbQueryInterface);
    expected = false;
    actual = fourthDispatcher.booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [&fourthDispatcher] { fourthDispatcher.resultDispatch(); });
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchFollows) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::ASSIGN, "a"},
                                PqlToken{TokenType::READ, "r"}};
    ClauseDispatcher firstDispatcher(first, setUpTests.pkbQueryInterface);
    ClauseResult firstExpected = ClauseResult({{"a", {"4"}}, {"r", {"5"}}});
    ClauseResult firstActual = firstDispatcher.resultDispatch();
    Assert::IsTrue(firstExpected == firstActual);
    Assert::ExpectException<const char *>(
        [&firstDispatcher] { firstDispatcher.booleanDispatch(); });

    ParsedRelationship second = {TokenType::FOLLOWS,
                                 PqlToken{TokenType::ASSIGN, "a"},
                                 PqlToken{TokenType::STMT, "s"}};
    ClauseDispatcher secondDispatcher(second, setUpTests.pkbQueryInterface);
    ClauseResult secondExpected =
        ClauseResult({{"a", {"4", "10", "16"}}, {"s", {"5", "11", "17"}}});
    ClauseResult secondActual = secondDispatcher.resultDispatch();
    Assert::IsTrue(secondExpected == secondActual);
    Assert::ExpectException<const char *>(
        [&secondDispatcher] { secondDispatcher.booleanDispatch(); });

    ParsedRelationship third = {TokenType::FOLLOWS,
                                PqlToken{TokenType::UNDERSCORE},
                                PqlToken{TokenType::READ, "r"}};
    ClauseDispatcher thirdDispatcher(third, setUpTests.pkbQueryInterface);
    ClauseResult thirdExpected =
        ClauseResult({{"r", {"2", "5", "8", "9", "13"}}});
    ClauseResult thirdActual = thirdDispatcher.resultDispatch();
    Assert::IsTrue(thirdExpected == thirdActual);
    Assert::ExpectException<const char *>(
        [&thirdDispatcher] { thirdDispatcher.booleanDispatch(); });
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchMatch) {
    PatternSpec spec1 = PatternSpec{PatternMatchType::CompleteMatch};
    PkbTables::AST qminus1;
    TNode T3 = TNode(TNode::Op::Minus);
    T3.left = new TNode("q");
    T3.right = new TNode("1");
    qminus1 = T3;
    spec1.value = &qminus1;
    ParsedPattern first = {PqlToken{TokenType::ASSIGN, "a"},
                           PqlToken{TokenType::STRING, "q"}, spec1};
    ClauseDispatcher firstDispatcher(first, setUpTests.pkbQueryInterface);
    ClauseResult firstExpected = ClauseResult({{"a", {"20", "24"}}});
    ClauseResult firstActual = firstDispatcher.resultDispatch();
    Assert::IsTrue(firstExpected == firstActual);
    Assert::ExpectException<const char *>(
        [&firstDispatcher] { firstDispatcher.booleanDispatch(); });

    PatternSpec spec2 = PatternSpec{PatternMatchType::SubTreeMatch};
    PkbTables::AST nodex = TNode("x");
    spec2.value = &nodex;
    ParsedPattern second = {PqlToken{TokenType::ASSIGN, "a"},
                            PqlToken{TokenType::STRING, "y"}, spec2};
    ClauseDispatcher secondDispatcher(second, setUpTests.pkbQueryInterface);
    std::unordered_map<SYMBOL, std::vector<VALUE>> expectedMap = {{"a", {"4"}}};
    ClauseResult secondExpected = ClauseResult(expectedMap);
    ClauseResult secondActual = secondDispatcher.resultDispatch();
    Assert::IsTrue(secondExpected == secondActual);
    Assert::ExpectException<const char *>(
        [&secondDispatcher] { secondDispatcher.booleanDispatch(); });

    PatternSpec spec3 = PatternSpec{PatternMatchType::SubTreeMatch};
    PkbTables::AST const1 = TNode("1");
    spec3.value = &const1;
    ParsedPattern third = {PqlToken{TokenType::ASSIGN, "a"},
                           PqlToken{TokenType::STRING, "q"}, spec3};
    ClauseDispatcher thirdDispatcher(third, setUpTests.pkbQueryInterface);
    ClauseResult thirdExpected = ClauseResult({{"a", {"20", "24"}}});
    ClauseResult thirdActual = thirdDispatcher.resultDispatch();
    Assert::IsTrue(thirdExpected == thirdActual);
    Assert::ExpectException<const char *>(
        [&thirdDispatcher] { thirdDispatcher.booleanDispatch(); });
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchRepeatedSynonym) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::STMT, "s"},
                                PqlToken{TokenType::STMT, "s"}};
    ClauseDispatcher firstDispatcher(first, setUpTests.pkbQueryInterface);
    ClauseResult firstExpected = ClauseResult({{"s", std::vector<VALUE>()}});
    ClauseResult firstActual = firstDispatcher.resultDispatch();
    Assert::IsTrue(firstExpected == firstActual);
    Assert::ExpectException<const char *>(
        [&firstDispatcher] { firstDispatcher.booleanDispatch(); });
  } // namespace UnitTesting

  TEST_METHOD(TestEvaluateParsedQuery_SingleSuchThatClause) {
    // stmt s; Select s such that Follows(3, s)
    ParsedQuery pq = {{{"s", TokenType::STMT}},
                      {"s"},
                      {{TokenType::FOLLOWS,
                        {TokenType::NUMBER, "3"},
                        {TokenType::STMT, "s"}}}};
    std::list<std::string> expected = {"6"};
    std::list<std::string> actual = PQL::evaluate(pq, setUpTests.pkbTables);
    Assert::IsTrue(expected == actual);

    // read r1; read r2; Select r2 such that Follows(r1, r2)
    pq = {{{"r1", TokenType::READ}, {"r2", TokenType::READ}},
          {"r2"},
          {{TokenType::FOLLOWS,
            {TokenType::READ, "r1"},
            {TokenType::READ, "r2"}}}};
    expected = {"2", "9", "13"};
    actual = PQL::evaluate(pq, setUpTests.pkbTables);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // call c; Select c such that Follows(_, c)
    pq = {{{"c", TokenType::CALL}},
          {"c"},
          {{TokenType::FOLLOWS,
            {TokenType::UNDERSCORE},
            {TokenType::CALL, "c"}}}};
    expected = {"11", "25"};
    actual = PQL::evaluate(pq, setUpTests.pkbTables);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // while w; if i; print p; Select p such that Follows(w, i)
    pq = {
        {{"w", TokenType::WHILE},
         {"i", TokenType::IF},
         {"p", TokenType::PRINT}},
        {"p"},
        {{TokenType::FOLLOWS, {TokenType::WHILE, "w"}, {TokenType::IF, "i"}}}};
    expected = {"6", "21", "22", "26"};
    actual = PQL::evaluate(pq, setUpTests.pkbTables);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // assign a; print p; stmt s; Select s such that Follows(a, p)
    pq = {{{"a", TokenType::ASSIGN},
           {"p", TokenType::PRINT},
           {"s", TokenType::STMT}},
          {"s"},
          {{TokenType::FOLLOWS,
            {TokenType::ASSIGN, "a"},
            {TokenType::PRINT, "p"}}}};
    expected = {};
    actual = PQL::evaluate(pq, setUpTests.pkbTables);
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SinglePatternClause) {
    // assign a; Select a pattern a ("q", "1")
    PatternSpec spec = PatternSpec{PatternMatchType::SubTreeMatch};
    PkbTables::AST const1 = TNode("1");
    spec.value = &const1;
    ParsedQuery pq = {{{"a", TokenType::ASSIGN}},
                      {"a"},
                      {},
                      {ParsedPattern{PqlToken{TokenType::ASSIGN, "a"},
                                     PqlToken{TokenType::STRING, "q"}, spec}}};
    std::list<std::string> expected = {"20", "24"};
    std::list<std::string> actual = PQL::evaluate(pq, setUpTests.pkbTables);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_NoClause) {
    // while w; if i; Select i
    ParsedQuery pq = {
        {{"w", TokenType::WHILE}, {"i", TokenType::IF}}, {"i"}, {}};
    std::list<std::string> expected = {"15", "19"};
    std::list<std::string> actual = PQL::evaluate(pq, setUpTests.pkbTables);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }
};
} // namespace IntegrationTesting
