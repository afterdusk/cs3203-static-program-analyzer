#include "CppUnitTest.h"
#include "stdafx.h"
#include <string>
#include <unordered_set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestFactorParser){
  public : TEST_METHOD(TestFactorParserVar){Token token("x");
std::vector<Token> tokens;
tokens.push_back(token);
TNode *n = new TNode();
FactorParser factorParser(tokens, 1, n);
factorParser.parseFactor();
std::unordered_set<Token> result;
result.insert(token);
Assert::IsTrue(factorParser.getUsedVar() == result);
Assert::IsTrue(n->left == nullptr);
Assert::IsTrue(n->right == nullptr);
Assert::IsTrue(n->op == TNode::Op::Unknown);
Assert::IsTrue(n->value == "x");
} // namespace UnitTesting

TEST_METHOD(TestFactorParserConstant) {
  Token token("1000");
  std::vector<Token> tokens;
  tokens.push_back(token);
  TNode *n = new TNode();
  FactorParser factorParser(tokens, 1, n);
  factorParser.parseFactor();
  std::unordered_set<Token> result;
  result.insert(token);
  Assert::IsTrue(factorParser.getUsedVar().empty());
  Assert::IsTrue(n->left == nullptr);
  Assert::IsTrue(n->right == nullptr);
  Assert::IsTrue(n->op == TNode::Op::Unknown);
  Assert::IsTrue(n->value == "1000");
}

TEST_METHOD(TestFactorParserExpr) {
  Token token1("(");
  Token token2("x");
  Token token3("*");
  Token token4("y");
  Token token5(")");
  std::vector<Token> tokens;
  tokens.push_back(token1);
  tokens.push_back(token2);
  tokens.push_back(token3);
  tokens.push_back(token4);
  tokens.push_back(token5);
  TNode *n = new TNode();
  FactorParser factorParser(tokens, 1, n);
  factorParser.parseFactor();
  std::unordered_set<Token> result;
  result.insert(token2);
  result.insert(token4);
  Assert::IsTrue(factorParser.getUsedVar() == result);
  Assert::IsTrue(n->left != nullptr);
  Assert::IsTrue(n->right != nullptr);
  Assert::IsTrue(n->op == TNode::Op::Times);
  Assert::IsTrue(n->value == "");
}
}
;

TEST_CLASS(TestTermParser){
  public : TEST_METHOD(TestTermParserSimple){// x * y
                                             Token token1("x");
Token token2("*");
Token token3("y");
std::vector<Token> tokens;
tokens.push_back(token1);
tokens.push_back(token2);
tokens.push_back(token3);
TNode *n = new TNode();
TermParser termParser(tokens, 1, n);
termParser.parseTerm();
std::unordered_set<Token> result;
result.insert(token1);
result.insert(token3);
Assert::IsTrue(termParser.getUsedVar() == result);
Assert::IsTrue(n->op == TNode::Op::Times);
Assert::IsTrue(n->value == "");
Assert::IsTrue(n->left->value == "x");
Assert::IsTrue(n->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->left->left == nullptr);
Assert::IsTrue(n->left->right == nullptr);
Assert::IsTrue(n->right->value == "y");
Assert::IsTrue(n->right->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->left == nullptr);
Assert::IsTrue(n->right->right == nullptr);
}

// make sure it treats expression inside bracket as a term
TEST_METHOD(TestTermParserComplex) {
  // x*(y*z)
  Token token1("x");
  Token token2("*");
  Token token3("(");
  Token token4("y");
  Token token5("*");
  Token token6("z");
  Token token7(")");
  std::vector<Token> tokens;
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
  std::unordered_set<Token> result;
  result.insert(token1);
  result.insert(token4);
  result.insert(token6);
  Assert::IsTrue(termParser.getUsedVar() == result);
  // n
  Assert::IsTrue(n->op == TNode::Op::Times);
  Assert::IsTrue(n->value == "");

  // n->left
  Assert::IsTrue(n->left->value == "x");
  Assert::IsTrue(n->left->op == TNode::Op::Unknown);
  Assert::IsTrue(n->left->left == nullptr);
  Assert::IsTrue(n->left->right == nullptr);

  // n->right
  Assert::IsTrue(n->right->op == TNode::Op::Times);
  Assert::IsTrue(n->right->value == "");
  Assert::IsTrue(n->right->left->value == "y");
  Assert::IsTrue(n->right->left->op == TNode::Op::Unknown);
  Assert::IsTrue(n->right->right->value == "z");
  Assert::IsTrue(n->right->right->op == TNode::Op::Unknown);
}
}
;

TEST_CLASS(TestExprParser){
  public : TEST_METHOD(TestExprParserSimple){Token token1("x");
Token token2("+");
Token token3("y");
Token token4("-");
Token token5("z");
std::vector<Token> tokens;
tokens.push_back(token1);
tokens.push_back(token2);
tokens.push_back(token3);
tokens.push_back(token4);
tokens.push_back(token5);
TNode *n = new TNode();
ExpressionParser expParser(tokens, 1, n);
expParser.parseExpression();

std::unordered_set<Token> result;
result.insert(token1);
result.insert(token3);
result.insert(token5);
Assert::IsTrue(expParser.getUsedVar() == result);

// n
Assert::IsTrue(n->op == TNode::Op::Minus);
Assert::IsTrue(n->value == "");

// n->left
Assert::IsTrue(n->left->value == "");
Assert::IsTrue(n->left->op == TNode::Op::Plus);
Assert::IsTrue(n->left->left->value == "x");
Assert::IsTrue(n->left->left->op == TNode::Op::Unknown);
Assert::IsTrue(n->left->right->value == "y");
Assert::IsTrue(n->left->left->op == TNode::Op::Unknown);

// n->right
Assert::IsTrue(n->right->op == TNode::Op::Unknown);
Assert::IsTrue(n->right->value == "z");
Assert::IsTrue(n->right->left == nullptr);
Assert::IsTrue(n->right->right == nullptr);
}
}
;

TEST_CLASS(TestExprParserWrapper){public : TEST_METHOD(TestExpressionParsing){
    Tokenizer tokenizer("x+y +(d+ a*(b+c))");
std::vector<Token> tokens = tokenizer.tokenize();
TNode *n = new TNode();
ExprParserWrapper wrapper(tokens, 1, n);
wrapper.parse();

std::unordered_set<Token> result;
result.insert(Token("x"));
result.insert(Token("y"));
result.insert(Token("d"));
result.insert(Token("a"));
result.insert(Token("b"));
result.insert(Token("c"));

Assert::IsTrue(wrapper.getUsedVar() == result);
// n
Assert::IsTrue(n->op == TNode::Op::Plus);
Assert::IsTrue(n->value == "");

// n->left
Assert::IsTrue(n->left->op == TNode::Op::Plus);
Assert::IsTrue(n->left->left->value == "x");
Assert::IsTrue(n->left->right->value == "y");

// n->right
Assert::IsTrue(n->right->op == TNode::Op::Plus);
Assert::IsTrue(n->right->left->value == "d");
Assert::IsTrue(n->right->right->op == TNode::Op::Times);
Assert::IsTrue(n->right->right->left->value == "a");
Assert::IsTrue(n->right->right->right->op == TNode::Op::Plus);
Assert::IsTrue(n->right->right->right->left->value == "b");
Assert::IsTrue(n->right->right->right->right->value == "c");
}
}
;
}
