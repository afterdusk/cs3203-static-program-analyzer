#include "SimpleParser.h"
#include <iostream>
#include <stdio.h>

Parser::Parser(std::string program, PkbTables *pkbTables) {
  Tokenizer tokenizer = Tokenizer(program);
  try {
    tokens = tokenizer.tokenize();
  } catch (ParseException &p) {
    ignore(p);
    throw;
  }
  this->pkbTables = pkbTables;
}

void Parser::parse() {
  ProgramParser *programParser = new ProgramParser(tokens);
  SubParser::ProcedureUtil *procedureUtil = new SubParser::ProcedureUtil();
  SubParser::LineNumberCounter *lineNoCounter =
      new SubParser::LineNumberCounter();
  programParser->parse(lineNoCounter, pkbTables);
  programParser->topologicalSortProcedures();
  programParser->populate(pkbTables, procedureUtil);
  pkbTables->deriveTables();
}
