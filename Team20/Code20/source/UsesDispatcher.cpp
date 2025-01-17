#include "Dispatchers.h"

UsesDispatcher::UsesDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                               PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(firstArgument));
  pkbParameters.push_back(toParam(secondArgument));
}

int UsesDispatcher::dispatchPriority() {
  if (cachedPriority >= 0)
    return cachedPriority;

  cachedPriority = 0;
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      cachedPriority = 2;
    }
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      cachedPriority = 3;
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      cachedPriority = 2;
    }
  }
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  if (Procedure *first = std::get_if<Procedure>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      cachedPriority = 2;
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      cachedPriority = 1;
    }
  }
  return cachedPriority;
}

bool UsesDispatcher::booleanDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return handler->uses(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->uses(*first, *second);
    }
  }
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return handler->uses(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->uses(*first, *second);
    }
  }
  throw "Invalid: Parameters provided do not return boolean";
}

EvaluationTable UsesDispatcher::resultDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
  }
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
  }
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
  }
  if (Procedure *first = std::get_if<Procedure>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return toEvaluationTable(handler->uses(*first, *second));
    }
  }
  throw "Invalid: Parameters provided do not return values";
}
