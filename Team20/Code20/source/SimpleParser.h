#pragma once
#include "PkbTables.h"
#include "SimpleCondParserWrapper.h"
#include "SimpleExprParserWrapper.h"
#include "SimpleParseExceptions.h"
#include "SimpleTokenizer.h"

int Parse();

typedef std::vector<SimpleToken> CODE_CONTENT;
typedef std::tuple<PkbTables::LINE_NO, PkbTables::CALL_BRANCH_LABEL>
    RETURN_NEXT_BIP;
typedef PkbTables::NEXT LINE_NEXT_BIP;
typedef std::variant<LINE_NEXT_BIP, RETURN_NEXT_BIP> NEXT_BIP;
typedef std::set<NEXT_BIP> NEXT_BIPS;

std::pair<CODE_CONTENT, CODE_CONTENT>
isolateFirstBlock(CODE_CONTENT p, SimpleToken::TokenType open,
                  SimpleToken::TokenType close);

class StatementListParser;

// The component responsible for generating line numbers
class LineNumberCounter {
  size_t current = 0;

public:
  PkbTables::LINE_NO get() {
    current++;
    return current;
  }
  void reset() { current = 1; }
};

// The component responsible for carrying procedure information for NextBip
// relationship
class ProcedureUtil {
  std::unordered_map<PkbTables::PROC, std::pair<PkbTables::LINE_NO, NEXT_BIPS>>
      procedureEntryAndExits;

public:
  void put(PkbTables::PROC proc, PkbTables::LINE_NO entry, NEXT_BIPS exits) {
    procedureEntryAndExits.insert(
        std::make_pair(proc, std::make_pair(entry, exits)));
  }

  std::pair<PkbTables::LINE_NO, NEXT_BIPS> get(PkbTables::PROC proc) {
    return procedureEntryAndExits.at(proc);
  }
};

// Parent class of ProcedureParser, StatementListParser and StatementParser.
// Created as they follow similar workflow.
class SubParser {
protected:
  PkbTables::VARS varsUsed;
  PkbTables::VARS varsModified;
  std::unordered_set<PkbTables::PROC> procsUsed;
  PkbTables::NEXTS exits;
  NEXT_BIPS bipExits;

public:
  PkbTables::VARS getVarsUsed() { return varsUsed; }
  PkbTables::VARS getVarsModified() { return varsModified; }
  std::unordered_set<PkbTables::PROC> getProcsUsed() { return procsUsed; }
  PkbTables::LINE_NOS getExits() { return exits; }
  NEXT_BIPS getBipExits() { return bipExits; }

  /** Parsing includes the following processes:
   * 1. Creation and parsing of subparsers under the current parser
   * 2. Generate and populate line number for StatementParsers
   * 3. Populate the pkbTables varTable
   * 4. Populate the locally stored varsUsed, varsModified tables without
   *unioning the vars used/modified from the subparsers.
   * 5. Populate the locally stored procsUsed table and union with the procUsed
   *table from the child subparsers. i.e for statementListParser, need to union
   *procsTable of all StatementParsers under it.
   * 6. For Assignment, If, While parsers, populate Constant table.
   **/
  virtual void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables) = 0;

  /** Populating includes the following processes:
   * 1. Union the locally stored varsUsed/varsModified table with all child
   *subparsers to obtain the complete varsUsed/varsModified in current Parser.
   * 2. For ProcedureParser and StatementParser, populate the PkbTables
   *varsUsed/varsModified table.
   * 3. For While/If StatementParsers, populate Parent table for the
   *statementLists under the statements.
   * 4. For StatementListParsers, populate Follows table
   **/
  virtual void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil) = 0;
};

class StatementParser : public SubParser {
protected:
  PkbTables::LINE_NO lineNo;
  PkbTables::PROC parentProcedure;
  PkbTables::StatementType statementType;

public:
  virtual void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables) = 0;
  virtual void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil) = 0;
  PkbTables::LINE_NO getLineNumber() { return lineNo; }
  void populateStatementTables(PkbTables *pkbTables);
  void populateRelationshipTables(PkbTables *pkbTables);
  PkbTables::StatementType getType() { return statementType; }
};

class AssignmentStatementParser : public StatementParser {
  PkbTables::VAR left;
  CODE_CONTENT right;
  SimpleExprParserWrapper *rightParser;

public:
  AssignmentStatementParser(PkbTables::VAR name, CODE_CONTENT expression,
                            PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

class CallStatementParser : public StatementParser {
  PkbTables::PROC proc;

public:
  CallStatementParser(PkbTables::PROC name, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

class PrintStatementParser : public StatementParser {
  PkbTables::VAR var;

public:
  PrintStatementParser(PkbTables::VAR name, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

class ReadStatementParser : public StatementParser {
  PkbTables::VAR var;

public:
  ReadStatementParser(PkbTables::VAR name, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

class WhileStatementParser : public StatementParser {
  CODE_CONTENT conditionContent;
  CODE_CONTENT stmtlistContent;
  SimpleCondParserWrapper *conditionParser;
  StatementListParser *stmtlistParser;

public:
  WhileStatementParser(CODE_CONTENT condition, CODE_CONTENT content,
                       PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

class IfStatementParser : public StatementParser {
  CODE_CONTENT conditionContent;
  CODE_CONTENT ifStmtlistContent;
  CODE_CONTENT elseStmtlistContent;
  SimpleCondParserWrapper *conditionParser;
  StatementListParser *ifStmtlistParser;
  StatementListParser *elseStmtlistParser;

public:
  IfStatementParser(CODE_CONTENT condition, CODE_CONTENT ifContent,
                    CODE_CONTENT elseContent, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

class StatementListParser : public SubParser {
  PkbTables::PROC parentProcedure;
  CODE_CONTENT stmtlistContent;
  std::vector<StatementParser *> statementParsers;

  void extractStatements(CODE_CONTENT content);

public:
  StatementListParser(CODE_CONTENT content, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
  std::vector<PkbTables::LINE_NO> getStatementsLineNo();
};

class ProcedureParser : public SubParser {
  PkbTables::PROC procedureName;
  CODE_CONTENT procedureContent;
  StatementListParser *statementListParser;

public:
  ProcedureParser(PkbTables::PROC name, CODE_CONTENT content);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

class Parser {
  CODE_CONTENT tokens;
  std::vector<int> vars;
  std::vector<PkbTables::PROC> procs;
  std::vector<ProcedureParser *> procedureParsers;
  LineNumberCounter lineNo;
  PkbTables *pkbTables;

public:
  Parser(std::string program, PkbTables *pkbTables);
  void parse();
  std::vector<ProcedureParser *> topologicalSortProcedures();
  void DFSrec(int u, std::vector<int> *visited, std::vector<int> *prev,
              std::unordered_map<int, std::unordered_map<int, bool>> *m, int n,
              std::vector<int> *output, std::vector<int> rec_stack);
  std::vector<std::pair<PkbTables::PROC, CODE_CONTENT>>
  extractProcedures(CODE_CONTENT tokens);
};
