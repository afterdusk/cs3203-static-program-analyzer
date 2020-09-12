#include "CppUnitTest.h"
#include "stdafx.h"
#include <iostream>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
/// LEXER TESTS
TEST_CLASS(TestPQL){public : TEST_METHOD(TestLex_Declarations){
    const auto actualTokens = PQL::lex("procedure a; assign b;");
const std::vector<PqlToken> expectedTokens = {
    {TokenType::PROCEDURE}, {TokenType::SYNONYM, "a"}, {TokenType::SEMICOLON},
    {TokenType::ASSIGN},    {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON}};
Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLex_Select) {
  const auto actualTokens = PQL::lex("Select w");
  const std::vector<PqlToken> expectedTokens = {{TokenType::SELECT},
                                                {TokenType::SYNONYM, "w"}};

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting

TEST_METHOD(TestLex_Pattern) {
  const auto actualTokens = PQL::lex("Select a pattern a (\"x\", _\"x + y\"_)");
  const std::vector<PqlToken> expectedTokens = {
      {TokenType::SELECT},
      {TokenType::SYNONYM, "a"},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::STRING, "x"},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::STRING, "x + y"},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
  };

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLex_PatternLHSExprRHSCompleteMatch) {
  const auto actualTokens = PQL::lex("Select a pattern a (\"x\", \"x + y\")");
  const std::vector<PqlToken> expectedTokens = {
      {TokenType::SELECT},
      {TokenType::SYNONYM, "a"},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::STRING, "x"},
      {TokenType::COMMA},
      {TokenType::STRING, "x + y"},
      {TokenType::CLOSED_PARENTHESIS},
  };

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting

TEST_METHOD(TestLex_PatternLHSAnyRHSAny) {
  const auto actualTokens = PQL::lex("Select a pattern a (_, _)");
  const std::vector<PqlToken> expectedTokens = {
      {TokenType::SELECT},
      {TokenType::SYNONYM, "a"},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::UNDERSCORE},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
  };

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLex_Relationship) {
  const auto actualTokens = PQL::lex("Select p such that Follows (p, q)");
  const std::vector<PqlToken> expectedTokens = {
      {TokenType::SELECT},       {TokenType::SYNONYM, "p"},
      {TokenType::SUCH},         {TokenType::THAT},
      {TokenType::FOLLOWS},      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"}, {TokenType::CLOSED_PARENTHESIS},
  };

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting

/// PARSER TESTS
TEST_METHOD(TestParse_Declarations) {
  const std::vector<PqlToken> input = {
      {TokenType::PROCEDURE}, {TokenType::SYNONYM, "a"}, {TokenType::SEMICOLON},
      {TokenType::ASSIGN},    {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},    {TokenType::SYNONYM, "a"},
  };
  const auto actualResult = PQL::parse(input).declaration_clause;
  const std::unordered_map<std::string, TokenType> expectedDeclarations = {
      {"a", TokenType::PROCEDURE}, {"b", TokenType::ASSIGN}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting
TEST_METHOD(TestParse_DeclarationsMultipleSynonyms) {
  const std::vector<PqlToken> input = {
      {TokenType::PROCEDURE},    {TokenType::SYNONYM, "a"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "c"}, {TokenType::SEMICOLON},    {TokenType::ASSIGN},
      {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON},    {TokenType::SELECT},
      {TokenType::SYNONYM, "a"},
  };
  const auto actualResult = PQL::parse(input).declaration_clause;
  const std::unordered_map<std::string, TokenType> expectedDeclarations = {
      {"a", TokenType::PROCEDURE},
      {"b", TokenType::ASSIGN},
      {"c", TokenType::PROCEDURE}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_Select) {
  const std::vector<PqlToken> input = {
      {TokenType::STMT},         {TokenType::SYNONYM, "p"},
      {TokenType::SEMICOLON},    {TokenType::STMT},
      {TokenType::SYNONYM, "q"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "p"},
      {TokenType::SUCH},         {TokenType::THAT},
      {TokenType::FOLLOWS},      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"}, {TokenType::CLOSED_PARENTHESIS},
  };

  const auto actualResult = PQL::parse(input).result_clause;
  const std::vector<std::string> expectedDeclarations = {"p"};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_FollowsRelationshipTwoSynonyms) {
  const std::vector<PqlToken> input = {
      {TokenType::STMT},         {TokenType::SYNONYM, "p"},
      {TokenType::SEMICOLON},    {TokenType::STMT},
      {TokenType::SYNONYM, "q"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "p"},
      {TokenType::SUCH},         {TokenType::THAT},
      {TokenType::FOLLOWS},      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"}, {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = PQL::parse(input).relationship_clauses;
  const std::vector<ParsedRelationship> expectedRelationship = {
      ParsedRelationship{
          TokenType::FOLLOWS, {TokenType::STMT, "p"}, {TokenType::STMT, "q"}}};
  Assert::IsTrue(actualResult == expectedRelationship);
} // namespace UnitTesting

TEST_METHOD(TestParse_ModifiesRelationshipOneSynonymOneString) {
  const std::vector<PqlToken> input = {
      {TokenType::STMT},
      {TokenType::SYNONYM, "s"},
      {TokenType::SEMICOLON},
      {TokenType::SELECT},
      {TokenType::SYNONYM, "s"},
      {TokenType::SUCH},
      {TokenType::THAT},
      {TokenType::MODIFIES},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "s"},
      {TokenType::COMMA},
      {TokenType::STRING, "x"},
      {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = PQL::parse(input).relationship_clauses;
  const std::vector<ParsedRelationship> expectedDeclarations = {
      ParsedRelationship{TokenType::MODIFIES,
                         {TokenType::STMT, "s"},
                         {TokenType::STRING, "x"}}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_PatternLHSExprRHSCompleteMatch) {
  const std::vector<PqlToken> input = {
      {TokenType::ASSIGN},       {TokenType::SYNONYM, "a"},
      {TokenType::SEMICOLON},    {TokenType::SELECT},
      {TokenType::SYNONYM, "a"}, {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"}, {TokenType::OPEN_PARENTHESIS},
      {TokenType::STRING, "z"},  {TokenType::COMMA},
      {TokenType::STRING, "x"},  {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = PQL::parse(input).pattern_clauses;
  const std::vector<ParsedPattern> expectedDeclarations{
      ParsedPattern{{TokenType::ASSIGN, "a"},
                    {TokenType::STRING, "z"},
                    PatternSpec{PatternMatchType::CompleteMatch, "x"}}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_PatternLHSUnderscoreRHSSubtreeMatch) {
  const std::vector<PqlToken> input = {
      {TokenType::ASSIGN},       {TokenType::SYNONYM, "a"},
      {TokenType::SEMICOLON},    {TokenType::SELECT},
      {TokenType::SYNONYM, "a"}, {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"}, {TokenType::OPEN_PARENTHESIS},
      {TokenType::UNDERSCORE},   {TokenType::COMMA},
      {TokenType::UNDERSCORE},   {TokenType::STRING, "x"},
      {TokenType::UNDERSCORE},   {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = PQL::parse(input).pattern_clauses;
  const std::vector<ParsedPattern> expectedDeclarations{
      ParsedPattern{{TokenType::ASSIGN, "a"},
                    {TokenType::UNDERSCORE},
                    PatternSpec{PatternMatchType::SubTreeMatch, "x"}}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_PatternLHSUnderscoreRHSAny) {
  const std::vector<PqlToken> input = {
      {TokenType::ASSIGN},       {TokenType::SYNONYM, "a"},
      {TokenType::SEMICOLON},    {TokenType::SELECT},
      {TokenType::SYNONYM, "a"}, {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"}, {TokenType::OPEN_PARENTHESIS},
      {TokenType::UNDERSCORE},   {TokenType::COMMA},
      {TokenType::UNDERSCORE},   {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = PQL::parse(input).pattern_clauses;
  const std::vector<ParsedPattern> expectedDeclarations{
      ParsedPattern{{TokenType::ASSIGN, "a"},
                    {TokenType::UNDERSCORE},
                    PatternSpec{PatternMatchType::Any}}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

// Lexer and parser
TEST_METHOD(TestLexAndParse_NoSuchThatNoPattern) {
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
} // namespace UnitTesting

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
}
; // namespace UnitTesting
