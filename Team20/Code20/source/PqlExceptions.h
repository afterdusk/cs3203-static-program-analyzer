#pragma once

#include <string>
/*
 * Exception when its a semantic boolean error
 */
class PqlSemanticErrorWithBooleanResultException : public std::exception {
public:
  const char *what() const throw() override;
};
