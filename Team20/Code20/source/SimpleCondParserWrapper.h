#pragma once
#include "PkbTables.h"
#include "SimpleToken.h"
#include <unordered_set>
#include <vector>

// Wrapper class that handles parsing of a full condition
class SimpleCondParserWrapper {
private:
  // condition expression to be parsed
  std::vector<SimpleToken> condition;

  // line number
  PkbTables::LINE_NO lineNo;

  // used variables
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

  // check if any invalid parenthesis
  int invalidParenthesis() const;

  // check if any invalid TokenEnumValue
  bool hasInvalidTokenEnum() const;

public:
  // Constructor
  SimpleCondParserWrapper(std::vector<SimpleToken> cond,
                          PkbTables::LINE_NO line);

  // main function that parses the condition
  void parse();

  // return a set of all used variables
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;
};

class CondExpressionParser {
private:
  // condition expression to be parsed
  std::vector<SimpleToken> condExpression;

  // line number
  PkbTables::LINE_NO lineNo;

  // used variables
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

  // check if format of condition is correct
  bool isValidFormat() const;

  // check for !, && and ||, return the position
  int opPosition() const;

public:
  // constructir
  CondExpressionParser(std::vector<SimpleToken> cond, PkbTables::LINE_NO line);

  // main parser function
  void parse();

  // return a set of all used variables
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;
};

class RelExpressionParser {
private:
  // rel expression to be parsed
  std::vector<SimpleToken> relExpression;

  // line number
  PkbTables::LINE_NO lineNo;

  // used variables
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

  // check if a token contains comparison operator as tokenenum
  bool isComparisonOp(SimpleToken t) const;

  // check if the format is correct
  bool isValidFormat() const;

  // return position of operator
  int opPosition() const;

public:
  // constructor
  RelExpressionParser(std::vector<SimpleToken> rel, PkbTables::LINE_NO line);

  // main parser function
  void parse();

  // return a set of all used variables
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;
};

class RelFactorParser {
private:
  // Rel factor to be parsed
  std::vector<SimpleToken> relFactor;

  // line no.
  PkbTables::LINE_NO lineNo;

  // Store a vector of variable used.
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

public:
  // Constructor
  RelFactorParser(std::vector<SimpleToken> rel, PkbTables::LINE_NO line);

  // main function that parses the factor
  void parse();

  // return used variables
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;
};
