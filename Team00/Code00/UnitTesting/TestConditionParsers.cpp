#include "CppUnitTest.h"
#include "stdafx.h"
#include <string>
#include <unordered_set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestRelFactorParser){
  public : TEST_METHOD(TestVarConstants){Token var("x");
std::vector<Token> vars;
vars.push_back(var);
RelFactorParser relFactorParser1(vars, 0);
relFactorParser1.parse();
std::unordered_set<Token> result1;
result1.insert(var);
Assert::IsTrue(relFactorParser1.getUsedVar() == result1);

Token constant("1000");
std::vector<Token> constants;
constants.push_back(constant);
RelFactorParser relFactorParser2(constants, 1);
relFactorParser2.parse();
Assert::IsTrue(relFactorParser2.getUsedVar().empty());
} // namespace UnitTesting

TEST_METHOD(TestExpression) {
  Tokenizer tokenizer("(x +(y-z)*1)");
  std::vector<Token> exp = tokenizer.tokenize();
  RelFactorParser relFactorParser(exp, 0);
  relFactorParser.parse();
  std::unordered_set<Token> result;
  result.insert(Token("x"));
  result.insert(Token("y"));
  result.insert(Token("z"));
  Assert::IsTrue(relFactorParser.getUsedVar() == result);
}
}
;

TEST_CLASS(TestRelExprParser){public : TEST_METHOD(TestRelExpressionSimple){
    std::vector<Token> relExp = Tokenizer("x >= y").tokenize();
RelExpressionParser relExprParser(relExp, 0);
relExprParser.parse();
std::unordered_set<Token> result;
result.insert(Token("x"));
result.insert(Token("y"));
Assert::IsTrue(relExprParser.getUsedVar() == result);
}

TEST_METHOD(TestRelExpressionComplex) {
  std::vector<Token> relExp =
      Tokenizer("x != (y * y * x + 10 - z* w)").tokenize();
  RelExpressionParser relExprParser(relExp, 0);
  relExprParser.parse();
  std::unordered_set<Token> result;
  result.insert(Token("x"));
  result.insert(Token("y"));
  result.insert(Token("w"));
  result.insert(Token("z"));
  Assert::IsTrue(relExprParser.getUsedVar() == result);
}
}
;

TEST_CLASS(TestCondExpressionParser){
  public : TEST_METHOD(TestCondExpressionNOT){
      std::vector<Token> condExp =
          Tokenizer("!((y * y * x + 10 - z* w % 10) == 0)").tokenize();
CondExpressionParser condExprParser(condExp, 0);
condExprParser.parse();
std::unordered_set<Token> result;
result.insert(Token("y"));
result.insert(Token("x"));
result.insert(Token("z"));
result.insert(Token("w"));
Assert::IsTrue(condExprParser.getUsedVar() == result);
}

TEST_METHOD(TestCondExpressionAND) {
  std::vector<Token> condExp =
      Tokenizer(" (x < 10) && ((w * 25) > k)").tokenize();
  CondExpressionParser condExprParser(condExp, 0);
  condExprParser.parse();
  std::unordered_set<Token> result;
  result.insert(Token("x"));
  result.insert(Token("w"));
  result.insert(Token("k"));
  Assert::IsTrue(condExprParser.getUsedVar() == result);
}

TEST_METHOD(TestCondExpressionComplex) {
  std::vector<Token> condExp =
      Tokenizer("(!((x < 10) && ((w * 25) > k)))||((y % 2) == 0)").tokenize();
  CondExpressionParser condExprParser(condExp, 0);
  condExprParser.parse();
  std::unordered_set<Token> result;
  result.insert(Token("x"));
  result.insert(Token("w"));
  result.insert(Token("k"));
  result.insert(Token("y"));
  Assert::IsTrue(condExprParser.getUsedVar() == result);
}
}
;

TEST_CLASS(TestCondWrapper){public : TEST_METHOD(TestWrapper){
    std::vector<Token> condExp =
        Tokenizer("(!((x < 10) && ((w * 25) > k)))||((y % 2) == 0)").tokenize();
CondParserWrapper wrapper(condExp, 0);
wrapper.parse();
std::unordered_set<Token> result;
result.insert(Token("x"));
result.insert(Token("w"));
result.insert(Token("k"));
result.insert(Token("y"));
Assert::IsTrue(wrapper.getUsedVar() == result);
}
}
;
}
