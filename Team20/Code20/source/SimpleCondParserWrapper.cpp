
#include "SimpleCondParserWrapper.h"
#include "SimpleExprParserWrapper.h"
#include "SimpleParseExceptions.h"

// SimpleCondParserWrapper
// check if parenthesis are valid.
int SimpleCondParserWrapper::invalidParenthesis() const {
  int unpairedParen = 0;
  size_t i;
  for (i = 0; i < condition.size(); i++) {
    if (condition[i].getTokenType() == SimpleToken::TokenType::OPEN_P) {
      unpairedParen++;
    } else if (condition[i].getTokenType() == SimpleToken::TokenType::CLOSE_P) {
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
bool SimpleCondParserWrapper::hasInvalidTokenEnum() const {
  for (size_t i = 0; i < condition.size(); i++) {
    switch (condition[i].getTokenType()) {
    case SimpleToken::TokenType::SEMI_COLON:
    case SimpleToken::TokenType::OPEN_B:
    case SimpleToken::TokenType::CLOSE_B:
    case SimpleToken::TokenType::ASSIGN:
    case SimpleToken::TokenType::OTHER:
      return true;
    }
  }
  return false;
}

// SimpleCondParserWrapper constructor
SimpleCondParserWrapper::SimpleCondParserWrapper(std::vector<SimpleToken> cond,
                                                 PkbTables::LINE_NO line)
    : condition(cond), lineNo(line) {
  if (hasInvalidTokenEnum() || invalidParenthesis() != 0) {
    throw InvalidConditionException(lineNo, condition);
  }
}

// main functionn that parses the expression
void SimpleCondParserWrapper::parse() {
  try {
    CondExpressionParser condExpParser(condition, lineNo);
    condExpParser.parse();
    std::unordered_set<SimpleToken> tmp1 = condExpParser.getUsedVar();
    usedVariables.insert(tmp1.begin(), tmp1.end());
    std::unordered_set<SimpleToken> tmp2 = condExpParser.getUsedConstants();
    usedConstants.insert(tmp2.begin(), tmp2.end());
  } catch (InvalidConditionException &i) {
    ignore(i);
    throw InvalidConditionException(lineNo, condition);
  }
}

// Getter function for used variables
std::unordered_set<SimpleToken> SimpleCondParserWrapper::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<SimpleToken>
SimpleCondParserWrapper::getUsedConstants() const {
  return usedConstants;
}

// CondExpressionParser

// check if format of condition is correct
bool CondExpressionParser::isValidFormat() const {
  int numOp = 0;
  if (condExpression[0].getTokenType() == SimpleToken::TokenType::NOT) {
    numOp++;
  }
  int unpairedParen = 0;
  for (int i = condExpression.size() - 1; i >= 0; i--) {
    if (condExpression[i].getTokenType() == SimpleToken::TokenType::CLOSE_P) {
      unpairedParen++;
    } else if (condExpression[i].getTokenType() ==
               SimpleToken::TokenType::OPEN_P) {
      unpairedParen--;
    }
    if (unpairedParen == 0 &&
        (condExpression[i].getTokenType() == SimpleToken::TokenType::AND ||
         condExpression[i].getTokenType() == SimpleToken::TokenType::OR)) {
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
  if (condExpression[0].getTokenType() == SimpleToken::TokenType::NOT) {
    return 0;
  }
  int unpairedParen = 0;
  for (int i = condExpression.size() - 1; i >= 0; i--) {
    if (condExpression[i].getTokenType() == SimpleToken::TokenType::CLOSE_P) {
      unpairedParen++;
    } else if (condExpression[i].getTokenType() ==
               SimpleToken::TokenType::OPEN_P) {
      unpairedParen--;
    }
    if (unpairedParen == 0 &&
        (condExpression[i].getTokenType() == SimpleToken::TokenType::AND ||
         condExpression[i].getTokenType() == SimpleToken::TokenType::OR)) {
      return i;
    }
  }
  return -1;
}

// constructor
CondExpressionParser::CondExpressionParser(std::vector<SimpleToken> cond,
                                           PkbTables::LINE_NO line)
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
      if (condExpression[0].getTokenType() != SimpleToken::TokenType::NOT) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      if (!(condExpression[1].getTokenType() ==
                SimpleToken::TokenType::OPEN_P &&
            condExpression[condExpression.size() - 1].getTokenType() ==
                SimpleToken::TokenType::CLOSE_P)) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      try {
        std::vector<SimpleToken> subConExp(condExpression.begin() + 2,
                                           condExpression.end() - 1);
        CondExpressionParser condExpParser(subConExp, lineNo);
        condExpParser.parse();
        std::unordered_set<SimpleToken> tmp1 = condExpParser.getUsedVar();
        usedVariables.insert(tmp1.begin(), tmp1.end());
        std::unordered_set<SimpleToken> tmp2 = condExpParser.getUsedConstants();
        usedConstants.insert(tmp2.begin(), tmp2.end());
      } catch (InvalidConditionException &i) {
        ignore(i);
        throw;
      }
    } else {
      // operator is && or ||
      if (condExpression[opPos].getTokenType() != SimpleToken::TokenType::AND &&
          condExpression[opPos].getTokenType() != SimpleToken::TokenType::OR) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      if (opPos <= 2 || opPos >= (int)condExpression.size() - 3) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      if (!(condExpression[0].getTokenType() ==
                SimpleToken::TokenType::OPEN_P &&
            condExpression[opPos - 1].getTokenType() ==
                SimpleToken::TokenType::CLOSE_P &&
            condExpression[opPos + 1].getTokenType() ==
                SimpleToken::TokenType::OPEN_P &&
            condExpression[condExpression.size() - 1].getTokenType() ==
                SimpleToken::TokenType::CLOSE_P)) {
        throw InvalidConditionException(lineNo, condExpression);
      }
      try {
        std::vector<SimpleToken> subConExp1(condExpression.begin() + 1,
                                            condExpression.begin() + opPos - 1);
        CondExpressionParser condExpParser1(subConExp1, lineNo);
        condExpParser1.parse();
        std::unordered_set<SimpleToken> tmp1 = condExpParser1.getUsedVar();
        usedVariables.insert(tmp1.begin(), tmp1.end());
        std::unordered_set<SimpleToken> tmp2 =
            condExpParser1.getUsedConstants();
        usedConstants.insert(tmp2.begin(), tmp2.end());

        std::vector<SimpleToken> subConExp2(condExpression.begin() + opPos + 2,
                                            condExpression.end() - 1);
        CondExpressionParser condExpParser2(subConExp2, lineNo);
        condExpParser2.parse();
        std::unordered_set<SimpleToken> tmp3 = condExpParser2.getUsedVar();
        usedVariables.insert(tmp3.begin(), tmp3.end());
        std::unordered_set<SimpleToken> tmp4 =
            condExpParser2.getUsedConstants();
        usedConstants.insert(tmp4.begin(), tmp4.end());
      } catch (InvalidConditionException i) {
        ignore(i);
        throw;
      }
    }
  } else {
    try {
      RelExpressionParser relExpParser(condExpression, lineNo);
      relExpParser.parse();
      std::unordered_set<SimpleToken> tmp1 = relExpParser.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<SimpleToken> tmp2 = relExpParser.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidConditionException &i) {
      ignore(i);
      throw;
    }
  }
}

// return a set of all used variables
std::unordered_set<SimpleToken> CondExpressionParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<SimpleToken> CondExpressionParser::getUsedConstants() const {
  return usedConstants;
}

// check if a token contains comparison operator as tokenenum
bool RelExpressionParser::isComparisonOp(SimpleToken t) const {
  switch (t.getTokenType()) {
  case SimpleToken::TokenType::EQUAL:
  case SimpleToken::TokenType::NOT_EQ:
  case SimpleToken::TokenType::GREATER:
  case SimpleToken::TokenType::LESS:
  case SimpleToken::TokenType::GREATER_EQ:
  case SimpleToken::TokenType::LESS_EQ:
    return true;
  default:
    return false;
  }
}

// look for comparison operator and return  position
bool RelExpressionParser::isValidFormat() const {
  int comparisonOpNum = 0;
  for (int i = relExpression.size() - 1; i >= 0; i--) {
    if (isComparisonOp(relExpression[i])) {
      comparisonOpNum++;
    }
  }
  if (comparisonOpNum != 1) {
    return false; // invalid format
  }
  return true;
}

int RelExpressionParser::opPosition() const {
  int unpairedParen = 0;
  for (int i = relExpression.size() - 1; i >= 0; i--) {
    if (relExpression[i].getTokenType() == SimpleToken::TokenType::CLOSE_P) {
      unpairedParen++;
    } else if (relExpression[i].getTokenType() ==
               SimpleToken::TokenType::OPEN_P) {
      unpairedParen--;
    }
    if (unpairedParen == 0 && isComparisonOp(relExpression[i])) {
      return i;
    }
  }
  return -1;
}

// Constructor
RelExpressionParser::RelExpressionParser(std::vector<SimpleToken> rel,
                                         PkbTables::LINE_NO line)
    : relExpression(rel), lineNo(line) {
  if (!isValidFormat()) {
    throw InvalidConditionException(lineNo, relExpression);
  }
}

// main function that parses the factor
void RelExpressionParser::parse() {
  int opPos = opPosition();
  if (opPos == -1 || opPos == 0 || opPos == relExpression.size() - 1) {
    throw InvalidConditionException(lineNo, relExpression);
  }
  try {
    std::vector<SimpleToken> relFactor1(relExpression.begin(),
                                        relExpression.begin() + opPos);
    RelFactorParser relFactorParser1(relFactor1, lineNo);
    relFactorParser1.parse();
    std::unordered_set<SimpleToken> tmp1 = relFactorParser1.getUsedVar();
    usedVariables.insert(tmp1.begin(), tmp1.end());
    std::unordered_set<SimpleToken> tmp2 = relFactorParser1.getUsedConstants();
    usedConstants.insert(tmp2.begin(), tmp2.end());

    std::vector<SimpleToken> relFactor2(relExpression.begin() + opPos + 1,
                                        relExpression.end());
    RelFactorParser relFactorParser2(relFactor2, lineNo);
    relFactorParser2.parse();
    std::unordered_set<SimpleToken> tmp3 = relFactorParser2.getUsedVar();
    usedVariables.insert(tmp3.begin(), tmp3.end());
    std::unordered_set<SimpleToken> tmp4 = relFactorParser2.getUsedConstants();
    usedConstants.insert(tmp4.begin(), tmp4.end());
  } catch (InvalidConditionException &i) {
    ignore(i);
    throw;
  }
}

// return used variables
std::unordered_set<SimpleToken> RelExpressionParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<SimpleToken> RelExpressionParser::getUsedConstants() const {
  return usedConstants;
}

// RelFactorParser
// Constructor
RelFactorParser::RelFactorParser(std::vector<SimpleToken> rel,
                                 PkbTables::LINE_NO line)
    : relFactor(rel), lineNo(line) {}

void RelFactorParser::parse() {
  // constant or var
  if (relFactor.size() == 1) {
    if (relFactor[0].getTokenType() == SimpleToken::TokenType::WORD) {
      usedVariables.insert(relFactor[0]);
    } else if (relFactor[0].getTokenType() ==
               SimpleToken::TokenType::CONSTANT) {
      usedConstants.insert(relFactor[0]);
    } else {
      throw InvalidConditionException(lineNo, relFactor);
    }
  } else {
    // expression
    try {
      std::vector<SimpleToken> exp(relFactor.begin(), relFactor.end());
      SimpleExprParserWrapper expParserWrapper(exp, lineNo);
      expParserWrapper.parse();
      std::unordered_set<SimpleToken> tmp1 = expParserWrapper.getUsedVar();
      usedVariables.insert(tmp1.begin(), tmp1.end());
      std::unordered_set<SimpleToken> tmp2 =
          expParserWrapper.getUsedConstants();
      usedConstants.insert(tmp2.begin(), tmp2.end());
    } catch (InvalidExpressionException &i) {
      ignore(i);
      throw InvalidConditionException(lineNo, relFactor);
    }
  }
}

// return used variables
std::unordered_set<SimpleToken> RelFactorParser::getUsedVar() const {
  return usedVariables;
}

// Getter function for used constants
std::unordered_set<SimpleToken> RelFactorParser::getUsedConstants() const {
  return usedConstants;
}
