#pragma once
#include "PkbTables.h"
#include "SimpleCondParserWrapper.h"
#include "SimpleExprParserWrapper.h"
#include "SimpleParseExceptions.h"
#include "SimpleTokenizer.h"

/** @brief Parent class of StatementParser, StatementListParser, ProcedureParser
 * and ProgramParser.
 */
class SubParser {
public:
  typedef std::vector<SimpleToken> CODE_CONTENT;
  typedef std::pair<CODE_CONTENT, CODE_CONTENT> SPLIT_CODE_CONTENT;
  typedef std::tuple<PkbTables::LINE_NO, PkbTables::CALL_BRANCH_LABEL>
      RETURN_NEXT_BIP;
  typedef std::vector<PkbTables::PROC> PROCS_VECTOR;
  typedef std::vector<PkbTables::LINE_NO> LINE_NOS_VECTOR;
  typedef PkbTables::NEXT LINE_NEXT_BIP;
  typedef std::variant<LINE_NEXT_BIP, RETURN_NEXT_BIP> NEXT_BIP;
  typedef std::set<NEXT_BIP> NEXT_BIPS;

  /** @brief A counter class used to generate line number
   */
  class LineNumberCounter {
    size_t current = 0;

  public:
    PkbTables::LINE_NO get() {
      current++;
      return current;
    }
  };

  /** @brief The component responsible for carrying procedure information for
   * NextBip relationship
   */
  class ProcedureUtil {
    std::unordered_map<PkbTables::PROC,
                       std::pair<PkbTables::LINE_NO, NEXT_BIPS>>
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

  PkbTables::VARS getVarsUsed() { return varsUsed; }
  PkbTables::VARS getVarsModified() { return varsModified; }
  std::unordered_set<PkbTables::PROC> getProcsUsed() { return procsUsed; }
  PkbTables::LINE_NOS getExits() { return exits; }
  NEXT_BIPS getBipExits() { return bipExits; }

  /** @brief Parsing includes the following processes:
   * 1. Creation and parsing of subparsers under the current parser
   * 2. Generate and populate line number for StatementParsers
   * 3. Populate the pkbTables varTable
   * 4. Populate the locally stored varsUsed, varsModified tables without
   * unioning the vars used/modified from the subparsers.
   * 5. Populate the locally stored procsUsed table and union with the procUsed
   * table from the child subparsers. i.e for statementListParser, need to union
   * procsTable of all StatementParsers under it.
   * 6. For Assignment, If, While parsers, populate Constant table.
   **/
  virtual void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables) = 0;

  /** @brief Populating includes the following processes:
   * 1. Union the locally stored varsUsed/varsModified table with all child
   * subparsers to obtain the complete varsUsed/varsModified in current Parser.
   * 2. For ProcedureParser and StatementParser, populate the PkbTables
   * varsUsed/varsModified table.
   * 3. For While/If StatementParsers, populate Parent, Next and NextBip table
   * for the statementLists under the statements.
   * 4. For StatementListParsers, populate Follows, Next and NextBip Tables
   **/
  virtual void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil) = 0;

  /** @brief union the elements in from and target and put it into target.
   */
  template <typename T>
  static void unionSet(std::unordered_set<T> *from,
                       std::unordered_set<T> *target);

  /** @brief union the elements in from and target and put it into target.
   */
  template <class T>
  static void unionSet(std::set<T> *from, std::set<T> *target);

  /** @brief Create NextBip Relationship from all elements in nextBips
   * to the line referenced by prev
   */
  static void addNextBips(PkbTables *pkbTables, NEXT_BIPS nextBips,
                          PkbTables::LINE_NO prev);

  /** @brief Separates the first block contained by the specified open
   * and close brackets
   */
  static std::pair<CODE_CONTENT, CODE_CONTENT>
  isolateFirstBlock(CODE_CONTENT p, SimpleToken::TokenType open,
                    SimpleToken::TokenType close);

protected:
  PkbTables::VARS varsUsed;
  PkbTables::VARS varsModified;
  std::unordered_set<PkbTables::PROC> procsUsed;
  PkbTables::NEXTS exits;
  NEXT_BIPS bipExits;
};

/** @brief Parser responsible for parsing a statement
 */
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
  void populateModifiesUsesTables(PkbTables *pkbTables);
  PkbTables::StatementType getType() { return statementType; }
};

/** @brief Parser responsible for parsing a statementlist.
 * The statementlist could be contained in a procedure,
 * an if statement or a while statement.
 */
class StatementListParser : public SubParser {
  PkbTables::PROC parentProcedure;
  CODE_CONTENT stmtlistContent;
  std::vector<StatementParser *> statementParsers;

  void extractStatements(CODE_CONTENT content);
  void populateNextTable(PkbTables *pkbTables);
  void populateNextBipTable(PkbTables *pkbTables);
  void populateFollowsTable(PkbTables *pkbTables);

public:
  StatementListParser(CODE_CONTENT content, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
  LINE_NOS_VECTOR getStatementsLineNo();
};

/** @brief Parser responsible for parsing an Assignment statement
 */
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

/** @brief Parser responsible for parsing a Call statement
 */
class CallStatementParser : public StatementParser {
  PkbTables::PROC proc;
  void handleAndPopulateNextBipTable(PkbTables *pkbTables,
                                     ProcedureUtil *procedureUtil);

public:
  CallStatementParser(PkbTables::PROC name, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

/** @brief Parser responsible for parsing a Print statement
 */
class PrintStatementParser : public StatementParser {
  PkbTables::VAR var;

public:
  PrintStatementParser(PkbTables::VAR name, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

/** @brief Parser responsible for parsing a Read statement
 */
class ReadStatementParser : public StatementParser {
  PkbTables::VAR var;

public:
  ReadStatementParser(PkbTables::VAR name, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

/** @brief Parser responsible for parsing a While statement
 */
class WhileStatementParser : public StatementParser {
  CODE_CONTENT conditionContent;
  CODE_CONTENT stmtlistContent;
  SimpleCondParserWrapper *conditionParser;
  StatementListParser *stmtlistParser;
  void populateParentTable(PkbTables *pkbTables);
  void populateNextTable(PkbTables *pkbTables);
  void populateNextBipTable(PkbTables *pkbTables);

public:
  WhileStatementParser(CODE_CONTENT condition, CODE_CONTENT content,
                       PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

/** @brief Parser responsible for parsing an If statement
 */
class IfStatementParser : public StatementParser {
  CODE_CONTENT conditionContent;
  CODE_CONTENT ifStmtlistContent;
  CODE_CONTENT elseStmtlistContent;
  SimpleCondParserWrapper *conditionParser;
  StatementListParser *ifStmtlistParser;
  StatementListParser *elseStmtlistParser;
  void populateParentTable(PkbTables *pkbTables);
  void populateNextTable(PkbTables *pkbTables);
  void populateNextBipTable(PkbTables *pkbTables);

public:
  IfStatementParser(CODE_CONTENT condition, CODE_CONTENT ifContent,
                    CODE_CONTENT elseContent, PkbTables::PROC parent);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
};

/** @brief Parser responsible for parsing a procedure
 */
class ProcedureParser : public SubParser {
  PkbTables::PROC procedureName;
  CODE_CONTENT procedureContent;
  StatementListParser *statementListParser;

public:
  ProcedureParser(PkbTables::PROC name, CODE_CONTENT content);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
  void populateUsesModifiesTables(PkbTables *pkbTables);
  void populateCallsTable(PkbTables *callsTable);
};

/** @brief Parser responsible for parsing the entire program
 */
class ProgramParser : public SubParser {
  CODE_CONTENT programContent;
  std::vector<ProcedureParser *> procedureParsers;

public:
  PROCS_VECTOR procs;
  ProgramParser(CODE_CONTENT content);
  void parse(LineNumberCounter *lineCounter, PkbTables *pkbTables);
  void populate(PkbTables *pkbTables, ProcedureUtil *procedureUtil);
  std::vector<std::pair<PkbTables::PROC, CODE_CONTENT>>
  extractProcedures(CODE_CONTENT tokens);
  void DFSrec(int u, std::vector<int> *visited, std::vector<int> *prev,
              std::unordered_map<int, std::unordered_map<int, bool>> *m, int n,
              std::vector<int> *output, std::vector<int> recStack);
  void topologicalSortProcedures();
};
