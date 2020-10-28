#include "Dispatchers.h"

WithElementPairDispatcher::WithElementPairDispatcher(
    TokenType firstType, Element firstElement, TokenType secondType,
    Element secondElement, PkbQueryInterface *queryHandler)
    : WithDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(PqlToken{firstType, firstElement.synonym}));
  pkbParameters.push_back(toParam(PqlToken{secondType, secondElement.synonym}));
  elements = {firstElement, secondElement};
  types = {firstType, secondType};
}

int WithElementPairDispatcher::dispatchPriority() {
  // With clauses should always be prioritised as results are restrictive
  return 1;
}

EvaluationTable WithElementPairDispatcher::resultDispatch() {
  COMPARABLE first =
      getComparable(types.first, elements.first.refType, pkbParameters[0]);
  COMPARABLE second =
      getComparable(types.second, elements.second.refType, pkbParameters[1]);
  TABLE *table = new TABLE;
  SYMBOL lhsSymbol = symbols[0];
  SYMBOL rhsSymbol = symbols[1];
  (*table)[lhsSymbol] = {};
  (*table)[rhsSymbol] = {};

  // If LHS is equal to RHS
  if (lhsSymbol == rhsSymbol) {
    if (JUST_VALUE *lhs = std::get_if<JUST_VALUE>(&first)) {
      for (std::vector<VALUE>::size_type i = 0; i < lhs->size(); i++) {
        (*table)[lhsSymbol].push_back((*lhs)[i]);
      }
    }
    if (VALUE_ATTR_PAIR *lhs = std::get_if<VALUE_ATTR_PAIR>(&first)) {
      SYMBOL lhsAttrSymbol =
          elementAttrToSymbol(types.first, elements.first).value();
      (*table)[lhsAttrSymbol] = {};
      for (std::vector<VALUE>::size_type i = 0; i < lhs->second.size(); i++) {
        (*table)[lhsSymbol].push_back(lhs->first[i]);
        (*table)[lhsAttrSymbol].push_back(lhs->second[i]);
      }
    }
    return EvaluationTable(table);
  }

  if (JUST_VALUE *lhs = std::get_if<JUST_VALUE>(&first)) {
    if (JUST_VALUE *rhs = std::get_if<JUST_VALUE>(&second)) {
      for (std::vector<VALUE>::size_type i = 0; i < lhs->size(); i++) {
        for (std::vector<VALUE>::size_type j = 0; j < rhs->size(); j++) {
          if ((*lhs)[i] == (*rhs)[j]) {
            (*table)[lhsSymbol].push_back((*lhs)[i]);
            (*table)[rhsSymbol].push_back((*rhs)[j]);
          }
        }
      }
    }
    if (VALUE_ATTR_PAIR *rhs = std::get_if<VALUE_ATTR_PAIR>(&second)) {
      SYMBOL rhsAttrSymbol =
          elementAttrToSymbol(types.second, elements.second).value();
      (*table)[rhsAttrSymbol] = {};
      for (std::vector<VALUE>::size_type i = 0; i < lhs->size(); i++) {
        for (std::vector<VALUE>::size_type j = 0; j < rhs->second.size(); j++) {
          if ((*lhs)[i] == rhs->second[j]) {
            (*table)[lhsSymbol].push_back((*lhs)[i]);
            (*table)[rhsSymbol].push_back(rhs->first[j]);
            (*table)[rhsAttrSymbol].push_back(rhs->second[j]);
          }
        }
      }
    }
  }
  if (VALUE_ATTR_PAIR *lhs = std::get_if<VALUE_ATTR_PAIR>(&first)) {
    SYMBOL lhsAttrSymbol =
        elementAttrToSymbol(types.first, elements.first).value();
    (*table)[lhsAttrSymbol] = {};
    if (JUST_VALUE *rhs = std::get_if<JUST_VALUE>(&second)) {
      for (std::vector<VALUE>::size_type i = 0; i < lhs->second.size(); i++) {
        for (std::vector<VALUE>::size_type j = 0; j < rhs->size(); j++) {
          if (lhs->second[i] == (*rhs)[j]) {
            (*table)[lhsSymbol].push_back(lhs->first[i]);
            (*table)[lhsAttrSymbol].push_back(lhs->second[i]);
            (*table)[rhsSymbol].push_back((*rhs)[j]);
          }
        }
      }
    }
    if (VALUE_ATTR_PAIR *rhs = std::get_if<VALUE_ATTR_PAIR>(&second)) {
      SYMBOL rhsAttrSymbol =
          elementAttrToSymbol(types.second, elements.second).value();
      (*table)[rhsAttrSymbol] = {};
      for (std::vector<VALUE>::size_type i = 0; i < lhs->second.size(); i++) {
        for (std::vector<VALUE>::size_type j = 0; j < rhs->second.size(); j++) {
          if (lhs->second[i] == rhs->second[j]) {
            (*table)[lhsSymbol].push_back(lhs->first[i]);
            (*table)[lhsAttrSymbol].push_back(lhs->second[i]);
            (*table)[rhsSymbol].push_back(rhs->first[j]);
            (*table)[rhsAttrSymbol].push_back(rhs->second[j]);
          }
        }
      }
    }
  }
  return EvaluationTable(table);
}
