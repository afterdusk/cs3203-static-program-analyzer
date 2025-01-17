#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPqlEvaluator) {
public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::A);

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
    Element element = {"c", AttributeRefType::NONE};
    dispatcher = ClauseDispatcher::FromElement(TokenType::CALL, element,
                                               pkb.getQueryInterface());
    actual = dispatcher->willReturnBoolean();
    delete dispatcher;
    Assert::IsTrue(!actual);

    // With clause
    dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"c", AttributeRefType::VALUE}),
         Reference(Element{"n", AttributeRefType::NONE})},
        {{"n", TokenType::PROG_LINE}, {"c", TokenType::CONSTANT}},
        pkb.getQueryInterface());
    ;
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
    EvaluationTable expected =
        EvaluationTable(new TABLE({{"a", {"4"}}, {"r", {"5"}}}));
    EvaluationTable actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    pr = {TokenType::FOLLOWS, PqlToken{TokenType::ASSIGN, "a"},
          PqlToken{TokenType::STMT, "s"}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    expected = EvaluationTable(
        new TABLE({{"a", {"4", "10", "16"}}, {"s", {"5", "11", "17"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    pr = {TokenType::FOLLOWS, PqlToken{TokenType::UNDERSCORE},
          PqlToken{TokenType::READ, "r"}};
    dispatcher =
        ClauseDispatcher::FromRelationship(pr, pkb.getQueryInterface());
    expected = EvaluationTable(new TABLE({{"r", {"2", "5", "8", "9", "13"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchMatch) {
    PatternSpec spec1 = PatternSpec{PatternMatchType::CompleteMatch};
    PkbTables::AST T3 = std::make_shared<TNode>(TNode::Op::Minus);
    T3->left = std::make_shared<TNode>("q");
    T3->right = std::make_shared<TNode>("1");
    spec1.value = T3;
    ParsedPattern pp = {PqlToken{TokenType::ASSIGN, "a"},
                        PqlToken{TokenType::STRING, "q"}, spec1};
    ClauseDispatcher *dispatcher =
        ClauseDispatcher::FromPattern(pp, pkb.getQueryInterface());
    EvaluationTable expected =
        EvaluationTable(new TABLE({{"a", {"20", "24"}}}));
    EvaluationTable actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    PatternSpec spec2 = PatternSpec{PatternMatchType::SubTreeMatch};
    PkbTables::AST nodex = std::make_shared<TNode>("x");
    spec2.value = nodex;
    pp = {PqlToken{TokenType::ASSIGN, "a"}, PqlToken{TokenType::STRING, "y"},
          spec2};
    dispatcher = ClauseDispatcher::FromPattern(pp, pkb.getQueryInterface());
    expected = EvaluationTable(new TABLE(
        std::unordered_map<SYMBOL, std::vector<VALUE>>({{"a", {"4"}}})));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    PatternSpec spec3 = PatternSpec{PatternMatchType::SubTreeMatch};
    PkbTables::AST const1 = std::make_shared<TNode>("1");
    spec3.value = const1;
    pp = {PqlToken{TokenType::ASSIGN, "a"}, PqlToken{TokenType::STRING, "q"},
          spec3};
    dispatcher = ClauseDispatcher::FromPattern(pp, pkb.getQueryInterface());
    expected = EvaluationTable(new TABLE({{"a", {"20", "24"}}}));
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
    EvaluationTable expected =
        EvaluationTable(new TABLE({{"s", std::vector<VALUE>()}}));
    EvaluationTable actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchWithElementPair) {
    /* JUST_VALUE = JUST_VALUE
       constant c; prog_line n; with c.value = n
     */
    ClauseDispatcher *dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"c", AttributeRefType::VALUE}),
         Reference(Element{"n", AttributeRefType::NONE})},
        {{"n", TokenType::PROG_LINE}, {"c", TokenType::CONSTANT}},
        pkb.getQueryInterface());
    EvaluationTable expected =
        EvaluationTable(new TABLE({{"c", {"1", "5"}}, {"n", {"1", "5"}}}));
    EvaluationTable actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    /* JUST_VALUE = VALUE_ATTR_PAIR
       call c; procedure p; with p.procName = c.procName
     */
    dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"c", AttributeRefType::PROCNAME}),
         Reference(Element{"p", AttributeRefType::PROCNAME})},
        {{"c", TokenType::CALL}, {"p", TokenType::PROCEDURE}},
        pkb.getQueryInterface());
    expected = EvaluationTable(new TABLE(
        {{"c", {"7", "11", "25", "27", "28", "29", "30", "31", "32", "33"}},
         {"c.procName",
          {"aux", "complicate", "extra", "extra", "aux", "main", "extratwo",
           "extrathree", "main", "extrathree"}},
         {"p",
          {"aux", "complicate", "extra", "extra", "aux", "main", "extratwo",
           "extrathree", "main", "extrathree"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    /* VALUE_ATTR_PAIR = JUST_VALUE
       print pn; variable v; with pn.varName = v.varName
     */
    dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"pn", AttributeRefType::VARNAME}),
         Reference(Element{"v", AttributeRefType::VARNAME})},
        {{"pn", TokenType::PRINT}, {"v", TokenType::VARIABLE}},
        pkb.getQueryInterface());
    expected = EvaluationTable(new TABLE({{"pn", {"6", "21", "22", "26"}},
                                          {"pn.varName", {"x", "q", "t", "k"}},
                                          {"v", {"x", "q", "t", "k"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    /* VALUE_ATTR_PAIR = VALUE_ATTR_PAIR
       print pn; read rd; with pn.varName = rd.varName
     */
    dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"pn", AttributeRefType::VARNAME}),
         Reference(Element{"rd", AttributeRefType::VARNAME})},
        {{"pn", TokenType::PRINT}, {"rd", TokenType::READ}},
        pkb.getQueryInterface());
    expected =
        EvaluationTable(new TABLE({{"pn", {"6", "6", "21", "22"}},
                                   {"pn.varName", {"x", "x", "q", "t"}},
                                   {"rd", {"1", "8", "12", "13"}},
                                   {"rd.varName", {"x", "x", "q", "t"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;
  }

  TEST_METHOD(TestClauseDispatcher_ResultsDispatchWithElementRawPair) {
    /* JUST_VALUE (number)
       prog_line n; with n = 20
     */
    ClauseDispatcher *dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"n", AttributeRefType::NONE}),
         Reference(PqlToken{TokenType::NUMBER, "20"})},
        {{"n", TokenType::PROG_LINE}}, pkb.getQueryInterface());
    EvaluationTable expected = EvaluationTable(new TABLE({{"n", {"20"}}}));
    EvaluationTable actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    /* JUST_VALUE (number)
       constant c; with c.value = 5
     */
    dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"c", AttributeRefType::VALUE}),
         Reference(PqlToken{TokenType::NUMBER, "5"})},
        {{"c", TokenType::CONSTANT}}, pkb.getQueryInterface());
    expected = EvaluationTable(new TABLE({{"c", {"5"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    /* JUST_VALUE (string)
       variable v; with v.varName = "k"
     */
    dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"v", AttributeRefType::VARNAME}),
         Reference(PqlToken{TokenType::STRING, "k"})},
        {{"v", TokenType::VARIABLE}}, pkb.getQueryInterface());
    expected = EvaluationTable(new TABLE({{"v", {"k"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;

    /* VALUE_ATTR_PAIR
       print pn; with pn.varName = "q"
     */
    dispatcher = ClauseDispatcher::FromWith(
        {Reference(Element{"pn", AttributeRefType::VARNAME}),
         Reference(PqlToken{TokenType::STRING, "q"})},
        {{"pn", TokenType::PRINT}}, pkb.getQueryInterface());
    expected =
        EvaluationTable(new TABLE({{"pn.varName", {"q"}}, {"pn", {"21"}}}));
    actual = dispatcher->resultDispatch();
    Assert::IsTrue(expected == actual);
    Assert::ExpectException<const char *>(
        [dispatcher] { dispatcher->booleanDispatch(); });
    delete dispatcher;
  }

  TEST_METHOD(TestEvaluateParsedQuery_SingleSuchThatClause) {
    // stmt s; Select s such that Follows(3, s)
    ParsedQuery pq = {{{"s", TokenType::STMT}},
                      {PqlResultType::Tuple, {{"s", AttributeRefType::NONE}}},
                      {{TokenType::FOLLOWS,
                        {TokenType::NUMBER, "3"},
                        {TokenType::STMT, "s"}}}};
    std::list<std::string> expected = {"6"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    Assert::IsTrue(expected == actual);

    // read r1; read r2; Select r2 such that Follows(r1, r2)
    pq = {{{"r1", TokenType::READ}, {"r2", TokenType::READ}},
          {PqlResultType::Tuple, {{"r2", AttributeRefType::NONE}}},
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
          {PqlResultType::Tuple, {{"c", AttributeRefType::NONE}}},
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
        {PqlResultType::Tuple, {{"p", AttributeRefType::NONE}}},
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
          {PqlResultType::Tuple, {{"s", AttributeRefType::NONE}}},
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
    PkbTables::AST const1 = std::make_shared<TNode>("1");
    spec.value = const1;
    ParsedQuery pq = {{{"a", TokenType::ASSIGN}},
                      {PqlResultType::Tuple, {{"a", AttributeRefType::NONE}}},
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

  TEST_METHOD(TestEvaluateParsedQuery_SingleWithClause) {
    // read rd; print pn; Select pn.varName with pn.varName = rd.varName
    ParsedQuery pq = {
        {{"rd", TokenType::READ}, {"pn", TokenType::PRINT}},
        {PqlResultType::Tuple, {{"pn", AttributeRefType::VARNAME}}},
        {},
        {},
        {{Reference(Element{"pn", AttributeRefType::VARNAME}),
          Reference(Element{"rd", AttributeRefType::VARNAME})}}};
    std::list<std::string> expected = {"x", "q", "t"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // call c; Select c, c.procName with "aux" = c.procName
    pq = {{{"c", TokenType::CALL}},
          {PqlResultType::Tuple,
           {{"c", AttributeRefType::PROCNAME}, {"c", AttributeRefType::NONE}}},
          {},
          {},
          {{Reference(PqlToken{TokenType::STRING, "aux"}),
            Reference(Element{"c", AttributeRefType::PROCNAME})}}};
    expected = {"aux 7", "aux 28"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectStatementNoClause) {
    // while w; if i; Select i
    ParsedQuery pq = {{{"w", TokenType::WHILE}, {"i", TokenType::IF}},
                      {PqlResultType::Tuple, {{"i", AttributeRefType::NONE}}},
                      {}};
    std::list<std::string> expected = {"15", "19"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // call c; Select c
    pq = {{{"c", TokenType::CALL}},
          {PqlResultType::Tuple, {{"c", AttributeRefType::NONE}}},
          {}};
    expected = {"7", "11", "25", "27", "28", "29", "30", "31", "32", "33"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectProcedureNoClause) {
    // procedure p; Select p
    ParsedQuery pq = {{{"p", TokenType::PROCEDURE}},
                      {PqlResultType::Tuple, {{"p", AttributeRefType::NONE}}},
                      {}};
    std::list<std::string> expected = {"main",     "extra",    "complicate",
                                       "aux",      "extratwo", "extrathree",
                                       "extrafour"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectVariableNoClause) {
    // variable v; Select v
    ParsedQuery pq = {{{"v", TokenType::VARIABLE}},
                      {PqlResultType::Tuple, {{"v", AttributeRefType::NONE}}},
                      {}};
    std::list<std::string> expected = {{"x", "y", "r", "m", "q", "t", "k"}};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectConstantNoClause) {
    // constant c; Select c
    ParsedQuery pq = {{{"c", TokenType::CONSTANT}},
                      {PqlResultType::Tuple, {{"c", AttributeRefType::NONE}}},
                      {}};
    std::list<std::string> expected = {
        "0", "1", "5", "11111111111111111111111111111111111111"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectAttributeNoClause) {
    // call c; Select c.procName
    ParsedQuery pq = {
        {{"c", TokenType::CALL}},
        {PqlResultType::Tuple, {{"c", AttributeRefType::PROCNAME}}},
        {}};
    std::list<std::string> expected = {"aux",  "complicate", "extra",
                                       "main", "extratwo",   "extrathree"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // print pn; read rd; Select <pn.varName, rd.varName>
    pq = {{{"pn", TokenType::PRINT}, {"rd", TokenType::READ}},
          {PqlResultType::Tuple,
           {{"pn", AttributeRefType::VARNAME},
            {"rd", AttributeRefType::VARNAME}}},
          {}};
    expected = {"x x", "t x", "q x", "k x", "x r", "t r", "q r",
                "k r", "x y", "t y", "q y", "k y", "x t", "t t",
                "q t", "k t", "x q", "t q", "q q", "k q"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectPseudoAttributeNoClause) {
    // procedure p; Select p.procName
    ParsedQuery pq = {
        {{"p", TokenType::PROCEDURE}},
        {PqlResultType::Tuple, {{"p", AttributeRefType::PROCNAME}}},
        {}};
    std::list<std::string> expected = {"aux",      "complicate", "extra",
                                       "main",     "extratwo",   "extrathree",
                                       "extrafour"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    // if i; constant c; Select <i.stmt#, c.value>
    pq = {{{"i", TokenType::IF}, {"c", TokenType::CONSTANT}},
          {PqlResultType::Tuple,
           {{"i", AttributeRefType::STATEMENT_NUM},
            {"c", AttributeRefType::VALUE}}},
          {}};
    expected = {
        "15 0", "15 1", "15 5", "15 11111111111111111111111111111111111111",
        "19 0", "19 1", "19 5", "19 11111111111111111111111111111111111111"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectTupleNoClause) {
    // constant c; procedure p; Select <p, c>
    ParsedQuery pq = {
        {{"c", TokenType::CONSTANT}, {"p", TokenType::PROCEDURE}},
        {PqlResultType::Tuple,
         {{"p", AttributeRefType::NONE}, {"c", AttributeRefType::NONE}}},
        {}};
    std::list<std::string> expected = {
        "main 0",       "main 1",
        "main 5",       "main 11111111111111111111111111111111111111",
        "extra 0",      "extra 1",
        "extra 5",      "extra 11111111111111111111111111111111111111",
        "complicate 0", "complicate 1",
        "complicate 5", "complicate 11111111111111111111111111111111111111",
        "aux 0",        "aux 1",
        "aux 5",        "aux 11111111111111111111111111111111111111",
        "extratwo 0",   "extratwo 1",
        "extratwo 5",   "extratwo 11111111111111111111111111111111111111",
        "extrathree 0", "extrathree 1",
        "extrathree 5", "extrathree 11111111111111111111111111111111111111",
        "extrafour 0",  "extrafour 1",
        "extrafour 5",  "extrafour 11111111111111111111111111111111111111",

    };
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectTuple) {
    /* All seen synonyms
       read r1; read r2; Select <r2, r1> such that Follows(r1, r2)
     */
    ParsedQuery pq = {
        {{"r1", TokenType::READ}, {"r2", TokenType::READ}},
        {PqlResultType::Tuple,
         {{"r2", AttributeRefType::NONE}, {"r1", AttributeRefType::NONE}}},
        {{TokenType::FOLLOWS,
          {TokenType::READ, "r1"},
          {TokenType::READ, "r2"}}}};
    std::list<std::string> expected = {"2 1", "9 8", "13 12"};
    std::list<std::string> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    /* All seen synonyms, one unseen synonym
       while w; if i; print p; Select <w, p, i> such that Follows(w, i)
     */
    pq = {
        {{"w", TokenType::WHILE},
         {"i", TokenType::IF},
         {"p", TokenType::PRINT}},
        {PqlResultType::Tuple,
         {{"w", AttributeRefType::NONE},
          {"p", AttributeRefType::NONE},
          {"i", AttributeRefType::NONE}}},
        {{TokenType::FOLLOWS, {TokenType::WHILE, "w"}, {TokenType::IF, "i"}}}};
    expected = {"17 6 19", "17 21 19", "17 22 19", "17 26 19"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    expected.sort();
    actual.sort();
    Assert::IsTrue(expected == actual);

    /* One seen, one unseen synonym
       assign a; read r; call c; Select <c, a> such that Follows(r, a)
     */
    pq = {{{"a", TokenType::ASSIGN},
           {"r", TokenType::READ},
           {"c", TokenType::CALL}},
          {PqlResultType::Tuple,
           {{"c", AttributeRefType::NONE}, {"a", AttributeRefType::NONE}}},
          {{TokenType::FOLLOWS,
            {TokenType::READ, "r"},
            {TokenType::ASSIGN, "a"}}}};
    expected = {"31 10", "7 10",  "27 10", "11 10", "25 10",
                "28 10", "29 10", "30 10", "32 10", "33 10"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    Assert::IsTrue(expected == actual);
  }

  TEST_METHOD(TestEvaluateParsedQuery_SelectBoolean) {
    /* Boolean true
       Select BOOLEAN such that Follows(1, 2)
     */
    ParsedQuery pq = {{},
                      {PqlResultType::Boolean},
                      {{TokenType::FOLLOWS,
                        {TokenType::NUMBER, "1"},
                        {TokenType::NUMBER, "2"}}}};
    std::list<VALUE> expected = {"TRUE"};
    std::list<VALUE> actual;
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    Assert::IsTrue(expected == actual);

    /* Boolean false
       Select BOOLEAN such that Follows(1, 3)
     */
    pq = {{},
          {PqlResultType::Boolean},
          {{TokenType::FOLLOWS,
            {TokenType::NUMBER, "1"},
            {TokenType::NUMBER, "3"}}}};
    expected = {"FALSE"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    Assert::IsTrue(expected == actual);

    /* No values (indirect false)
       read r; if i; Select BOOLEAN such that Parent*(i, r)
     */
    pq = {{{"r", TokenType::READ}, {"i", TokenType::IF}},
          {PqlResultType::Boolean},
          {{TokenType::PARENT, {TokenType::IF, "i"}, {TokenType::READ, "r"}}}};
    expected = {"FALSE"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    Assert::IsTrue(expected == actual);

    /* With values (indirect true)
       assign a; read r; call c; Select BOOLEAN such that Follows(r, a)
     */
    pq = {{{"a", TokenType::ASSIGN},
           {"r", TokenType::READ},
           {"c", TokenType::CALL}},
          {PqlResultType::Boolean},
          {{TokenType::FOLLOWS,
            {TokenType::READ, "r"},
            {TokenType::ASSIGN, "a"}}}};
    expected = {"TRUE"};
    actual.clear();
    Pql::evaluate(pq, pkb.getQueryInterface(), actual);
    Assert::IsTrue(expected == actual);
  }
};
} // namespace IntegrationTesting
