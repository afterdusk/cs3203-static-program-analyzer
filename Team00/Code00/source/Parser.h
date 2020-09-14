#pragma once
#include "CondParserWrapper.h"
#include "ExprParserWrapper.h"
#include "PKB.h"
#include "Tokenizer.h"
#include <list>
#include <set>
#include <string>
#include <vector>

int Parse();

typedef std::vector<Token> CODE_CONTENT;

std::pair<CODE_CONTENT, CODE_CONTENT>
isolateFirstBlock(CODE_CONTENT p, TokenEnum open, TokenEnum close);

class StatementListParser;

// The component responsible for generating line numbers
class LineNumberCounter {
  int current = 0;

public:
  LINE_NO get() {
    current++;
    return std::to_string(current);
  }
  void reset() { current = 1; }
};

// Parent class of ProcedureParser, StatementListParser and StatementParser.
// Created as they follow similar workflow.
class SubParser {
protected:
  VAR_TABLE_INDEXES varsUsed;
  VAR_TABLE_INDEXES varsModified;
  std::unordered_set<PROC> procsUsed;

public:
  VAR_TABLE_INDEXES getVarsUsed() { return varsUsed; }
  VAR_TABLE_INDEXES getVarsModified() { return varsModified; }
  std::unordered_set<PROC> getProcsUsed() { return procsUsed; }

  /** Parsing includes the following processes:
   * 1. Creation and parsing of subparsers under the current parser
   * 2. Generate and populate line number for StatementParsers
   * 3. Populate the pkb varTable
   * 4. Populate the locally stored varsUsed, varsModified tables without
   *unioning the vars used/modified from the subparsers.
   * 5. Populate the locally stored procsUsed table and union with the procUsed
   *table from the child subparsers. i.e for statementListParser, need to union
   *procsTable of all StatementParsers under it.
   * 6. For Assignment, If, While parsers, populate Constant table.
   **/
  virtual void parse(LineNumberCounter *lineCounter, PKB *pkb) = 0;

  /** Populating includes the following processes:
   * 1. Union the locally stored varsUsed/varsModified table with all child
   *subparsers to obtain the complete varsUsed/varsModified in current Parser.
   * 2. For ProcedureParser and StatementParser, populate the PKB
   *varsUsed/varsModified table.
   * 3. For While/If StatementParsers, populate Parent table for the
   *statementLists under the statements.
   * 4. For StatementListParsers, populate Follows table
   **/
  virtual void populate(PKB *pkb) = 0;
};

class StatementParser : public SubParser {
protected:
  LINE_NO lineNo;
  PROC parentProcedure;
  StatementType statementType;

public:
  virtual void parse(LineNumberCounter *lineCounter, PKB *pkb) = 0;
  virtual void populate(PKB *pkb) = 0;
  LINE_NO getLineNumber() { return lineNo; }
  void populateStatementTables(PKB *pkb);
  void populateRelationshipTables(PKB *pkb);
};

class AssignmentStatementParser : public StatementParser {
  VAR left;
  CODE_CONTENT right;
  ExprParserWrapper *rightParser;

public:
  AssignmentStatementParser(VAR name, CODE_CONTENT expression, PROC parent);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
};

class CallStatementParser : public StatementParser {
  PROC proc;

public:
  CallStatementParser(PROC name, PROC parent);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
};

class PrintStatementParser : public StatementParser {
  VAR var;

public:
  PrintStatementParser(VAR name, PROC parent);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
};

class ReadStatementParser : public StatementParser {
  VAR var;

public:
  ReadStatementParser(VAR name, PROC parent);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
};

class WhileStatementParser : public StatementParser {
  CODE_CONTENT conditionContent;
  CODE_CONTENT stmtlistContent;
  CondParserWrapper *conditionParser;
  StatementListParser *stmtlistParser;

public:
  WhileStatementParser(CODE_CONTENT condition, CODE_CONTENT content,
                       PROC parent);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
};

class IfStatementParser : public StatementParser {
  CODE_CONTENT conditionContent;
  CODE_CONTENT ifStmtlistContent;
  CODE_CONTENT elseStmtlistContent;
  CondParserWrapper *conditionParser;
  StatementListParser *ifStmtlistParser;
  StatementListParser *elseStmtlistParser;

public:
  IfStatementParser(CODE_CONTENT condition, CODE_CONTENT ifContent,
                    CODE_CONTENT elseContent, PROC parent);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
};

class StatementListParser : public SubParser {
  PROC parentProcedure;
  CODE_CONTENT stmtlistContent;
  std::vector<StatementParser *> statementParsers;

  void extractStatements(CODE_CONTENT content);

public:
  StatementListParser(CODE_CONTENT content, PROC parent);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
  std::vector<LINE_NO> getStatementsLineNo();
};

class ProcedureParser : public SubParser {
  PROC procedureName;
  CODE_CONTENT procedureContent;
  StatementListParser *statementListParser;

public:
  ProcedureParser(PROC name, CODE_CONTENT content);
  void parse(LineNumberCounter *lineCounter, PKB *pkb);
  void populate(PKB *pkb);
};

class Parser {
  CODE_CONTENT tokens;
  std::vector<int> vars;
  std::vector<PROC> procs;
  std::vector<ProcedureParser *> procedureParsers;
  LineNumberCounter lineNo;
  PKB *pkb;

public:
  Parser(std::string program, PKB *pkb);
  void parse();
  std::vector<ProcedureParser *> topologicalSortProcedures();
  void DFSrec(int u, std::vector<int> *visited, std::vector<int> *prev,
              std::unordered_map<int, std::unordered_map<int, bool>> *m, int n,
              std::vector<int> *output, std::vector<int> rec_stack);
  std::vector<std::pair<PROC, CODE_CONTENT>>
  extractProcedures(CODE_CONTENT tokens);
};
