#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PqlLexer.h"
#include "PqlParser.h"
#include "SimpleExprParserWrapper.h"
#include "SimpleTokenizer.h"

std::unordered_set<TokenType> attributeNames = {
    TokenType::PROCNAME, TokenType::VARNAME, TokenType::VALUE,
    TokenType::STATEMENT_NUM, TokenType::BOOLEAN};

std::unordered_set<TokenType> abstractions = {
    TokenType::PARENT,    TokenType::PARENT_T, TokenType::FOLLOWS,
    TokenType::FOLLOWS_T, TokenType::MODIFIES, TokenType::USES,

    TokenType::MATCH,
};

std::unordered_map<AttributeRefType, TokenType> validElementToRawComparisons = {
    {AttributeRefType::NONE, TokenType::NUMBER},
    {AttributeRefType::PROCNAME, TokenType::STRING},
    {AttributeRefType::STATEMENT_NUM, TokenType::NUMBER},
    {AttributeRefType::VALUE, TokenType::NUMBER},
    {AttributeRefType::VARNAME, TokenType::STRING},

};

std::unordered_map<AttributeRefType, std::unordered_set<TokenType>> attributes =
    {
        {
            AttributeRefType::PROCNAME,
            {TokenType::PROCEDURE, TokenType::CALL},
        },
        {
            AttributeRefType::VARNAME,
            {TokenType::VARIABLE, TokenType::READ, TokenType::PRINT},
        },
        {
            AttributeRefType::VALUE,
            {TokenType::CONSTANT},
        },
        {
            AttributeRefType::STATEMENT_NUM,
            {TokenType::STMT, TokenType::READ, TokenType::PRINT,
             TokenType::CALL, TokenType::WHILE, TokenType::IF,
             TokenType::ASSIGN},
        },
        {
            AttributeRefType::NONE,
            {TokenType::PROG_LINE},
        },
};

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
  semanticErrorPresent = false;
}

void PqlParser::parseDeclaration() {

  const auto token = getNextToken();
  const auto entityIdentifier = token.type;
  if (!setContains(entities, entityIdentifier)) {
    throw "Entity identifier not found";
  }
  const auto nextToken = getNextExpectedToken(TokenType::SYNONYM);
  if (mapContains(pq.declarations, nextToken.value)) {
    semanticErrorPresent = true;
  }
  pq.declarations[nextToken.value] = entityIdentifier;
  while (it != end && it->type == TokenType::COMMA) {
    getNextExpectedToken(TokenType::COMMA);
    const auto nextToken = getNextExpectedToken(TokenType::SYNONYM);
    pq.declarations[nextToken.value] = entityIdentifier;
  }
  parseEndOfStatement();
}

TokenType PqlParser::getDeclarationForSynonym(PqlToken &token) {
  if (!mapContains(pq.declarations, token.value)) {
    semanticErrorPresent = true;
    return TokenType::SYNONYM;
  }
  return pq.declarations[token.value];
}
PqlToken PqlParser::getNextTokenWithDeclarationTypeInArgumentsList(
    std::unordered_set<TokenType> &argumentsList) {
  PqlToken token;
  if (it != end && it->type == TokenType::STRING)
    token = getIdentInString();
  else
    token = getNextToken();
  for (auto it = stringTokenMap.begin(); it != stringTokenMap.end(); ++it)
    if (it->second == token.type && mapContains(pq.declarations, it->first))
      token = {TokenType::SYNONYM, it->first};
  if (token.type == TokenType::SYNONYM)
    token.type = getDeclarationForSynonym(token);

  if (!setContains(argumentsList, token.type)) {
    semanticErrorPresent = true;
  }
  return token;
}

void PqlParser::parseRelationship() {
  const auto token = getNextToken();
  const auto relationshipIdentifier = token.type;
  const auto validArgumentsLists =
      allowedArgumentsForRelationships.at(relationshipIdentifier);
  const int argumentsCount = validArgumentsLists.size();
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
    return getIdentInString();
  default:
    auto synonymToken = getNextExpectedToken(TokenType::SYNONYM);
    synonymToken.type = getDeclarationForSynonym(synonymToken);
    if (synonymToken.type != TokenType::VARIABLE) {
      semanticErrorPresent = true;
    }
    return synonymToken;
  }
}

PatternSpec PqlParser::getParsedRHSOfPattern() {
  if (it == end) {
    throw "ERROR: Expected token for RHS";
  }
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
    // TODO: Determine if this is a syntactic or semantic error
    throw "ERROR: Expected an assign if, or while typed synonym";
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
  auto synonym = getNextExpectedToken(TokenType::SYNONYM);
  if (it != end && it->type == TokenType::DOT) {
    pq.results.results.push_back(getAttrRef(synonym));
  } else {
    getDeclarationForSynonym(synonym);
    pq.results.results.push_back(
        Element{synonym.value, AttributeRefType::NONE});
  }
}

std::unordered_map<TokenType, AttributeRefType> tokenTypeToAttributeRefType{
    {TokenType::PROCNAME, AttributeRefType::PROCNAME},
    {TokenType::VARNAME, AttributeRefType::VARNAME},
    {TokenType::VALUE, AttributeRefType::VALUE},
    {TokenType::STATEMENT_NUM, AttributeRefType::STATEMENT_NUM}};

Element PqlParser::getAttrRef(PqlToken &synonym) {
  AttributeRefType refType;
  if (it != end && it->type == TokenType::DOT) {
    getNextExpectedToken(TokenType::DOT);
    const auto nextToken = getNextToken();
    if (!setContains(attributeNames, nextToken.type)) {
      throw "ERROR: Expected next token to be an attribute name but attribute "
            "name not found";
    }
    refType = tokenTypeToAttributeRefType[nextToken.type];
  } else {
    refType = AttributeRefType::NONE;
  }
  const auto acceptableEntityTypes = attributes[refType];
  const auto declaration = getDeclarationForSynonym(synonym);
  if (!setContains(acceptableEntityTypes, declaration)) {
    semanticErrorPresent = true;
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

WITH createWithClauseFromReferences(Reference &firstReference,
                                    Reference &secondReference) {
  if (secondReference.referenceType == ReferenceType::ELEMENT &&
      firstReference.referenceType == ReferenceType::RAW_VALUE) {
    return {secondReference, firstReference};
  }
  return {firstReference, secondReference};
}

void PqlParser::checkSemanticCorrectnessOfWith(WITH &with) {
  auto firstRef = with.first;
  auto secondRef = with.second;

  // 1. For comparison of two raw values, that they are of the same type (i.e.
  // string = string or number = number)
  if (firstRef.referenceType == ReferenceType::RAW_VALUE &&
      secondRef.referenceType == ReferenceType::RAW_VALUE) {
    if (firstRef.pqlToken.type != secondRef.pqlToken.type) {
      semanticErrorPresent = true;
    }
  }

  // 2. For comparison of an element with a raw value, ensure that the type
  // corresponds to a type map
  else if (firstRef.referenceType == ReferenceType::ELEMENT &&
           secondRef.referenceType == ReferenceType::RAW_VALUE) {
    if (validElementToRawComparisons.at(firstRef.element.refType) !=
        secondRef.pqlToken.type) {
      semanticErrorPresent = true;
    }
  }
  // 3. For comparison of an element with another element, ensure that the type
  // corresponds to the type map
  else if (firstRef.referenceType == ReferenceType::ELEMENT &&
           secondRef.referenceType == ReferenceType::ELEMENT) {
    if (validElementToRawComparisons.at(firstRef.element.refType) !=
        validElementToRawComparisons.at(secondRef.element.refType)) {
      semanticErrorPresent = true;
    }
  }
}

void PqlParser::parseAttributeCompare() {
  auto firstRef = getRef();
  getNextExpectedToken(TokenType::EQUALS);
  auto secondRef = getRef();
  WITH withClause = createWithClauseFromReferences(firstRef, secondRef);
  checkSemanticCorrectnessOfWith(withClause);
  pq.withs.push_back(withClause);
}

Reference PqlParser::getRef() {
  if (it == end) {
    throw "ERROR: Expected a token for ref";
  }
  switch (it->type) {
  case TokenType::STRING: {
    return Reference{getIdentInString()};
  }
  case TokenType::NUMBER: {
    return Reference{getNextExpectedToken(TokenType::NUMBER)};
  }

  default: {
    return Reference{getAttrRef(getNextExpectedToken(TokenType::SYNONYM))};
  }
  }
}
PqlToken PqlParser::getIdentInString() {
  PqlToken result = getNextExpectedToken(TokenType::STRING);
  if (!PqlLexer::isAlphaNumeric(result.value)) {
    throw "ERROR: Expected IDENT in string";
  }
  return result;
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
  while (it != end && setContains<TokenType>(entities, it->type)) {
    parseDeclaration();
  }
  parseClausesFromSelectOnwards();
  semanticErrorCheck();
  return pq;
}

void PqlParser::semanticErrorCheck() {

  if (semanticErrorPresent) {
    if (pq.results.resultType == PqlResultType::Boolean) {
      throw PqlSemanticErrorWithBooleanResultException();
    } else {
      throw "ERROR: Semantic error";
    }
  }
}
