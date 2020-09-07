#include "Token.h"
#include "SyntaxChecker.h"
// constructor
Token::Token(std::string value) {
  val = value;
  translate();
}

// Getters
TokenEnum Token::getTokenEnum() const { return tokenEnum; }

std::string Token::getVal() const { return val; }

// Tranlate value  into TokenEnum
void Token::translate() {
  if (val == "(") {
    tokenEnum = TokenEnum::OPEN_P;
  } else if (val == ")") {
    tokenEnum = TokenEnum::CLOSE_P;
  } else if (val == "{") {
    tokenEnum = TokenEnum::OPEN_B;
  } else if (val == "}") {
    tokenEnum = TokenEnum::CLOSE_B;
  } else if (val == ";") {
    tokenEnum = TokenEnum::SEMI_COLON;
  }

  // operators
  else if (val == "+") {
    tokenEnum = TokenEnum::PLUS;
  } else if (val == "-") {
    tokenEnum = TokenEnum::MINUS;
  } else if (val == "*") {
    tokenEnum = TokenEnum::TIMES;
  } else if (val == "/") {
    tokenEnum = TokenEnum::DIV;
  } else if (val == "%") {
    tokenEnum = TokenEnum::MOD;
  } else if (val == "=") {
    tokenEnum = TokenEnum::ASSIGN;
  }

  // condition operators
  else if (val == "==") {
    tokenEnum = TokenEnum::EQUAL;
  } else if (val == ">") {
    tokenEnum = TokenEnum::GREATER;
  } else if (val == ">=") {
    tokenEnum = TokenEnum::GREATER_EQ;
  } else if (val == "<") {
    tokenEnum = TokenEnum::LESS;
  } else if (val == "<=") {
    tokenEnum = TokenEnum::LESS_EQ;
  } else if (val == "!=") {
    tokenEnum = TokenEnum::NOT_EQ;
  } else if (val == "&&") {
    tokenEnum = TokenEnum::AND;
  } else if (val == "||") {
    tokenEnum = TokenEnum::OR;
  } else if (val == "!") {
    tokenEnum = TokenEnum::NOT;
  } else if (SyntaxChecker::isConstant(val)) {
    tokenEnum = TokenEnum::CONSTANT;
  } else if (SyntaxChecker::isName(val)) {
    tokenEnum = TokenEnum::WORD;
  } else {
    tokenEnum = TokenEnum::OTHER;
  }
}

bool operator==(const Token &token1, const Token &token2) {
  return token1.getVal() == token2.getVal();
}
