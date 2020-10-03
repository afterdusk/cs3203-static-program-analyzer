#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPqlParserLexerIntegration){
  public :
      // Lexer and parser
      TEST_METHOD(TestLexAndParse_NoSuchThatNoPattern){
          const std::string input = "procedure p;\nSelect p";
const auto actualResult = Pql::parse(Pql::lex(input));
const DECLARATIONS expectedDeclarations{{"p", TokenType::PROCEDURE}};
const RESULTS expectedResults{PqlResultType::Tuple,
                              {{"p", AttributeRefType::NONE}}};
const RELATIONSHIPS expectedRelationships{};
const PATTERNS expectedPatterns{};
const WITHS expectedWiths{};
Assert::IsTrue(actualResult.declarations == expectedDeclarations);
Assert::IsTrue(actualResult.results == expectedResults);
Assert::IsTrue(actualResult.relationships == expectedRelationships);
Assert::IsTrue(actualResult.patterns == expectedPatterns);
Assert::IsTrue(actualResult.withs == expectedWiths);
} // namespace IntegrationTesting

TEST_METHOD(TestLexAndParse_SuchThatFollowsStarNoPattern) {
  const std::string input = "stmt s;\n\nSelect s such that Follows* (6, s)";
  const auto actualResult = Pql::parse(Pql::lex(input));
  const DECLARATIONS expectedDeclarations{{"s", TokenType::STMT}};
  const RESULTS expectedResults{PqlResultType::Tuple,
                                {{"s", AttributeRefType::NONE}}};
  const RELATIONSHIPS expectedRelationships{
      {TokenType::FOLLOWS_T, {TokenType::NUMBER, "6"}, {TokenType::STMT, "s"}}};
  const PATTERNS expectedPatterns{};
  const WITHS expectedWiths{};
  Assert::IsTrue(actualResult.declarations == expectedDeclarations);
  Assert::IsTrue(actualResult.results == expectedResults);
  Assert::IsTrue(actualResult.relationships == expectedRelationships);
  Assert::IsTrue(actualResult.patterns == expectedPatterns);
  Assert::IsTrue(actualResult.withs == expectedWiths);
} // namespace UnitTesting
TEST_METHOD(TestLexAndParse_SuchThatUsesPattern) {
  const std::string input = "assign a; variable v;\n\nSelect a such that Uses "
                            "(a, v) pattern a (v, _)";
  const auto actualResult = Pql::parse(Pql::lex(input));
  const DECLARATIONS expectedDeclarations{{"a", TokenType::ASSIGN},
                                          {"v", TokenType::VARIABLE}};
  const RESULTS expectedResults{PqlResultType::Tuple,
                                {{"a", AttributeRefType::NONE}}};
  const RELATIONSHIPS expectedRelationships{
      {TokenType::USES, {TokenType::ASSIGN, "a"}, {TokenType::VARIABLE, "v"}}};
  const PATTERNS expectedPatterns{{{TokenType::ASSIGN, "a"},
                                   {TokenType::VARIABLE, "v"},
                                   {PatternMatchType::Any}}};
  const WITHS expectedWiths{};
  Assert::IsTrue(actualResult.declarations == expectedDeclarations);
  Assert::IsTrue(actualResult.results == expectedResults);
  Assert::IsTrue(actualResult.relationships == expectedRelationships);
  Assert::IsTrue(actualResult.patterns == expectedPatterns);
  Assert::IsTrue(actualResult.withs == expectedWiths);
} // namespace UnitTesting
TEST_METHOD(TestLexAndParse_SynonymDeclaredWithKeyword) {
  const std::string input = "if if; Select if such that Follows* (if, 16)";
  const auto actualResult = Pql::parse(Pql::lex(input));
  const DECLARATIONS expectedDeclarations{{"if", TokenType::IF}};
  const RESULTS expectedResults{PqlResultType::Tuple,
                                {{"if", AttributeRefType::NONE}}};
  const RELATIONSHIPS expectedRelationships{
      {TokenType::FOLLOWS_T, {TokenType::IF, "if"}, {TokenType::NUMBER, "16"}}};
  const PATTERNS expectedPatterns{};
  const WITHS expectedWiths{};
  Assert::IsTrue(actualResult.declarations == expectedDeclarations);
  Assert::IsTrue(actualResult.results == expectedResults);
  Assert::IsTrue(actualResult.relationships == expectedRelationships);
  Assert::IsTrue(actualResult.patterns == expectedPatterns);
  Assert::IsTrue(actualResult.withs == expectedWiths);
} // namespace IntegrationTesting

TEST_METHOD(TestLexAndParse_TupleResultWithWithClause) {
  const std::string input =
      "print p; variable v;\n\nSelect <p.stmt#, v> such that Uses "
      "(p, v) with p.varName = v.varName";
  const auto actualResult = Pql::parse(Pql::lex(input));
  const DECLARATIONS expectedDeclarations{{"p", TokenType::PRINT},
                                          {"v", TokenType::VARIABLE}};
  const RESULTS expectedResults{PqlResultType::Tuple,
                                {
                                    {"p", AttributeRefType::STATEMENT_NUM},
                                    {"v", AttributeRefType::NONE},
                                }};
  const RELATIONSHIPS expectedRelationships{
      {TokenType::USES, {TokenType::PRINT, "p"}, {TokenType::VARIABLE, "v"}}};
  const PATTERNS expectedPatterns{};
  const WITHS expectedWiths = {
      {Reference{Element{"p", AttributeRefType::VARNAME}},
       Reference{Element{"v", AttributeRefType::VARNAME}}}};
  Assert::IsTrue(actualResult.declarations == expectedDeclarations);
  Assert::IsTrue(actualResult.results == expectedResults);
  Assert::IsTrue(actualResult.relationships == expectedRelationships);
  Assert::IsTrue(actualResult.patterns == expectedPatterns);
  Assert::IsTrue(actualResult.withs == expectedWiths);
} // namespace UnitTesting
TEST_METHOD(TestLexAndParse_BooleanWithProgline) {
  const std::string input =
      "prog_line pl;assign a; variable v; \n\nSelect BOOLEAN such that Uses "
      "(pl, v) pattern a (v, _)";
  const auto actualResult = Pql::parse(Pql::lex(input));
  const DECLARATIONS expectedDeclarations{{"a", TokenType::ASSIGN},
                                          {"v", TokenType::VARIABLE},
                                          {"pl", TokenType::PROG_LINE}};
  const RESULTS expectedResults{PqlResultType::Boolean};
  const RELATIONSHIPS expectedRelationships{{TokenType::USES,
                                             {TokenType::PROG_LINE, "pl"},
                                             {TokenType::VARIABLE, "v"}}};
  const PATTERNS expectedPatterns{{{TokenType::ASSIGN, "a"},
                                   {TokenType::VARIABLE, "v"},
                                   {PatternMatchType::Any}}};
  const WITHS expectedWiths = {};
  Assert::IsTrue(actualResult.declarations == expectedDeclarations);
  Assert::IsTrue(actualResult.results == expectedResults);
  Assert::IsTrue(actualResult.relationships == expectedRelationships);
  Assert::IsTrue(actualResult.patterns == expectedPatterns);
  Assert::IsTrue(actualResult.withs == expectedWiths);
} // namespace UnitTesting
}
;
} // namespace IntegrationTesting
