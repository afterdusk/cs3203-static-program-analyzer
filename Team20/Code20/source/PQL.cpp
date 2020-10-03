#include "Pql.h"
#include "PqlLexer.h"
#include "PqlParser.h"

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
    {"<", TokenType::OPEN_ANGLED_BRACKET},
    {">", TokenType::CLOSED_ANGLED_BRACKET},
    {".", TokenType::DOT},
    {"procName", TokenType::PROCNAME},
    {"varName", TokenType::VARNAME},
    {"value", TokenType::VALUE},
    {"stmt#", TokenType::STATEMENT_NUM},
    {",", TokenType::COMMA},
    {"with", TokenType::WITH},
    {"and", TokenType::AND},
    {"=", TokenType::EQUALS},
    {"BOOLEAN", TokenType::BOOLEAN},
    {"progline", TokenType::PROG_LINE}};

ParsedQuery Pql::parse(std::vector<PqlToken> query) {
  auto parser = PqlParser(query);
  return parser.parse();
}

std::vector<PqlToken> Pql::lex(std::string query) {
  auto lexer = PqlLexer(query);
  return lexer.lex();
}
