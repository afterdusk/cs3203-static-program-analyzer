#pragma once
#include "Token.h"
#include <unordered_set>
#include <vector>
class CondParserWrapper {
private:
  // condition expression to be parsed
  std::vector<Token> condition;

  // line number
  int lineNo;

  // used variables
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

  // check if any invalid parenthesis
  int invalidParenthesis() const;

  // check if any invalid TokenEnumValue
  bool hasInvalidTokenEnum() const;

public:
  // Constructor
  CondParserWrapper(std::vector<Token> cond, int line);

  // main function that parses the condition
  void parse();

  // return a set of all used variables
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;
};

class CondExpressionParser {
private:
  // condition expression to be parsed
  std::vector<Token> condExpression;

  // line number
  int lineNo;

  // used variables
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

  // check if format of condition is correct
  bool isValidFormat() const;

  // check for !, && and ||, return the position
  int opPosition() const;

public:
  // constructir
  CondExpressionParser(std::vector<Token> cond, int line);

  // main parser function
  void parse();

  // return a set of all used variables
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;
};

class RelExpressionParser {
private:
  // rel expression to be parsed
  std::vector<Token> relExpression;

  // line number
  int lineNo;

  // used variables
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

  // check if a token contains comparison operator as tokenenum
  bool isComparisonOp(Token t) const;

  // check if the format is correct
  bool isValidFormat() const;

  // return position of operator
  int opPosition() const;

public:
  // constructor
  RelExpressionParser(std::vector<Token> rel, int line);

  // main parser function
  void parse();

  // return a set of all used variables
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;
};

class RelFactorParser {
private:
  // Rel factor to be parsed
  std::vector<Token> relFactor;

  // line no.
  int lineNo;

  // Store a vector of variable used.
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

public:
  // Constructor
  RelFactorParser(std::vector<Token> rel, int line);

  // main function that parses the factor
  void parse();

  // return used variables
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;
};
