#include "PQL.h"
#include "PQLLexer.h"
#include "PQLParser.h"

std::unordered_map<std::string, TokenType> stringTokenMap = {
    {";", TokenType::SEMICOLON},
    {"stmt", TokenType::STMT},
    {"read", TokenType::READ},
    {"print", TokenType::PRINT},
    {"call", TokenType::CALL},
    {"while", TokenType::WHILE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"assign", TokenType::ASSIGN},
    {"variable", TokenType::VARIABLE},
    {"constant", TokenType::CONSTANT},
    {"procedure", TokenType::PROCEDURE},
    {"assign", TokenType::ASSIGN},
    {"Select", TokenType::SELECT},
    {"such", TokenType::SUCH},
    {"that", TokenType::THAT},
    {"Follows", TokenType::FOLLOWS},
    {"Follows*", TokenType::FOLLOWS_T},
    {"Parent", TokenType::PARENT},
    {"Parent*", TokenType::PARENT_T},
    {"Uses", TokenType::USES},
    {"Modifies", TokenType::MODIFIES},
    {"pattern", TokenType::PATTERN},
    {"_", TokenType::UNDERSCORE},
    {"(", TokenType::OPEN_PARENTHESIS},
    {")", TokenType::CLOSED_PARENTHESIS},
    {",", TokenType::COMMA}};

ParsedQuery PQL::parse(std::vector<PqlToken> query) {
  auto parser = PQLParser(query);
  return parser.parse();
}

std::vector<PqlToken> PQL::lex(std::string query) {
  auto lexer = PQLLexer(query);
  return lexer.lex();
}
