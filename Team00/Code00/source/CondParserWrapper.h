#pragma once
#include "Token.h"
#include <set>
#include <vector>
class CondParserWrapper {
private:
  // condition expression to be parsed
  std::vector<Token> condition;

  // line number
  int lineNo;

  // used variables
  std::set<Token> usedVariables;

public:
  // Constructor
  CondParserWrapper(std::vector<Token> cond, int line);

  // main function that parses the condition
  void parse();

  // return a set of all used variables
  std::set<Token> getUsedVar() const;

  // check if any invalid parenthesis
  int invalidParenthesis() const;

  // check if any invalid TokenEnumValue
  bool hasInvalidTokenEnum() const;
};

class CondExpressionParser {
private:
  // condition expression to be parsed
  std::vector<Token> condExpression;

  // line number
  int lineNo;

  // used variables
  std::set<Token> usedVariables;

  // check if format of condition is correct
  bool isValidFormat() const;

  // check for !, && and ||, return the position
  int checkOperator() const;

public:
  // constructir
  CondExpressionParser(std::vector<Token> cond, int line);

  // main parser function
  void parse();

  // return a set of all used variables
  std::set<Token> getUsedVar() const;
};

class RelExpressionParser {
private:
  // rel expression to be parsed
  std::vector<Token> relExpression;

  // line number
  int lineNo;

  // used variables
  std::set<Token> usedVariables;

  // check if a token contains comparison operator as tokenenum
  bool isComparisonOp(Token t) const;

  // look for comparison operator and return  position
  int opPosition() const;

public:
  // constructor
  RelExpressionParser(std::vector<Token> rel, int line);

  // main parser function
  void parse();

  // return a set of all used variables
  std::set<Token> getUsedVar() const;
};

class RelFactorParser {
private:
  // Rel factor to be parsed
  std::vector<Token> relFactor;

  // line no.
  int lineNo;

  // Store a vector of variable used.
  std::set<Token> usedVariables;

public:
  // Constructor
  RelFactorParser(std::vector<Token> rel, int line);

  // main function that parses the factor
  void parse();

  // return used variables
  std::set<Token> getUsedVar() const;
};
