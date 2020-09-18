/** @file */
#pragma once

#include "Pkb.h"
#include "PkbQueryEntityTypes.h"
#include "PkbQueryInterface.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class TokenType {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ELSE,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,

  SYNONYM,
  NUMBER,
  STRING,
  UNDERSCORE,

  SELECT,
  PARENT,
  PARENT_T,
  FOLLOWS,
  FOLLOWS_T,
  MODIFIES,
  USES,
  MATCH, // Representation of pattern clause as a relationship

  SUCH,
  THAT,
  PATTERN,

  SEMICOLON,
  COMMA,
  OPEN_PARENTHESIS,
  CLOSED_PARENTHESIS,
};

/** @brief Token used as intermediary between the lexer and the parser
 */
struct PqlToken {
  TokenType type;
  std::string value;
  bool operator==(const PqlToken &other) const {
    return type == other.type && value == other.value;
  }
  PqlToken(TokenType specifiedTokenType, std::string specifiedValue = "")
      : type{specifiedTokenType}, value{specifiedValue} {}
};

/** @brief ParsedRelationship holds the information of a parsed relationship
 */
struct ParsedRelationship {
  TokenType relationship;
  PqlToken first_argument;
  PqlToken second_argument;
  bool operator==(const ParsedRelationship &other) const {
    return relationship == other.relationship &&
           first_argument == other.first_argument &&
           second_argument == other.second_argument;
  }
};

enum class ExpressionSpecType { CompleteMatch, SubTreeMatch, Any };

struct ParsedPattern {
  PqlToken synonym;
  PqlToken lhs;
  PatternSpec rhs;
  bool operator==(const ParsedPattern &other) const {
    return lhs == other.lhs && rhs == other.rhs;
  }
};

typedef std::unordered_map<std::string, TokenType> DECLARATIONS;
typedef std::vector<ParsedRelationship> RELATIONSHIPS;
typedef std::vector<std::string> RESULTS;
typedef std::vector<ParsedPattern> PATTERNS;

struct ParsedQuery {
  DECLARATIONS declaration_clause;
  RESULTS result_clause;
  RELATIONSHIPS relationship_clauses;
  PATTERNS pattern_clauses;
};

namespace PQL {
/** @brief Lexes a string into a vector of tokens.
 *  Lexing is done by splitting according to spaces and splitting before or
 *  after special characters. Exceptions are thrown on syntatically incorrect
 *  queries.
 *  @param query String containing pql query.
 *  @return vector of tokens that can be passed into the parse function.
 */
std::vector<PqlToken> lex(std::string query);

/** @brief Parses a vector of PqlTokens into a ParsedQuery object.
 *  parse internally calls subparses that will parse the clauses.
 *  @param query Vector of tokens.
 *  @return parsed query object to be used by the evaulator.
 */
ParsedQuery parse(std::vector<PqlToken> query);

/** @brief Evaluates a parsed query object and returns the result.
 *  This function serves as the entrypoint for the PQLEvaluator.
 */
std::list<std::string> evaluate(ParsedQuery pq,
                                PkbQueryInterface *queryHandler);
}; // namespace PQL
