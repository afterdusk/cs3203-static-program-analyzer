#pragma once
#include <string>
#include <vector>

#include "PQL.h"

class PQLParser {
public:
  ParsedQuery parse();
  PQLParser(std::vector<PqlToken> &);

private:
  std::vector<PqlToken>::iterator it;
  std::vector<PqlToken>::iterator end;
  ParsedQuery pq;
  TokenType getTokenDeclarationTypeInArgumentsList(
      PqlToken &token, std::unordered_set<TokenType> &argumentsList);
  void parseRelationship();
  void parseEndOfStatement();
  void parseDeclaration();
  TokenType getDeclaration(PqlToken &token);

  void parseSuchThat();
  void parsePattern();
  PqlToken getParsedLHSOfPattern();
  PatternSpec getParsedRHSOfPattern();
  void parseClausesFromSelectOnwards();
  PqlToken getNextToken();
  PqlToken getNextExpectedToken(TokenType expectedTokenType);
};
