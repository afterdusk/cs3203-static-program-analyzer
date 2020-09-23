#include "Dispatchers.h"

SelectDispatcher::SelectDispatcher(PqlToken token,
                                   PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(token));
}

ClauseResult SelectDispatcher::resultDispatch() {
  if (Statement *entity = std::get_if<Statement>(&pkbParameters[0])) {
    return toClauseResult(handler->select(*entity));
  }
  if (Procedure *entity = std::get_if<Procedure>(&pkbParameters[0])) {
    return toClauseResult(handler->select(*entity));
  }
  if (Variable *entity = std::get_if<Variable>(&pkbParameters[0])) {
    return toClauseResult(handler->select(*entity));
  }
  if (Constant *entity = std::get_if<Constant>(&pkbParameters[0])) {
    return toClauseResult(handler->select(*entity));
  }
  throw "Invalid: Parameters provided do not return ClauseResult";
}
