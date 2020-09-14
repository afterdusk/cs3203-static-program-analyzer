#pragma once
#include "TNode.h"
#include "Token.h"
#include <string>
#include <unordered_set>
#include <vector>

class ExprParserWrapper {
private:
  // Pointer to root node of AST of assignment pattern
  TNode *rootNode;

  // expression to be parsed
  std::vector<Token> expression;

  // line number
  int lineNo;

  // all used variables in the assignment
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

  // check if any invalid parenthesis
  int invalidParenthesis() const;

  // check if any invalid TokenEnumValue
  bool hasInvalidTokenEnum() const;

public:
  // constructor
  ExprParserWrapper(std::vector<Token> expr, int line, TNode *root);

  // main functionn that parses the expression
  void parse();

  // return list of variable used in the expression
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;

  // return root node of the AST
  const TNode *getRootNodePtr() const;
};

// Responsible for parsing expressions
// Extract out +/- signs and sperate the expression into sub-part and continue
// parsing
class ExpressionParser {

private:
  // expression tokens to be parsed.
  std::vector<Token> expression;

  // line no.
  int lineNo;

  // Parent node
  TNode *currNode;

  // Store a vector of variable used.
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

  // check for +/- sign outside brackets and return the position of the last one
  int checkPlusMinus() const;

public:
  // Constructor
  ExpressionParser(std::vector<Token> exp, int line, TNode *node);

  // main function that parses the expression
  void parseExpression();

  // return used variables
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;
};

// Responsible for parsing terms
// Extract out * or / or % signs and sperate the expression into sub-part and
// continue parsing
class TermParser {
private:
  // Term string to be parsed
  std::vector<Token> term;

  // line no.
  int lineNo;

  // Parent node
  TNode *currNode;

  // Store a vector of variable used.
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

  // check for * or / or % sign and return the position of the last one
  int checkMulDivMod() const;

public:
  // Constructor
  TermParser(std::vector<Token> t, int line, TNode *node);

  // main function that parses the term
  void parseTerm();

  // return used variables
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;
};

// Responsible for parsing factors (i.e. constant or variable or expressions
// enclosed in brackets)
class FactorParser {
private:
  // Factor string to be parsed
  std::vector<Token> factor;

  // line no.
  int lineNo;

  // Parent node
  TNode *currNode;

  // Store a vector of variable used.
  std::unordered_set<Token> usedVariables;

  // all used constants
  std::unordered_set<Token> usedConstants;
  ;

  // check for brackets
  bool hasParenthesis() const;

public:
  // Constructor
  FactorParser(std::vector<Token> f, int line, TNode *node);

  // main function that parses the factor
  void parseFactor();

  // return used variables
  std::unordered_set<Token> getUsedVar() const;

  // return used constants
  std::unordered_set<Token> getUsedConstants() const;
};
