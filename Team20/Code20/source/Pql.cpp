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
std::unordered_map<TokenType, std::vector<std::unordered_set<TokenType>>>
    allowedArgumentsForRelationships = {
        {TokenType::FOLLOWS,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::FOLLOWS_T,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::PARENT,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::PARENT_T,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
              TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
              TokenType::IF, TokenType::ASSIGN, TokenType::CONSTANT,
              TokenType::UNDERSCORE, TokenType::NUMBER},
         }},
        {TokenType::MODIFIES,
         {
             {TokenType::STMT, TokenType::PROG_LINE, TokenType::ASSIGN,
              TokenType::READ, TokenType::WHILE, TokenType::IF, TokenType::CALL,
              TokenType::PROCEDURE, TokenType::NUMBER, TokenType::STRING},
             {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
         }},
        {
            TokenType::USES,
            {
                {TokenType::STMT, TokenType::PROG_LINE, TokenType::ASSIGN,
                 TokenType::PRINT, TokenType::WHILE, TokenType::IF,
                 TokenType::CALL, TokenType::PROCEDURE, TokenType::NUMBER,
                 TokenType::STRING, TokenType::NUMBER},
                {TokenType::STRING, TokenType::VARIABLE, TokenType::UNDERSCORE},
            },
        },
        {TokenType::CALL,
         {
             {TokenType::PROCEDURE, TokenType::STRING, TokenType::UNDERSCORE},
             {TokenType::PROCEDURE, TokenType::STRING, TokenType::UNDERSCORE},
         }},
        {
            TokenType::CALL_T,
            {
                {TokenType::PROCEDURE, TokenType::STRING,
                 TokenType::UNDERSCORE},
                {TokenType::PROCEDURE, TokenType::STRING,
                 TokenType::UNDERSCORE},
            },
        },
        {
            TokenType::AFFECTS,
            {
                {TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
                 TokenType::PROG_LINE, TokenType::UNDERSCORE},
                {TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
                 TokenType::PROG_LINE, TokenType::UNDERSCORE},
            },
        },
        {TokenType::AFFECTS_T,
         {{TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
           TokenType::PROG_LINE, TokenType::UNDERSCORE},
          {TokenType::ASSIGN, TokenType::NUMBER, TokenType::STMT,
           TokenType::PROG_LINE, TokenType::UNDERSCORE}}},
        {
            TokenType::NEXT,
            {
                {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
                 TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
                 TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
                 TokenType::NUMBER},
                {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
                 TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
                 TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
                 TokenType::NUMBER},
            },
        },
        {
            TokenType::NEXT_T,
            {
                {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
                 TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
                 TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
                 TokenType::NUMBER},
                {TokenType::STMT, TokenType::PROG_LINE, TokenType::READ,
                 TokenType::PRINT, TokenType::CALL, TokenType::WHILE,
                 TokenType::IF, TokenType::ASSIGN, TokenType::UNDERSCORE,
                 TokenType::NUMBER},
            },
        },
};
