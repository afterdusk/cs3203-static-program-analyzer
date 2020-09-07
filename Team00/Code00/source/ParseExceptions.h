#pragma once
#include "Token.h"
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
/*
 * Base class for all possible exceptions during parsing
 */
class ParseException : public std::exception {
public:
  std::string ToString(std::vector<Token> tokens) {
    std::string string;
    for (size_t i = 0; i < tokens.size() - 1; i++) {
      string.append(tokens[i].getVal());
      string.append(" ");
    }
    string.append(tokens[tokens.size() - 1].getVal());
    return string;
  }
  // supposed to be overriden by other Exception classes.
  virtual const char *what() const throw() override {
    return "Parse exception happened";
  }
};

/*
 * Exception when a non-existing procedure is called.
 */
class NoProcedureException : public ParseException {
private:
  std::string proc_name;

public:
  NoProcedureException(int line, Token name) : proc_name(name.getVal()) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: The procedure \"" << proc_name << "\" does not exist.";
    std::string str = ss.str();
    char *copy = new char[str.size() + 1];
    str.copy(copy, str.size(), 0);
    copy[str.size()] = '\0';
    return copy;
  }
};

/*
 * Exception when (/) are missing in if/while condition.
 */
class NoParenthesisException : public ParseException {
private:
  int line_no;
  std::string line_content;
  char parenthesis;

public:
  NoParenthesisException(int line, std::vector<Token> content, char bracket)
      : line_no(line), line_content(ParseException::ToString(content)),
        parenthesis(bracket) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no << ": There is a parenthesis '"
       << parenthesis << "' missing on the line \"" << line_content << "\".";
    std::string str = ss.str();
    char *copy = new char[str.size() + 1];
    str.copy(copy, str.size(), 0);
    copy[str.size()] = '\0';
    return copy;
  }
};

/*
 * Exception when {/} are missing in stmtlst
 */
class NoCurlyBracketException : public ParseException {
private:
  int line_no;
  std::string line_content;
  char curly_bracket;

public:
  NoCurlyBracketException(int line, std::vector<Token> content, char bracket)
      : line_no(line), line_content(ParseException::ToString(content)),
        curly_bracket(bracket) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no << ": There is a curly bracket '"
       << curly_bracket << "' missing on the line \"" << line_content << "\".";
    std::string str = ss.str();
    char *copy = new char[str.size() + 1];
    str.copy(copy, str.size(), 0);
    copy[str.size()] = '\0';
    return copy;
  }
};

/*
 * Exception when ; is missing in stmt
 */
class NoSemiColonException : public ParseException {
private:
  int line_no;
  std::string line_content;

public:
  NoSemiColonException(int line, std::vector<Token> content)
      : line_no(line), line_content(ParseException::ToString(content)) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no << ": Semicolon is missing on the line \""
       << line_content << "\".";
    std::string str = ss.str();
    char *copy = new char[str.size() + 1];
    str.copy(copy, str.size(), 0);
    copy[str.size()] = '\0';
    return copy;
  }
};

/*
 * Exception for invalid expression in the SIMPLE program
 */
class IllegalExpressionException : public ParseException {
private:
  int line_no;
  std::string line_content;

public:
  IllegalExpressionException(int line, std::vector<Token> content)
      : line_no(line), line_content(ParseException::ToString(content)) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no
       << ": There is illegal expression(s) on the line \"" << line_content
       << "\". Please check syntax grammar for SIMPLE program.";
    std::string str = ss.str();
    char *copy = new char[str.size() + 1];
    str.copy(copy, str.size(), 0);
    copy[str.size()] = '\0';
    return copy;
  }
};

/*
 * Exception for invalid assignment expression in the SIMPLE program
 */
class InvalidExpressionException : public ParseException {
private:
  int line_no;
  std::string line_content;

public:
  InvalidExpressionException(int line, std::vector<Token> content)
      : line_no(line), line_content(ParseException::ToString(content)) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no
       << ": The assignment expression on the line \"" << line_content
       << "\" is invalid. Please check syntax grammar for SIMPLE program.";
    std::string str = ss.str();
    char *copy = new char[str.size() + 1];
    str.copy(copy, str.size(), 0);
    copy[str.size()] = '\0';
    return copy;
  }
};

/*
 * Exception for invalid assignment expression in the SIMPLE program
 */
class InvalidConditionException : public ParseException {
private:
  int line_no;
  std::string line_content;

public:
  InvalidConditionException(int line, std::vector<Token> content)
      : line_no(line), line_content(ParseException::ToString(content)) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no
       << ": The condition expression on the line \"" << line_content
       << "\" is invalid. Please check syntax grammar for SIMPLE program.";
    std::string str = ss.str();
    char *copy = new char[str.size() + 1];
    str.copy(copy, str.size(), 0);
    copy[str.size()] = '\0';
    return copy;
  }
};
