#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPQLParserLexerIntegration){
  public :
      // Lexer and parser
      TEST_METHOD(TestLexAndParse_NoSuchThatNoPattern){
          const std::string input = "procedure p;\nSelect p";
const auto actualResult = PQL::parse(PQL::lex(input));
const DECLARATIONS expectedDeclarations{{"p", TokenType::PROCEDURE}};
const RESULTS expectedResults{{"p"}};
const RELATIONSHIPS expectedRelationships{};
const PATTERNS expectedPatterns{};
Assert::IsTrue(actualResult.declaration_clause == expectedDeclarations);
Assert::IsTrue(actualResult.result_clause == expectedResults);
Assert::IsTrue(actualResult.relationship_clauses == expectedRelationships);
Assert::IsTrue(actualResult.pattern_clauses == expectedPatterns);
} // namespace IntegrationTesting

TEST_METHOD(TestLexAndParse_SuchThatFollowsStarNoPattern) {
  const std::string input = "stmt s;\n\nSelect s such that Follows* (6, s)";
  const auto actualResult = PQL::parse(PQL::lex(input));
  const DECLARATIONS expectedDeclarations{{"s", TokenType::STMT}};
  const RESULTS expectedResults{{"s"}};
  const RELATIONSHIPS expectedRelationships{
      {TokenType::FOLLOWS_T, {TokenType::NUMBER, "6"}, {TokenType::STMT, "s"}}};
  const PATTERNS expectedPatterns{};
  Assert::IsTrue(actualResult.declaration_clause == expectedDeclarations);
  Assert::IsTrue(actualResult.result_clause == expectedResults);
  Assert::IsTrue(actualResult.relationship_clauses == expectedRelationships);
  Assert::IsTrue(actualResult.pattern_clauses == expectedPatterns);
} // namespace UnitTesting
TEST_METHOD(TestLexAndParse_SuchThatUsesPattern) {
  const std::string input = "assign a; variable v;\n\nSelect a such that Uses "
                            "(a, v) pattern a (v, _)";
  const auto actualResult = PQL::parse(PQL::lex(input));
  const DECLARATIONS expectedDeclarations{{"a", TokenType::ASSIGN},
                                          {"v", TokenType::VARIABLE}};
  const RESULTS expectedResults{{"a"}};
  const RELATIONSHIPS expectedRelationships{
      {TokenType::USES, {TokenType::ASSIGN, "a"}, {TokenType::VARIABLE, "v"}}};
  const PATTERNS expectedPatterns{{{TokenType::ASSIGN, "a"},
                                   {TokenType::SYNONYM, "v"},
                                   {PatternMatchType::Any}}};
  Assert::IsTrue(actualResult.declaration_clause == expectedDeclarations);
  Assert::IsTrue(actualResult.result_clause == expectedResults);
  Assert::IsTrue(actualResult.relationship_clauses == expectedRelationships);
  Assert::IsTrue(actualResult.pattern_clauses == expectedPatterns);
} // namespace UnitTesting
}
;
} // namespace IntegrationTesting
