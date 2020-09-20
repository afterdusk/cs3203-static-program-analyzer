#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ExprParserWrapper.h"
#include "PQL.h"
#include "Tokenizer.h"

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

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;

  while (getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}

// TODO: Consider long strings when converting to int

std::vector<std::string> delimit(std::string s) {
  std::vector<char> result;
  bool isWithinStringLiterals = false;
  for (const char c : s) {
    switch (c) {
    // 0. Differentiate between cases within string literals and out of string
    // literals
    case '"':
      isWithinStringLiterals = !isWithinStringLiterals;
      break;
    // 0. Replace spaces w delimiter
    case ' ':
    case '\n':
      if (!isWithinStringLiterals) {
        result.push_back('#');
        continue;
      }
      break;
    // 1. Prepend delimiter for special characters
    // 1A. Characters that may appear within string literals
    case ')':
    case '(':
      if (!isWithinStringLiterals) {
        result.push_back('#');
      }
      break;
    // 1B. Characters that wont appear within string literals
    case ',':
    case ';':
    case '_':
      result.push_back('#');
      break;
    default:
      break;
    }
    // 2. Push character into the token array
    result.push_back(c);
    // 3. Postpend delimiters for special characters
    switch (c) {
    // 3A. Characters that might appear within string literals
    case '(':
    case ')':
      if (!isWithinStringLiterals) {
        result.push_back('#');
      }
      break;
    // 3B. Characters that won't appear within string literals
    case '_':
    case ',':
      result.push_back('#');
      break;
    default:
      break;
    }
  }
  if (isWithinStringLiterals) {
    // TODO:
    std::cout << "EXCEPTION";
  }
  return split(std::string(result.begin(), result.end()), '#');
}

std::unordered_map<std::string, TokenType> stringTokenMap{
    {";", TokenType::SEMICOLON},
    {"stmt", TokenType::STMT},
    {"read", TokenType::READ},
    {"print", TokenType::PRINT},
    {"call", TokenType::CALL},
    {"while", TokenType::WHILE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"assign", TokenType::ASSIGN},
    {"variable", TokenType::VARIABLE},
    {"constant", TokenType::CONSTANT},
    {"procedure", TokenType::PROCEDURE},
    {"assign", TokenType::ASSIGN},
    {"Select", TokenType::SELECT},
    {"such", TokenType::SUCH},
    {"that", TokenType::THAT},
    {"Follows", TokenType::FOLLOWS},
    {"Follows*", TokenType::FOLLOWS_T},
    {"Parent", TokenType::PARENT},
    {"Parent*", TokenType::PARENT_T},
    {"Uses", TokenType::USES},
    {"Modifies", TokenType::MODIFIES},
    {"pattern", TokenType::PATTERN},
    {"_", TokenType::UNDERSCORE},
    {"(", TokenType::OPEN_PARENTHESIS},
    {")", TokenType::CLOSED_PARENTHESIS},
    {",", TokenType::COMMA}};

bool isAlphaNumeric(std::string s) {
  for (const auto c : s) {
    if (!isalnum(c)) {
      return false;
    }
  }
  return true;
}

bool startsWithDigit(std::string s) { return isdigit(s[0]); }

bool isDigits(std::string s) {
  for (const auto c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}

bool isStringToken(std::string token) {
  return token.size() >= 2 && token.at(0) == '"' && token.back() == '"';
}

std::vector<PqlToken> PQL::lex(std::string query) {
  std::vector<std::string> rawTokens = delimit(query);
  std::vector<PqlToken> result;
  for (const auto token : rawTokens) {
    if (token.size() == 0) {
      continue;
    }

    if (stringTokenMap.find(token) != stringTokenMap.end()) {
      result.push_back(PqlToken{stringTokenMap[token], ""});
    } else if (isStringToken(token)) {
      const std::string content = token.substr(1, token.size() - 2);
      result.push_back(PqlToken{TokenType::STRING, content});
    } else if (!startsWithDigit(token) && isAlphaNumeric(token)) {
      result.push_back(PqlToken{TokenType::SYNONYM, token});
    } else if (isDigits(token)) {
      result.push_back(PqlToken{TokenType::NUMBER, token});
    } else {
      throw "ERROR: Unrecognised token" + token + "\n";
    }
  }

  return result;
}

template <class T> bool contains(std::unordered_set<T> set, T item) {
  return set.find(item) != set.end();
}

PqlToken getNextToken(std::vector<PqlToken>::iterator &it,
                      std::vector<PqlToken>::iterator end) {
  if (it == end) {
    throw "Error: no more tokens";
  }
  const auto nextValue = *it;
  it++;
  return nextValue;
}

PqlToken getNextExpectedToken(std::vector<PqlToken>::iterator &it,
                              std::vector<PqlToken>::iterator end,
                              TokenType expectedTokenType) {
  const PqlToken token = getNextToken(it, end);
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

void parseEndOfStatement(std::vector<PqlToken>::iterator &tokenIterator,
                         const std::vector<PqlToken>::iterator endMarker) {
  getNextExpectedToken(tokenIterator, endMarker, TokenType::SEMICOLON);
}

void parseDeclaration(std::vector<PqlToken>::iterator &tokenIterator,
                      const std::vector<PqlToken>::iterator endMarker,
                      ParsedQuery &pq) {

  const auto token = getNextToken(tokenIterator, endMarker);
  const auto entityIdentifier = token.type;
  if (!contains(entities, entityIdentifier)) {
    throw "Entity identifier not found";
  }

  const auto nextToken =
      getNextExpectedToken(tokenIterator, endMarker, TokenType::SYNONYM);
  pq.declaration_clause[nextToken.value] = entityIdentifier;
  while (tokenIterator != endMarker &&
         tokenIterator->type == TokenType::COMMA) {
    getNextExpectedToken(tokenIterator, endMarker, TokenType::COMMA);
    const auto nextToken =
        getNextExpectedToken(tokenIterator, endMarker, TokenType::SYNONYM);
    pq.declaration_clause[nextToken.value] = entityIdentifier;
  }
  parseEndOfStatement(tokenIterator, endMarker);
}

TokenType getDeclaration(PqlToken &token, ParsedQuery &pq) {
  if (token.type != TokenType::SYNONYM) {
    return token.type;
  }
  if (pq.declaration_clause.find(token.value) == pq.declaration_clause.end()) {
    throw "ERROR: token not found in declaration clauses";
  }
  return pq.declaration_clause[token.value];
}
TokenType getTokenDeclarationTypeInArgumentsList(
    PqlToken &token, ParsedQuery &pq,
    std::unordered_set<TokenType> argumentsList) {
  const TokenType declaredType = getDeclaration(token, pq);
  if (!contains(argumentsList, declaredType)) {
    throw "Token for relationship does not match.";
  }
  return declaredType;
}

void parseRelationship(std::vector<PqlToken>::iterator &tokenIterator,
                       const std::vector<PqlToken>::iterator endMarker,
                       ParsedQuery &pq) {
  const auto token = getNextToken(tokenIterator, endMarker);
  const auto relationshipIdentifier = token.type;
  const auto validArgumentsLists = relationships[relationshipIdentifier];
  const int argumentsCount = validArgumentsLists.size();
  if (relationships.find(relationshipIdentifier) == relationships.end()) {
    throw "Relationship not found";
  }
  getNextExpectedToken(tokenIterator, endMarker, TokenType::OPEN_PARENTHESIS);
  std::vector<PqlToken> relationshipArgs;
  for (int i = 0; i < argumentsCount; i++) {
    auto nextToken = getNextToken(tokenIterator, endMarker);

    auto validArgumentsList = validArgumentsLists[i];
    auto tokenType = getTokenDeclarationTypeInArgumentsList(nextToken, pq,
                                                            validArgumentsList);
    nextToken.type = tokenType;
    relationshipArgs.push_back(nextToken);

    if (i != argumentsCount - 1) {
      getNextExpectedToken(tokenIterator, endMarker, TokenType::COMMA);
    }
  }
  getNextExpectedToken(tokenIterator, endMarker, TokenType::CLOSED_PARENTHESIS);

  if (relationshipArgs.size() == 2) {
    pq.relationship_clauses.push_back(ParsedRelationship{
        relationshipIdentifier, relationshipArgs[0], relationshipArgs[1]});
  } else {
    throw "Incorrect number of arguments";
  }
}

void parseSuchThat(std::vector<PqlToken>::iterator &tokenIterator,
                   const std::vector<PqlToken>::iterator endMarker,
                   ParsedQuery &pq) {
  // TODO: Handle "AND"
  getNextExpectedToken(tokenIterator, endMarker, TokenType::SUCH);
  getNextExpectedToken(tokenIterator, endMarker, TokenType::THAT);

  parseRelationship(tokenIterator, endMarker, pq);
}

const std::unordered_set<TokenType> allowedPatternTypes = {
    TokenType::ASSIGN, TokenType::WHILE, TokenType::IF};

PqlToken getParsedLHSOfPattern(std::vector<PqlToken>::iterator &tokenIterator,
                               const std::vector<PqlToken>::iterator endMarker,
                               ParsedQuery &pq) {
  if (tokenIterator == endMarker) {
    throw "ERROR: End of tokens when retrieving LHS of pattern";
  }
  switch (tokenIterator->type) {
  case TokenType::UNDERSCORE:
    return getNextExpectedToken(tokenIterator, endMarker,
                                TokenType::UNDERSCORE);
  case TokenType::STRING:
    return getNextExpectedToken(tokenIterator, endMarker, TokenType::STRING);
  default:
    // TODO: Check if LHS is from list of accepted LHS
    auto synonymToken =
        getNextExpectedToken(tokenIterator, endMarker, TokenType::SYNONYM);
    const auto declarationType = getDeclaration(synonymToken, pq);
    synonymToken.type = declarationType;
    return synonymToken;
  }
}

PatternSpec
getParsedRHSOfPattern(std::vector<PqlToken>::iterator &tokenIterator,
                      const std::vector<PqlToken>::iterator endMarker) {
  switch (tokenIterator->type) {
  case TokenType::UNDERSCORE:
    getNextExpectedToken(tokenIterator, endMarker, TokenType::UNDERSCORE);
    switch (tokenIterator->type) {
    case TokenType::STRING: {
      const auto nextToken =
          getNextExpectedToken(tokenIterator, endMarker, TokenType::STRING);
      PatternSpec result =
          PatternSpec{PatternMatchType::SubTreeMatch, nextToken.value};
      getNextExpectedToken(tokenIterator, endMarker, TokenType::UNDERSCORE);
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
    const auto nextToken =
        getNextExpectedToken(tokenIterator, endMarker, TokenType::STRING);
    return PatternSpec{PatternMatchType::CompleteMatch, nextToken.value};
    break;
  }
  default:
    throw "ERROR: Unrecognised token after parsing pattern token";
    break;
  }
}
void parsePattern(std::vector<PqlToken>::iterator &tokenIterator,
                  const std::vector<PqlToken>::iterator endMarker,
                  ParsedQuery &pq) {

  getNextExpectedToken(tokenIterator, endMarker, TokenType::PATTERN);
  auto synonymToken =
      getNextExpectedToken(tokenIterator, endMarker, TokenType::SYNONYM);
  const auto declarationType = getDeclaration(synonymToken, pq);
  if (!contains(allowedPatternTypes, declarationType)) {
    throw "ERROR: not from allowed pattern types";
  }
  synonymToken.type = declarationType;

  // TODO: Handle syn-if: it accepts 3 parameters instead of 2
  getNextExpectedToken(tokenIterator, endMarker, TokenType::OPEN_PARENTHESIS);
  PqlToken lhs = getParsedLHSOfPattern(tokenIterator, endMarker, pq);
  getNextExpectedToken(tokenIterator, endMarker, TokenType::COMMA);
  PatternSpec rhs = getParsedRHSOfPattern(tokenIterator, endMarker);
  pq.pattern_clauses.push_back(ParsedPattern{synonymToken, lhs, rhs});
  getNextExpectedToken(tokenIterator, endMarker, TokenType::CLOSED_PARENTHESIS);
}

void parseClausesFromSelectOnwards(
    std::vector<PqlToken>::iterator &tokenIterator,
    const std::vector<PqlToken>::iterator endMarker, ParsedQuery &pq) {
  const auto token =
      getNextExpectedToken(tokenIterator, endMarker, TokenType::SELECT);

  // TODO: Handle multiple returns
  const auto nextToken =
      getNextExpectedToken(tokenIterator, endMarker, TokenType::SYNONYM);

  if (pq.declaration_clause.find(nextToken.value) ==
      pq.declaration_clause.end()) {
    throw "ERROR: Result not in declaration clause";
  }
  pq.result_clause.push_back(nextToken.value);

  while (tokenIterator != endMarker) {
    switch (tokenIterator->type) {
    case TokenType::SUCH:
      parseSuchThat(tokenIterator, endMarker, pq);
      break;
    case TokenType::PATTERN:
      parsePattern(tokenIterator, endMarker, pq);
      break;
    default:
      throw "EXPECTED SUCH THAT OR PATTERN";
      break;
    }
  }
}

ParsedQuery PQL::parse(std::vector<PqlToken> query) {
  std::vector<PqlToken>::iterator it = query.begin();
  std::vector<PqlToken>::iterator endMarker = query.end();
  ParsedQuery result;
  // TODO: Parse assignment
  while (it != endMarker && contains<TokenType>(entities, it->type)) {
    parseDeclaration(it, endMarker, result);
  }
  parseClausesFromSelectOnwards(it, endMarker, result);
  return result;
}
