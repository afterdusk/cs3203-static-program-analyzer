#include "ExprParserWrapper.h"
#include "ParseExceptions.h"
#include "Token.h"

// check if parenthesis are valid.
int ExprParserWrapper::invalidParenthesis() const {
  int unpaired_paren = 0;
  size_t i;
  for (i = 0; i < expression.size(); i++) {
    if (expression[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren++;
    } else if (expression[i].getTokenEnum() == TokenEnum::CLOSE_P) {
      unpaired_paren--;
    }
    if (unpaired_paren < 0) {
      // open parenthesis missing
      return -1;
    }
  }
  if (unpaired_paren > 0) {
    // closed parenthesis missing
    return 1;
  }
  return 0;
}

// check if any invalid TokenEnumValue
bool ExprParserWrapper::hasInvalidTokenEnum() const {
  for (size_t i = 0; i < expression.size(); i++) {
    switch (expression[i].getTokenEnum()) {
    case TokenEnum::OPEN_P:
    case TokenEnum::CLOSE_P:
    case TokenEnum::PLUS:
    case TokenEnum::MINUS:
    case TokenEnum::TIMES:
    case TokenEnum::DIV:
    case TokenEnum::MOD:
    case TokenEnum::WORD:
    case TokenEnum::CONSTANT:
      break;
    default:
      return true;
    }
  }
  return false;
}

// ExprWrapperParser constructor
ExprParserWrapper::ExprParserWrapper(std::vector<Token> expr, int line,
                                     TNode *root)
    : expression(expr), lineNo(line), rootNode(root) {
  if (hasInvalidTokenEnum() || invalidParenthesis() != 0) {
    throw InvalidExpressionException(lineNo, expression);
  }
}

// main functionn that parses the expression
void ExprParserWrapper::parse() {
  try {
    ExpressionParser expParser(expression, lineNo, rootNode);
    expParser.parseExpression();
    std::unordered_set<Token> tmp1 = expParser.getUsedVar();
    usedVariables.insert(tmp1.begin(), tmp1.end());
    std::unordered_set<Token> tmp2 = expParser.getUsedConstants();
    usedConstants.insert(tmp2.begin(), tmp2.end());
  } catch (InvalidExpressionException &i) {
    ignore(i);
    throw InvalidExpressionException(lineNo, expression);
  }
}

// Getter function for used variables
std::unordered_set<Token> ExprParserWrapper::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<Token> ExprParserWrapper::getUsedConstants() const {
  return usedConstants;
}

// return root node
const TNode *ExprParserWrapper::getRootNodePtr() const { return rootNode; }

// look for +/- sign outside brackets and return the position of the right most
// one
int ExpressionParser::checkPlusMinus() const {
  int unpaired_paren = 0;
  for (int i = expression.size() - 1; i >= 0; i--) {
    if (expression[i].getTokenEnum() == TokenEnum::CLOSE_P) {
      unpaired_paren++;
    } else if (expression[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren--;
    }
    if (unpaired_paren == 0 &&
        (expression[i].getTokenEnum() == TokenEnum::MINUS ||
         expression[i].getTokenEnum() == TokenEnum::PLUS)) {
      return i;
    }
  }
  return -1;
}

// ExpressionaParser
ExpressionParser::ExpressionParser(std::vector<Token> exp, int line,
                                   TNode *node)
    : expression(exp), lineNo(line), currNode(node) {}

// main function that parses the expression
void ExpressionParser::parseExpression() {
  int operator_pos = checkPlusMinus();
  if (operator_pos == -1) {
    // case: no +/- operator
    try {
      TermParser termParser(expression, lineNo, currNode);
      termParser.parseTerm();
      // append vector of var to the var used in termParser
      std::unordered_set<Token> tmp1 = termParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<Token> tmp2 = termParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  } else {
    if (operator_pos == 0 || operator_pos == expression.size() - 1) {
      // exception, turn tokens into string to throw excepton
      throw InvalidExpressionException(lineNo, expression);
    }
    // assign TNode::Op as parent node
    if (expression[operator_pos].getTokenEnum() == TokenEnum::PLUS) {
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
      std::vector<Token> subExpression(expression.begin(),
                                       expression.begin() + operator_pos);
      ExpressionParser expParser(subExpression, lineNo, left);
      expParser.parseExpression();
      std::unordered_set<Token> tmp1 = expParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<Token> tmp2 = expParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());

      // let term parser to parse the last term
      std::vector<Token> term(expression.begin() + operator_pos + 1,
                              expression.end());
      TermParser termParser(term, lineNo, right);
      termParser.parseTerm();
      std::unordered_set<Token> tmp3 = termParser.getUsedVar();
      usedVariables.insert(tmp3.begin(), tmp3.end());
      std::unordered_set<Token> tmp4 = termParser.getUsedConstants();
      usedConstants.insert(tmp4.begin(), tmp4.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  }
}

// return used variables
std::unordered_set<Token> ExpressionParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<Token> ExpressionParser::getUsedConstants() const {
  return usedConstants;
}

// TermParser

// check for * or / or % sign and return the position of the last one
int TermParser::checkMulDivMod() const {
  int unpaired_paren = 0;
  for (int i = term.size() - 1; i >= 0; i--) {
    if (term[i].getTokenEnum() == TokenEnum::CLOSE_P) {
      unpaired_paren++;
    } else if (term[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren--;
    }
    if (unpaired_paren == 0 && (term[i].getTokenEnum() == TokenEnum::TIMES ||
                                term[i].getTokenEnum() == TokenEnum::DIV ||
                                term[i].getTokenEnum() == TokenEnum::MOD)) {
      return i;
    }
  }
  return -1;
}

// Constructor
TermParser::TermParser(std::vector<Token> t, int line, TNode *node)
    : term(t), lineNo(line), currNode(node) {}

// main function that parses the term
void TermParser::parseTerm() {
  int operator_pos = checkMulDivMod();
  if (operator_pos == -1) {
    // case: no +/- operator
    try {
      FactorParser factorParser(term, lineNo, currNode);
      factorParser.parseFactor();
      // append vector of var to the var used in termParser
      std::unordered_set<Token> tmp = factorParser.getUsedVar();
      usedVariables.insert(tmp.begin(), tmp.end());
      std::unordered_set<Token> tmp2 = factorParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  } else {
    if (operator_pos == 0 || operator_pos == term.size() - 1) {
      // exception, turn tokens into string to throw excepton
      throw InvalidExpressionException(lineNo, term);
    }
    // assign TNode::Op as parent node
    if (term[operator_pos].getTokenEnum() == TokenEnum::TIMES) {
      currNode->op = TNode::Op::Times;
    } else if (term[operator_pos].getTokenEnum() == TokenEnum::DIV) {
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
      std::vector<Token> subTerm(term.begin(), term.begin() + operator_pos);
      TermParser termParser(subTerm, lineNo, left);
      termParser.parseTerm();
      std::unordered_set<Token> tmp1 = termParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<Token> tmp2 = termParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());

      // let factor parser to parse the last factor
      std::vector<Token> factor(term.begin() + operator_pos + 1, term.end());
      FactorParser factorParser(factor, lineNo, right);
      factorParser.parseFactor();
      std::unordered_set<Token> tmp3 = factorParser.getUsedVar();
      usedVariables.insert(tmp3.begin(), tmp3.end());
      std::unordered_set<Token> tmp4 = factorParser.getUsedConstants();
      usedConstants.insert(tmp4.begin(), tmp4.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  }
}

// return used variables
std::unordered_set<Token> TermParser::getUsedVar() const {
  return usedVariables;
}

// return used constants
std::unordered_set<Token> TermParser::getUsedConstants() const {
  return usedConstants;
}

// FactorParser

// check for brackets
bool FactorParser::hasParenthesis() const {
  return factor[0].getTokenEnum() == TokenEnum::OPEN_P &&
         factor[factor.size() - 1].getTokenEnum() == TokenEnum::CLOSE_P;
}

// Constructor
FactorParser::FactorParser(std::vector<Token> f, int line, TNode *node)
    : factor(f), lineNo(line), currNode(node) {}

// main function that parses the factor
void FactorParser::parseFactor() {
  if (!hasParenthesis()) {
    if (factor.size() != 1) {
      throw InvalidExpressionException(lineNo, factor);
    }
    if (factor[0].getTokenEnum() == TokenEnum::WORD) {
      usedVariables.insert(factor[0]);
    } else if (factor[0].getTokenEnum() == TokenEnum::CONSTANT) {
      usedConstants.insert(factor[0]);
    } else {
      throw InvalidExpressionException(lineNo, factor);
    }
    currNode->value = factor[0].getVal();
  } else {
    try {
      std::vector<Token> subExpression(factor.begin() + 1, factor.end() - 1);
      ExpressionParser expParser(subExpression, lineNo, currNode);
      expParser.parseExpression();
      std::unordered_set<Token> tmp1 = expParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<Token> tmp2 = expParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw;
    }
  }
}

// return used variables
std::unordered_set<Token> FactorParser::getUsedVar() const {
  return usedVariables;
}

// return used constants
std::unordered_set<Token> FactorParser::getUsedConstants() const {
  return usedConstants;
}
