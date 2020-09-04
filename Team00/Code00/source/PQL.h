#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class TokenType {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ELSE,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,

  SYNONYM,
  NUMBER,
  STRING,
  UNDERSCORE,

  SELECT,
  PARENT,
  PARENT_T,
  FOLLOWS,
  FOLLOWS_T,
  MODIFIES,
  USES,
  MATCH, // Representation of pattern clause as a relationship

  SUCH,
  THAT,
  PATTERN,

  SEMICOLON,
  COMMA,
  OPEN_PARENTHESIS,
  CLOSED_PARENTHESIS,
};

struct PqlToken {
  TokenType type;
  std::string value;
  bool operator==(const PqlToken &other) const {
    return type == other.type && value == other.value;
  }
  PqlToken(TokenType specifiedTokenType, std::string specifiedValue = "")
      : type{specifiedTokenType}, value{specifiedValue} {}
};

struct ParsedRelationship {
  TokenType relationship;
  PqlToken first_argument;
  PqlToken second_argument;
  bool operator==(const ParsedRelationship &other) const {
    return relationship == other.relationship &&
           first_argument == other.first_argument &&
           second_argument == other.second_argument;
  }
};

struct ParsedQuery {
  std::unordered_map<std::string, TokenType> declaration_clause;
  std::vector<std::string> result_clause;
  std::vector<ParsedRelationship> relationship_clause;
};

class PQL {
public:
};
