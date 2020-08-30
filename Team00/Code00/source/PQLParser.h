#pragma once
#include "PQL.h"
#include <string>
#include <vector>
std::vector<PqlToken> lex(std::string query);

ParsedQuery parse(std::vector<PqlToken> query);
