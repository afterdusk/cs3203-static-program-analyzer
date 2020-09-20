#include "Tokenizer.h"
#include "ParseExceptions.h"
#include <sstream>
#include <string>

// Tokenizer part

// Constructor
Tokenizer::Tokenizer(std::string input) : program(input) {}

// main program that change string of program into vector of tokens
std::vector<Token> Tokenizer::tokenize() const {
  char delimiters[4] = {'\n', '\r', '\t', ' '};
  // divide program into vector of strings using delimiters
  std::vector<std::string> tmp;
  std::vector<std::string> tmp_divided;
  std::vector<std::string> result;
  tmp_divided.push_back(program);
  for (int i = 0; i < 4; i++) {
    for (size_t j = 0; j < tmp_divided.size(); j++) {
      tmp = divideString(tmp_divided[j], delimiters[i]);
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    if (i != 3) {
      tmp_divided = result;
      result.clear();
    }
  }
  std::vector<Token> tmp_tokens;
  std::vector<Token> tokens;
  for (size_t i = 0; i < result.size(); i++) {
    if (result[i] != "") {
      tmp_tokens = tokenizeWord(result[i]);
      tokens.insert(tokens.end(), tmp_tokens.begin(), tmp_tokens.end());
    }
  }
  std::vector<Token> illegalExpressions;
  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i].getTokenEnum() == TokenEnum::OTHER) {
      illegalExpressions.push_back(tokens[i]);
    }
  }
  if (!illegalExpressions.empty()) {
    throw IllegalExpressionException(illegalExpressions);
  }
  return tokens;
}

// divide string into list of strings using the delimiter
std::vector<std::string> Tokenizer::divideString(std::string input,
                                                 char delim) const {
  std::stringstream ss(input);
  std::vector<std::string> strings;
  std::string temp;
  while (std::getline(ss, temp, delim)) {
    strings.push_back(temp);
  }
  return strings;
}

// Divide a word into list of strings
std::vector<Token> Tokenizer::tokenizeWord(std::string word) const {
  std::vector<Token> tokens;
  int last_token_pos = -1;
  for (size_t i = 0; i < word.size(); i++) {
    // only single character: +/-/*///%/{/}/(/)/; -> 37, 40-43, 45 47, 59
    // 123, 125,
    if (word[i] == 37 || word[i] == 45 || word[i] == 47 || word[i] == 59 ||
        word[i] == 123 || word[i] == 125 || (word[i] >= 40 && word[i] <= 45)) {
      // check if there is any untranslated piece
      if ((int)i > last_token_pos + 1) {
        tokens.push_back(
            Token(word.substr(last_token_pos + 1, i - last_token_pos - 1)));
      }
      tokens.push_back(Token(word.substr(i, 1)));
      last_token_pos = i;
    }
    // possibly combine with other character: =/>/</|/&/!
    //=/>/</!
    else if ((word[i] >= 60 && word[i] <= 62) || word[i] == 33) {
      // check if there is any untranslated piece
      if ((int)i > last_token_pos + 1) {
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
    }
    // || and &&
    else if (word[i] == 38 && word[i] == 124) {
      // check if there is any untranslated piece
      if ((int)i > last_token_pos + 1) {
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
  if (last_token_pos < (int)word.size() - 1) {
    tokens.push_back(Token(word.substr(last_token_pos + 1)));
  }
  return tokens;
}
