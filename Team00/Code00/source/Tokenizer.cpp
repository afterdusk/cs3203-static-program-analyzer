#include "Tokenizer.h"
#include <sstream>
#include <string>

// Tokenizer part

// Constructor
Tokenizer::Tokenizer(std::string input) : program(input) {}

// main program that change string of program into vector of tokens
std::vector<Token> Tokenizer::tokenize() const {
  // divide program into lines using '\n'
  std::vector<std::string> divideByTabs = divideString(program, '\t');
  std::vector<std::string> divideByLines;
  std::vector<std::string> divideBySpaces;
  // divide lines into list of string
  std::vector<std::string> words;
  std::vector<Token> tokens;
  for (size_t i = 0; i < divideByTabs.size(); i++) {
    divideByLines.insert(divideByLines.end(),
                         divideString(divideByTabs[i], '\n').begin(),
                         divideString(divideByTabs[i], '\n').end());
  }
  for (size_t i = 0; i < divideByLines.size(); i++) {
    divideBySpaces.insert(divideBySpaces.end(),
                          divideString(divideByLines[i], ' ').begin(),
                          divideString(divideByLines[i], ' ').end());
  }
  for (size_t i = 0; i < divideBySpaces.size(); i++) {
    tokens.insert(tokens.end(), tokenizeWord(divideBySpaces[i]).begin(),
                  tokenizeWord(divideBySpaces[i]).end());
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
    }
    // || and &&
    else if (word[i] == 38 && word[i] == 124) {
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
