#include "CppUnitTest.h"
#include "stdafx.h"
#include <string>
#include <unordered_set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestFactorParser){
  public : TEST_METHOD(TestFactorParserVar){SimpleToken token("x");
std::vector<SimpleToken> tokens;
tokens.push_back(token);
TNode *n = new TNode();
FactorParser factorParser(tokens, 1, n);
factorParser.parseFactor();
std::unordered_set<SimpleToken> vars;
vars.insert(token);
Assert::IsTrue(factorParser.getUsedVar() == vars);
Assert::IsTrue(factorParser.getUsedConstants().empty());

// AST
Assert::IsTrue(n->left == nullptr);
Assert::IsTrue(n->right == nullptr);
Assert::IsTrue(n->op == TNode::Op::Unknown);
Assert::IsTrue(n->value == "x");
} // namespace UnitTesting

TEST_METHOD(TestFactorParserConstant) {
  SimpleToken token("1000");
  std::vector<SimpleToken> tokens;
  tokens.push_back(token);
  TNode *n = new TNode();
  FactorParser factorParser(tokens, 1, n);
  factorParser.parseFactor();
  std::unordered_set<SimpleToken> constants;
  constants.insert(token);
  Assert::IsTrue(factorParser.getUsedVar().empty());
  Assert::IsTrue(factorParser.getUsedConstants() == constants);

  // AST
  Assert::IsTrue(n->left == nullptr);
  Assert::IsTrue(n->right == nullptr);
  Assert::IsTrue(n->op == TNode::Op::Unknown);
  Assert::IsTrue(n->value == "1000");
}

TEST_METHOD(TestFactorParserExpr) {
  SimpleToken token1("(");
  SimpleToken token2("x");
  SimpleToken token3("%");
  SimpleToken token4("3");
  SimpleToken token5(")");
  std::vector<SimpleToken> tokens;
  tokens.push_back(token1);
  tokens.push_back(token2);
  tokens.push_back(token3);
  tokens.push_back(token4);
  tokens.push_back(token5);
  TNode *n = new TNode();
  FactorParser factorParser(tokens, 1, n);
  factorParser.parseFactor();
  std::unordered_set<SimpleToken> vars;
  vars.insert(token2);
  std::unordered_set<SimpleToken> constants;
  constants.insert(token4);
  Assert::IsTrue(factorParser.getUsedVar() == vars);
  Assert::IsTrue(factorParser.getUsedConstants() == constants);

  // AST
  Assert::IsTrue(n->op == TNode::Op::Modulo);
  Assert::IsTrue(n->value == "");

  Assert::IsTrue(n->left->op == TNode::Op::Unknown);
  Assert::IsTrue(n->left->value == "x");
  Assert::IsTrue(n->left->left == nullptr);
  Assert::IsTrue(n->left->right == nullptr);

  Assert::IsTrue(n->right->op == TNode::Op::Unknown);
  Assert::IsTrue(n->right->value == "3");
  Assert::IsTrue(n->right->left == nullptr);
  Assert::IsTrue(n->right->right == nullptr);
}
}
;

TEST_CLASS(TestTermParser){
  public : TEST_METHOD(TestTermParserSimple){// x * y
                                             SimpleToken token1("x");
SimpleToken token2("*");
SimpleToken token3("y");
std::vector<SimpleToken> tokens;
tokens.push_back(token1);
tokens.push_back(token2);
tokens.push_back(token3);
TNode *n = new TNode();
TermParser termParser(tokens, 1, n);
termParser.parseTerm();
std::unordered_set<SimpleToken> vars;
vars.insert(token1);
vars.insert(token3);
Assert::IsTrue(termParser.getUsedVar() == vars);
Assert::IsTrue(termParser.getUsedConstants().empty());

// AST
Assert::IsTrue(n->op == TNode::Op::Times);
Assert::IsTrue(n->value == "");

Assert::IsTrue(n->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->left->value == "x");
Assert::IsTrue(n->left->left == nullptr);
Assert::IsTrue(n->left->right == nullptr);

Assert::IsTrue(n->right->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->value == "y");
Assert::IsTrue(n->right->left == nullptr);
Assert::IsTrue(n->right->right == nullptr);
}

// make sure it treats expression inside bracket as a term
TEST_METHOD(TestTermParserComplex) {
  // x*(y*z)
  SimpleToken token1("x");
  SimpleToken token2("*");
  SimpleToken token3("(");
  SimpleToken token4("y");
  SimpleToken token5("+");
  SimpleToken token6("5");
  SimpleToken token7(")");
  std::vector<SimpleToken> tokens;
  tokens.push_back(token1);
  tokens.push_back(token2);
  tokens.push_back(token3);
  tokens.push_back(token4);
  tokens.push_back(token5);
  tokens.push_back(token6);
  tokens.push_back(token7);
  TNode *n = new TNode();
  TermParser termParser(tokens, 1, n);
  termParser.parseTerm();
  std::unordered_set<SimpleToken> vars;
  vars.insert(token1);
  vars.insert(token4);
  std::unordered_set<SimpleToken> constants;
  constants.insert(token6);
  Assert::IsTrue(termParser.getUsedVar() == vars);
  Assert::IsTrue(termParser.getUsedConstants() == constants);

  // AST
  Assert::IsTrue(n->op == TNode::Op::Times);
  Assert::IsTrue(n->value == "");

  Assert::IsTrue(n->left->value == "x");
  Assert::IsTrue(n->left->op == TNode::Op::Unknown);
  Assert::IsTrue(n->left->left == nullptr);
  Assert::IsTrue(n->left->right == nullptr);

  Assert::IsTrue(n->right->op == TNode::Op::Plus);
  Assert::IsTrue(n->right->value == "");

  Assert::IsTrue(n->right->left->value == "y");
  Assert::IsTrue(n->right->left->op == TNode::Op::Unknown);
  Assert::IsTrue(n->right->left->left == nullptr);
  Assert::IsTrue(n->right->left->right == nullptr);

  Assert::IsTrue(n->right->right->value == "5");
  Assert::IsTrue(n->right->right->op == TNode::Op::Unknown);
  Assert::IsTrue(n->right->right->left == nullptr);
  Assert::IsTrue(n->right->right->right == nullptr);
}
}
;

TEST_CLASS(TestExprParser){
  public : TEST_METHOD(TestExprParserSimple){SimpleToken token1("x");
SimpleToken token2("+");
SimpleToken token3("y");
SimpleToken token4("-");
SimpleToken token5("z");
std::vector<SimpleToken> tokens;
tokens.push_back(token1);
tokens.push_back(token2);
tokens.push_back(token3);
tokens.push_back(token4);
tokens.push_back(token5);
TNode *n = new TNode();
ExpressionParser expParser(tokens, 1, n);
expParser.parseExpression();

std::unordered_set<SimpleToken> vars;
vars.insert(token1);
vars.insert(token3);
vars.insert(token5);
Assert::IsTrue(expParser.getUsedVar() == vars);
Assert::IsTrue(expParser.getUsedConstants().empty());

// AST
Assert::IsTrue(n->op == TNode::Op::Minus);
Assert::IsTrue(n->value == "");

Assert::IsTrue(n->left->value == "");
Assert::IsTrue(n->left->op == TNode::Op::Plus);

Assert::IsTrue(n->left->left->value == "x");
Assert::IsTrue(n->left->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->left->left->left == nullptr);
Assert::IsTrue(n->left->left->right == nullptr);

Assert::IsTrue(n->left->right->value == "y");
Assert::IsTrue(n->left->right->op == TNode::Op::Unknown);
Assert::IsTrue(n->left->right->left == nullptr);
Assert::IsTrue(n->left->right->right == nullptr);

Assert::IsTrue(n->right->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->value == "z");
Assert::IsTrue(n->right->left == nullptr);
Assert::IsTrue(n->right->right == nullptr);
}
}
;

TEST_CLASS(TestExprParserWrapper){public : TEST_METHOD(TestExpressionParsing){
    Tokenizer tokenizer("9+y +(d+ 1000*(b+c))");
std::vector<SimpleToken> tokens = tokenizer.tokenize();
TNode *n = new TNode();
SimpleExprParserWrapper wrapper(tokens, 1, n);
wrapper.parse();

std::unordered_set<SimpleToken> vars;
vars.insert(SimpleToken("y"));
vars.insert(SimpleToken("d"));
vars.insert(SimpleToken("b"));
vars.insert(SimpleToken("c"));
std::unordered_set<SimpleToken> constants;
constants.insert(SimpleToken("9"));
constants.insert(SimpleToken("1000"));

Assert::IsTrue(wrapper.getUsedVar() == vars);
Assert::IsTrue(wrapper.getUsedConstants() == constants);

// AST
Assert::IsTrue(n->op == TNode::Op::Plus);
Assert::IsTrue(n->value == "");

Assert::IsTrue(n->left->op == TNode::Op::Plus);
Assert::IsTrue(n->left->value == "");

Assert::IsTrue(n->left->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->left->left->value == "9");
Assert::IsTrue(n->left->left->left == nullptr);
Assert::IsTrue(n->left->left->right == nullptr);

Assert::IsTrue(n->left->right->op == TNode::Op::Unknown);
Assert::IsTrue(n->left->right->value == "y");
Assert::IsTrue(n->left->right->left == nullptr);
Assert::IsTrue(n->left->right->right == nullptr);

Assert::IsTrue(n->right->op == TNode::Op::Plus);
Assert::IsTrue(n->right->value == "");

Assert::IsTrue(n->right->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->left->value == "d");
Assert::IsTrue(n->right->left->left == nullptr);
Assert::IsTrue(n->right->left->right == nullptr);

Assert::IsTrue(n->right->right->op == TNode::Op::Times);
Assert::IsTrue(n->right->right->value == "");

Assert::IsTrue(n->right->right->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->right->left->value == "1000");
Assert::IsTrue(n->right->right->left->left == nullptr);
Assert::IsTrue(n->right->right->left->right == nullptr);

Assert::IsTrue(n->right->right->right->op == TNode::Op::Plus);
Assert::IsTrue(n->right->right->right->value == "");

Assert::IsTrue(n->right->right->right->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->right->right->left->value == "b");
Assert::IsTrue(n->right->right->right->left->left == nullptr);
Assert::IsTrue(n->right->right->right->left->right == nullptr);

Assert::IsTrue(n->right->right->right->right->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->right->right->right->value == "c");
Assert::IsTrue(n->right->right->right->right->left == nullptr);
Assert::IsTrue(n->right->right->right->right->right == nullptr);
}
TEST_METHOD(TestInvalidExpression) {

  std::vector<SimpleToken> tokens;
  tokens = Tokenizer("x != 1").tokenize();
  try {
    SimpleExprParserWrapper(tokens, 1, new TNode()).parse();
    Assert::Fail();
  } catch (InvalidExpressionException &i) {
    ignore(i);
  } catch (std::exception &e) {
    ignore(e);
    Assert::Fail();
  }

  tokens = Tokenizer("x + y ( z+q)").tokenize();
  try {
    SimpleExprParserWrapper(tokens, 1, new TNode()).parse();
    Assert::Fail();
  } catch (InvalidExpressionException &i) {
    ignore(i);
  } catch (std::exception &e) {
    ignore(e);
    Assert::Fail();
  }

  tokens = Tokenizer("x + (y + z)) * 8").tokenize();
  try {
    SimpleExprParserWrapper(tokens, 1, new TNode()).parse();
    Assert::Fail();
  } catch (InvalidExpressionException &i) {
    ignore(i);
  } catch (std::exception &e) {
    ignore(e);
    Assert::Fail();
  }

  tokens = Tokenizer("x ++ 1* m").tokenize();
  try {
    SimpleExprParserWrapper(tokens, 1, new TNode()).parse();
    Assert::Fail();
  } catch (InvalidExpressionException &i) {
    ignore(i);
  } catch (std::exception &e) {
    ignore(e);
    Assert::Fail();
  }

  tokens = Tokenizer("x / y -12 %").tokenize();
  try {
    SimpleExprParserWrapper(tokens, 1, new TNode()).parse();
    Assert::Fail();
  } catch (InvalidExpressionException &i) {
    ignore(i);
  } catch (std::exception &e) {
    ignore(e);
    Assert::Fail();
  }
}
}
;
}
