#pragma once

#include "SimpleToken.h"
#include <string>
#include <vector>

class Tokenizer {
private:
  std::string program;

  // divide input to list of strings
  std::vector<std::string> divideString(std::string input, char delim) const;

  // translate word into list of tokens
  std::vector<SimpleToken> tokenizeWord(std::string word) const;

public:
  // constructor
  Tokenizer(std::string input);

  // main program that divide input program to tokens
  std::vector<SimpleToken> tokenize() const;
};
