#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class Token {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,

  SYNONYM,
  NUMBER,
  STRING,
  UNDERSCORE,

  PARENT,
  PARENT_T,
  FOLLOWS,
  FOLLOWS_T,
  MODIFIES,
  USES,
  MATCH, // Representation of pattern clause as a relationship

  SUCHTHAT,
  PATTERN,

  // Remove if unnecessary
  SEMICOLON,
  COMMA,
  OPEN_PARENTHESIS,
  CLOSED_PARENTHESIS,
};

std::unordered_set<Token> entities = {
    Token::STMT,     Token::READ,      Token::PRINT,  Token::CALL,
    Token::WHILE,    Token::IF,        Token::ASSIGN, Token::VARIABLE,
    Token::CONSTANT, Token::PROCEDURE,
};

std::unordered_set<Token> abstractions = {
    Token::PARENT,    Token::PARENT_T, Token::FOLLOWS,
    Token::FOLLOWS_T, Token::MODIFIES, Token::USES,

    Token::MATCH,
};

std::unordered_map<Token, std::vector<std::vector<Token>>> relationships = {
    {Token::FOLLOWS,
     {
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
     }},
    {Token::FOLLOWS_T,
     {
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
     }},
    {Token::PARENT,
     {
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
     }},
    {Token::PARENT_T,
     {
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
         {Token::STMT, Token::READ, Token::PRINT, Token::CALL, Token::WHILE,
          Token::IF, Token::ASSIGN, Token::CONSTANT, Token::UNDERSCORE},
     }},
    {Token::MODIFIES,
     {
         {Token::STMT, Token::ASSIGN, Token::READ, Token::WHILE, Token::IF,
          Token::CALL, Token::PROCEDURE},
         {Token::STRING, Token::VARIABLE, Token::UNDERSCORE},
     }},
    {Token::USES,
     {
         {Token::STMT, Token::ASSIGN, Token::PRINT, Token::WHILE, Token::IF,
          Token::CALL, Token::PROCEDURE},
         {Token::STRING, Token::VARIABLE, Token::UNDERSCORE},
     }},
    {Token::MATCH,
     {
         {Token::STRING, Token::UNDERSCORE},
         {Token::STRING, Token::UNDERSCORE},
     }},
};

class PQL {
public:
};
