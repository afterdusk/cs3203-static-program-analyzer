#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPQLEvaluator) {
public:
  PKB pkb;
  PkbQueryInterface queryHandler;

  // TODO: Code duplication with PKB tests. Refactor to utils file.
  TEST_METHOD_INITIALIZE(SetUpPkb) {
    Statement w, r, a, c, i, p;
    LineNumber c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
        c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c999;
    Stmt stmt;
    Variable variable;
    Procedure procedure;
    Underscore underscore;

    w.type = StatementType::WHILE;
    r.type = StatementType::READ;
    a.type = StatementType::ASSIGN;
    c.type = StatementType::CALL;
    i.type = StatementType::IF;
    p.type = StatementType::PRINT;

    c1.number = "1";
    c2.number = "2";
    c3.number = "3";
    c4.number = "4";
    c5.number = "5";
    c6.number = "6";
    c7.number = "7";
    c8.number = "8";
    c9.number = "9";
    c10.number = "10";
    c11.number = "11";
    c12.number = "12";
    c13.number = "13";
    c14.number = "14";
    c15.number = "15";
    c16.number = "16";
    c17.number = "17";
    c18.number = "18";
    c19.number = "19";
    c20.number = "20";
    c21.number = "21";
    c22.number = "22";
    c23.number = "23";
    c24.number = "24";
    c25.number = "25";
    c26.number = "26";
    c999.number = "999";

    pkb.addFollow("1", "2");
    pkb.addFollow("2", "3");
    pkb.addFollow("3", "6");
    pkb.addFollow("4", "5");
    pkb.addFollow("7", "8");
    pkb.addFollow("8", "9");
    pkb.addFollow("9", "10");
    pkb.addFollow("10", "11");
    pkb.addFollow("12", "13");
    pkb.addFollow("13", "14");
    pkb.addFollow("14", "25");
    pkb.addFollow("15", "23");
    pkb.addFollow("16", "17");
    pkb.addFollow("17", "19");

    pkb.addStatementType("1", StatementType::READ);
    pkb.addStatementType("2", StatementType::READ);
    pkb.addStatementType("3", StatementType::WHILE);
    pkb.addStatementType("4", StatementType::ASSIGN);
    pkb.addStatementType("5", StatementType::READ);
    pkb.addStatementType("6", StatementType::PRINT);
    pkb.addStatementType("7", StatementType::CALL);
    pkb.addStatementType("8", StatementType::READ);
    pkb.addStatementType("9", StatementType::READ);
    pkb.addStatementType("10", StatementType::ASSIGN);
    pkb.addStatementType("11", StatementType::CALL);
    pkb.addStatementType("12", StatementType::READ);
    pkb.addStatementType("13", StatementType::READ);
    pkb.addStatementType("14", StatementType::WHILE);
    pkb.addStatementType("15", StatementType::IF);
    pkb.addStatementType("16", StatementType::ASSIGN);
    pkb.addStatementType("17", StatementType::WHILE);
    pkb.addStatementType("18", StatementType::ASSIGN);
    pkb.addStatementType("19", StatementType::IF);
    pkb.addStatementType("20", StatementType::ASSIGN);
    pkb.addStatementType("21", StatementType::PRINT);
    pkb.addStatementType("22", StatementType::PRINT);
    pkb.addStatementType("23", StatementType::WHILE);
    pkb.addStatementType("24", StatementType::ASSIGN);
    pkb.addStatementType("25", StatementType::CALL);
    pkb.addStatementType("26", StatementType::PRINT);

    queryHandler = PkbQueryInterface(pkb);
  }

  TEST_METHOD(TestClauseDispatcher_WillReturnBoolean_True) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::NUMBER, "1"},
                                PqlToken{TokenType::NUMBER, "2"}};
    ClauseDispatcher firstDispatcher(first, queryHandler);
    Assert::IsTrue(firstDispatcher.willReturnBoolean());

    ParsedRelationship second = {TokenType::FOLLOWS_T,
                                 PqlToken{TokenType::NUMBER, "1"},
                                 PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher secondDispatcher(second, queryHandler);
    Assert::IsTrue(secondDispatcher.willReturnBoolean());

    ParsedRelationship third = {TokenType::PARENT,
                                PqlToken{TokenType::UNDERSCORE},
                                PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher thirdDispatcher(second, queryHandler);
    Assert::IsTrue(thirdDispatcher.willReturnBoolean());

    ParsedRelationship fourth = {TokenType::USES,
                                 PqlToken{TokenType::STRING, "proc"},
                                 PqlToken{TokenType::STRING, "x"}};
    ClauseDispatcher fourthDispatcher(fourth, queryHandler);
    Assert::IsTrue(fourthDispatcher.willReturnBoolean());
  }

  TEST_METHOD(TestClauseDispatcher_WillReturnBoolean_False) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::NUMBER, "1"},
                                PqlToken{TokenType::STMT, "s"}};
    ClauseDispatcher firstDispatcher(first, queryHandler);
    Assert::IsTrue(!firstDispatcher.willReturnBoolean());

    ParsedRelationship second = {TokenType::FOLLOWS_T,
                                 PqlToken{TokenType::CALL, "c"},
                                 PqlToken{TokenType::ASSIGN, "a"}};
    ClauseDispatcher secondDispatcher(second, queryHandler);
    Assert::IsTrue(!secondDispatcher.willReturnBoolean());

    ParsedRelationship third = {TokenType::USES,
                                PqlToken{TokenType::PROCEDURE, "p"},
                                PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher thirdDispatcher(second, queryHandler);
    Assert::IsTrue(!thirdDispatcher.willReturnBoolean());

    // Query without a relationship
    TokenType fourth = TokenType::CALL;
    ClauseDispatcher fourthDispatcher(fourth, queryHandler);
    Assert::IsTrue(!fourthDispatcher.willReturnBoolean());
  }

  TEST_METHOD(TestClauseDispatcher_BooleanDispatchFollows) {
    ParsedRelationship first = {TokenType::FOLLOWS,
                                PqlToken{TokenType::NUMBER, "1"},
                                PqlToken{TokenType::NUMBER, "2"}};
    ClauseDispatcher firstDispatcher(first, queryHandler);
    bool expected = true;
    bool actual = firstDispatcher.booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [&firstDispatcher] { firstDispatcher.resultDispatch(); });

    ParsedRelationship second = {TokenType::FOLLOWS,
                                 PqlToken{TokenType::NUMBER, "1"},
                                 PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher secondDispatcher(second, queryHandler);
    expected = true;
    actual = secondDispatcher.booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [&secondDispatcher] { secondDispatcher.resultDispatch(); });

    ParsedRelationship third = {TokenType::FOLLOWS,
                                PqlToken{TokenType::UNDERSCORE},
                                PqlToken{TokenType::UNDERSCORE}};
    ClauseDispatcher thirdDispatcher(third, queryHandler);
    expected = true;
    actual = thirdDispatcher.booleanDispatch();
    Assert::IsTrue(actual == expected);
    Assert::ExpectException<const char *>(
        [&thirdDispatcher] { thirdDispatcher.resultDispatch(); });

    ParsedRelationship fourth = {TokenType::FOLLOWS,
                                 PqlToken{TokenType::NUMBER, "3"},
                                 PqlToken{TokenType::NUMBER, "4"}};
    ClauseDispatcher fourthDispatcher(fourth, queryHandler);
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
    ClauseDispatcher firstDispatcher(first, queryHandler);
    ClauseResult firstExpected = ClauseResult({{"a", {"4"}}, {"r", {"5"}}});
    ClauseResult firstActual = firstDispatcher.resultDispatch();
    Assert::IsTrue(firstExpected == firstActual);
    Assert::ExpectException<const char *>(
        [&firstDispatcher] { firstDispatcher.booleanDispatch(); });

    ParsedRelationship second = {TokenType::FOLLOWS,
                                 PqlToken{TokenType::ASSIGN, "a"},
                                 PqlToken{TokenType::STMT, "s"}};
    ClauseDispatcher secondDispatcher(second, queryHandler);
    ClauseResult secondExpected =
        ClauseResult({{"a", {"4", "10", "16"}}, {"s", {"5", "11", "17"}}});
    ClauseResult secondActual = secondDispatcher.resultDispatch();
    Assert::IsTrue(secondExpected == secondActual);
    Assert::ExpectException<const char *>(
        [&secondDispatcher] { secondDispatcher.booleanDispatch(); });

    ParsedRelationship third = {TokenType::FOLLOWS,
                                PqlToken{TokenType::UNDERSCORE},
                                PqlToken{TokenType::READ, "r"}};
    ClauseDispatcher thirdDispatcher(third, queryHandler);
    ClauseResult thirdExpected =
        ClauseResult({{"r", {"2", "5", "8", "9", "13"}}});
    ClauseResult thirdActual = thirdDispatcher.resultDispatch();
    Assert::IsTrue(thirdExpected == thirdActual);
    Assert::ExpectException<const char *>(
        [&thirdDispatcher] { thirdDispatcher.booleanDispatch(); });
  }

  TEST_METHOD(TestEvaluateParsedQuery_SingleClause) {
    // stmt s; Select s such that follows(3, s);
    ParsedQuery pq = {{{"s", TokenType::STMT}},
                      {"s"},
                      {{TokenType::FOLLOWS,
                        {TokenType::NUMBER, "3"},
                        {TokenType::STMT, "s"}}}};
    std::list<std::string> expected = {"6"};
    std::list<std::string> actual = PQL::evaluate(pq, pkb);
    Assert::IsTrue(expected == actual);

    // read r1; read r2; Select r2 such that follows(r1, r2);
    pq = {{{"r1", TokenType::READ}, {"r2", TokenType::READ}},
          {"r2"},
          {{TokenType::FOLLOWS,
            {TokenType::READ, "r1"},
            {TokenType::READ, "r2"}}}};
    expected = {"2", "9", "13"};
    actual = PQL::evaluate(pq, pkb);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // call c; Select c such that follows(_, c);
    pq = {{{"c", TokenType::CALL}},
          {"c"},
          {{TokenType::FOLLOWS,
            {TokenType::UNDERSCORE},
            {TokenType::CALL, "c"}}}};
    expected = {"11", "25"};
    actual = PQL::evaluate(pq, pkb);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // while w; if i; print p; Select p such that follows(w, i);
    pq = {
        {{"w", TokenType::WHILE},
         {"i", TokenType::IF},
         {"p", TokenType::PRINT}},
        {"p"},
        {{TokenType::FOLLOWS, {TokenType::WHILE, "w"}, {TokenType::IF, "i"}}}};
    expected = {"6", "21", "22", "26"};
    actual = PQL::evaluate(pq, pkb);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // assign a; print p; stmt s; Select s such that follows(a, p);
    pq = {{{"a", TokenType::ASSIGN},
           {"p", TokenType::PRINT},
           {"s", TokenType::STMT}},
          {"s"},
          {{TokenType::FOLLOWS,
            {TokenType::ASSIGN, "a"},
            {TokenType::PRINT, "p"}}}};
    expected = {};
    actual = PQL::evaluate(pq, pkb);
    Assert::IsTrue(expected == actual);
  }
};
} // namespace IntegrationTesting
