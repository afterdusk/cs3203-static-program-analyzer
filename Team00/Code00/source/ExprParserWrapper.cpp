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
      return false;
    }
  }
  return true;
}

// ExprWrapperParser constructor
ExprParserWrapper::ExprParserWrapper(std::vector<Token> expr, int line,
                                     TNode *root)
    : expression(expr), lineNo(line), rootNode(root) {
  if (hasInvalidTokenEnum()) {
    throw InvalidExpressionException(lineNo, expression);
  }
  if (invalidParenthesis() > 0) {
    throw NoParenthesisException(lineNo, expression, ')');
  } else if (invalidParenthesis() < 0) {
    throw NoParenthesisException(lineNo, expression, '(');
  }
}

// main functionn that parses the expression
void ExprParserWrapper::parse() {
  ExpressionParser expParser(expression, lineNo, rootNode);
  expParser.parseExpression();
  usedVariables.insert(expParser.getUsedVar().begin(),
                       expParser.getUsedVar().end());
}

// Getter function for used variables
std::unordered_set<Token> ExprParserWrapper::getUsedVar() const {
  return usedVariables;
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
    TermParser termParser(expression, lineNo, currNode);
    termParser.parseTerm();
    // append vector of var to the var used in termParser
    usedVariables.insert(termParser.getUsedVar().begin(),
                         termParser.getUsedVar().end());
  } else {
    if (operator_pos == 0 || operator_pos == expression.size() - 1) {
      // exception, turn tokens into string to throw excepton
      throw InvalidExpressionException(lineNo, expression);
    }
    // assign Op as parent node
    if (expression[operator_pos].getTokenEnum() == TokenEnum::PLUS) {
      currNode->op = Op::Plus;
    } else {
      currNode->op = Op::Minus;
    }
    TNode *left = new TNode();
    TNode *right = new TNode();
    currNode->left = left;
    currNode->right = right;
    // let expression parser to parse the remaining expression (other than last
    // term)
    std::vector<Token> subExpression(expression.begin(),
                                     expression.begin() + operator_pos - 1);
    ExpressionParser expParser(subExpression, lineNo, left);
    expParser.parseExpression();
    usedVariables.insert(expParser.getUsedVar().begin(),
                         expParser.getUsedVar().end());

    // let term parser to parse the last term
    std::vector<Token> term(expression.begin() + operator_pos + 1,
                            expression.end());
    TermParser termParser(term, lineNo, right);
    termParser.parseTerm();
    usedVariables.insert(termParser.getUsedVar().begin(),
                         termParser.getUsedVar().end());
  }
}

// return used variables
std::unordered_set<Token> ExpressionParser::getUsedVar() const {
  return usedVariables;
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
    FactorParser factorParser(term, lineNo, currNode);
    factorParser.parseFactor();
    // append vector of var to the var used in termParser
    usedVariables.insert(factorParser.getUsedVar().begin(),
                         factorParser.getUsedVar().end());
  } else {
    if (operator_pos == 0 || operator_pos == term.size() - 1) {
      // exception, turn tokens into string to throw excepton
      throw InvalidExpressionException(lineNo, term);
    }
    // assign Op as parent node
    if (term[operator_pos].getTokenEnum() == TokenEnum::TIMES) {
      currNode->op = Op::Times;
    } else if (term[operator_pos].getTokenEnum() == TokenEnum::DIV) {
      currNode->op = Op::Divide;
    } else {
      currNode->op = Op::Modulo;
    }
    TNode *left = new TNode();
    TNode *right = new TNode();
    currNode->left = left;
    currNode->right = right;
    // let term parser to parse the remaining term (other than last
    // factor)
    std::vector<Token> subTerm(term.begin(), term.begin() + operator_pos - 1);
    TermParser termParser(subTerm, lineNo, left);
    termParser.parseTerm();
    usedVariables.insert(termParser.getUsedVar().begin(),
                         termParser.getUsedVar().end());

    // let factor parser to parse the last factor
    std::vector<Token> factor(term.begin() + operator_pos + 1, term.end());
    FactorParser factorParser(factor, lineNo, right);
    factorParser.parseFactor();
    usedVariables.insert(factorParser.getUsedVar().begin(),
                         factorParser.getUsedVar().end());
  }
}

// return used variables
std::unordered_set<Token> TermParser::getUsedVar() const {
  return usedVariables;
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
    currNode->value = factor[0].getVal();
    if (factor[0].getTokenEnum() == TokenEnum::WORD) {
      usedVariables.insert(factor[0]);
    }
  } else {
    std::vector<Token> subExpression(factor.begin() + 1, factor.end() - 1);
    ExpressionParser expParser(subExpression, lineNo, currNode);
  }
}

// return used variables
std::unordered_set<Token> FactorParser::getUsedVar() const {
  return usedVariables;
}
