#include "CppUnitTest.h"
#include "stdafx.h"
#include <iostream>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPqlLexer){public : TEST_METHOD(TestLex_Declarations){
    const auto actualTokens = Pql::lex("procedure a; assign b;");
const std::vector<PqlToken> expectedTokens = {
    {TokenType::PROCEDURE}, {TokenType::SYNONYM, "a"}, {TokenType::SEMICOLON},
    {TokenType::ASSIGN},    {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON}};
Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLex_Select) {
  const auto actualTokens = Pql::lex("Select w");
  const std::vector<PqlToken> expectedTokens = {{TokenType::SELECT},
                                                {TokenType::SYNONYM, "w"}};

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting

TEST_METHOD(TestLex_Pattern) {
  const auto actualTokens = Pql::lex("Select a pattern a (\"x\", _\"x + y\"_)");
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
  const auto actualTokens = Pql::lex("Select a pattern a (\"x\", \"x + y\")");
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
  const auto actualTokens = Pql::lex("Select a pattern a (_, _)");
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
  const auto actualTokens = Pql::lex("Select p such that Follows (p, q)");
  const std::vector<PqlToken> expectedTokens = {
      {TokenType::SELECT},       {TokenType::SYNONYM, "p"},
      {TokenType::SUCH},         {TokenType::THAT},
      {TokenType::FOLLOWS},      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"}, {TokenType::CLOSED_PARENTHESIS},
  };

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLex_BracketWithinString) {
  const auto actualTokens = Pql::lex("Select a pattern a(_, \"(x+y)\")");
  const std::vector<PqlToken> expectedTokens = {
      {TokenType::SELECT},
      {TokenType::SYNONYM, "a"},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::UNDERSCORE},
      {TokenType::COMMA},
      {TokenType::STRING, "(x+y)"},
      {TokenType::CLOSED_PARENTHESIS},
  };

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
TEST_METHOD(TestLex_SanitizeProgLine) {
  const auto actualTokens = Pql::lex("prog_line p,q;");
  const std::vector<PqlToken> expectedTokens = {{TokenType::PROG_LINE},
                                                {TokenType::SYNONYM, "p"},
                                                {TokenType::COMMA},
                                                {TokenType::SYNONYM, "q"},
                                                {TokenType::SEMICOLON}};

  Assert::IsTrue(expectedTokens == actualTokens);
} // namespace UnitTesting
}
;
TEST_CLASS(TestPqlParser){
  public :
      /// PARSER TESTS
      TEST_METHOD(TestParse_Declarations){const std::vector<PqlToken> input = {
                                              {TokenType::PROCEDURE},
                                              {TokenType::SYNONYM, "a"},
                                              {TokenType::SEMICOLON},
                                              {TokenType::ASSIGN},
                                              {TokenType::SYNONYM, "b"},
                                              {TokenType::SEMICOLON},
                                              {TokenType::SELECT},
                                              {TokenType::SYNONYM, "a"},
                                          };
const auto actualResult = Pql::parse(input).declarations;
const std::unordered_map<std::string, TokenType> expectedDeclarations = {
    {"a", TokenType::PROCEDURE},
    {"b", TokenType::ASSIGN},
};
Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting
TEST_METHOD(TestParse_DeclarationsMultipleSynonyms) {
  const std::vector<PqlToken> input = {
      {TokenType::PROCEDURE},    {TokenType::SYNONYM, "a"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "c"}, {TokenType::SEMICOLON},    {TokenType::ASSIGN},
      {TokenType::SYNONYM, "b"}, {TokenType::SEMICOLON},    {TokenType::SELECT},
      {TokenType::SYNONYM, "a"},
  };
  const auto actualResult = Pql::parse(input).declarations;
  const std::unordered_map<std::string, TokenType> expectedDeclarations = {
      {"a", TokenType::PROCEDURE},
      {"b", TokenType::ASSIGN},
      {"c", TokenType::PROCEDURE},
  };
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_SimpleSelect) {
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

  const auto actualResult = Pql::parse(input).results;
  const PqlResult expectedResults = {
      PqlResultType::Tuple,
      {{"p", AttributeRefType::NONE}},
  };
  Assert::IsTrue(actualResult == expectedResults);
} // namespace UnitTesting

TEST_METHOD(TestParse_SimpleSelectAttrRef) {
  const std::vector<PqlToken> input = {
      {TokenType::STMT},         {TokenType::SYNONYM, "p"},
      {TokenType::SEMICOLON},    {TokenType::STMT},
      {TokenType::SYNONYM, "q"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "p"},
      {TokenType::DOT},          {TokenType::STATEMENT_NUM},
      {TokenType::SUCH},         {TokenType::THAT},
      {TokenType::FOLLOWS},      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"}, {TokenType::CLOSED_PARENTHESIS},
  };

  const auto actualResult = Pql::parse(input).results;
  const PqlResult expectedResults = {
      PqlResultType::Tuple,
      {{"p", AttributeRefType::STATEMENT_NUM}},
  };
  Assert::IsTrue(actualResult == expectedResults);
} // namespace UnitTesting

TEST_METHOD(TestParse_MultipleSelect) {
  const std::vector<PqlToken> input = {
      {TokenType::STMT},          {TokenType::SYNONYM, "p"},
      {TokenType::SEMICOLON},     {TokenType::STMT},
      {TokenType::SYNONYM, "q"},  {TokenType::SEMICOLON},
      {TokenType::SELECT},        {TokenType::OPEN_ANGLED_BRACKET},
      {TokenType::SYNONYM, "p"},  {TokenType::DOT},
      {TokenType::STATEMENT_NUM}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"},  {TokenType::CLOSED_ANGLED_BRACKET},
      {TokenType::SUCH},          {TokenType::THAT},
      {TokenType::FOLLOWS},       {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"},  {TokenType::COMMA},
      {TokenType::SYNONYM, "q"},  {TokenType::CLOSED_PARENTHESIS},
  };

  const auto actualResult = Pql::parse(input).results;
  const PqlResult expectedResults = PqlResult{
      PqlResultType::Tuple,
      {
          Element{"p", AttributeRefType::STATEMENT_NUM},
          Element{"q", AttributeRefType::NONE},
      },
  };
  Assert::IsTrue(actualResult == expectedResults);
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
  const auto actualResult = Pql::parse(input).relationships;
  const std::vector<ParsedRelationship> expectedRelationship = {
      ParsedRelationship{
          TokenType::FOLLOWS,
          PqlToken{TokenType::STMT, "p"},
          PqlToken{TokenType::STMT, "q"},
      },
  };
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
  const auto actualRelationships = Pql::parse(input).relationships;
  const std::vector<ParsedRelationship> expectedRelationships = {
      ParsedRelationship{
          TokenType::MODIFIES,
          PqlToken{TokenType::STMT, "s"},
          PqlToken{TokenType::STRING, "x"},
      },
  };
  Assert::IsTrue(actualRelationships == expectedRelationships);
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
  const auto actualPatterns = Pql::parse(input).patterns;
  const std::vector<ParsedPattern> expectedPatterns{
      ParsedPattern{{TokenType::ASSIGN, "a"},
                    {TokenType::STRING, "z"},
                    PatternSpec{PatternMatchType::CompleteMatch, "x"}}};
  Assert::IsTrue(actualPatterns == expectedPatterns);
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
  const auto actualPatterns = Pql::parse(input).patterns;
  const std::vector<ParsedPattern> expectedPatterns{
      ParsedPattern{
          PqlToken{TokenType::ASSIGN, "a"},
          PqlToken{TokenType::UNDERSCORE},
          PatternSpec{PatternMatchType::SubTreeMatch, "x"},
      },
  };
  Assert::IsTrue(actualPatterns == expectedPatterns);
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
  const auto actualResult = Pql::parse(input).patterns;
  const std::vector<ParsedPattern> expectedDeclarations{
      ParsedPattern{
          {TokenType::ASSIGN, "a"},
          {TokenType::UNDERSCORE},
          PatternSpec{PatternMatchType::Any},
      },
  };
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_LHSPatternDeclarationUsingReservedEntityName) {
  const std::vector<PqlToken> input = {
      {TokenType::ASSIGN},
      {TokenType::PROCEDURE},
      {TokenType::SEMICOLON},
      {TokenType::VARIABLE},
      {TokenType::VARIABLE},
      {TokenType::SEMICOLON},
      {TokenType::SELECT},
      {TokenType::PROCEDURE},
      {TokenType::PATTERN},
      {TokenType::PROCEDURE},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::VARIABLE},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = Pql::parse(input).patterns;
  const std::vector<ParsedPattern> expectedDeclarations{
      ParsedPattern{{TokenType::ASSIGN, "procedure"},
                    {TokenType::VARIABLE, "variable"},
                    PatternSpec{PatternMatchType::Any}}};
  Assert::IsTrue(actualResult == expectedDeclarations);
} // namespace UnitTesting

TEST_METHOD(TestParse_SelectedResultNotInDeclaration_ThrowsException) {
  const std::vector<PqlToken> input = {
      {TokenType::ASSIGN}, {TokenType::SYNONYM, "a"}, {TokenType::SEMICOLON},
      {TokenType::SELECT}, {TokenType::SYNONYM, "s"},
  };
  Assert::ExpectException<const char *>([input] { Pql::parse(input); });
} // namespace UnitTesting

TEST_METHOD(TestParse_LHSPatternSynonymButNotVariable_ThrowsException) {
  const std::vector<PqlToken> input = {
      {TokenType::ASSIGN},       {TokenType::SYNONYM, "a"},
      {TokenType::SEMICOLON},    {TokenType::SELECT},
      {TokenType::SYNONYM, "a"}, {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"}, {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "a"}, {TokenType::COMMA},
      {TokenType::UNDERSCORE},   {TokenType::CLOSED_PARENTHESIS},
  };
  Assert::ExpectException<const char *>([input] { Pql::parse(input); });
} // namespace UnitTesting

TEST_METHOD(TestParse_MultiplePatternsWithAnd) {
  const std::vector<PqlToken> input = {
      {TokenType::ASSIGN},
      {TokenType::SYNONYM, "a"},
      {TokenType::SEMICOLON},
      {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"},
      {TokenType::SEMICOLON},
      {TokenType::SELECT},
      {TokenType::SYNONYM, "a"},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "a"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "v"},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
      {TokenType::AND},
      {TokenType::SYNONYM, "a"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "v"},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
      {TokenType::AND},
      {TokenType::SYNONYM, "a"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "v"},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = Pql::parse(input).patterns;
  const std::vector<ParsedPattern> expectedPatterns{
      ParsedPattern{
          PqlToken{TokenType::ASSIGN, "a"},
          PqlToken{TokenType::VARIABLE, "v"},
          PatternSpec{PatternMatchType::Any},
      },
      ParsedPattern{
          PqlToken{TokenType::ASSIGN, "a"},
          PqlToken{TokenType::VARIABLE, "v"},
          PatternSpec{PatternMatchType::Any},
      },
      ParsedPattern{
          {TokenType::ASSIGN, "a"},
          PqlToken{TokenType::VARIABLE, "v"},
          PatternSpec{PatternMatchType::Any},
      },
  };
  Assert::IsTrue(actualResult == expectedPatterns);
}
TEST_METHOD(TestParse_MultipleRelationshipsWithAnd) {
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
      {TokenType::AND},
      {TokenType::USES},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "s"},
      {TokenType::COMMA},
      {TokenType::STRING, "x"},
      {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = Pql::parse(input).relationships;
  const std::vector<ParsedRelationship> expectedRelationships = {
      ParsedRelationship{
          TokenType::MODIFIES,
          {TokenType::STMT, "s"},
          {TokenType::STRING, "x"},
      },
      ParsedRelationship{
          TokenType::USES,
          {TokenType::STMT, "s"},
          {TokenType::STRING, "x"},
      },
  };
  Assert::IsTrue(actualResult == expectedRelationships);
}
TEST_METHOD(TestParse_WhilePatternStatement) {
  const std::vector<PqlToken> input = {
      {TokenType::WHILE},
      {TokenType::SYNONYM, "w"},
      {TokenType::SEMICOLON},
      {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"},
      {TokenType::SEMICOLON},
      {TokenType::SELECT},
      {TokenType::SYNONYM, "w"},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "w"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "v"},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = Pql::parse(input).patterns;
  const std::vector<ParsedPattern> expectedPatterns{
      ParsedPattern{
          {TokenType::WHILE, "w"},
          {TokenType::VARIABLE, "v"},
          PatternSpec{PatternMatchType::Any},
      },
  };
  Assert::IsTrue(actualResult == expectedPatterns);
}
TEST_METHOD(TestParse_IfPatternStatement) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},
      {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},
      {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"},
      {TokenType::SEMICOLON},
      {TokenType::SELECT},
      {TokenType::SYNONYM, "v"},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "i"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "v"},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
  };
  const auto actualResult = Pql::parse(input).patterns;
  const std::vector<ParsedPattern> expectedPatterns{
      ParsedPattern{
          {TokenType::IF, "i"},
          {TokenType::VARIABLE, "v"},
          PatternSpec{PatternMatchType::Any},
      },
  };
  Assert::IsTrue(actualResult == expectedPatterns);
}
TEST_METHOD(TestParse_WithClause) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},           {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},    {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "v"},
      {TokenType::WITH},         {TokenType::SYNONYM, "i"},
      {TokenType::DOT},          {TokenType::STATEMENT_NUM},
      {TokenType::EQUALS},       {TokenType::NUMBER, "34"},
  };
  const auto actual = Pql::parse(input).withs;
  const WITHS expected = {
      {
          Reference{Element{"i", AttributeRefType::STATEMENT_NUM}},
          Reference{PqlToken{TokenType::NUMBER, "34"}},
      },
  };
  Assert::IsTrue(actual == expected);
}

TEST_METHOD(TestParse_WithClauseRawRefsNotMatching_ThrowsException) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},           {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},    {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "v"},
      {TokenType::WITH},         {TokenType::STRING, "i"},
      {TokenType::EQUALS},       {TokenType::NUMBER, "34"},
  };

  Assert::ExpectException<const char *>([input] { Pql::parse(input); });
} // namespace UnitTesting
TEST_METHOD(TestParse_ReorderWithClauseIfRawBeforeElem) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},           {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},    {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "v"},
      {TokenType::WITH},         {TokenType::NUMBER, "34"},
      {TokenType::EQUALS},       {TokenType::SYNONYM, "i"},
      {TokenType::DOT},          {TokenType::STATEMENT_NUM},
  };
  const auto actual = Pql::parse(input).withs;
  const WITHS expected = {
      {Reference{Element{"i", AttributeRefType::STATEMENT_NUM}},
       Reference{PqlToken{TokenType::NUMBER, "34"}}}};
  Assert::IsTrue(actual == expected);
}

TEST_METHOD(
    TestParse_SemanticIncorrectWithStatementNumberToString_ThrowsException) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},           {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},    {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "v"},
      {TokenType::WITH},         {TokenType::STRING, "34"},
      {TokenType::EQUALS},       {TokenType::SYNONYM, "i"},
      {TokenType::DOT},          {TokenType::STATEMENT_NUM},
  };
  Assert::ExpectException<const char *>([input] { Pql::parse(input); });
}

TEST_METHOD(TestParse_SemanticIncorrectWithVarNameToNumber_ThrowsException) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},           {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},    {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "v"},
      {TokenType::WITH},         {TokenType::NUMBER, "34"},
      {TokenType::EQUALS},       {TokenType::SYNONYM, "v"},
      {TokenType::DOT},          {TokenType::VARNAME},
  };
  Assert::ExpectException<const char *>([input] { Pql::parse(input); });
}

TEST_METHOD(TestParse_ExpectReferencesToMatch) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},           {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},    {TokenType::VARIABLE},
      {TokenType::SYNONYM, "v"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::SYNONYM, "v"},
      {TokenType::WITH},         {TokenType::SYNONYM, "i"},
      {TokenType::DOT},          {TokenType::STATEMENT_NUM},
      {TokenType::EQUALS},       {TokenType::SYNONYM, "v"},
      {TokenType::DOT},          {TokenType::VARNAME},
  };
  Assert::ExpectException<const char *>([input] { Pql::parse(input); });
}
TEST_METHOD(
    TestParse_SemanticErrorWhenParsingDeclaration_ThrowsSemanticException) {
  const std::vector<PqlToken> input = {
      {TokenType::IF},       {TokenType::SYNONYM, "i"}, {TokenType::SEMICOLON},
      {TokenType::VARIABLE}, {TokenType::SYNONYM, "i"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},   {TokenType::BOOLEAN}};
  Assert::ExpectException<const PqlSemanticErrorWithBooleanResultException>(
      [input] { Pql::parse(input); });
}
TEST_METHOD(TestParse_SemanticErrorWhenParsingPattern_ThrowsSemanticException) {
  // First argument is a synonym that is NOT declared a variable
  const std::vector<PqlToken> input = {
      {TokenType::IF},
      {TokenType::SYNONYM, "i"},
      {TokenType::SEMICOLON},
      {TokenType::WHILE},
      {TokenType::SYNONYM, "w"},
      {TokenType::SEMICOLON},
      {TokenType::SELECT},
      {TokenType::BOOLEAN},
      {TokenType::PATTERN},
      {TokenType::SYNONYM, "i"},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "w"},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::COMMA},
      {TokenType::UNDERSCORE},
      {TokenType::CLOSED_PARENTHESIS},
  };
  Assert::ExpectException<const PqlSemanticErrorWithBooleanResultException>(
      [input] { Pql::parse(input); });
}

TEST_METHOD(
    TestParse_SemanticErrorWhenParsingRelationship_ThrowsSemanticException) {
  // First argument is not from list of valid tokens
  const std::vector<PqlToken> input = {
      {TokenType::PROCEDURE},    {TokenType::SYNONYM, "p"},
      {TokenType::SEMICOLON},    {TokenType::STMT},
      {TokenType::SYNONYM, "q"}, {TokenType::SEMICOLON},
      {TokenType::SELECT},       {TokenType::BOOLEAN},
      {TokenType::SUCH},         {TokenType::THAT},
      {TokenType::FOLLOWS},      {TokenType::OPEN_PARENTHESIS},
      {TokenType::SYNONYM, "p"}, {TokenType::COMMA},
      {TokenType::SYNONYM, "q"}, {TokenType::CLOSED_PARENTHESIS},
  };
  Assert::ExpectException<const PqlSemanticErrorWithBooleanResultException>(
      [input] { Pql::parse(input); });
}

TEST_METHOD(TestParse_NonIdentInStringInRelationship_ThrowsException) {
  // First argument is not from list of valid tokens
  const std::vector<PqlToken> input = {
      {TokenType::PROCEDURE},
      {TokenType::SYNONYM, "p"},
      {TokenType::SEMICOLON},
      {TokenType::STMT},
      {TokenType::SYNONYM, "q"},
      {TokenType::SEMICOLON},
      {TokenType::SELECT},
      {TokenType::BOOLEAN},
      {TokenType::SUCH},
      {TokenType::THAT},
      {TokenType::FOLLOWS},
      {TokenType::OPEN_PARENTHESIS},
      {TokenType::STRING, "helo@"},
      {TokenType::COMMA},
      {TokenType::SYNONYM, "q"},
      {TokenType::CLOSED_PARENTHESIS},
  };
  Assert::ExpectException<const char *>([input] { Pql::parse(input); });
}
}
;
}
