#include "CppUnitTest.h"
#include "stdafx.h"
#include <string>
#include <unordered_set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestRelFactorParser){
  public : TEST_METHOD(TestVarConstants){SimpleToken var("x");
std::vector<SimpleToken> vars;
vars.push_back(var);
RelFactorParser relFactorParser1(vars, 0);
relFactorParser1.parse();
std::unordered_set<SimpleToken> result1;
result1.insert(var);
Assert::IsTrue(relFactorParser1.getUsedVar() == result1);
Assert::IsTrue(relFactorParser1.getUsedConstants().empty());

SimpleToken constant("1000");
std::vector<SimpleToken> constants;
constants.push_back(constant);
RelFactorParser relFactorParser2(constants, 1);
relFactorParser2.parse();
std::unordered_set<SimpleToken> result2;
result2.insert(constant);
Assert::IsTrue(relFactorParser2.getUsedVar().empty());
Assert::IsTrue(relFactorParser2.getUsedConstants() == result2);

} // namespace UnitTesting

TEST_METHOD(TestExpression) {
  Tokenizer tokenizer("(x +(y-z)*1)");
  std::vector<SimpleToken> exp = tokenizer.tokenize();
  RelFactorParser relFactorParser(exp, 0);
  relFactorParser.parse();
  std::unordered_set<SimpleToken> vars;
  vars.insert(SimpleToken("x"));
  vars.insert(SimpleToken("y"));
  vars.insert(SimpleToken("z"));
  std::unordered_set<SimpleToken> constants;
  constants.insert(SimpleToken("1"));
  Assert::IsTrue(relFactorParser.getUsedVar() == vars);
  Assert::IsTrue(relFactorParser.getUsedConstants() == constants);
}
}
;

TEST_CLASS(TestRelExprParser){public : TEST_METHOD(TestRelExpressionSimple){
    std::vector<SimpleToken> relExp = Tokenizer("x >= y").tokenize();
RelExpressionParser relExprParser(relExp, 0);
relExprParser.parse();
std::unordered_set<SimpleToken> result;
result.insert(SimpleToken("x"));
result.insert(SimpleToken("y"));
Assert::IsTrue(relExprParser.getUsedVar() == result);
Assert::IsTrue(relExprParser.getUsedConstants().empty());
}

TEST_METHOD(TestRelExpressionComplex) {
  std::vector<SimpleToken> relExp =
      Tokenizer("x != (y * y * x + 10 - z* w)").tokenize();
  RelExpressionParser relExprParser(relExp, 0);
  relExprParser.parse();
  std::unordered_set<SimpleToken> vars;
  vars.insert(SimpleToken("x"));
  vars.insert(SimpleToken("y"));
  vars.insert(SimpleToken("w"));
  vars.insert(SimpleToken("z"));
  std::unordered_set<SimpleToken> constants;
  constants.insert(SimpleToken("10"));
  Assert::IsTrue(relExprParser.getUsedVar() == vars);
  Assert::IsTrue(relExprParser.getUsedConstants() == constants);
}
}
;

TEST_CLASS(TestCondExpressionParser){
  public : TEST_METHOD(TestCondExpressionNOT){
      std::vector<SimpleToken> condExp =
          Tokenizer("!((y * y * x + 10 - z* w % 10) == 0)").tokenize();
CondExpressionParser condExprParser(condExp, 0);
condExprParser.parse();
std::unordered_set<SimpleToken> vars;
vars.insert(SimpleToken("y"));
vars.insert(SimpleToken("x"));
vars.insert(SimpleToken("z"));
vars.insert(SimpleToken("w"));
std::unordered_set<SimpleToken> constants;
constants.insert(SimpleToken("10"));
constants.insert(SimpleToken("0"));
Assert::IsTrue(condExprParser.getUsedVar() == vars);
Assert::IsTrue(condExprParser.getUsedConstants() == constants);
}

TEST_METHOD(TestCondExpressionAND) {
  std::vector<SimpleToken> condExp =
      Tokenizer(" (x < 10) && ((w * 25) > k)").tokenize();
  CondExpressionParser condExprParser(condExp, 0);
  condExprParser.parse();
  std::unordered_set<SimpleToken> vars;
  vars.insert(SimpleToken("x"));
  vars.insert(SimpleToken("w"));
  vars.insert(SimpleToken("k"));
  std::unordered_set<SimpleToken> constants;
  constants.insert(SimpleToken("10"));
  constants.insert(SimpleToken("25"));
  Assert::IsTrue(condExprParser.getUsedVar() == vars);
  Assert::IsTrue(condExprParser.getUsedConstants() == constants);
}

TEST_METHOD(TestCondExpressionComplex) {
  std::vector<SimpleToken> condExp =
      Tokenizer("(!((x < 10) && ((w * 25) > k)))||((y % 2) == 0)").tokenize();
  CondExpressionParser condExprParser(condExp, 0);
  condExprParser.parse();
  std::unordered_set<SimpleToken> vars;
  vars.insert(SimpleToken("x"));
  vars.insert(SimpleToken("w"));
  vars.insert(SimpleToken("k"));
  vars.insert(SimpleToken("y"));
  std::unordered_set<SimpleToken> constants;
  constants.insert(SimpleToken("10"));
  constants.insert(SimpleToken("25"));
  constants.insert(SimpleToken("2"));
  constants.insert(SimpleToken("0"));
  Assert::IsTrue(condExprParser.getUsedVar() == vars);
  Assert::IsTrue(condExprParser.getUsedConstants() == constants);
}
}
;

TEST_CLASS(TestCondWrapper){public : TEST_METHOD(TestWrapper){
    std::vector<SimpleToken> condExp =
        Tokenizer("(!((x < 10) && ((w * 25) > k)))||((y % 2) == 0)").tokenize();
SimpleCondParserWrapper wrapper(condExp, 0);
wrapper.parse();
std::unordered_set<SimpleToken> result;
result.insert(SimpleToken("x"));
result.insert(SimpleToken("w"));
result.insert(SimpleToken("k"));
result.insert(SimpleToken("y"));
std::unordered_set<SimpleToken> constants;
constants.insert(SimpleToken("10"));
constants.insert(SimpleToken("25"));
constants.insert(SimpleToken("2"));
constants.insert(SimpleToken("0"));
Assert::IsTrue(wrapper.getUsedVar() == result);
Assert::IsTrue(wrapper.getUsedConstants() == constants);
}
}
;
}
