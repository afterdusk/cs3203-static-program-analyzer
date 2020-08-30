#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PQLParser.h"

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

std::unordered_map<TokenType, std::vector<std::vector<TokenType>>>
    relationships = {
        {TokenType::FOLLOWS,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
         }},
        {TokenType::FOLLOWS_T,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
         }},
        {TokenType::PARENT,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
         }},
        {TokenType::PARENT_T,
         {
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
             {TokenType::STMT, TokenType::READ, TokenType::PRINT,
              TokenType::CALL, TokenType::WHILE, TokenType::IF,
              TokenType::ASSIGN, TokenType::CONSTANT, TokenType::UNDERSCORE},
         }},
        {TokenType::MODIFIES,
         {
             {TokenType::STMT, TokenType::ASSIGN, TokenType::READ,
              TokenType::WHILE, TokenType::IF, TokenType::CALL,
              TokenType::PROCEDURE},
             {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
         }},
        {TokenType::USES,
         {
             {TokenType::STMT, TokenType::ASSIGN, TokenType::PRINT,
              TokenType::WHILE, TokenType::IF, TokenType::CALL,
              TokenType::PROCEDURE},
             {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
         }},
        {TokenType::MATCH,
         {
             {TokenType::STRING, TokenType::UNDERSCORE},
             {TokenType::STRING, TokenType::UNDERSCORE},
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
  bool isWithStringLiterals = false;
  for (const char c : s) {
    switch (c) {
    case '"':
      isWithStringLiterals = !isWithStringLiterals;
      break;
    case '\n':
    case ')':
    case ';':
      result.push_back('#');
      break;
    case ' ':
      if (!isWithStringLiterals) {
        result.push_back('#');
        continue;
      }
      break;
    default:
      break;
    }
    result.push_back(c);
    // TODO: Handle SUCHTHAT
    switch (c) {
    case '(':
      result.push_back('#');
      break;
    default:
      break;
    }
  }
  if (isWithStringLiterals) {
    // TODO:
    std::cout << "EXCEPTION";
  }
  return split(std::string(result.begin(), result.end()), '#');
}

std::unordered_map<std::string, TokenType> stringTokenMap{
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {";", TokenType::SEMICOLON},
    {"procedure", TokenType::PROCEDURE},
    {"assign", TokenType::ASSIGN}};

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

std::vector<PqlToken> lex(std::string query) {
  std::vector<std::string> rawTokens = delimit(query);
  std::vector<PqlToken> result;
  for (const auto token : rawTokens) {
    if (token.size() == 0) {
      continue;
    }

    if (stringTokenMap.find(token) != stringTokenMap.end()) {
      result.push_back(PqlToken{stringTokenMap[token], ""});
    } else if (token.size() >= 2 && token[0] == '"' && token[-1] == '"') {
      const std::string content = token.substr(1, token.size() - 2);
      result.push_back(PqlToken{TokenType::STRING, content});
    } else if (!startsWithDigit(token) && isAlphaNumeric(token)) {
      result.push_back(PqlToken{TokenType::SYNONYM, token});
    } else if (isDigits(token)) {
      result.push_back(PqlToken{TokenType::NUMBER, token});
    } else {
      // TODO: Throw error on unrecognised token
      std::cout << "ERROR: Unrecognised token: " << token << "\n";
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

void parseEndOfStatement(std::vector<PqlToken>::iterator &tokenIterator,
                         const std::vector<PqlToken>::iterator endMarker) {
  const auto token = getNextToken(tokenIterator, endMarker);
  if (token.type != TokenType::SEMICOLON) {
    throw "Expected semicolon at end of statement";
  }
}

void parseDeclaration(std::vector<PqlToken>::iterator &tokenIterator,
                      const std::vector<PqlToken>::iterator endMarker,
                      ParsedQuery &pq) {

  const auto token = getNextToken(tokenIterator, endMarker);
  const auto entityIdentifier = token.type;
  if (contains(entities, entityIdentifier)) {
    throw "Entity identifier not found";
  }

  const auto nextToken = getNextToken(tokenIterator, endMarker);
  if (nextToken.type != TokenType::SYNONYM) {
    throw "Expected SYNONYM";
  }
  parseEndOfStatement(tokenIterator, endMarker);

  // TODO: check if map previously contained value;
  pq.declaration_clause[nextToken.value] = entityIdentifier;
}

void parseRelationship(std::vector<PqlToken>::iterator &tokenIterator,
                       const std::vector<PqlToken>::iterator endMarker,
                       ParsedQuery &pq) {
  const auto token = getNextToken(tokenIterator, endMarker);
  const auto relationshipIdentifier = token.type;
  const auto validArgumentsList = relationships[relationshipIdentifier];
  const int argumentsCount = validArgumentsList.size();
  if (relationships.find(relationshipIdentifier) != relationships.end()) {
    throw "Relationship not found";
  }
  std::vector<PqlToken> relationshipArgs;
  for (int i = 0; i < argumentsCount; i++) {
    const auto nextToken = getNextToken(tokenIterator, endMarker);

    const auto argumentList = validArgumentsList[i];
    if (find(argumentList.begin(), argumentList.end(), nextToken.type) !=
        argumentList.end()) {
      relationshipArgs.push_back(nextToken);
    } else {
      throw "Token for relationship does not match.";
    }
  }

  if (relationshipArgs.size() == 2) {
    pq.relationship_clause.push_back(ParsedRelationship{
        relationshipIdentifier, relationshipArgs[0], relationshipArgs[1]});
  } else {
    throw "Incorrect number of arguments";
  }
}

void parseSelect(std::vector<PqlToken>::iterator &tokenIterator,
                 const std::vector<PqlToken>::iterator endMarker,
                 ParsedQuery &pq) {
  const auto token = getNextToken(tokenIterator, endMarker);
  if (token.type != TokenType::SELECT) {
    throw "Expected SELECT";
  }

  // TODO: Handle multiple returns
  const auto nextToken = getNextToken(tokenIterator, endMarker);
  if (nextToken.type != TokenType::STRING) {
    throw "Expected string";
  }

  // TODO: check if result clause previously contained value;
  pq.result_clause.push_back(nextToken.value);

  const auto suchThatToken = getNextToken(tokenIterator, endMarker);
  if (suchThatToken.type != TokenType::SUCHTHAT) {
    throw "Expected SUCH THAT";
  }

  parseRelationship(tokenIterator, endMarker, pq);
  // TODO: Handle pattern statements
}

ParsedQuery parse(std::vector<PqlToken> query) {
  std::vector<PqlToken>::iterator it = query.begin();
  std::vector<PqlToken>::iterator endMarker = query.end();
  ParsedQuery result;
  // TODO: Parse assignment
  while (it != endMarker) {
    TokenType nextToken = it->type;
    if (contains<TokenType>(entities, nextToken)) {
      parseDeclaration(it, endMarker, result);
    } else if (nextToken == TokenType::SELECT) {
      parseSelect(it, endMarker, result);
    }
  }
  return result;
}
