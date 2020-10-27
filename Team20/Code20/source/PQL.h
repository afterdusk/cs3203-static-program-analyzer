/** @file */
#pragma once

#include "PkbQueryEntityTypes.h"
#include "PkbQueryInterface.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class TokenType {
  STMT,
  PROG_LINE,
  READ,
  PRINT,
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
  AFFECTS,
  AFFECTS_T,
  NEXT,
  NEXT_T,
  CALL,
  CALL_T,
  MODIFIES,
  USES,
  MATCH, // Representation of pattern clause as a relationship

  SUCH,
  THAT,
  AND,
  PATTERN,
  WITH,

  SEMICOLON,
  COMMA,
  DOT,
  OPEN_PARENTHESIS,
  CLOSED_PARENTHESIS,
  OPEN_ANGLED_BRACKET,
  CLOSED_ANGLED_BRACKET,

  PROCNAME,
  VARNAME,
  VALUE,
  STATEMENT_NUM,
  BOOLEAN,
  EQUALS
};

/** @brief Token used as intermediary between the lexer and the parser
 */
struct PqlToken {
  enum TokenType type;
  std::string value;
  bool operator==(const PqlToken &other) const {
    return type == other.type && value == other.value;
  }
  // Default constructor to construct an empty PqlToken
  PqlToken() : type{TokenType::STRING}, value{""} {}
  PqlToken(enum TokenType specifiedTokenType, std::string specifiedValue = "")
      : type{specifiedTokenType}, value{specifiedValue} {}
};

enum class AttributeRefType {
  PROCNAME,
  VARNAME,
  VALUE,
  STATEMENT_NUM,
  NONE,
};

struct Element {
  std::string synonym;
  AttributeRefType refType;
  bool operator==(const Element &other) const {
    return synonym == other.synonym && refType == other.refType;
  }
};

// Used in attrCompare
enum class ReferenceType { ELEMENT, RAW_VALUE };
struct Reference {
  ReferenceType referenceType;
  PqlToken pqlToken;
  Element element;
  Reference(PqlToken specifiedToken)
      : referenceType{ReferenceType::RAW_VALUE}, pqlToken{specifiedToken},
        element{Element()} {}
  Reference(Element specifiedElement)
      : referenceType{ReferenceType::ELEMENT}, pqlToken{PqlToken()},
        element{specifiedElement} {}

  bool operator==(const Reference &other) const {
    return referenceType == other.referenceType && pqlToken == other.pqlToken &&
           element == other.element;
  }
};

typedef std::vector<Element> TUPLE;

enum class PqlResultType { Boolean, Tuple };

struct PqlResult {
  PqlResultType resultType;
  TUPLE results;
  bool operator==(const PqlResult &other) const {
    return resultType == other.resultType && results == other.results;
  }
};

/** @brief ParsedRelationship holds the information of a parsed relationship
 */
struct ParsedRelationship {
  enum TokenType relationship;
  PqlToken firstArgument;
  PqlToken secondArgument;
  bool operator==(const ParsedRelationship &other) const {
    return relationship == other.relationship &&
           firstArgument == other.firstArgument &&
           secondArgument == other.secondArgument;
  }
};
/** @brief ParsedPattern holds information of a parsed pattern clause
 */
struct ParsedPattern {
  PqlToken synonym;
  PqlToken lhs;
  PatternSpec rhs;
  bool operator==(const ParsedPattern &other) const {
    return lhs == other.lhs && rhs == other.rhs;
  }
};

typedef std::unordered_map<std::string, enum TokenType> DECLARATIONS;
typedef PqlResult RESULTS;
typedef std::vector<ParsedRelationship> RELATIONSHIPS;
typedef std::vector<ParsedPattern> PATTERNS;
typedef std::vector<std::pair<Reference, Reference>> WITHS;

struct ParsedQuery {
  DECLARATIONS declarations;
  RESULTS results;
  RELATIONSHIPS relationships;
  PATTERNS patterns;
  WITHS withs;
};

extern std::unordered_map<std::string, enum TokenType> stringTokenMap;

namespace Pql {
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
 *  @param pq parsed query object returned by `PQL::parse`
 *  @param queryHandler the PKB object that will be queried
 *  @param result list to be populated with query results
 */
void evaluate(ParsedQuery pq, PkbQueryInterface *queryHandler,
              std::list<std::string> &result);
}; // namespace Pql
