#pragma once
#include "Token.h"
#include <string>
#include <vector>
/*
 * Base class for all possible exceptions during parsing
 */
class ParseException : public std::exception {
public:
  std::string toString(std::vector<Token> tokens);
  // supposed to be overriden by other Exception classes.
  virtual const char *what() const throw() override;
};

/*
 * Exception when the program is empty.
 */
class EmptyProgramException : public std::exception {
public:
  const char *what() const throw() override;
};

/*
 * Exception when a non-existing procedure is called.
 */
class NoProcedureException : public ParseException {
private:
  std::string proc_name;

public:
  NoProcedureException(int line, std::string name);

  const char *what() const throw() override;
};

/*
 * Exception for repeating procedure names in the SIMPLE program
 */
class RepeatedProcedureException : public ParseException {
private:
  std::string procName;

public:
  RepeatedProcedureException(std::string name);

  const char *what() const throw() override;
};

/*
 * Exception for invalid procedure declarations
 */
class InvalidProcedureDeclarationException : public ParseException {
private:
  std::string invalidMessage;

public:
  InvalidProcedureDeclarationException(std::string message);

  const char *what() const throw() override;
};

/*
 * Exception for cyclical call of procedures
 */
class CyclicalProcedureCallException : public ParseException {
public:
  const char *what() const throw() override;
};

/*
 * Exception for empty statementlist.
 */
class EmptyStatementListException : public ParseException {
private:
  std::string proc;

public:
  EmptyStatementListException(std::string procName);

  const char *what() const throw() override;
};

/*
 * Exception for invalid expression in the SIMPLE program
 */
class IllegalExpressionException : public ParseException {
private:
  std::vector<std::string> errorContent;

public:
  IllegalExpressionException(std::vector<Token> content);
  const char *what() const throw() override;
};

/*
 * Exception for invalid assignment expression in the SIMPLE program
 */
class InvalidExpressionException : public ParseException {
private:
  int line_no;
  std::string line_content;

public:
  InvalidExpressionException(int line, std::vector<Token> content);
  const char *what() const throw() override;
};

/*
 * Exception for invalid assignment expression in the SIMPLE program
 */
class InvalidConditionException : public ParseException {
private:
  int line_no;
  std::string line_content;

public:
  InvalidConditionException(int line, std::vector<Token> content);
  const char *what() const throw() override;
};

/*
 * Exception for invalid statement declaration
 */
class InvalidStatementSyntaxException : public ParseException {
private:
  std::string additionalMessage;

public:
  InvalidStatementSyntaxException(std::string name);
  const char *what() const throw() override;
};

void ignore(const std::exception &p);
