#pragma once
#include <string>
#include <vector>

#include "Pql.h"

class PqlParser {
public:
  ParsedQuery parse();
  PqlParser(std::vector<PqlToken> &);

private:
  std::vector<PqlToken>::iterator it;
  std::vector<PqlToken>::iterator end;
  ParsedQuery pq;
  void parseRelationship();
  void parseEndOfStatement();
  void parseDeclaration();
  TokenType getDeclarationForSynonym(PqlToken &token);

  PqlToken getElem();
  void parseElemInResult();

  void parseSuchThatClause();
  void parsePatternClause();
  void parseResultClause();
  void parseWithClause();
  void parsePattern();
  void parseAttributeCompare();

  PqlToken getParsedLHSOfPattern();
  PatternSpec getParsedRHSOfPattern();
  void parseClausesFromSelectOnwards();
  PqlToken getNextToken();
  PqlToken getNextExpectedToken(TokenType expectedTokenType);
  PqlToken getNextTokenWithDeclarationTypeInArgumentsList(
      std::unordered_set<TokenType> &argumentsList);
  void getRef();
};
