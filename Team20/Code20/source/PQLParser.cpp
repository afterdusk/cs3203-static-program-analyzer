#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Pql.h"
#include "PqlParser.h"
#include "SimpleExprParserWrapper.h"
#include "SimpleTokenizer.h"

std::unordered_set<TokenType> entities = {
    TokenType::STMT,     TokenType::PROG_LINE, TokenType::READ,
    TokenType::PRINT,    TokenType::CALL,      TokenType::WHILE,
    TokenType::IF,       TokenType::ASSIGN,    TokenType::VARIABLE,
    TokenType::CONSTANT, TokenType::PROCEDURE};

std::unordered_set<TokenType> attributeNames = {
    TokenType::PROCNAME, TokenType::VARNAME, TokenType::VALUE,
    TokenType::STATEMENT_NUM, TokenType::BOOLEAN};

std::unordered_set<TokenType> abstractions = {
    TokenType::PARENT,    TokenType::PARENT_T, TokenType::FOLLOWS,
    TokenType::FOLLOWS_T, TokenType::MODIFIES, TokenType::USES,

    TokenType::MATCH,
};

std::unordered_map<TokenType, std::vector<std::unordered_set<TokenType>>>
    relationships = {
        {TokenType::FOLLOWS,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::FOLLOWS_T,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::PARENT,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::PARENT_T,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::MODIFIES,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::ASSIGN,
              TokenType::READ, TokenType::WHILE, TokenType::IF, TokenType::CALL,
              TokenType::PROCEDURE, TokenType::NUMBER, TokenType::STRING},
             {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
         }},
        {TokenType::USES,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::ASSIGN,
              TokenType::PRINT, TokenType::WHILE, TokenType::IF,
              TokenType::CALL, TokenType::PROCEDURE, TokenType::NUMBER,
              TokenType::STRING, TokenType::NUMBER},
             {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
         }},
        {TokenType::CALL,
         {{TokenType::PROCEDURE, TokenType::STRING, TokenType::UNDERSCORE},
          {TokenType::PROCEDURE, TokenType::STRING, TokenType::UNDERSCORE}}},
        {TokenType::CALL_T,
         {{TokenType::PROCEDURE, TokenType::STRING, TokenType::UNDERSCORE},
          {TokenType::PROCEDURE, TokenType::STRING, TokenType::UNDERSCORE}}},
        {TokenType::AFFECTS,
         {{TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
           TokenType::PROG_LINE, TokenType::UNDERSCORE},
          {TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
           TokenType::PROG_LINE, TokenType::UNDERSCORE}}},
        {TokenType::AFFECTS_T,
         {{TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
           TokenType::PROG_LINE, TokenType::UNDERSCORE},
          {TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
           TokenType::PROG_LINE, TokenType::UNDERSCORE}}},
        {TokenType::NEXT,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
              TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
              TokenType::NUMBER},
         }},
        {TokenType::NEXT_T,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
              TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
              TokenType::NUMBER},
         }

        }};
std::unordered_map<AttributeRefType, std::unordered_set<TokenType>> attributes =
    {
        {AttributeRefType::PROCNAME, {TokenType::PROCEDURE, TokenType::CALL}},
        {AttributeRefType::VARNAME,
         {TokenType::VARIABLE, TokenType::READ, TokenType::PRINT}},
        {AttributeRefType::VALUE, {TokenType::CONSTANT}},
        {AttributeRefType::STATEMENT_NUM,
         {TokenType::STMT, TokenType::READ, TokenType::PRINT, TokenType::CALL,
          TokenType::WHILE, TokenType::IF, TokenType::ASSIGN}},
};

template <class T> bool contains(std::unordered_set<T> set, T item) {
  return set.find(item) != set.end();
}

PqlToken PqlParser::getNextToken() {
  if (it == end) {
    throw "Error: no more tokens";
  }
  const auto nextValue = *it;
  it++;
  return nextValue;
}

PqlToken PqlParser::getNextExpectedToken(TokenType expectedTokenType) {
  const PqlToken token = getNextToken();
  if (token.type != expectedTokenType) {
    if (expectedTokenType == TokenType::SYNONYM) {
      for (auto it = stringTokenMap.begin(); it != stringTokenMap.end(); ++it)
        if (it->second == token.type)
          return PqlToken{TokenType::SYNONYM, it->first};
    }
    throw "Retrieved token is not the expected token type";
  }
  return token;
}

void PqlParser::parseEndOfStatement() {
  getNextExpectedToken(TokenType::SEMICOLON);
}

PqlParser::PqlParser(std::vector<PqlToken> &tokens) {
  it = tokens.begin();
  end = tokens.end();
  pq = ParsedQuery();
}

void PqlParser::parseDeclaration() {

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

TokenType PqlParser::getDeclarationForSynonym(PqlToken &token) {
  if (pq.declarations.find(token.value) == pq.declarations.end()) {
    throw "ERROR: token not found in declaration clauses";
  }
  return pq.declarations[token.value];
}
PqlToken PqlParser::getNextTokenWithDeclarationTypeInArgumentsList(
    std::unordered_set<TokenType> &argumentsList) {
  PqlToken token = getNextToken();
  for (auto it = stringTokenMap.begin(); it != stringTokenMap.end(); ++it)
    if (it->second == token.type &&
        pq.declarations.find(it->first) != pq.declarations.end())
      token = {TokenType::SYNONYM, it->first};
  if (token.type == TokenType::SYNONYM)
    token.type = getDeclarationForSynonym(token);
  if (!contains(argumentsList, token.type)) {
    throw "Token for relationship does not match.";
  }
  return token;
}

void PqlParser::parseRelationship() {
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
    auto validArgumentsList = validArgumentsLists[i];
    auto nextToken =
        getNextTokenWithDeclarationTypeInArgumentsList(validArgumentsList);
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

void PqlParser::parseSuchThatClause() {
  getNextExpectedToken(TokenType::SUCH);
  getNextExpectedToken(TokenType::THAT);
  parseRelationship();
  while (it != end && it->type == TokenType::AND) {
    getNextExpectedToken(TokenType::AND);
    parseRelationship();
  }
}

const std::unordered_set<TokenType> allowedPatternTypes = {
    TokenType::ASSIGN, TokenType::WHILE, TokenType::IF};

PqlToken PqlParser::getParsedLHSOfPattern() {
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
    synonymToken.type = getDeclarationForSynonym(synonymToken);
    if (synonymToken.type != TokenType::VARIABLE) {
      throw "ERROR: There should only be synonyms of variable type here";
    }
    return synonymToken;
  }
}

PatternSpec PqlParser::getParsedRHSOfPattern() {
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

void PqlParser::parsePatternClause() {
  getNextExpectedToken(TokenType::PATTERN);
  parsePattern();
  while (it != end && it->type == TokenType::AND) {
    getNextExpectedToken(TokenType::AND);
    parsePattern();
  }
}
void PqlParser::parsePattern() {
  auto synonymToken = getNextExpectedToken(TokenType::SYNONYM);
  const auto declarationType = getDeclarationForSynonym(synonymToken);
  synonymToken.type = declarationType;

  getNextExpectedToken(TokenType::OPEN_PARENTHESIS);
  switch (declarationType) {
  case TokenType::ASSIGN: {
    PqlToken lhs = getParsedLHSOfPattern();
    getNextExpectedToken(TokenType::COMMA);
    PatternSpec rhs = getParsedRHSOfPattern();
    pq.patterns.push_back(ParsedPattern{synonymToken, lhs, rhs});
    break;
  }
  case TokenType::IF: {
    PqlToken lhs = getParsedLHSOfPattern();
    getNextExpectedToken(TokenType::COMMA);
    getNextExpectedToken(TokenType::UNDERSCORE);
    getNextExpectedToken(TokenType::COMMA);
    getNextExpectedToken(TokenType::UNDERSCORE);
    pq.patterns.push_back(
        ParsedPattern{synonymToken, lhs, {PatternMatchType::Any}});
    break;
  }
  case TokenType::WHILE: {
    PqlToken lhs = getParsedLHSOfPattern();
    getNextExpectedToken(TokenType::COMMA);
    getNextExpectedToken(TokenType::UNDERSCORE);
    pq.patterns.push_back(
        ParsedPattern{synonymToken, lhs, {PatternMatchType::Any}});
    break;
  }
  default:
    throw "ERROR: Expected an assignm if, or while typed synonym";
  }
  getNextExpectedToken(TokenType::CLOSED_PARENTHESIS);
}

void PqlParser::parseResultClause() {
  switch (it->type) {
  case TokenType::BOOLEAN:
    getNextExpectedToken(TokenType::BOOLEAN);
    pq.results.resultType = PqlResultType::Boolean;
    break;

  case TokenType::OPEN_ANGLED_BRACKET:
    pq.results.resultType = PqlResultType::Tuple;
    getNextExpectedToken(TokenType::OPEN_ANGLED_BRACKET);
    parseElemInResult();
    while (it != end && it->type != TokenType::CLOSED_ANGLED_BRACKET) {
      getNextExpectedToken(TokenType::COMMA);
      parseElemInResult();
    }
    getNextExpectedToken(TokenType::CLOSED_ANGLED_BRACKET);
    break;
  default:
    pq.results.resultType = PqlResultType::Tuple;
    parseElemInResult();
    break;
  }
}

void PqlParser::parseElemInResult() {
  const auto element = getElem();
  pq.results.results.push_back(element);
}

std::unordered_map<TokenType, AttributeRefType> tokenTypeToAttributeRefType{
    {TokenType::PROCNAME, AttributeRefType::PROCNAME},
    {TokenType::VARNAME, AttributeRefType::VARNAME},
    {TokenType::VALUE, AttributeRefType::VALUE},
    {TokenType::STATEMENT_NUM, AttributeRefType::STATEMENT_NUM}};

Element PqlParser::getElem() {
  auto synonym = getNextExpectedToken(TokenType::SYNONYM);
  const auto declaration = getDeclarationForSynonym(synonym);
  if (it == end || it->type != TokenType::DOT) {
    return Element{synonym.value, AttributeRefType::NONE};
  }
  getNextExpectedToken(TokenType::DOT);
  const auto nextToken = getNextToken();
  if (!contains(attributeNames, nextToken.type)) {
    throw "ERROR: Expected next token to be an attribute name but attribute "
          "name not found";
  }
  const auto refType = tokenTypeToAttributeRefType[nextToken.type];
  const auto acceptableEntityTypes = attributes[refType];
  if (acceptableEntityTypes.find(declaration) == acceptableEntityTypes.end()) {
    throw "ERROR: Declaration not found in acceptable entity types";
  }
  return Element{synonym.value, refType};
}

void PqlParser::parseWithClause() {
  getNextExpectedToken(TokenType::WITH);
  parseAttributeCompare();
  while (it != end && it->type == TokenType::AND) {
    getNextExpectedToken(TokenType::AND);
    parseAttributeCompare();
  }
}

void PqlParser::parseAttributeCompare() {
  const auto firstRef = getRef();
  getNextExpectedToken(TokenType::EQUALS);
  const auto secondRef = getRef();
  // TODO: Perform check on attribute compare to check if ref is valid, and to
  // check if at least one is an elem
  if (firstRef.referenceType == ReferenceType::RAW_VALUE &&
      secondRef.referenceType == ReferenceType::RAW_VALUE) {
    if (firstRef.pqlToken.type != secondRef.pqlToken.type) {
      throw "ERROR: Expecting string to compare with string or number to "
            "compare with number";
    }
  }
  pq.withs.push_back({firstRef, secondRef});
}

Reference PqlParser::getRef() {
  switch (it->type) {
  case TokenType::STRING: {
    return Reference{getNextExpectedToken(TokenType::STRING)};
  }
  case TokenType::NUMBER: {
    return Reference{getNextExpectedToken(TokenType::NUMBER)};
  }

  default: {
    return Reference{getElem()};
  }
  }
}

void PqlParser::parseClausesFromSelectOnwards() {
  const auto token = getNextExpectedToken(TokenType::SELECT);

  parseResultClause();

  while (it != end) {
    switch (it->type) {
    case TokenType::SUCH:
      parseSuchThatClause();
      break;
    case TokenType::PATTERN:
      parsePatternClause();
      break;
    case TokenType::WITH:
      parseWithClause();
      break;
    default:
      throw "EXPECTED SUCH THAT OR PATTERN";
      break;
    }
  }
}

ParsedQuery PqlParser::parse() {
  while (it != end && contains<TokenType>(entities, it->type)) {
    parseDeclaration();
  }
  parseClausesFromSelectOnwards();
  return pq;
}
