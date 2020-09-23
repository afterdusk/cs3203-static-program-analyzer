#include "SimpleTokenizer.h"
#include "SimpleParseExceptions.h"
#include <sstream>
#include <string>

// Tokenizer part

// Constructor
Tokenizer::Tokenizer(std::string input) : program(input) {}

// main program that change string of program into vector of tokens
std::vector<SimpleToken> Tokenizer::tokenize() const {
  char delimiters[4] = {'\n', '\r', '\t', ' '};
  // divide program into vector of strings using delimiters
  std::vector<std::string> tmp;
  std::vector<std::string> tmpDivided;
  std::vector<std::string> result;
  tmpDivided.push_back(program);
  for (int i = 0; i < 4; i++) {
    for (size_t j = 0; j < tmpDivided.size(); j++) {
      tmp = divideString(tmpDivided[j], delimiters[i]);
      result.insert(result.end(), tmp.begin(), tmp.end());
    }
    if (i != 3) {
      tmpDivided = result;
      result.clear();
    }
  }
  std::vector<SimpleToken> tmpTokens;
  std::vector<SimpleToken> tokens;
  for (size_t i = 0; i < result.size(); i++) {
    if (result[i] != "") {
      tmpTokens = tokenizeWord(result[i]);
      tokens.insert(tokens.end(), tmpTokens.begin(), tmpTokens.end());
    }
  }
  std::vector<SimpleToken> illegalExpressions;
  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i].getTokenType() == SimpleToken::TokenType::OTHER) {
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
std::vector<SimpleToken> Tokenizer::tokenizeWord(std::string word) const {
  std::vector<SimpleToken> tokens;
  int lastTokenPos = -1;
  for (size_t i = 0; i < word.size(); i++) {
    // only single character: +/-/*///%/{/}/(/)/; -> 37, 40-43, 45 47, 59
    // 123, 125,
    if (word[i] == '+' || word[i] == '-' || word[i] == '*' || word[i] == '/' ||
        word[i] == '%' || word[i] == '{' || word[i] == '}' || word[i] == '(' ||
        word[i] == ')' || word[i] == ';') {
      // check if there is any untranslated piece
      if ((int)i > lastTokenPos + 1) {
        tokens.push_back(
            SimpleToken(word.substr(lastTokenPos + 1, i - lastTokenPos - 1)));
      }
      tokens.push_back(SimpleToken(word.substr(i, 1)));
      lastTokenPos = i;
    }
    // possibly combine with other character: =/>/</|/&/!
    //=/>/</!
    else if (word[i] == '=' || word[i] == '>' || word[i] == '<' ||
             word[i] == '!') {
      // check if there is any untranslated piece
      if ((int)i > lastTokenPos + 1) {
        tokens.push_back(
            SimpleToken(word.substr(lastTokenPos + 1, i - lastTokenPos - 1)));
      }
      if (word[i + 1] == '=') {
        tokens.push_back(SimpleToken(word.substr(i, 2)));
        lastTokenPos = i + 1;
        i++;
      } else {
        tokens.push_back(SimpleToken(word.substr(i, 1)));
        lastTokenPos = i;
      }
    }
    // || and &&
    else if (word[i] == '|' || word[i] == '&') {
      // check if there is any untranslated piece
      if ((int)i > lastTokenPos + 1) {
        tokens.push_back(
            SimpleToken(word.substr(lastTokenPos + 1, i - lastTokenPos - 1)));
      }
      if (word[i + 1] == word[i]) {
        tokens.push_back(SimpleToken(word.substr(i, 2)));
        lastTokenPos = i + 1;
        i++;
      } else {
        tokens.push_back(SimpleToken(word.substr(i, 1)));
        lastTokenPos = i;
      }
    }
  }
  if (lastTokenPos < (int)word.size() - 1) {
    tokens.push_back(SimpleToken(word.substr(lastTokenPos + 1)));
  }
  return tokens;
}
