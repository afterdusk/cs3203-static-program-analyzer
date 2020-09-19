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
Assert::IsTrue(relFactorParser1.getUsedConstants().empty());

Token constant("1000");
std::vector<Token> constants;
constants.push_back(constant);
RelFactorParser relFactorParser2(constants, 1);
relFactorParser2.parse();
std::unordered_set<Token> result2;
result2.insert(constant);
Assert::IsTrue(relFactorParser2.getUsedVar().empty());
Assert::IsTrue(relFactorParser2.getUsedConstants() == result2);

} // namespace UnitTesting

TEST_METHOD(TestExpression) {
  Tokenizer tokenizer("(x +(y-z)*1)");
  std::vector<Token> exp = tokenizer.tokenize();
  RelFactorParser relFactorParser(exp, 0);
  relFactorParser.parse();
  std::unordered_set<Token> vars;
  vars.insert(Token("x"));
  vars.insert(Token("y"));
  vars.insert(Token("z"));
  std::unordered_set<Token> constants;
  constants.insert(Token("1"));
  Assert::IsTrue(relFactorParser.getUsedVar() == vars);
  Assert::IsTrue(relFactorParser.getUsedConstants() == constants);
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
Assert::IsTrue(relExprParser.getUsedConstants().empty());
}

TEST_METHOD(TestRelExpressionComplex) {
  std::vector<Token> relExp =
      Tokenizer("x != (y * y * x + 10 - z* w)").tokenize();
  RelExpressionParser relExprParser(relExp, 0);
  relExprParser.parse();
  std::unordered_set<Token> vars;
  vars.insert(Token("x"));
  vars.insert(Token("y"));
  vars.insert(Token("w"));
  vars.insert(Token("z"));
  std::unordered_set<Token> constants;
  constants.insert(Token("10"));
  Assert::IsTrue(relExprParser.getUsedVar() == vars);
  Assert::IsTrue(relExprParser.getUsedConstants() == constants);
}
}
;

TEST_CLASS(TestCondExpressionParser){
  public : TEST_METHOD(TestCondExpressionNOT){
      std::vector<Token> condExp =
          Tokenizer("!((y * y * x + 10 - z* w % 10) == 0)").tokenize();
CondExpressionParser condExprParser(condExp, 0);
condExprParser.parse();
std::unordered_set<Token> vars;
vars.insert(Token("y"));
vars.insert(Token("x"));
vars.insert(Token("z"));
vars.insert(Token("w"));
std::unordered_set<Token> constants;
constants.insert(Token("10"));
constants.insert(Token("0"));
Assert::IsTrue(condExprParser.getUsedVar() == vars);
Assert::IsTrue(condExprParser.getUsedConstants() == constants);
}

TEST_METHOD(TestCondExpressionAND) {
  std::vector<Token> condExp =
      Tokenizer(" (x < 10) && ((w * 25) > k)").tokenize();
  CondExpressionParser condExprParser(condExp, 0);
  condExprParser.parse();
  std::unordered_set<Token> vars;
  vars.insert(Token("x"));
  vars.insert(Token("w"));
  vars.insert(Token("k"));
  std::unordered_set<Token> constants;
  constants.insert(Token("10"));
  constants.insert(Token("25"));
  Assert::IsTrue(condExprParser.getUsedVar() == vars);
  Assert::IsTrue(condExprParser.getUsedConstants() == constants);
}

TEST_METHOD(TestCondExpressionComplex) {
  std::vector<Token> condExp =
      Tokenizer("(!((x < 10) && ((w * 25) > k)))||((y % 2) == 0)").tokenize();
  CondExpressionParser condExprParser(condExp, 0);
  condExprParser.parse();
  std::unordered_set<Token> vars;
  vars.insert(Token("x"));
  vars.insert(Token("w"));
  vars.insert(Token("k"));
  vars.insert(Token("y"));
  std::unordered_set<Token> constants;
  constants.insert(Token("10"));
  constants.insert(Token("25"));
  constants.insert(Token("2"));
  constants.insert(Token("0"));
  Assert::IsTrue(condExprParser.getUsedVar() == vars);
  Assert::IsTrue(condExprParser.getUsedConstants() == constants);
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
std::unordered_set<Token> constants;
constants.insert(Token("10"));
constants.insert(Token("25"));
constants.insert(Token("2"));
constants.insert(Token("0"));
Assert::IsTrue(wrapper.getUsedVar() == result);
Assert::IsTrue(wrapper.getUsedConstants() == constants);
}
}
;
}
