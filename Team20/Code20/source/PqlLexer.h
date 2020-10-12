#pragma once

#include <string>
#include <vector>

#include "Pql.h"

class PqlLexer {
public:
  PqlLexer(std::string query);

  /** @brief Main entrypoint function for PqlLexer.
   *  Lexes a string into a vector of tokens.
   *  Lexing is done by splitting according to spaces and splitting before or
   *  after special characters. Exceptions are thrown on syntatically incorrect
   *  queries.
   *  @param query String containing pql query.
   *  @return vector of tokens that can be passed into the parse function.
   */
  std::vector<PqlToken> lex();

private:
  // Query to lex to tokens
  std::string query;

  /** @brief Delimits query into vector of strings
   *  @param s String containing pql query.
   *  @param delim Character delimiter to split by
   *  @return vector of strings
   */
  std::vector<std::string> delimit(std::string s);

  /** @brief Splits string by the delimiter
   *  @param s String containing pql query.
   *  @param delim Character delimiter to split by
   *  @return vector of strings
   */
  std::vector<std::string> split(const std::string &s, char delim);

  /** @brief Remaps problematic parts of the query into a more feasible query.
   *  Currently, this only remaps `prog_line`. Modifies query in-place.
   *  @param s String containing pql query.
   *  @return void
   */
  void sanitize(std::string &s);
};
