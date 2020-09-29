#include "Dispatchers.h"

ModifiesDispatcher::ModifiesDispatcher(PqlToken firstArgument,
                                       PqlToken secondArgument,
                                       PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(firstArgument));
  pkbParameters.push_back(toParam(secondArgument));
}

bool ModifiesDispatcher::booleanDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return handler->modifies(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->modifies(*first, *second);
    }
  }
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return handler->modifies(*first, *second);
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return handler->modifies(*first, *second);
    }
  }
  throw "Invalid: Parameters provided do not return boolean";
}

EvaluationTable ModifiesDispatcher::resultDispatch() {
  if (LineNumber *first = std::get_if<LineNumber>(&pkbParameters[0])) {
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
  }
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
  }
  if (String *first = std::get_if<String>(&pkbParameters[0])) {
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
  }
  if (Procedure *first = std::get_if<Procedure>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      return toEvaluationTable(handler->modifies(*first, *second));
    }
  }
  throw "Invalid: Parameters provided do not return values";
}
