#pragma once
#include "SimpleSubParser.h"

/** @brief The main Parser exposed to user interface. Calling parse() completes
 * both input validation and table population
 */
class Parser {

  SubParser::CODE_CONTENT tokens;
  std::vector<PkbTables::PROC> procs;
  std::vector<ProcedureParser *> procedureParsers;
  PkbTables *pkbTables;

public:
  Parser(std::string program, PkbTables *pkbTables);
  void parse();
};
