#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQL.h"
#include "PQLParser.h"
#include "SimpleExprParserWrapper.h"
#include "SimpleTokenizer.h"

std::unordered_set<TokenType> entities = {
    TokenType::STMT,      TokenType::READ,     TokenType::PRINT,
    TokenType::CALL,      TokenType::WHILE,    TokenType::IF,
    TokenType::ASSIGN,    TokenType::VARIABLE, TokenType::CONSTANT,
    TokenType::PROCEDURE,
};

std::unordered_set<TokenType> abstractions = {
    TokenType::PARENT,    TokenType::PARENT_T, TokenType::FOLLOWS,
    TokenType::FOLLOWS_T, TokenType::MODIFIES, TokenType::USES,

    TokenType::MATCH,
};

std::unordered_map<TokenType, std::vector<std::unordered_set<TokenType>>>
    relationships = {
        {TokenType::FOLLOWS,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
         }},
        {TokenType::FOLLOWS_T,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
         }},
        {TokenType::PARENT,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
         }},
        {TokenType::PARENT_T,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE,
              TokenType::NUMBER},
         }},
        {TokenType::MODIFIES,
         {
             {TokenType::STMT, TokenType::ASSIGN, TokenType::READ,
              TokenType::WHILE, TokenType::IF, TokenType::CALL,
              TokenType::PROCEDURE, TokenType::NUMBER, TokenType::STRING},
             {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
         }},
        {TokenType::USES,
         {
             {TokenType::STMT, TokenType::ASSIGN, TokenType::PRINT,
              TokenType::WHILE, TokenType::IF, TokenType::CALL,
              TokenType::PROCEDURE, TokenType::NUMBER, TokenType::STRING,
              TokenType::NUMBER},
             {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
         }},
};

template <class T> bool contains(std::unordered_set<T> set, T item) {
  return set.find(item) != set.end();
}

PqlToken PQLParser::getNextToken() {
  if (it == end) {
    throw "Error: no more tokens";
  }
  const auto nextValue = *it;
  it++;
  return nextValue;
}

PqlToken PQLParser::getNextExpectedToken(TokenType expectedTokenType) {
  const PqlToken token = getNextToken();
  if (token.type != expectedTokenType) {
    if (expectedTokenType == TokenType::SYNONYM &&
        contains<TokenType>(entities, token.type)) {
      for (auto it = stringTokenMap.begin(); it != stringTokenMap.end(); ++it)
        if (it->second == token.type)
          return PqlToken{TokenType::SYNONYM, it->first};
    }
    throw "Retrieved token is not the expected token type";
  }
  return token;
}

void PQLParser::parseEndOfStatement() {
  getNextExpectedToken(TokenType::SEMICOLON);
}

PQLParser::PQLParser(std::vector<PqlToken> &tokens) {
  it = tokens.begin();
  end = tokens.end();
  pq = ParsedQuery();
}

void PQLParser::parseDeclaration() {

  const auto token = getNextToken();
  const auto entityIdentifier = token.type;
  if (!contains(entities, entityIdentifier)) {
    throw "Entity identifier not found";
  }

  const auto nextToken = getNextExpectedToken(TokenType::SYNONYM);
  pq.declarations[nextToken.value] = entityIdentifier;
  while (it != end && it->type == TokenType::COMMA) {
    getNextExpectedToken(TokenType::COMMA);
    const auto nextToken = getNextExpectedToken(TokenType::SYNONYM);
    pq.declarations[nextToken.value] = entityIdentifier;
  }
  parseEndOfStatement();
}

TokenType PQLParser::getDeclaration(PqlToken &token) {
  if (token.type != TokenType::SYNONYM) {
    return token.type;
  }
  if (pq.declarations.find(token.value) == pq.declarations.end()) {
    throw "ERROR: token not found in declaration clauses";
  }
  return pq.declarations[token.value];
}
TokenType PQLParser::getTokenDeclarationTypeInArgumentsList(
    PqlToken &token, std::unordered_set<TokenType> &argumentsList) {
  const TokenType declaredType = getDeclaration(token);
  if (!contains(argumentsList, declaredType)) {
    throw "Token for relationship does not match.";
  }
  return declaredType;
}

void PQLParser::parseRelationship() {
  const auto token = getNextToken();
  const auto relationshipIdentifier = token.type;
  const auto validArgumentsLists = relationships[relationshipIdentifier];
  const int argumentsCount = validArgumentsLists.size();
  if (relationships.find(relationshipIdentifier) == relationships.end()) {
    throw "Relationship not found";
  }
  getNextExpectedToken(TokenType::OPEN_PARENTHESIS);
  std::vector<PqlToken> relationshipArgs;
  for (int i = 0; i < argumentsCount; i++) {
    auto nextToken = getNextToken();
    auto validArgumentsList = validArgumentsLists[i];
    auto tokenType =
        getTokenDeclarationTypeInArgumentsList(nextToken, validArgumentsList);
    nextToken.type = tokenType;
    relationshipArgs.push_back(nextToken);

    if (i != argumentsCount - 1) {
      getNextExpectedToken(TokenType::COMMA);
    }
  }
  getNextExpectedToken(TokenType::CLOSED_PARENTHESIS);

  if (relationshipArgs.size() == 2) {
    pq.relationships.push_back(ParsedRelationship{
        relationshipIdentifier, relationshipArgs[0], relationshipArgs[1]});
  } else {
    throw "Incorrect number of arguments";
  }
}

void PQLParser::parseSuchThat() {
  // TODO: Handle "AND"
  getNextExpectedToken(TokenType::SUCH);
  getNextExpectedToken(TokenType::THAT);

  parseRelationship();
}

const std::unordered_set<TokenType> allowedPatternTypes = {
    TokenType::ASSIGN, TokenType::WHILE, TokenType::IF};

PqlToken PQLParser::getParsedLHSOfPattern() {
  if (it == end) {
    throw "ERROR: End of tokens when retrieving LHS of pattern";
  }
  switch (it->type) {
  case TokenType::UNDERSCORE:
    return getNextExpectedToken(TokenType::UNDERSCORE);
  case TokenType::STRING:
    return getNextExpectedToken(TokenType::STRING);
  default:
    auto synonymToken = getNextExpectedToken(TokenType::SYNONYM);
    const auto declarationType = getDeclaration(synonymToken);
    if (declarationType != TokenType::VARIABLE) {
      throw "ERROR: There should only be synonyms of variable type here";
    }
    synonymToken.type = declarationType;
    return synonymToken;
  }
}

PatternSpec PQLParser::getParsedRHSOfPattern() {
  switch (it->type) {
  case TokenType::UNDERSCORE:
    getNextExpectedToken(TokenType::UNDERSCORE);
    switch (it->type) {
    case TokenType::STRING: {
      const auto nextToken = getNextExpectedToken(TokenType::STRING);
      PatternSpec result =
          PatternSpec{PatternMatchType::SubTreeMatch, nextToken.value};
      getNextExpectedToken(TokenType::UNDERSCORE);
      return result;
      break;
    }
    case TokenType::CLOSED_PARENTHESIS: {
      return PatternSpec{PatternMatchType::Any};
      break;
    }
    default:
      throw "ERROR: Unrecognised token after underscore";
      break;
    }
  case TokenType::STRING: {
    const auto nextToken = getNextExpectedToken(TokenType::STRING);
    return PatternSpec{PatternMatchType::CompleteMatch, nextToken.value};
    break;
  }
  default:
    throw "ERROR: Unrecognised token after parsing pattern token";
    break;
  }
}
void PQLParser::parsePattern() {

  getNextExpectedToken(TokenType::PATTERN);
  auto synonymToken = getNextExpectedToken(TokenType::SYNONYM);
  const auto declarationType = getDeclaration(synonymToken);
  if (!contains(allowedPatternTypes, declarationType)) {
    throw "ERROR: not from allowed pattern types";
  }
  synonymToken.type = declarationType;

  // TODO: Handle syn-if: it accepts 3 parameters instead of 2
  getNextExpectedToken(TokenType::OPEN_PARENTHESIS);
  PqlToken lhs = getParsedLHSOfPattern();
  getNextExpectedToken(TokenType::COMMA);
  PatternSpec rhs = getParsedRHSOfPattern();
  pq.patterns.push_back(ParsedPattern{synonymToken, lhs, rhs});
  getNextExpectedToken(TokenType::CLOSED_PARENTHESIS);
}

void PQLParser::parseClausesFromSelectOnwards() {
  const auto token = getNextExpectedToken(TokenType::SELECT);

  // TODO: Handle multiple returns
  const auto nextToken = getNextExpectedToken(TokenType::SYNONYM);

  if (pq.declarations.find(nextToken.value) == pq.declarations.end()) {
    throw "ERROR: Result not in declaration clause";
  }
  pq.results.push_back(nextToken.value);

  while (it != end) {
    switch (it->type) {
    case TokenType::SUCH:
      parseSuchThat();
      break;
    case TokenType::PATTERN:
      parsePattern();
      break;
    default:
      throw "EXPECTED SUCH THAT OR PATTERN";
      break;
    }
  }
}

ParsedQuery PQLParser::parse() {
  while (it != end && contains<TokenType>(entities, it->type)) {
    parseDeclaration();
  }
  parseClausesFromSelectOnwards();
  return pq;
}
