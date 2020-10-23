#include "Dispatchers.h"

FollowsDispatcher::FollowsDispatcher(PqlToken firstArgument,
                                     PqlToken secondArgument,
                                     PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(firstArgument));
  pkbParameters.push_back(toParam(secondArgument));
}

bool FollowsDispatcher::booleanDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      return handler->follows(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->follows(*first, *second);
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      return handler->follows(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->follows(*first, *second);
    }
  }
  throw "Invalid: Parameters provided do not return boolean";
}

int FollowsDispatcher::dispatchPriority() {
  if (cachedPriority >= 0)
    return cachedPriority;

  cachedPriority = 0;
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      cachedPriority = 3;
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  return cachedPriority;
}

EvaluationTable FollowsDispatcher::resultDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      return toEvaluationTable(handler->follows(*first, *second));
    }
  }
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      return toEvaluationTable(handler->follows(*first, *second));
    }
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      return toEvaluationTable(handler->follows(*first, *second));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return toEvaluationTable(handler->follows(*first, *second));
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      return toEvaluationTable(handler->follows(*first, *second));
    }
  }
  throw "Invalid: Parameters provided do not return values";
}
