#include "CppUnitTest.h"
#include "stdafx.h"
#include <iostream>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPQL){public : TEST_METHOD(TestLex_Declarations){
    const auto actualTokens = lex("procedure a; assign b;");
const std::vector<PqlToken> expectedTokens = {
    {TokenType::PROCEDURE}, {TokenType::SYNONYM, "a"}, {TokenType::SEMICOLON},
    {TokenType::ASSIGN},    {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON}};
Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLex_Select) {
  const auto actualTokens = lex("Select w");
  const std::vector<PqlToken> expectedTokens = {{TokenType::SELECT},
                                                {TokenType::SYNONYM, "w"}};

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting

TEST_METHOD(TestLex_Pattern) {
  const auto actualTokens = lex("Select a pattern a (\"x\", _\"x + y\"_)");
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
  const auto actualTokens = lex("Select a pattern a (\"x\", \"x + y\")");
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
  const auto actualTokens = lex("Select a pattern a (_, _)");
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
  const auto actualTokens = lex("Select p such that Follows (p, q)");
  const std::vector<PqlToken> expectedTokens = {
      {TokenType::SELECT},       {TokenType::SYNONYM, "p"},
      {TokenType::SUCH},         {TokenType::THAT},
      {TokenType::FOLLOWS},      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"}, {TokenType::CLOSED_PARENTHESIS},
  };

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting

TEST_METHOD(TestParse_Declarations) {
  const std::vector<PqlToken> input = {
      {TokenType::PROCEDURE},    {TokenType::SYNONYM, "a"},
      {TokenType::SEMICOLON},    {TokenType::ASSIGN},
      {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON}};
  const auto actualResult = parse(input).declaration_clause;
  const std::unordered_map<std::string, TokenType> expectedDeclarations = {
      {"a", TokenType::PROCEDURE}, {"b", TokenType::ASSIGN}};
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

  const auto actualResult = parse(input).result_clause;
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
  const auto actualResult = parse(input).relationship_clauses;
  const std::vector<ParsedRelationship> expectedRelationship = {
      ParsedRelationship{TokenType::FOLLOWS,
                         {TokenType::SYNONYM, "p"},
                         {TokenType::SYNONYM, "q"}}};
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
  const auto actualResult = parse(input).relationship_clauses;
  const std::vector<ParsedRelationship> expectedDeclarations = {
      ParsedRelationship{TokenType::MODIFIES,
                         {TokenType::SYNONYM, "s"},
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
  const auto actualResult = parse(input).pattern_clauses;
  const std::vector<ParsedPattern> expectedDeclarations{
      ParsedPattern{{TokenType::STRING, "z"},
                    ExpressionSpec{ExpressionSpecType::CompleteMatch, "x"}}};
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
  const auto actualResult = parse(input).pattern_clauses;
  const std::vector<ParsedPattern> expectedDeclarations{
      ParsedPattern{{TokenType::UNDERSCORE},
                    ExpressionSpec{ExpressionSpecType::SubTreeMatch, "x"}}};
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
  const auto actualResult = parse(input).pattern_clauses;
  const std::vector<ParsedPattern> expectedDeclarations{ParsedPattern{
      {TokenType::UNDERSCORE}, ExpressionSpec{ExpressionSpecType::Any}}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting
}
;
}
; // namespace UnitTesting
