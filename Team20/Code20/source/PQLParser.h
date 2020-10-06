#pragma once
#include <string>
#include <vector>

#include "PQL.h"

class PqlParser {
public:
  ParsedQuery parse();
  PqlParser(std::vector<PqlToken> &);

private:
  std::vector<PqlToken>::iterator it;
  std::vector<PqlToken>::iterator end;
  ParsedQuery pq;

  void parseSuchThatClause();
  void parsePatternClause();
  void parseResultClause();
  void parseWithClause();
  void parseElemInResult();
  void parseRelationship();
  void parseEndOfStatement();
  void parseDeclaration();
  void parsePattern();
  void parseAttributeCompare();
  void parseClausesFromSelectOnwards();

  PqlToken getParsedLHSOfPattern();
  PatternSpec getParsedRHSOfPattern();
  PqlToken getNextToken();
  PqlToken getNextExpectedToken(TokenType expectedTokenType);
  PqlToken getNextTokenWithDeclarationTypeInArgumentsList(
      std::unordered_set<TokenType> &argumentsList);
  Reference getRef();
  TokenType getDeclarationForSynonym(PqlToken &token);
  Element getElem();
};
