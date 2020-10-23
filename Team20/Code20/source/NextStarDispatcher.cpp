#include "Dispatchers.h"

NextStarDispatcher::NextStarDispatcher(PqlToken firstArgument,
                                       PqlToken secondArgument,
                                       PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(firstArgument));
  pkbParameters.push_back(toParam(secondArgument));
}

int NextStarDispatcher::dispatchPriority() {
  if (cachedPriority >= 0)
    return cachedPriority;

  cachedPriority = 15;
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      cachedPriority = 20;
    }
  }
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      cachedPriority = 20;
    }
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      cachedPriority = 30;
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      cachedPriority = 20;
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      cachedPriority = 20;
    }
  }
  return cachedPriority;
}

bool NextStarDispatcher::booleanDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      return handler->nextStar(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->nextStar(*first, *second);
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      return handler->nextStar(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->nextStar(*first, *second);
    }
  }
  throw "Invalid: Parameters provided do not return boolean";
}

EvaluationTable NextStarDispatcher::resultDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      return toEvaluationTable(handler->nextStar(*first, *second));
    }
  }
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (LineNumber *second = std::get_if<LineNumber>(&pkbParameters[1])) {
      return toEvaluationTable(handler->nextStar(*first, *second));
    }
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      return toEvaluationTable(handler->nextStar(*first, *second));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return toEvaluationTable(handler->nextStar(*first, *second));
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (Statement *second = std::get_if<Statement>(&pkbParameters[1])) {
      return toEvaluationTable(handler->nextStar(*first, *second));
    }
  }
  throw "Invalid: Parameters provided do not return values";
}
