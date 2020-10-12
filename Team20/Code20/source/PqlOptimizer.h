#pragma once
#include <string>
#include <vector>

#include "Pql.h"
#include "PqlExceptions.h"
class PqlOptimizer {

public:
  ParsedQuery optimize();
  PqlOptimizer(ParsedQuery parsedQuery);

private:
  ParsedQuery pq;

  /** @brief Rewrites relationship clause if raw value for element was
   * specified. Algorithm retrieves all specified raw values for a synonym. It
   * then rewrites synonyms if they are used in the relationship clauses into
   * these raw values. Modifies the query in-place.
   */
  void rewriteRelationshipsWithSpecifiedRawValueForElement();

  /** @brief  Remove with clauses if both sides of the references have the same
   * values, throws exception if two raw values are compared and they do not
   * have the same values.
   */
  void removeRedundantWithClauses();

  /** @brief  Remove duplicate relationship, pattern, and with clauses
   */
  void deleteDuplicateClauses();

  /** @brief Early terminate if two statement number attribute refs are compared
   * and are of different entity types.
   */
  void identifyImpossibleWiths();

  /** @brief Helper function to throw semantic error
   */
  void throwSemanticError();
};
