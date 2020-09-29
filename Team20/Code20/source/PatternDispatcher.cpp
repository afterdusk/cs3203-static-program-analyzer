#include "Dispatchers.h"

PatternDispatcher::PatternDispatcher(ParsedPattern pp,
                                     PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(pp.synonym));
  pkbParameters.push_back(toParam(pp.lhs));
  pkbParameters.push_back(pp.rhs);
}

EvaluationTable PatternDispatcher::resultDispatch() {
  if (Statement *first = std::get_if<Statement>(&pkbParameters[0])) {
    if (String *second = std::get_if<String>(&pkbParameters[1])) {
      PatternSpec third = std::get<PatternSpec>(pkbParameters[2]);
      return toEvaluationTable((handler->match(*first, *second, third)));
    }
    if (Variable *second = std::get_if<Variable>(&pkbParameters[1])) {
      PatternSpec third = std::get<PatternSpec>(pkbParameters[2]);
      return toEvaluationTable((handler->match(*first, *second, third)));
    }
    if (Underscore *second = std::get_if<Underscore>(&pkbParameters[1])) {
      PatternSpec third = std::get<PatternSpec>(pkbParameters[2]);
      return toEvaluationTable((handler->match(*first, *second, third)));
    }
  }
  throw "Invalid: Parameters provided do not return values";
}
