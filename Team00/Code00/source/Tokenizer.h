#include <list>
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

  EQUAL,
  GREATER,
  GREATER_EQ,
  LESS,
  LESS_EQ,
  NOT_EQ,

  AND,
  OR,
  NOT,

  PROCEDURE, //"procedure"
  IF,
  WHILE,
  CALL,
  READ,
  PRINT,

  NAME, // procedure or variable name
  CONSTANT,
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

class Tokenizer {
private:
  std::string program;

  // divide input to list of strings
  std::list<std::string> divideString(std::string input, char delim) const;

  // translate word into list of tokens
  std::list<Token> tokenizeWord(std::string word) const;

public:
  // constructor
  Tokenizer(std::string input);

  // main program that divide input program to tokens
  std::list<Token> tokenize() const;
};
