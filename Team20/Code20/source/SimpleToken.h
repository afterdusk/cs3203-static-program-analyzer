#pragma once

#include <string>

class SimpleToken {
public:
  enum class TokenType {
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
  SimpleToken(std::string value);
  TokenType getTokenType() const;
  std::string getVal() const;

private:
  std::string val;
  TokenType tokenType;
  // translate val into TokenEnum
  void translate();
};

bool operator==(const SimpleToken &token1, const SimpleToken &token2);

// custom specialization of std::hash for use of unordered set
namespace std {
template <> struct hash<SimpleToken> {
  std::size_t operator()(const SimpleToken &token) const noexcept {
    return std::hash<std::string>{}(token.getVal());
  }
};
} // namespace std
