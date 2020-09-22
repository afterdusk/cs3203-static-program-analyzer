#include "SimpleParseExceptions.h"
#include <cstring>
#include <sstream>

std::string ParseException::toString(std::vector<SimpleToken> tokens) {
  std::string string;
  for (size_t i = 0; i < tokens.size() - 1; i++) {
    string.append(tokens[i].getVal());
    string.append(" ");
  }
  string.append(tokens[tokens.size() - 1].getVal());
  return string;
}

const char *ParseException::what() const throw() {
  return "Parse exception happened";
}

const char *EmptyProgramException::what() const throw() {
  return "Error: The program contains no procedure";
}

NoProcedureException::NoProcedureException(int line, std::string name)
    : procName(name) {}

const char *NoProcedureException::what() const throw() {
  std::stringstream ss;
  ss << "Error: The procedure \"" << procName << "\" does not exist.";
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}

RepeatedProcedureException::RepeatedProcedureException(std::string name)
    : procName(name) {}

const char *RepeatedProcedureException::what() const throw() {
  std::stringstream ss;
  ss << "Error: The procedure \"" << procName
     << "\" has multiple definitions in the program.";
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}
InvalidProcedureDeclarationException::InvalidProcedureDeclarationException(
    std::string message)
    : invalidMessage(message) {}

const char *InvalidProcedureDeclarationException::what() const throw() {

  std::stringstream ss;
  ss << "Error: Procedure Declaration is invalid: \"" << invalidMessage;
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}

const char *CyclicalProcedureCallException::what() const throw() {
  return "Error: Cyclical procedure calls detected in the program.";
}

EmptyStatementListException::EmptyStatementListException(std::string procName)
    : proc(procName){};

const char *EmptyStatementListException::what() const throw() {

  std::stringstream ss;
  ss << "Error: Statementlist in procedure \"" << proc << "\" is empty";
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}

IllegalExpressionException::IllegalExpressionException(
    std::vector<SimpleToken> content) {
  for (size_t i = 0; i < content.size(); i++) {
    errorContent.push_back(content[i].getVal());
  }
}
const char *IllegalExpressionException::what() const throw() {
  std::stringstream ss;
  ss << "Error: There is illegal expression(s) ";
  for (size_t i = 0; i < errorContent.size() - 1; i++) {
    ss << "\"" << errorContent[i] << "\", ";
  }
  ss << "\"" << errorContent[errorContent.size() - 1] << "\". "
     << "Please check syntax grammar for SIMPLE program.";
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}

InvalidExpressionException::InvalidExpressionException(
    int line, std::vector<SimpleToken> content)
    : lineNo(line), lineContent(ParseException::toString(content)) {}
const char *InvalidExpressionException::what() const throw() {
  std::stringstream ss;
  ss << "Error: Line " << lineNo << ": The assignment expression on the line \""
     << lineContent
     << "\" is invalid. Please check syntax grammar for SIMPLE program.";
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}

InvalidConditionException::InvalidConditionException(
    int line, std::vector<SimpleToken> content)
    : lineNo(line), lineContent(ParseException::toString(content)) {}
const char *InvalidConditionException::what() const throw() {
  std::stringstream ss;
  ss << "Error: Line " << lineNo << ": The condition expression on the line \""
     << lineContent
     << "\" is invalid. Please check syntax grammar for SIMPLE program.";
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}

InvalidStatementSyntaxException::InvalidStatementSyntaxException(
    std::string name)
    : additionalMessage(name) {}
const char *InvalidStatementSyntaxException::what() const throw() {
  std::stringstream ss;
  ss << "Error: syntax error detected in statement declaration. \""
     << additionalMessage;
  std::string str = ss.str();
  char *copy = new char[str.size() + 1];
  str.copy(copy, str.size(), 0);
  copy[str.size()] = '\0';
  return copy;
}

void ignore(const std::exception &p) {}
