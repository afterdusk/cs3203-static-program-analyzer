#include "Pql.h"
#include "PqlLexer.h"
#include "PqlOptimizer.h"
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
    {"Calls", TokenType::CALL},
    {"Calls*", TokenType::CALL_T},
    {"Next", TokenType::NEXT},
    {"Next*", TokenType::NEXT_T},
    {"Affects", TokenType::AFFECTS},
    {"Affects*", TokenType::AFFECTS_T},
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
    {"prog*line", TokenType::PROG_LINE}};

std::unordered_set<TokenType> entities = {
    TokenType::STMT,     TokenType::PROG_LINE, TokenType::READ,
    TokenType::PRINT,    TokenType::CALL,      TokenType::WHILE,
    TokenType::IF,       TokenType::ASSIGN,    TokenType::VARIABLE,
    TokenType::CONSTANT, TokenType::PROCEDURE};

ParsedQuery Pql::parse(std::vector<PqlToken> query) {
  auto parser = PqlParser(query);
  return parser.parse();
}

std::vector<PqlToken> Pql::lex(std::string query) {
  auto lexer = PqlLexer(query);
  return lexer.lex();
}

ParsedQuery Pql::optimize(ParsedQuery parsedQuery) {
  auto optimizer = PqlOptimizer(parsedQuery);
  return optimizer.optimize();
}
