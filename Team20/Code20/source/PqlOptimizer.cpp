#include "PqlOptimizer.h"
#include <unordered_map>

PqlOptimizer::PqlOptimizer(ParsedQuery parsedQuery) { pq = parsedQuery; }

ParsedQuery PqlOptimizer::optimize() {
  rewriteRelationshipsWithSpecifiedRawValueForElement();
  removeRedundantWithClauses();
  deleteDuplicateClauses();
  identifyImpossibleWiths();
  return pq;
}

void PqlOptimizer::rewriteRelationshipsWithSpecifiedRawValueForElement() {
  // 1. Find the matching with clauses
  std::unordered_map<std::string, PqlToken> synonymToNumberMapping;

  for (const auto &clause : pq.withs) {
    auto firstReference = clause.first;
    auto secondReference = clause.second;

    if (firstReference.referenceType == ReferenceType::ELEMENT &&
        secondReference.referenceType == ReferenceType::RAW_VALUE) {
      synonymToNumberMapping[firstReference.element.synonym] =
          secondReference.pqlToken;
    }
  }
  // 2. Find the matching
  // TODO: Talk to LJ. Only next next_t can't sub string for READ. I can either
  // check or
  for (auto &relationship : pq.relationships) {
    if (contains(entities, relationship.firstArgument.type) &&
        synonymToNumberMapping.find(relationship.firstArgument.value) !=
            synonymToNumberMapping.end()) {
      relationship.firstArgument =
          synonymToNumberMapping[relationship.firstArgument.value];
    }
    if (contains(entities, relationship.secondArgument.type) &&
        synonymToNumberMapping.find(relationship.secondArgument.value) !=
            synonymToNumberMapping.end()) {
      relationship.secondArgument =
          synonymToNumberMapping[relationship.secondArgument.value];
    }
  }
}
void PqlOptimizer::removeRedundantWithClauses() {
  WITHS newWiths;
  for (const auto &clause : pq.withs) {
    auto firstReference = clause.first;
    auto secondReference = clause.second;

    // Early evaluate withs that compare two raw tokens
    if (firstReference.referenceType == ReferenceType::RAW_VALUE &&
        secondReference.referenceType == ReferenceType::RAW_VALUE) {
      if (firstReference.pqlToken == secondReference.pqlToken) {
        continue;
      } else {
        throwSemanticError();
      }
    } else if (firstReference.referenceType == ReferenceType::ELEMENT &&
               secondReference.referenceType == ReferenceType::ELEMENT &&
               (firstReference.element == secondReference.element)) {
      // If the elements are equal on both sides, ignore.
      continue;
    }
    newWiths.push_back(clause);
  }
  pq.withs = newWiths;
}

template <class T>
std::vector<T> removeCommonElements(std::vector<T> &elements) {
  std::vector<T> newElements;
  for (auto &element : elements) {
    auto seen = false;
    for (auto &seenElements : newElements) {
      if (element == seenElements) {
        seen = true;
        break;
      }
    }
    if (seen) {
      continue;
    }
    newElements.push_back(element);
  }
  return newElements;
}

void PqlOptimizer::deleteDuplicateClauses() {
  pq.withs = removeCommonElements(pq.withs);
  pq.patterns = removeCommonElements(pq.patterns);
  pq.relationships = removeCommonElements(pq.relationships);
}

void PqlOptimizer::identifyImpossibleWiths() {
  for (const auto &clause : pq.withs) {
    auto firstReference = clause.first;
    auto secondReference = clause.second;

    if (firstReference.referenceType == ReferenceType::ELEMENT &&
        secondReference.referenceType == ReferenceType::ELEMENT) {

      if (firstReference.element.refType == AttributeRefType::STATEMENT_NUM &&
          secondReference.element.refType == AttributeRefType::STATEMENT_NUM) {
        auto firstDeclaration = pq.declarations[firstReference.element.synonym];
        auto secondDeclaration =
            pq.declarations[secondReference.element.synonym];
        if (firstDeclaration != secondDeclaration) {
          if (firstDeclaration != TokenType::STMT &&
              secondDeclaration != TokenType::STMT) {
            throwSemanticError();
          } else {
            // TODO: At least one of them is a synonym, and we can change the
            // declaration from synonym to other delcaration. Make sure arg list
            // is not violated.
          }
        }
      }
    }
  }
}
void PqlOptimizer::throwSemanticError() {
  if (pq.results.resultType == PqlResultType::Boolean) {
    throw PqlSemanticErrorWithBooleanResultException();
  } else {
    throw "ERROR: Some other semantic error";
  }
}
