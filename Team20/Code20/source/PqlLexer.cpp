#include "PqlLexer.h"
#include "Pql.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const char delimiter = '^';

bool isAlphaNumeric(const std::string &s) {
  for (const auto c : s) {
    if (!isalnum(c)) {
      return false;
    }
  }
  return true;
}

bool startsWithAlphabet(const std::string &s) {
  if (s.empty()) {
    return false;
  }
  return isalpha(s[0]);
}

bool isDigits(const std::string &s) {
  for (const auto c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}

bool isStringToken(const std::string &token) {
  return token.size() >= 2 && token.at(0) == '"' && token.back() == '"';
}

bool PqlLexer::isIdent(const std::string &s) {
  return startsWithAlphabet(s) && isAlphaNumeric(s);
}

PqlLexer::PqlLexer(std::string query) { this->query = query; }

std::vector<PqlToken> PqlLexer::lex() {
  sanitize(query);
  std::vector<std::string> rawTokens = delimit(query);
  std::vector<PqlToken> result;
  for (const auto token : rawTokens) {
    if (token.size() == 0) {
      continue;
    }

    if (stringTokenMap.find(token) != stringTokenMap.end()) {
      result.push_back(PqlToken{stringTokenMap[token], ""});
    } else if (isStringToken(token)) {
      const std::string content = token.substr(1, token.size() - 2);
      result.push_back(PqlToken{TokenType::STRING, content});
    } else if (isIdent(token)) {
      result.push_back(PqlToken{TokenType::SYNONYM, token});
    } else if (isDigits(token)) {
      result.push_back(PqlToken{TokenType::NUMBER, token});
    } else {
      throw "ERROR: Unrecognised token" + token + "\n";
    }
  }

  return result;
}

std::vector<std::string> PqlLexer::split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;

  while (getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}

void PqlLexer::sanitize(std::string &s) {
  size_t pos = 0;
  std::string search = "prog_line";
  std::string replace = "prog*line";
  while ((pos = s.find(search, pos)) != std::string::npos) {
    s.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

std::vector<std::string> PqlLexer::delimit(std::string s) {
  std::vector<char> result;
  bool isWithinStringLiterals = false;
  for (const char c : s) {
    switch (c) {
    // 0. Differentiate between cases within string literals and out of string
    // literals
    case '"':
      isWithinStringLiterals = !isWithinStringLiterals;
      break;
    // 0. Replace spaces w delimiter
    case ' ':
    case '\n':
    case '\t':
      if (!isWithinStringLiterals) {
        result.push_back(delimiter);
        continue;
      }
      break;
    // 1. Prepend delimiter for special characters
    // 1A. Characters that may appear within string literals
    case ')':
    case '(':
      if (!isWithinStringLiterals) {
        result.push_back(delimiter);
      }
      break;
    // 1B. Characters that wont appear within string literals
    case ',':
    case ';':
    case '_':
    case '<':
    case '>':
    case '.':
    case '=':
      result.push_back(delimiter);
      break;
    default:
      break;
    }
    // 2. Push character into the token array
    result.push_back(c);
    // 3. Postpend delimiters for special characters
    switch (c) {
    // 3A. Characters that might appear within string literals
    case '(':
    case ')':
      if (!isWithinStringLiterals) {
        result.push_back(delimiter);
      }
      break;
    // 3B. Characters that won't appear within string literals
    case ';':
    case '_':
    case ',':
    case '<':
    case '>':
    case '.':
    case '=':
      result.push_back(delimiter);
      break;
    default:
      break;
    }
  }
  if (isWithinStringLiterals) {
    // TODO:
    std::cout << "EXCEPTION";
  }
  return split(std::string(result.begin(), result.end()), delimiter);
}
