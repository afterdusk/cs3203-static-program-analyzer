#include "Tokenizer.h"
#include <list>
#include <sstream>
#include <string>

// Tokenizer part

// Constructor
Tokenizer::Tokenizer(std::string input) : program(input) {}

// main program that change string of program into list of tokens
std::list<Token> Tokenizer::tokenize() const {
  // divide program into lines using '\n'
  std::list<std::string> lines = divideString(program, '\n');

  // divide lines into list of string
  std::list<std::string> words;
  std::list<Token> tokens;
  std::list<std::string>::const_iterator iteratorLines;
  std::list<std::string>::const_iterator iteratorWords;
  for (iteratorLines = lines.begin(); iteratorLines != lines.end();
       ++iteratorLines) {
    // divide each line into a list of strings
    words.splice(words.end(), divideString(*iteratorLines, ' '));
  }
  for (iteratorWords = words.begin(); iteratorWords != words.end();
       ++iteratorWords) {
    tokens.splice(tokens.end(), tokenizeWord(*iteratorWords));
  }
  return tokens;
}

// divide string into list of strings using the delimiter
std::list<std::string> Tokenizer::divideString(std::string input,
                                               char delim) const {
  std::stringstream ss(input);
  std::list<std::string> strings;
  std::string temp;
  while (std::getline(ss, temp, delim)) {
    strings.push_back(temp);
  }
  return strings;
}

// Divide a word into list of strings
std::list<Token> Tokenizer::tokenizeWord(std::string word) const {
  std::list<Token> tokens;
  size_t last_token_pos = -1;
  for (size_t i = 0; i < word.size(); i++) {
    // only single character: +/-/*///%/!/{/}/(/)/; -> 33, 37, 40-43, 45 47, 59
    // 123, 125,
    if (word[i] == 33 || word[i] == 37 || word[i] == 45 || word[i] == 47 ||
        word[i] == 59 || word[i] == 123 || word[i] == 125 ||
        (word[i] >= 40 && word[i] <= 45)) {
      // check if there is any untranslated piece
      if (i > last_token_pos + 1) {
        tokens.push_back(
            Token(word.substr(last_token_pos + 1, i - last_token_pos - 1)));
      }
      tokens.push_back(Token(word.substr(i, 1)));
      last_token_pos = i;
    }
    // possibly combine with other character: =/>/</|/&
    //=/>/<
    else if (word[i] >= 60 && word[i] <= 62) {
      // check if there is any untranslated piece
      if (i > last_token_pos + 1) {
        tokens.push_back(
            Token(word.substr(last_token_pos + 1, i - last_token_pos - 1)));
      }
      if (word[i + 1] == 61) {
        tokens.push_back(Token(word.substr(i, 2)));
        last_token_pos = i + 1;
        i++;
      } else {
        tokens.push_back(Token(word.substr(i, 1)));
        last_token_pos = i;
      }
    } else if (word[i] == 38 && word[i] == 124) {
      // check if there is any untranslated piece
      if (i > last_token_pos + 1) {
        tokens.push_back(
            Token(word.substr(last_token_pos + 1, i - last_token_pos - 1)));
      }
      if (word[i + 1] == word[i]) {
        tokens.push_back(Token(word.substr(i, 2)));
        last_token_pos = i + 1;
        i++;
      } else {
        tokens.push_back(Token(word.substr(i, 1)));
        last_token_pos = i;
      }
    }
  }
  if (last_token_pos < word.size() - 1) {
    tokens.push_back(Token(word.substr(last_token_pos + 1)));
  }
  return tokens;
}

// Token part

// constructor
Token::Token(std::string value) {
  val = value;
  translate();
}

// Getters
TokenEnum Token::getToken() const { return tokenEnum; }

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
  }

  // words
  else if (val == "procedure") {
    tokenEnum = TokenEnum::PROCEDURE;
  } else if (val == "if") {
    tokenEnum = TokenEnum::IF;
  } else if (val == "while") {
    tokenEnum = TokenEnum::WHILE;
  } else if (val == "call") {
    tokenEnum = TokenEnum::CALL;
  } else if (val == "read") {
    tokenEnum = TokenEnum::READ;
  } else if (val == "print") {
    tokenEnum = TokenEnum::PRINT;
  }
  // check syntax and return Token::CONSTANT and NAME
  // to be replaced by an IllegalExpressionException
  throw -1;
}
