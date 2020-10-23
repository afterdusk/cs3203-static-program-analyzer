#include "Dispatchers.h"

CallsDispatcher::CallsDispatcher(PqlToken firstArgument,
                                 PqlToken secondArgument,
                                 PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(firstArgument));
  pkbParameters.push_back(toParam(secondArgument));
}

int CallsDispatcher::dispatchPriority() {
  if (cachedPriority >= 0)
    return cachedPriority;

  cachedPriority = 0;
  if (Procedure *first = std::get_if<Procedure>(&pkbParameters[0])) {
    if (Procedure *second = std::get_if<Procedure>(&pkbParameters[1])) {
      cachedPriority = 2;
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (Procedure *second = std::get_if<Procedure>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (Procedure *second = std::get_if<Procedure>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  return cachedPriority;
}

bool CallsDispatcher::booleanDispatch() {
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return handler->calls(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->calls(*first, *second);
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return handler->calls(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->calls(*first, *second);
    }
  }
  throw "Invalid: Parameters provided do not return boolean";
}

EvaluationTable CallsDispatcher::resultDispatch() {
  if (Procedure *first = std::get_if<Procedure>(&pkbParameters[0])) {
    if (Procedure *second = std::get_if<Procedure>(&pkbParameters[1])) {
      return toEvaluationTable(handler->calls(*first, *second));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return toEvaluationTable(handler->calls(*first, *second));
    }
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return toEvaluationTable(handler->calls(*first, *second));
    }
  }
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (Procedure *second = std::get_if<Procedure>(&pkbParameters[1])) {
      return toEvaluationTable(handler->calls(*first, *second));
    }
  }
  if (Underscore *first = std::get_if<Underscore>(&pkbParameters[0])) {
    if (Procedure *second = std::get_if<Procedure>(&pkbParameters[1])) {
      return toEvaluationTable(handler->calls(*first, *second));
    }
  }
  throw "Invalid: Parameters provided do not return values";
}
