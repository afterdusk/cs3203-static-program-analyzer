/** @file */
#pragma once
#include "PQL.h"
#include <string>
#include <vector>

/** @brief Lexes a string into a vector of tokens.
 * Lexing is done by splitting according to spaces and splitting before or after
 * special characters. Exceptions are thrown on syntatically incorrect queries.
 * @param query String containing pql query.
 * @return vector of tokens that can be passed into the parse function.
 */
std::vector<PqlToken> lex(std::string query);

/** @brief Parses a vector of PqlTokens into a ParsedQuery object.
 * parse internally calls subparses that will parse the clauses.
 * @param query Vector of tokens.
 * @return parsed query object to be used by the evaulator.
 */
ParsedQuery parse(std::vector<PqlToken> query);
