#include "CppUnitTest.h"
#include "stdafx.h"
#include <iostream>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPQL){public : TEST_METHOD(TestLexDeclarations){
    const auto actualTokens = lex("procedure a; assign b;");
const std::vector<PqlToken> expectedTokens = {
    {TokenType::PROCEDURE}, {TokenType::SYNONYM, "a"}, {TokenType::SEMICOLON},
    {TokenType::ASSIGN},    {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON}};
Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLexSelect) {
  const auto actualTokens = lex("Select w");
  const std::vector<PqlToken> expectedTokens = {{TokenType::SELECT},
                                                {TokenType::SYNONYM, "w"}};

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLexRelationship) {
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

TEST_METHOD(TestParseDeclarations) {
  const std::vector<PqlToken> input = {
      {TokenType::PROCEDURE},    {TokenType::SYNONYM, "a"},
      {TokenType::SEMICOLON},    {TokenType::ASSIGN},
      {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON}};
  const auto actualResult = parse(input).declaration_clause;
  const std::unordered_map<std::string, TokenType> expectedDeclarations = {
      {"a", TokenType::PROCEDURE}, {"b", TokenType::ASSIGN}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParseSelect) {
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

TEST_METHOD(TestParseFollowsRelationshipTwoSynonyms) {
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
  const auto actualResult = parse(input).relationship_clause;
  const std::vector<ParsedRelationship> expectedRelationship = {
      ParsedRelationship{TokenType::FOLLOWS,
                         {TokenType::SYNONYM, "p"},
                         {TokenType::SYNONYM, "q"}}};
  Assert::IsTrue(actualResult == expectedRelationship);
} // namespace UnitTesting
TEST_METHOD(TestParseModifiesRelationshipOneSynonymOneString) {
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
  const auto actualResult = parse(input).relationship_clause;
  const std::vector<ParsedRelationship> expectedDeclarations = {
      ParsedRelationship{TokenType::MODIFIES,
                         {TokenType::SYNONYM, "s"},
                         {TokenType::STRING, "x"}}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting
}
;
}
; // namespace UnitTesting
