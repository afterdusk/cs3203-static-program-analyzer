#include "CppUnitTest.h"
#include "Token.h"
#include "stdafx.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestParseExceptions){public : TEST_METHOD(TestNoProcedureException){
    Token tokenProcedure = Token("procedure1");
NoProcedureException exception = NoProcedureException(10, tokenProcedure);
Assert::IsTrue(strcmp(exception.what(),
                      "Error: The procedure \"procedure1\" does not exist.") ==
               0);
} // namespace UnitTesting

TEST_METHOD(TestNoParenthesisException) {
  Token token1("if");
  Token token2("x");
  Token token3("==");
  Token token4("0");
  Token token5("{");
  std::vector<Token> tokens;
  tokens.push_back(token1);
  tokens.push_back(token2);
  tokens.push_back(token3);
  tokens.push_back(token4);
  tokens.push_back(token5);

  NoParenthesisException exception = NoParenthesisException(2, tokens, '(');
  Assert::IsTrue(strcmp(exception.what(),
                        "Error: Line 2: There is a parenthesis '(' "
                        "missing on the line \"if x == 0 {\".") == 0);
}

TEST_METHOD(TestNoCurlyBracketException) {
  Token token1("x");
  Token token2("=");
  Token token3("1");
  Token token4(";");
  std::vector<Token> tokens;
  tokens.push_back(token1);
  tokens.push_back(token2);
  tokens.push_back(token3);
  tokens.push_back(token4);
  NoCurlyBracketException exception = NoCurlyBracketException(3, tokens, '}');
  Assert::IsTrue(strcmp(exception.what(),
                        "Error: Line 3: There is a curly bracket '}' "
                        "missing on the line \"x = 1 ;\".") == 0);
}

TEST_METHOD(TestNoSemiColonException) {
  Token token1("call");
  Token token2("procedure1");
  std::vector<Token> tokens;
  tokens.push_back(token1);
  tokens.push_back(token2);

  NoSemiColonException exception = NoSemiColonException(4, tokens);
  Assert::IsTrue(strcmp(exception.what(),
                        "Error: Line 4: Semicolon is missing "
                        "on the line \"call procedure1\".") == 0);
}
}
;
}
