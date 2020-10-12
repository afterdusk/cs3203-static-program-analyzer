#include "PqlExceptions.h"

const char *PqlSemanticErrorWithBooleanResultException::what() const throw() {
  return "Some semantic error";
}
