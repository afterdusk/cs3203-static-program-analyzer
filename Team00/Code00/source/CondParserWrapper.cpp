#include "CondParserWrapper.h"
#include "ExprParserWrapper.h"
#include "ParseExceptions.h"

// CondParserWrapper
// check if parenthesis are valid.
int CondParserWrapper::invalidParenthesis() const {
  int unpaired_paren = 0;
  size_t i;
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
  for (size_t i = 0; i < condition.size(); i++) {
    switch (condition[i].getTokenEnum()) {
    case TokenEnum::SEMI_COLON:
    case TokenEnum::OPEN_B:
    case TokenEnum::CLOSE_B:
    case TokenEnum::ASSIGN:
    case TokenEnum::OTHER:
      return true;
    }
  }
  return false;
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
  std::unordered_set<Token> tmp1 = condExpParser.getUsedVar();
  usedVariables.insert(tmp1.begin(), tmp1.end());
  std::unordered_set<Token> tmp2 = condExpParser.getUsedConstants();
  usedConstants.insert(tmp2.begin(), tmp2.end());
}

// Getter function for used variables
std::unordered_set<Token> CondParserWrapper::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<Token> CondParserWrapper::getUsedConstants() const {
  return usedConstants;
}

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
int CondExpressionParser::opPosition() const {
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
  int opPos = opPosition();
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
      std::unordered_set<Token> tmp1 = condExpParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<Token> tmp2 = condExpParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } else {
      // operator is & or ||
      if (condExpression[opPos].getTokenEnum() != TokenEnum::AND &&
          condExpression[opPos].getTokenEnum() != TokenEnum::OR) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      std::vector<Token> subConExp1(condExpression.begin() + 1,
                                    condExpression.begin() + opPos - 1);
      CondExpressionParser condExpParser1(subConExp1, lineNo);
      condExpParser1.parse();
      std::unordered_set<Token> tmp1 = condExpParser1.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<Token> tmp2 = condExpParser1.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());

      std::vector<Token> subConExp2(condExpression.begin() + opPos + 2,
                                    condExpression.end() - 1);
      CondExpressionParser condExpParser2(subConExp2, lineNo);
      condExpParser2.parse();
      std::unordered_set<Token> tmp3 = condExpParser2.getUsedVar();
      usedVariables.insert(tmp3.begin(), tmp3.end());
      std::unordered_set<Token> tmp4 = condExpParser2.getUsedConstants();
      usedConstants.insert(tmp4.begin(), tmp4.end());
    }
  } else {
    RelExpressionParser relExpParser(condExpression, lineNo);
    relExpParser.parse();
    std::unordered_set<Token> tmp1 = relExpParser.getUsedVar();
    usedVariables.insert(tmp1.begin(), tmp1.end());
    std::unordered_set<Token> tmp2 = relExpParser.getUsedConstants();
    usedConstants.insert(tmp2.begin(), tmp2.end());
  }
}

// return a set of all used variables
std::unordered_set<Token> CondExpressionParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<Token> CondExpressionParser::getUsedConstants() const {
  return usedConstants;
}

// check if a token contains comparison operator as tokenenum
bool RelExpressionParser::isComparisonOp(Token t) const {
  switch (t.getTokenEnum()) {
  case TokenEnum::EQUAL:
  case TokenEnum::NOT_EQ:
  case TokenEnum::GREATER:
  case TokenEnum::LESS:
  case TokenEnum::GREATER_EQ:
  case TokenEnum::LESS_EQ:
    return true;
  default:
    return false;
  }
}

// look for comparison operator and return  position
bool RelExpressionParser::isValidFormat() const {
  int comparisonOpNum = 0;
  int unpaired_paren = 0;
  for (int i = relExpression.size() - 1; i >= 0; i--) {
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
    return false; // invalid format
  }
  return true;
}

int RelExpressionParser::opPosition() const {
  int unpaired_paren = 0;
  for (int i = relExpression.size() - 1; i >= 0; i--) {
    if (relExpression[i].getTokenEnum() == TokenEnum::CLOSE_P) {
      unpaired_paren++;
    } else if (relExpression[i].getTokenEnum() == TokenEnum::OPEN_P) {
      unpaired_paren--;
    }
    if (unpaired_paren == 0 && isComparisonOp(relExpression[i])) {
      return i;
    }
  }
  return -1;
}

// Constructor
RelExpressionParser::RelExpressionParser(std::vector<Token> rel, int line)
    : relExpression(rel), lineNo(line) {
  if (!isValidFormat()) {
    throw InvalidConditionException(lineNo, relExpression);
  }
}

// main function that parses the factor
void RelExpressionParser::parse() {
  int opPos = opPosition();
  if (opPos == -1) {
    throw InvalidConditionException(lineNo, relExpression);
  }
  std::vector<Token> relFactor1(relExpression.begin(),
                                relExpression.begin() + opPos);
  RelFactorParser relFactorParser1(relFactor1, lineNo);
  relFactorParser1.parse();
  std::unordered_set<Token> tmp1 = relFactorParser1.getUsedVar();
  usedVariables.insert(tmp1.begin(), tmp1.end());
  std::unordered_set<Token> tmp2 = relFactorParser1.getUsedConstants();
  usedConstants.insert(tmp2.begin(), tmp2.end());

  std::vector<Token> relFactor2(relExpression.begin() + opPos + 1,
                                relExpression.end());
  RelFactorParser relFactorParser2(relFactor2, lineNo);
  relFactorParser2.parse();
  std::unordered_set<Token> tmp3 = relFactorParser2.getUsedVar();
  usedVariables.insert(tmp3.begin(), tmp3.end());
  std::unordered_set<Token> tmp4 = relFactorParser2.getUsedConstants();
  usedConstants.insert(tmp4.begin(), tmp4.end());
}

// return used variables
std::unordered_set<Token> RelExpressionParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<Token> RelExpressionParser::getUsedConstants() const {
  return usedConstants;
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
    } else if (relFactor[0].getTokenEnum() == TokenEnum::CONSTANT) {
      usedConstants.insert(relFactor[0]);
    } else {
      throw InvalidConditionException(lineNo, relFactor);
    }
  } else {
    std::vector<Token> exp(relFactor.begin() + 1, relFactor.end() - 1);
    ExprParserWrapper expParser(exp, lineNo, &TNode());
    expParser.parse();
    std::unordered_set<Token> tmp1 = expParser.getUsedVar();
    usedVariables.insert(tmp1.begin(), tmp1.end());
    std::unordered_set<Token> tmp2 = expParser.getUsedConstants();
    usedConstants.insert(tmp2.begin(), tmp2.end());
  }
}

// return used variables
std::unordered_set<Token> RelFactorParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<Token> RelFactorParser::getUsedConstants() const {
  return usedConstants;
}
