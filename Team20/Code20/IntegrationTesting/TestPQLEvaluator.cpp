#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPqlEvaluator) {
public:
  Pkb pkb;
  SetUpTests setUpTests = SetUpTests::SetUpTests(pkb);

  TEST_METHOD(TestClauseDispatcher_WillReturnBoolean_True) {
    ParsedRelationship pr = {TokenType::FOLLOWS,
                             PqlToken{TokenType::NUMBER, "1"},
                             PqlToken{TokenType::NUMBER, "2"}};
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    bool actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(actual);

    pr = {TokenType::FOLLOWS_T, PqlToken{TokenType::NUMBER, "1"},
          PqlToken{TokenType::UNDERSCORE}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(actual);

    pr = {TokenType::PARENT, PqlToken{TokenType::UNDERSCORE},
          PqlToken{TokenType::UNDERSCORE}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(actual);

    pr = {TokenType::USES, PqlToken{TokenType::STRING, "proc"},
          PqlToken{TokenType::STRING, "x"}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(actual);
  }

  TEST_METHOD(TestClauseDispatcher_WillReturnBoolean_False) {
    ParsedRelationship pr = {TokenType::FOLLOWS,
                             PqlToken{TokenType::NUMBER, "1"},
                             PqlToken{TokenType::STMT, "s"}};
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    bool actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(!actual);

    pr = {TokenType::FOLLOWS_T, PqlToken{TokenType::CALL, "c"},
          PqlToken{TokenType::ASSIGN, "a"}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(!actual);

    pr = {TokenType::USES, PqlToken{TokenType::PROCEDURE, "p"},
          PqlToken{TokenType::UNDERSCORE}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(!actual);

    // Select synonym
    PqlToken token = {TokenType::CALL, "c"};
    dispatcher = ClauseDispatcher::FromToken(token, pkb.getQueryInterface());
    actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(!actual);
  }

  TEST_METHOD(TestClauseDispatcher_BooleanDispatchFollows) {
    ParsedRelationship pr = {TokenType::FOLLOWS,
                             PqlToken{TokenType::NUMBER, "1"},
                             PqlToken{TokenType::NUMBER, "2"}};
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    bool expected = true;
    bool actual = dispatcher->booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->resultDispatch(); });
    delete dispatcher;

    pr = {TokenType::FOLLOWS, PqlToken{TokenType::NUMBER, "1"},
          PqlToken{TokenType::UNDERSCORE}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    expected = true;
    actual = dispatcher->booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->resultDispatch(); });
    delete dispatcher;

    pr = {TokenType::FOLLOWS, PqlToken{TokenType::UNDERSCORE},
          PqlToken{TokenType::UNDERSCORE}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    expected = true;
    actual = dispatcher->booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->resultDispatch(); });
    delete dispatcher;

    pr = {TokenType::FOLLOWS, PqlToken{TokenType::NUMBER, "3"},
          PqlToken{TokenType::NUMBER, "4"}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    expected = false;
    actual = dispatcher->booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->resultDispatch(); });
    delete dispatcher;
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchFollows) {
    ParsedRelationship pr = {TokenType::FOLLOWS,
                             PqlToken{TokenType::ASSIGN, "a"},
                             PqlToken{TokenType::READ, "r"}};
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    ClauseResult expected = ClauseResult({{"a", {"4"}}, {"r", {"5"}}});
    ClauseResult actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    pr = {TokenType::FOLLOWS, PqlToken{TokenType::ASSIGN, "a"},
          PqlToken{TokenType::STMT, "s"}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    expected =
        ClauseResult({{"a", {"4", "10", "16"}}, {"s", {"5", "11", "17"}}});
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    pr = {TokenType::FOLLOWS, PqlToken{TokenType::UNDERSCORE},
          PqlToken{TokenType::READ, "r"}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    expected = ClauseResult({{"r", {"2", "5", "8", "9", "13"}}});
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchMatch) {
    PatternSpec spec1 = PatternSpec{PatternMatchType::CompleteMatch};
    PkbTables::AST qminus1;
    TNode T3 = TNode(TNode::Op::Minus);
    T3.left = new TNode("q");
    T3.right = new TNode("1");
    qminus1 = T3;
    spec1.value = &qminus1;
    ParsedPattern pp = {PqlToken{TokenType::ASSIGN, "a"},
                        PqlToken{TokenType::STRING, "q"}, spec1};
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromPattern(pp, pkb.getQueryInterface());
    ClauseResult expected = ClauseResult({{"a", {"20", "24"}}});
    ClauseResult actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    PatternSpec spec2 = PatternSpec{PatternMatchType::SubTreeMatch};
    PkbTables::AST nodex = TNode("x");
    spec2.value = &nodex;
    pp = {PqlToken{TokenType::ASSIGN, "a"}, PqlToken{TokenType::STRING, "y"},
          spec2};
    dispatcher = ClauseDispatcher::FromPattern(pp, pkb.getQueryInterface());
    expected = ClauseResult(
        std::unordered_map<SYMBOL, std::vector<VALUE>>({{"a", {"4"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    PatternSpec spec3 = PatternSpec{PatternMatchType::SubTreeMatch};
    PkbTables::AST const1 = TNode("1");
    spec3.value = &const1;
    pp = {PqlToken{TokenType::ASSIGN, "a"}, PqlToken{TokenType::STRING, "q"},
          spec3};
    dispatcher = ClauseDispatcher::FromPattern(pp, pkb.getQueryInterface());
    expected = ClauseResult({{"a", {"20", "24"}}});
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchRepeatedSynonym) {
    ParsedRelationship pr = {TokenType::FOLLOWS, PqlToken{TokenType::STMT, "s"},
                             PqlToken{TokenType::STMT, "s"}};
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    ClauseResult expected = ClauseResult({{"s", std::vector<VALUE>()}});
    ClauseResult actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;
  } // namespace UnitTesting

  TEST_METHOD(TestEvaluateParsedQuery_SingleSuchThatClause) {
    // stmt s; Select s such that Follows(3, s)
    ParsedQuery pq = {{{"s", TokenType::STMT}},
                      {"s"},
                      {{TokenType::FOLLOWS,
                        {TokenType::NUMBER, "3"},
                        {TokenType::STMT, "s"}}}};
    std::list<std::string> expected = {"6"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    Assert::IsTrue(expected == actual);

    // read r1; read r2; Select r2 such that Follows(r1, r2)
    pq = {{{"r1", TokenType::READ}, {"r2", TokenType::READ}},
          {"r2"},
          {{TokenType::FOLLOWS,
            {TokenType::READ, "r1"},
            {TokenType::READ, "r2"}}}};
    expected = {"2", "9", "13"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
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
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
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
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
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
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
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
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectStatementNoClause) {
    // while w; if i; Select i
    ParsedQuery pq = {
        {{"w", TokenType::WHILE}, {"i", TokenType::IF}}, {"i"}, {}};
    std::list<std::string> expected = {"15", "19"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // call c; Select c
    pq = {{{"c", TokenType::CALL}}, {"c"}, {}};
    expected = {"7", "11", "25"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectProcedureNoClause) {
    // procedure p; Select p
    ParsedQuery pq = {{{"p", TokenType::PROCEDURE}}, {"p"}, {}};
    std::list<std::string> expected = {"main", "extra", "complicate", "aux"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectVariableNoClause) {
    // variable v; Select v
    ParsedQuery pq = {{{"v", TokenType::VARIABLE}}, {"v"}, {}};
    std::list<std::string> expected = {{"x", "y", "r", "m", "q", "t", "k"}};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectConstantNoClause) {
    // constant c; Select c
    ParsedQuery pq = {{{"c", TokenType::CONSTANT}}, {"c"}, {}};
    std::list<std::string> expected = {
        "0", "1", "5", "11111111111111111111111111111111111111"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }
};
} // namespace IntegrationTesting
