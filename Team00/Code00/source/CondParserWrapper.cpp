#include "CondParserWrapper.h"
#include "ExprParserWrapper.h"
#include "ParseExceptions.h"
#include <assert.h>
// CondParserWrapper
// check if parenthesis are valid.
int CondParserWrapper::invalidParenthesis() const {
  int unpaired_paren = 0;
  int i;
  for (i = 0; i < condition.size(); i++) {
    if (condition[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren++;
    } else if (condition[i].getTokenEnum() == TokenEnum::CLOSE_P) {
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
bool CondParserWrapper::hasInvalidTokenEnum() const {
  for (int i = 0; i < condition.size(); i++) {
    switch (condition[i].getTokenEnum()) {
    case TokenEnum::OPEN_P:
    case TokenEnum::CLOSE_P:
    case TokenEnum::EQUAL:
    case TokenEnum::GREATER:
    case TokenEnum::LESS:
    case TokenEnum::GREATER_EQ:
    case TokenEnum::LESS_EQ:
    case TokenEnum::NOT_EQ:
    case TokenEnum::AND:
    case TokenEnum::OR:
    case TokenEnum::NOT:
      break;
    default:
      return false;
    }
  }
  return true;
}

// CondParserWrapper constructor
CondParserWrapper::CondParserWrapper(std::vector<Token> cond, int line)
    : condition(cond), lineNo(line) {
  if (hasInvalidTokenEnum()) {
    throw InvalidExpressionException(lineNo, condition);
  }
  if (invalidParenthesis() > 0) {
    throw NoParenthesisException(lineNo, condition, ')');
  } else if (invalidParenthesis() < 0) {
    throw NoParenthesisException(lineNo, condition, '(');
  }
}

// main functionn that parses the expression
void CondParserWrapper::parse() {
  CondExpressionParser condExpParser(condition, lineNo);
  condExpParser.parse();
  usedVariables.insert(condExpParser.getUsedVar().begin(),
                       condExpParser.getUsedVar().end());
}

// Getter function for used variables
std::set<Token> CondParserWrapper::getUsedVar() const { return usedVariables; }

// CondExpressionParser

// check if format of condition is correct
bool CondExpressionParser::isValidFormat() const {
  int numOp = 0;
  if (condExpression[0].getTokenEnum() == TokenEnum::NOT) {
    numOp++;
  }
  int unpaired_paren = 0;
  for (int i = condExpression.size() - 1; i >= 0; i--) {
    if (condExpression[i].getTokenEnum() == TokenEnum::CLOSE_P) {
      unpaired_paren++;
    } else if (condExpression[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren--;
    }
    if (unpaired_paren == 0 &&
        (condExpression[i].getTokenEnum() == TokenEnum::AND ||
         condExpression[i].getTokenEnum() == TokenEnum::OR)) {
      numOp++;
    }
  }
  if (numOp > 1) {
    return false;
  }
  return true;
}

// check for !, && and ||, return the position
int CondExpressionParser::checkOperator() const {
  if (condExpression[0].getTokenEnum() == TokenEnum::NOT) {
    return 0;
  }
  int unpaired_paren = 0;
  for (int i = condExpression.size() - 1; i >= 0; i--) {
    if (condExpression[i].getTokenEnum() == TokenEnum::CLOSE_P) {
      unpaired_paren++;
    } else if (condExpression[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren--;
    }
    if (unpaired_paren == 0 &&
        (condExpression[i].getTokenEnum() == TokenEnum::AND ||
         condExpression[i].getTokenEnum() == TokenEnum::OR)) {
      return i;
    }
  }
  return -1;
}

// constructor
CondExpressionParser::CondExpressionParser(std::vector<Token> cond, int line)
    : condExpression(cond), lineNo(line) {
  if (!isValidFormat()) {
    throw InvalidConditionException(lineNo, condExpression);
  }
}

// main function that parses the condition expression
// TODO: Exception handling, remove ();
void CondExpressionParser::parse() {
  int opPos = checkOperator();
  if (opPos != -1) {
    // case 1, operator is !
    if (opPos == 0) {
      if (condExpression[0].getTokenEnum() != TokenEnum::NOT) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      std::vector<Token> subConExp(condExpression.begin() + 2,
                                   condExpression.end() - 1);
      CondExpressionParser condExpParser(subConExp, lineNo);
      condExpParser.parse();
      usedVariables.insert(condExpParser.getUsedVar().begin(),
                           condExpParser.getUsedVar().end());
    } else {
      // operator is & or ||
      if (condExpression[opPos].getTokenEnum() != TokenEnum::AND &&
          condExpression[opPos].getTokenEnum() != TokenEnum::OR) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      std::vector<Token> subConExp1(condExpression.begin() + 1,
                                    condExpression.begin() + opPos - 2);
      CondExpressionParser condExpParser1(subConExp1, lineNo);
      condExpParser1.parse();
      usedVariables.insert(condExpParser1.getUsedVar().begin(),
                           condExpParser1.getUsedVar().end());
      std::vector<Token> subConExp2(condExpression.begin() + opPos + 2,
                                    condExpression.end() - 1);
      CondExpressionParser condExpParser2(subConExp2, lineNo);
      condExpParser2.parse();
      usedVariables.insert(condExpParser2.getUsedVar().begin(),
                           condExpParser2.getUsedVar().end());
    }
  } else {
    RelExpressionParser relExpParser(condExpression, lineNo);
    relExpParser.parse();
    usedVariables.insert(relExpParser.getUsedVar().begin(),
                         relExpParser.getUsedVar().end());
  }
}

// return a set of all used variables
std::set<Token> CondExpressionParser::getUsedVar() const {
  return usedVariables;
}

// check if a token contains comparison operator as tokenenum
bool RelExpressionParser::isComparisonOp(Token t) const {
  TokenEnum enumType = t.getTokenEnum();
  if (enumType == TokenEnum::EQUAL || enumType == TokenEnum::GREATER ||
      enumType == TokenEnum::LESS || enumType == TokenEnum::GREATER_EQ ||
      enumType == TokenEnum::LESS_EQ || enumType == TokenEnum::NOT_EQ) {
    return true;
  }
  return false;
}

// look for comparison operator and return  position
int RelExpressionParser::opPosition() const {
  int comparisonOpNum = 0;
  int unpaired_paren = 0;
  int i;
  for (i = relExpression.size() - 1; i >= 0; i--) {
    if (relExpression[i].getTokenEnum() == TokenEnum::CLOSE_P) {
      unpaired_paren++;
    } else if (relExpression[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren--;
    }
    if (unpaired_paren == 0 && isComparisonOp(relExpression[i])) {
      comparisonOpNum++;
    }
  }
  if (comparisonOpNum != 1) {
    return -1; // invalid format
  }
  return i;
}

// Constructor
RelExpressionParser::RelExpressionParser(std::vector<Token> rel, int line)
    : relExpression(rel), lineNo(line) {}

// main function that parses the factor
void RelExpressionParser::parse() {
  int opPos = opPosition();
  if (opPos == -1) {
    throw InvalidConditionException(lineNo, relExpression);
  }
  std::vector<Token> relFactor1(relExpression.begin(),
                                relExpression.begin() + opPos - 1);
  RelFactorParser relFactorParser1(relFactor1, lineNo);
  std::vector<Token> relFactor2(relExpression.begin() + opPos + 1,
                                relExpression.end());
  RelFactorParser relFactorParser2(relFactor2, lineNo);
  relFactorParser1.parse();
  relFactorParser2.parse();
  usedVariables.insert(relFactorParser1.getUsedVar().begin(),
                       relFactorParser1.getUsedVar().end());
  usedVariables.insert(relFactorParser2.getUsedVar().begin(),
                       relFactorParser2.getUsedVar().end());
}

// return used variables
std::set<Token> RelExpressionParser::getUsedVar() const {
  return usedVariables;
}

// RelFactorParser
// Constructor
RelFactorParser::RelFactorParser(std::vector<Token> rel, int line)
    : relFactor(rel), lineNo(line) {}

void RelFactorParser::parse() {
  // constant or var
  if (relFactor.size() == 1) {
    if (relFactor[0].getTokenEnum() == TokenEnum::WORD) {
      usedVariables.insert(relFactor[0]);
    } else if (!(relFactor[0].getTokenEnum() == TokenEnum::CONSTANT)) {
      throw InvalidConditionException(lineNo, relFactor);
    }
  } else {
    ExprParserWrapper expParser(relFactor, lineNo, &TNode());
    expParser.parse();
    usedVariables.insert(expParser.getUsedVar().begin(),
                         expParser.getUsedVar().end());
  }
}

// return used variables
std::set<Token> RelFactorParser::getUsedVar() const { return usedVariables; }
