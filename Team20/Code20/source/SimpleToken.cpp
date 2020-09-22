#include "SimpleToken.h"
#include "SimpleSyntaxChecker.h"
// constructor
SimpleToken::SimpleToken(std::string value) {
  val = value;
  translate();
}

// Getters
SimpleToken::TokenType SimpleToken::getTokenType() const { return tokenType; }

std::string SimpleToken::getVal() const { return val; }

// Tranlate value  into TokenEnum
void SimpleToken::translate() {
  if (val == "(") {
    tokenType = SimpleToken::TokenType::OPEN_P;
  } else if (val == ")") {
    tokenType = SimpleToken::TokenType::CLOSE_P;
  } else if (val == "{") {
    tokenType = SimpleToken::TokenType::OPEN_B;
  } else if (val == "}") {
    tokenType = SimpleToken::TokenType::CLOSE_B;
  } else if (val == ";") {
    tokenType = SimpleToken::TokenType::SEMI_COLON;
  }

  // operators
  else if (val == "+") {
    tokenType = SimpleToken::TokenType::PLUS;
  } else if (val == "-") {
    tokenType = SimpleToken::TokenType::MINUS;
  } else if (val == "*") {
    tokenType = SimpleToken::TokenType::TIMES;
  } else if (val == "/") {
    tokenType = SimpleToken::TokenType::DIV;
  } else if (val == "%") {
    tokenType = SimpleToken::TokenType::MOD;
  } else if (val == "=") {
    tokenType = SimpleToken::TokenType::ASSIGN;
  }

  // condition operators
  else if (val == "==") {
    tokenType = SimpleToken::TokenType::EQUAL;
  } else if (val == ">") {
    tokenType = SimpleToken::TokenType::GREATER;
  } else if (val == ">=") {
    tokenType = SimpleToken::TokenType::GREATER_EQ;
  } else if (val == "<") {
    tokenType = SimpleToken::TokenType::LESS;
  } else if (val == "<=") {
    tokenType = SimpleToken::TokenType::LESS_EQ;
  } else if (val == "!=") {
    tokenType = SimpleToken::TokenType::NOT_EQ;
  } else if (val == "&&") {
    tokenType = SimpleToken::TokenType::AND;
  } else if (val == "||") {
    tokenType = SimpleToken::TokenType::OR;
  } else if (val == "!") {
    tokenType = SimpleToken::TokenType::NOT;
  } else if (SimpleSyntaxChecker::isConstant(val)) {
    tokenType = SimpleToken::TokenType::CONSTANT;
  } else if (SimpleSyntaxChecker::isName(val)) {
    tokenType = SimpleToken::TokenType::WORD;
  } else {
    tokenType = SimpleToken::TokenType::OTHER;
  }
}

bool operator==(const SimpleToken &token1, const SimpleToken &token2) {
  return token1.getVal() == token2.getVal() &&
         token1.getTokenType() == token2.getTokenType();
}
