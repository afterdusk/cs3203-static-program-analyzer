#pragma once

#include <cstring>
#include <sstream>
#include <string>
/*
 * Base class for all possible exceptions during parsing
 */
class ParseException : public std::exception {
public:
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
  int line_no;
  std::string proc_name;

public:
  NoProcedureException(int line, std::string name)
      : line_no(line), proc_name(name) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no << ": The procedure \"" << proc_name
       << "\" does not exist.";
    char *copy = new char[ss.str().size() + 1];
    strcpy(copy, ss.str().c_str());
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
  NoParenthesisException(int line, std::string content, char bracket)
      : line_no(line), line_content(content), parenthesis(bracket) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no << ": There is a parenthesis '"
       << parenthesis << "' missing on the line \"" << line_content << "\".";
    char *copy = new char[ss.str().size() + 1];
    strcpy(copy, ss.str().c_str());
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
  NoCurlyBracketException(int line, std::string content, char bracket)
      : line_no(line), line_content(content), curly_bracket(bracket) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no << ": There is a curly bracket '"
       << curly_bracket << "' missing on the line \"" << line_content << "\".";
    char *copy = new char[ss.str().size() + 1];
    strcpy(copy, ss.str().c_str());
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
  NoSemiColonException(int line, std::string content)
      : line_no(line), line_content(content) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no << ": Semicolon is missing on the line \""
       << line_content << "\".";
    char *copy = new char[ss.str().size() + 1];
    strcpy(copy, ss.str().c_str());
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
  IllegalExpressionException(int line, std::string content)
      : line_no(line), line_content(content) {}
  const char *what() const throw() override {
    std::stringstream ss;
    ss << "Error: Line " << line_no
       << ": There is illegal expression(s) on the line \"" << line_content
       << "\". Please check syntax grammar for SIMPLE program.";
    char *copy = new char[ss.str().size() + 1];
    strcpy(copy, ss.str().c_str());
    return copy;
  }
};
