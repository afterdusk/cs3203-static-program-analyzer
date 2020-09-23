#include "SimpleExprParserWrapper.h"
#include "SimpleParseExceptions.h"
#include "SimpleToken.h"

// check if parenthesis are valid.
int SimpleExprParserWrapper::invalidParenthesis() const {
  int unpairedParen = 0;
  size_t i;
  for (i = 0; i < expression.size(); i++) {
    if (expression[i].getTokenType() == SimpleToken::TokenType::OPEN_P) {
      unpairedParen++;
    } else if (expression[i].getTokenType() ==
               SimpleToken::TokenType::CLOSE_P) {
      unpairedParen--;
    }
    if (unpairedParen < 0) {
      // open parenthesis missing
      return -1;
    }
  }
  if (unpairedParen > 0) {
    // closed parenthesis missing
    return 1;
  }
  return 0;
}

// check if any invalid TokenEnumValue
bool SimpleExprParserWrapper::hasInvalidTokenType() const {
  for (size_t i = 0; i < expression.size(); i++) {
    switch (expression[i].getTokenType()) {
    case SimpleToken::TokenType::OPEN_P:
    case SimpleToken::TokenType::CLOSE_P:
    case SimpleToken::TokenType::PLUS:
    case SimpleToken::TokenType::MINUS:
    case SimpleToken::TokenType::TIMES:
    case SimpleToken::TokenType::DIV:
    case SimpleToken::TokenType::MOD:
    case SimpleToken::TokenType::WORD:
    case SimpleToken::TokenType::CONSTANT:
      break;
    default:
      return true;
    }
  }
  return false;
}

// ExprWrapperParser constructor
SimpleExprParserWrapper::SimpleExprParserWrapper(std::vector<SimpleToken> expr,
                                                 int line, TNode *root)
    : expression(expr), lineNo(line), rootNode(root) {
  if (hasInvalidTokenType() || invalidParenthesis() != 0) {
    throw InvalidExpressionException(lineNo, expression);
  }
}

// main functionn that parses the expression
void SimpleExprParserWrapper::parse() {
  try {
    ExpressionParser expParser(expression, lineNo, rootNode);
    expParser.parseExpression();
    std::unordered_set<SimpleToken> tmp1 = expParser.getUsedVar();
    usedVariables.insert(tmp1.begin(), tmp1.end());
    std::unordered_set<SimpleToken> tmp2 = expParser.getUsedConstants();
    usedConstants.insert(tmp2.begin(), tmp2.end());
  } catch (InvalidExpressionException &i) {
    ignore(i);
    throw InvalidExpressionException(lineNo, expression);
  }
}

// Getter function for used variables
std::unordered_set<SimpleToken> SimpleExprParserWrapper::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<SimpleToken>
SimpleExprParserWrapper::getUsedConstants() const {
  return usedConstants;
}

// return root node
const TNode *SimpleExprParserWrapper::getRootNodePtr() const {
  return rootNode;
}

// look for +/- sign outside brackets and return the position of the right most
// one
int ExpressionParser::checkPlusMinus() const {
  int unpairedParen = 0;
  for (int i = expression.size() - 1; i >= 0; i--) {
    if (expression[i].getTokenType() == SimpleToken::TokenType::CLOSE_P) {
      unpairedParen++;
    } else if (expression[i].getTokenType() == SimpleToken::TokenType::OPEN_P) {
      unpairedParen--;
    }
    if (unpairedParen == 0 &&
        (expression[i].getTokenType() == SimpleToken::TokenType::MINUS ||
         expression[i].getTokenType() == SimpleToken::TokenType::PLUS)) {
      return i;
    }
  }
  return -1;
}

// ExpressionaParser
ExpressionParser::ExpressionParser(std::vector<SimpleToken> exp, int line,
                                   TNode *node)
    : expression(exp), lineNo(line), currNode(node) {}

// main function that parses the expression
void ExpressionParser::parseExpression() {
  int operatorPos = checkPlusMinus();
  if (operatorPos == -1) {
    // case: no +/- operator
    try {
      TermParser termParser(expression, lineNo, currNode);
      termParser.parseTerm();
      // append vector of var to the var used in termParser
      std::unordered_set<SimpleToken> tmp1 = termParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<SimpleToken> tmp2 = termParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  } else {
    if (operatorPos == 0 || operatorPos == expression.size() - 1) {
      // exception, turn tokens into string to throw excepton
      throw InvalidExpressionException(lineNo, expression);
    }
    // assign TNode::Op as parent node
    if (expression[operatorPos].getTokenType() ==
        SimpleToken::TokenType::PLUS) {
      currNode->op = TNode::Op::Plus;
    } else {
      currNode->op = TNode::Op::Minus;
    }
    TNode *left = new TNode();
    TNode *right = new TNode();
    currNode->left = left;
    currNode->right = right;
    // let expression parser to parse the remaining expression (other than last
    // term)
    try {
      std::vector<SimpleToken> subExpression(expression.begin(),
                                             expression.begin() + operatorPos);
      ExpressionParser expParser(subExpression, lineNo, left);
      expParser.parseExpression();
      std::unordered_set<SimpleToken> tmp1 = expParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<SimpleToken> tmp2 = expParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());

      // let term parser to parse the last term
      std::vector<SimpleToken> term(expression.begin() + operatorPos + 1,
                                    expression.end());
      TermParser termParser(term, lineNo, right);
      termParser.parseTerm();
      std::unordered_set<SimpleToken> tmp3 = termParser.getUsedVar();
      usedVariables.insert(tmp3.begin(), tmp3.end());
      std::unordered_set<SimpleToken> tmp4 = termParser.getUsedConstants();
      usedConstants.insert(tmp4.begin(), tmp4.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  }
}

// return used variables
std::unordered_set<SimpleToken> ExpressionParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<SimpleToken> ExpressionParser::getUsedConstants() const {
  return usedConstants;
}

// TermParser

// check for * or / or % sign and return the position of the last one
int TermParser::checkMulDivMod() const {
  int unpairedParen = 0;
  for (int i = term.size() - 1; i >= 0; i--) {
    if (term[i].getTokenType() == SimpleToken::TokenType::CLOSE_P) {
      unpairedParen++;
    } else if (term[i].getTokenType() == SimpleToken::TokenType::OPEN_P) {
      unpairedParen--;
    }
    if (unpairedParen == 0 &&
        (term[i].getTokenType() == SimpleToken::TokenType::TIMES ||
         term[i].getTokenType() == SimpleToken::TokenType::DIV ||
         term[i].getTokenType() == SimpleToken::TokenType::MOD)) {
      return i;
    }
  }
  return -1;
}

// Constructor
TermParser::TermParser(std::vector<SimpleToken> t, int line, TNode *node)
    : term(t), lineNo(line), currNode(node) {}

// main function that parses the term
void TermParser::parseTerm() {
  int operatorPos = checkMulDivMod();
  if (operatorPos == -1) {
    // case: no times/mod/divide operator
    try {
      FactorParser factorParser(term, lineNo, currNode);
      factorParser.parseFactor();
      // append vector of var to the var used in termParser
      std::unordered_set<SimpleToken> tmp = factorParser.getUsedVar();
      usedVariables.insert(tmp.begin(), tmp.end());
      std::unordered_set<SimpleToken> tmp2 = factorParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  } else {
    if (operatorPos == 0 || operatorPos == term.size() - 1) {
      // exception, turn tokens into string to throw excepton
      throw InvalidExpressionException(lineNo, term);
    }
    // assign TNode::Op as parent node
    if (term[operatorPos].getTokenType() == SimpleToken::TokenType::TIMES) {
      currNode->op = TNode::Op::Times;
    } else if (term[operatorPos].getTokenType() ==
               SimpleToken::TokenType::DIV) {
      currNode->op = TNode::Op::Divide;
    } else {
      currNode->op = TNode::Op::Modulo;
    }
    TNode *left = new TNode();
    TNode *right = new TNode();
    currNode->left = left;
    currNode->right = right;
    // let term parser to parse the remaining term (other than last
    // factor)
    try {
      std::vector<SimpleToken> subTerm(term.begin(),
                                       term.begin() + operatorPos);
      TermParser termParser(subTerm, lineNo, left);
      termParser.parseTerm();
      std::unordered_set<SimpleToken> tmp1 = termParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<SimpleToken> tmp2 = termParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());

      // let factor parser to parse the last factor
      std::vector<SimpleToken> factor(term.begin() + operatorPos + 1,
                                      term.end());
      FactorParser factorParser(factor, lineNo, right);
      factorParser.parseFactor();
      std::unordered_set<SimpleToken> tmp3 = factorParser.getUsedVar();
      usedVariables.insert(tmp3.begin(), tmp3.end());
      std::unordered_set<SimpleToken> tmp4 = factorParser.getUsedConstants();
      usedConstants.insert(tmp4.begin(), tmp4.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  }
}

// return used variables
std::unordered_set<SimpleToken> TermParser::getUsedVar() const {
  return usedVariables;
}

// return used constants
std::unordered_set<SimpleToken> TermParser::getUsedConstants() const {
  return usedConstants;
}

// FactorParser

// check for brackets
bool FactorParser::hasParenthesis() const {
  return factor[0].getTokenType() == SimpleToken::TokenType::OPEN_P &&
         factor[factor.size() - 1].getTokenType() ==
             SimpleToken::TokenType::CLOSE_P;
}

// Constructor
FactorParser::FactorParser(std::vector<SimpleToken> f, int line, TNode *node)
    : factor(f), lineNo(line), currNode(node) {}

// main function that parses the factor
void FactorParser::parseFactor() {
  if (!hasParenthesis()) {
    if (factor.size() != 1) {
      throw InvalidExpressionException(lineNo, factor);
    }
    if (factor[0].getTokenType() == SimpleToken::TokenType::WORD) {
      usedVariables.insert(factor[0]);
    } else if (factor[0].getTokenType() == SimpleToken::TokenType::CONSTANT) {
      usedConstants.insert(factor[0]);
    } else {
      throw InvalidExpressionException(lineNo, factor);
    }
    currNode->value = factor[0].getVal();
  } else {
    try {
      std::vector<SimpleToken> subExpression(factor.begin() + 1,
                                             factor.end() - 1);
      ExpressionParser expParser(subExpression, lineNo, currNode);
      expParser.parseExpression();
      std::unordered_set<SimpleToken> tmp1 = expParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<SimpleToken> tmp2 = expParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  }
}

// return used variables
std::unordered_set<SimpleToken> FactorParser::getUsedVar() const {
  return usedVariables;
}

// return used constants
std::unordered_set<SimpleToken> FactorParser::getUsedConstants() const {
  return usedConstants;
}
