#pragma once
#include "PkbTables.h"
#include "SimpleToken.h"
#include "TNode.h"
#include <string>
#include <unordered_set>
#include <vector>

// Wrapper class that handles parsing of a full expression
class SimpleExprParserWrapper {
private:
  // Pointer to root node of AST of assignment pattern
  std::unique_ptr<TNode> rootNode;

  // expression to be parsed
  std::vector<SimpleToken> expression;

  // line number
  PkbTables::LINE_NO lineNo;

  // all used variables in the assignment
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

  // check if any invalid parenthesis
  int invalidParenthesis() const;

  // check if any invalid TokenEnumValue
  bool hasInvalidTokenType() const;

public:
  // constructor
  SimpleExprParserWrapper(std::vector<SimpleToken> expr,
                          PkbTables::LINE_NO line);

  // main functionn that parses the expression
  void parse();

  // return list of variable used in the expression
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;

  // return root node of the AST
  std::unique_ptr<TNode> getRootNodePtr();
};

// Responsible for parsing expressions
// Extract out +/- signs and sperate the expression into sub-part and continue
// parsing
class ExpressionParser {

private:
  // expression tokens to be parsed.
  std::vector<SimpleToken> expression;

  // line no.
  PkbTables::LINE_NO lineNo;

  // Parent node
  std::unique_ptr<TNode> currNode;

  // Store a vector of variable used.
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

  // check for +/- sign outside brackets and return the position of the last one
  int checkPlusMinus() const;

public:
  // Constructor
  ExpressionParser(std::vector<SimpleToken> exp, PkbTables::LINE_NO line,
                   std::unique_ptr<TNode> &node);

  // main function that parses the expression
  void parseExpression();

  // return used variables
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;

  // return current node pointer
  std::unique_ptr<TNode> getNodePtr();
};

// Responsible for parsing terms
// Extract out * or / or % signs and sperate the expression into sub-part and
// continue parsing
class TermParser {
private:
  // Term string to be parsed
  std::vector<SimpleToken> term;

  // line no.
  PkbTables::LINE_NO lineNo;

  // Parent node
  std::unique_ptr<TNode> currNode;

  // Store a vector of variable used.
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

  // check for * or / or % sign and return the position of the last one
  int checkMulDivMod() const;

public:
  // Constructor
  TermParser(std::vector<SimpleToken> t, PkbTables::LINE_NO line,
             std::unique_ptr<TNode> &node);

  // main function that parses the term
  void parseTerm();

  // return used variables
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;

  // return current node pointer
  std::unique_ptr<TNode> getNodePtr();
};

// Responsible for parsing factors (i.e. constant or variable or expressions
// enclosed in brackets)
class FactorParser {
private:
  // Factor string to be parsed
  std::vector<SimpleToken> factor;

  // line no.
  PkbTables::LINE_NO lineNo;

  // Parent node
  std::unique_ptr<TNode> currNode;

  // Store a vector of variable used.
  std::unordered_set<SimpleToken> usedVariables;

  // all used constants
  std::unordered_set<SimpleToken> usedConstants;

  // check for brackets
  bool hasParenthesis() const;

public:
  // Constructor
  FactorParser(std::vector<SimpleToken> f, PkbTables::LINE_NO line,
               std::unique_ptr<TNode> &node);

  // main function that parses the factor
  void parseFactor();

  // return used variables
  std::unordered_set<SimpleToken> getUsedVar() const;

  // return used constants
  std::unordered_set<SimpleToken> getUsedConstants() const;

  // return current node pointer
  std::unique_ptr<TNode> getNodePtr();
};
