#pragma once
#include <string>
class SyntaxChecker {
public:
  // check if input satisfies name grammar
  static bool isName(std::string input);

  // check if input statisfies constant grammar
  static bool isConstant(std::string input);
};
