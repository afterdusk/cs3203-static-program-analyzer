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

class SyntaxChecker {
private:
  // Check if input is a digit
  static bool isDigit(const char character);

  // Check if input is an alphabet
  static bool isAlphabet(const char character);

public:
  // check if input satisfies name grammar
  static bool isName(std::string input);

  // check if input statisfies constant grammar
  static bool isConstant(std::string input);
};
