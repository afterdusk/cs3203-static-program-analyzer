#pragma once

#include <string>
#include <vector>

#include "Pql.h"

class PqlLexer {
public:
  PqlLexer(std::string query);
  std::vector<PqlToken> lex();

private:
  std::string query;
  std::vector<std::string> split(const std::string &s, char delim);
  std::vector<std::string> delimit(std::string s);
};