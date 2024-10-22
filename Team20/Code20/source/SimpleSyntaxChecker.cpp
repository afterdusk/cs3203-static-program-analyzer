#include "SimpleSyntaxChecker.h"
#include <cctype>
#include <string>

// check if input satisfies name grammar
bool SimpleSyntaxChecker::isName(std::string input) {
  if (!isalpha(input[0])) {
    return false;
  }
  for (size_t i = 1; i < input.size(); i++) {
    if (!isalpha(input[i]) && !isdigit(input[i])) {
      return false;
    }
  }
  return true;
}

// check if the input satisfies constant grammar
bool SimpleSyntaxChecker::isConstant(std::string input) {
  if (input.size() > 1 && input[0] == '0') {
    return false;
  }
  for (size_t i = 0; i < input.size(); i++) {
    if (!isdigit(input[i])) {
      return false;
    }
  }
  return true;
}
