#pragma once

#include <string>

enum class TokenEnum {
  OPEN_P,  // open parenthesis
  CLOSE_P, // closed parenthesis
  OPEN_B,  // open curly bracket
  CLOSE_B, // closed curly bracket
  SEMI_COLON,

  // operators
  PLUS,
  MINUS,
  TIMES,
  DIV,
  MOD,
  ASSIGN,

  // condition operators
  EQUAL,
  GREATER,
  GREATER_EQ,
  LESS,
  LESS_EQ,
  NOT_EQ,

  AND,
  OR,
  NOT,

  WORD, // all words that match grammar
  CONSTANT,
  OTHER,
};

class Token {
private:
  std::string val;
  TokenEnum tokenEnum;
  // translate val into TokenEnum
  void translate();

public:
  Token(std::string value);
  TokenEnum getTokenEnum() const;
  std::string getVal() const;
};

bool operator==(const Token &token1, const Token &token2);

// custom specialization of std::hash for use of unordered set
namespace std {
template <> struct hash<Token> {
  std::size_t operator()(const Token &token) const noexcept {
    return std::hash<std::string>{}(token.getVal());
  }
};
} // namespace std
