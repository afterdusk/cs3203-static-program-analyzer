#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "PKB.h"
#include "Parser.h"
#include "TNode.h"

template <typename T>
void unionSet(std::unordered_set<T> *from, std::unordered_set<T> *target);
void addSetToPkb(VAR_TABLE_INDEXES *vars, LINE_NO line,
                 std::string relationship, PKB *pkb);
int Parse() { return 0; }

// Parser

Parser::Parser(std::string program, PKB *pkb) {
  Tokenizer tokenizer = Tokenizer(program);
  tokens = tokenizer.tokenize();
  this->pkb = pkb;
}

void Parser::parse() {
  // initial parsing
  std::vector<std::pair<PROC, CODE_CONTENT>> procedureNameAndBodyList =
      extractProcedures(tokens);
  int noOfProcedures = procedureNameAndBodyList.size();

  for (int i = 0; i < noOfProcedures; i++) {
    PROC next = procedureNameAndBodyList[i].first;
    if (std::count(procs.begin(), procs.end(), next)) {
      exit(1);
    }
    procs.push_back(next);
    pkb->addProc(next);
  }

  // kicks off initial parse. By the end of this step all vars are populated
  // into PKB.
  for (int i = 0; i < noOfProcedures; i++) {
    std::pair<PROC, CODE_CONTENT> procedure = procedureNameAndBodyList[i];
    ProcedureParser *temp =
        new ProcedureParser(procedure.first, procedure.second);
    temp->parse(&lineNo, pkb);
    procedureParsers.push_back(temp);
  }

  // Perform reverse topological sort on procedure parsers. TODO: Error handling
  // for cycles. Algorithm already implemented but error handling not done.
  procedureParsers = topologicalSortProcedures();

  // populating tables.
  std::vector<ProcedureParser *>::iterator it;
  for (it = procedureParsers.begin(); it != procedureParsers.end(); ++it) {
    (*it)->populate(pkb);
  }
}

std::vector<std::pair<PROC, CODE_CONTENT>>
Parser::extractProcedures(CODE_CONTENT tokens) {
  std::vector<std::pair<PROC, CODE_CONTENT>> output;
  CODE_CONTENT temp = tokens;
  while (!temp.empty()) {
    Token procedureKeyWord = temp.front();
    temp.erase(temp.begin());
    Token procName = temp.front();
    temp.erase(temp.begin());
    std::pair<CODE_CONTENT, CODE_CONTENT> separatedBlocks =
        isolateFirstBlock(temp, TokenEnum::OPEN_B, TokenEnum::CLOSE_B);
    CODE_CONTENT body = separatedBlocks.first;
    temp = separatedBlocks.second;

    output.push_back(make_pair(procName.getVal(), body));
  }

  if (temp.size() > 0) {
    exit(1);
  }
  return output;
};

void Parser::DFSrec(int u, std::vector<int> *visited, std::vector<int> *prev,
                    std::unordered_map<int, std::unordered_map<int, bool>> *m,
                    int n, std::vector<int> *output,
                    std::vector<int> recStack) {
  visited->at(u) = 1;
  recStack[u] = 1;
  for (int i = 0; i < n; i++) {
    if ((*m)[u][i]) {
      if (recStack[i] == 1) {
        // throw error for cycle detection
        exit(1);
      }

      if ((visited->at(i) == 0)) {

        prev->at(i) = u;
        DFSrec(i, visited, prev, m, n, output, recStack);
      }
    }
  }
  output->push_back(u);
}

std::vector<ProcedureParser *> Parser::topologicalSortProcedures() {
  std::unordered_map<PROC, int> a(procs.size());
  std::unordered_map<int, std::unordered_map<int, bool>> adjMatrix;
  std::vector<int> recStack(procs.size());
  std::vector<int> visited(procs.size());
  std::vector<int> prev(procs.size());
  std::vector<int> output;

  for (size_t i = 0; i < procs.size(); i++) {
    visited[i] = 0;
    recStack[i] = 0;
    prev[i] = -1;
    a.insert(std::make_pair<>(procs[i], i));
    std::unordered_map<int, bool> innerTemp;
    adjMatrix.insert(std::make_pair<>(i, innerTemp));
  }

  for (size_t i = 0; i < procs.size(); i++) {
    std::unordered_set<PROC> temp = procedureParsers[i]->getProcsUsed();
    std::unordered_set<PROC>::iterator it = temp.begin();
    while (it != temp.end()) {
      adjMatrix[i].insert(std::make_pair<>(a[*it], true));
      it++;
    }
  }

  for (size_t i = 0; i < procs.size(); i++) {
    if (visited[i] == 0) {

      DFSrec(i, &visited, &prev, &adjMatrix, 5, &output, recStack);
    }
  }

  std::vector<ProcedureParser *> newSequence;
  for (size_t i = 0; i < procs.size(); i++) {
    newSequence.push_back(procedureParsers.at(output[i]));
  }

  return newSequence;
}

// StatementParser

void StatementParser::populateRelationshipTables(PKB *pkb) {
  addSetToPkb(&varsUsed, lineNo, "uses", pkb);
  addSetToPkb(&varsModified, lineNo, "modifies", pkb);
}

void StatementParser::populateStatementTables(PKB *pkb) {
  pkb->addStatementProc(lineNo, parentProcedure);
  pkb->addStatementType(lineNo, statementType);
}

// AssignmentStatementParser

AssignmentStatementParser::AssignmentStatementParser(VAR name,
                                                     CODE_CONTENT expression,
                                                     PROC parent) {
  left = name;
  right = expression;
  parentProcedure = parent;
  statementType = StatementType::ASSIGN;
};

void AssignmentStatementParser::parse(LineNumberCounter *lineCounter,
                                      PKB *pkb) {
  lineNo = lineCounter->get();
  populateStatementTables(pkb);
  AST *root = new AST();
  rightParser = new ExprParserWrapper(right, std::stoi(lineNo), root);
  rightParser->parse();

  // union the variables used in the expression with the statement's varsUsed
  std::unordered_set<Token> tempVarsUsed = rightParser->getUsedVar();
  std::unordered_set<Token>::iterator it = tempVarsUsed.begin();
  while (it != tempVarsUsed.end()) {
    VAR value = it->getVal();

    varsUsed.insert(pkb->addVar(value));
    it++;
  };
  varsModified.insert(pkb->addVar(left));

  // add the constants used in the expression with the statement's Constants
  // used
  std::unordered_set<Token> tempConstantsUsed = rightParser->getUsedConstants();
  it = tempConstantsUsed.begin();
  while (it != tempConstantsUsed.end()) {
    CONSTANT value = it->getVal();
    pkb->addConstant(value);
    it++;
  }

  // pass the completed AST of the current assignment
  pkb->addAssignAst(lineNo, *root);
};

void AssignmentStatementParser::populate(PKB *pkb) {
  populateRelationshipTables(pkb);
}

// CallStatementParser

CallStatementParser::CallStatementParser(PROC name, PROC parent) {
  proc = name;
  parentProcedure = parent;
  statementType = StatementType::CALL;
};

void CallStatementParser::parse(LineNumberCounter *lineCounter, PKB *pkb) {
  lineNo = lineCounter->get();
  populateStatementTables(pkb);

  PROC_TABLE procTable = pkb->getProcTable();
  if (procTable.map[proc] == 0) {
    exit(1);
  }

  procsUsed.insert(proc);
};

void CallStatementParser::populate(PKB *pkb) {
  PROC_TABLE procTable = pkb->getProcTable();
  if (procTable.map[proc] == 0) {
    exit(1);
  }

  MODIFIES_PROC_TABLE modifiesProcTable = pkb->getModifiesProcTable();
  USES_PROC_TABLE usesProcTable = pkb->getUsesProcTable();
  unionSet<>(&(usesProcTable.map[procTable.map[proc]]), &varsUsed);
  unionSet<>(&(modifiesProcTable.map[procTable.map[proc]]), &varsModified);

  populateRelationshipTables(pkb);
};

// PrintStatementParser

PrintStatementParser::PrintStatementParser(VAR name, PROC parent) {
  var = name;
  parentProcedure = parent;
  statementType = StatementType::PRINT;
}

void PrintStatementParser::parse(LineNumberCounter *lineCounter, PKB *pkb) {
  lineNo = lineCounter->get();
  populateStatementTables(pkb);
  varsUsed.insert(pkb->addVar(var));
};

void PrintStatementParser::populate(PKB *pkb) {
  populateRelationshipTables(pkb);
};

// ReadStatementParser
ReadStatementParser::ReadStatementParser(VAR name, PROC parent) {
  var = name;
  parentProcedure = parent;
  statementType = StatementType::READ;
}

void ReadStatementParser::parse(LineNumberCounter *lineCounter, PKB *pkb) {
  lineNo = lineCounter->get();
  populateStatementTables(pkb);
  varsModified.insert(pkb->addVar(var));
};

void ReadStatementParser::populate(PKB *pkb) {
  populateRelationshipTables(pkb);
};

// WhileStatementParser

WhileStatementParser::WhileStatementParser(CODE_CONTENT condition,
                                           CODE_CONTENT content, PROC parent) {
  conditionContent = condition;
  stmtlistContent = content;
  parentProcedure = parent;
  statementType = StatementType::WHILE;
};

void WhileStatementParser::parse(LineNumberCounter *lineCounter, PKB *pkb) {
  lineNo = lineCounter->get();
  populateStatementTables(pkb);
  conditionParser = new CondParserWrapper(conditionContent, std::stoi(lineNo));
  stmtlistParser = new StatementListParser(stmtlistContent, parentProcedure);
  conditionParser->parse();
  stmtlistParser->parse(lineCounter, pkb);
  std::unordered_set<Token> a = conditionParser->getUsedVar();
  std::unordered_set<Token>::iterator it = a.begin();
  // union vars used in the condition
  while (it != a.end()) {
    VAR value = it->getVal();
    varsUsed.insert(pkb->addVar(value));
    it++;
  };

  // populate the constants used in the condition
  std::unordered_set<Token> tempConstantsUsed =
      conditionParser->getUsedConstants();
  it = tempConstantsUsed.begin();
  while (it != tempConstantsUsed.end()) {
    CONSTANT value = it->getVal();
    pkb->addConstant(value);
    it++;
  }

  unionSet<PROC>(&(stmtlistParser->getProcsUsed()), &procsUsed);
};
void WhileStatementParser::populate(PKB *pkb) {
  stmtlistParser->populate(pkb);
  unionSet<>(&(stmtlistParser->getVarsUsed()), &varsUsed);
  unionSet<>(&(stmtlistParser->getVarsModified()), &varsModified);

  // populate parent table
  std::vector<LINE_NO> *temp = &(stmtlistParser->getStatementsLineNo());
  for (size_t i = 0; i < temp->size(); i++) {
    pkb->addParent(temp->at(i), lineNo);
  }

  // populate varsUsed/varsModified to pkb
  populateRelationshipTables(pkb);
};

// IfStatementParser
IfStatementParser::IfStatementParser(CODE_CONTENT condition,
                                     CODE_CONTENT ifContent,
                                     CODE_CONTENT elseContent, PROC parent) {
  conditionContent = condition;
  ifStmtlistContent = ifContent;
  elseStmtlistContent = elseContent;
  parentProcedure = parent;
  statementType = StatementType::IF;
};

void IfStatementParser::parse(LineNumberCounter *lineCounter, PKB *pkb) {
  lineNo = lineCounter->get();
  populateStatementTables(pkb);
  conditionParser = new CondParserWrapper(conditionContent, std::stoi(lineNo));
  ifStmtlistParser =
      new StatementListParser(ifStmtlistContent, parentProcedure);
  elseStmtlistParser =
      new StatementListParser(elseStmtlistContent, parentProcedure);
  conditionParser->parse();
  ifStmtlistParser->parse(lineCounter, pkb);
  elseStmtlistParser->parse(lineCounter, pkb);

  // union vars used in the condition and populate
  std::unordered_set<Token> a = conditionParser->getUsedVar();
  std::unordered_set<Token>::iterator it = a.begin();
  while (it != a.end()) {
    VAR value = it->getVal();
    varsUsed.insert(pkb->addVar(value));
    it++;
  };

  // populate the constants used in the condition
  std::unordered_set<Token> tempConstantsUsed =
      conditionParser->getUsedConstants();
  it = tempConstantsUsed.begin();
  while (it != tempConstantsUsed.end()) {
    CONSTANT value = it->getVal();
    pkb->addConstant(value);
    it++;
  }

  unionSet<PROC>(&(ifStmtlistParser->getProcsUsed()), &procsUsed);
  unionSet<PROC>(&(elseStmtlistParser->getProcsUsed()), &procsUsed);
}

void IfStatementParser::populate(PKB *pkb) {
  ifStmtlistParser->populate(pkb);
  elseStmtlistParser->populate(pkb);
  unionSet<>(&(ifStmtlistParser->getVarsModified()), &varsModified);
  unionSet<>(&(ifStmtlistParser->getVarsUsed()), &varsUsed);
  unionSet<>(&(elseStmtlistParser->getVarsModified()), &varsModified);
  unionSet<>(&(elseStmtlistParser->getVarsUsed()), &varsUsed);

  // populate Parent Table
  std::vector<LINE_NO> *temp = &(ifStmtlistParser->getStatementsLineNo());
  for (size_t i = 0; i < temp->size(); i++) {
    pkb->addParent(temp->at(i), lineNo);
  }
  temp = &(elseStmtlistParser->getStatementsLineNo());

  for (size_t i = 0; i < temp->size(); i++) {
    pkb->addParent(temp->at(i), lineNo);
  }

  // populate to PKB
  populateRelationshipTables(pkb);
};

// StatementListParser

StatementListParser::StatementListParser(CODE_CONTENT content, PROC parent) {
  stmtlistContent = content;
  parentProcedure = parent;
}

void StatementListParser::extractStatements(CODE_CONTENT content) {
  CODE_CONTENT temp = content;
  while (!temp.empty()) {
    int curserPos = 0;

    Token initial = temp.at(0);
    Token second = temp.at(1);

    // assignment
    if ((initial.getTokenEnum() == TokenEnum::WORD) &&
        (temp.at(1).getTokenEnum() == TokenEnum::ASSIGN)) {
      while (temp.at(curserPos).getTokenEnum() != TokenEnum::SEMI_COLON) {
        curserPos++;
      }
      CODE_CONTENT currentLine(temp.cbegin() + 2, temp.cbegin() + curserPos);
      temp = CODE_CONTENT(temp.cbegin() + curserPos + 1, temp.cend());
      AssignmentStatementParser *parser = new AssignmentStatementParser(
          initial.getVal(), currentLine, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // call
    if (initial.getVal() == "call") {
      PROC proc = second.getVal();
      temp = CODE_CONTENT(temp.cbegin() + 3, temp.cend());
      CallStatementParser *parser =
          new CallStatementParser(proc, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // print
    if (initial.getVal() == "print") {
      VAR var = second.getVal();
      temp = CODE_CONTENT(temp.cbegin() + 3, temp.cend());
      PrintStatementParser *parser =
          new PrintStatementParser(var, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // read
    if (initial.getVal() == "read") {
      VAR var = second.getVal();
      temp = CODE_CONTENT(temp.cbegin() + 3, temp.cend());
      ReadStatementParser *parser =
          new ReadStatementParser(var, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // while
    if (initial.getVal() == "while") {
      temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());
      std::pair<CODE_CONTENT, CODE_CONTENT> separated =
          isolateFirstBlock(temp, TokenEnum::OPEN_P, TokenEnum::CLOSE_P);
      CODE_CONTENT conditionBlock = separated.first;
      temp = separated.second;
      separated =
          isolateFirstBlock(temp, TokenEnum::OPEN_B, TokenEnum::CLOSE_B);
      CODE_CONTENT statementListBlock = separated.first;
      temp = separated.second;
      WhileStatementParser *parser = new WhileStatementParser(
          conditionBlock, statementListBlock, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // if
    if (initial.getVal() == "if") {
      temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());
      std::pair<CODE_CONTENT, CODE_CONTENT> separated =
          isolateFirstBlock(temp, TokenEnum::OPEN_P, TokenEnum::CLOSE_P);
      CODE_CONTENT conditionBlock = separated.first;
      temp = separated.second;
      if (temp.at(0).getVal() != "then") {
        exit(1);
      }
      temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());

      separated =
          isolateFirstBlock(temp, TokenEnum::OPEN_B, TokenEnum::CLOSE_B);
      CODE_CONTENT ifStatementlistBlock = separated.first;
      temp = separated.second;

      if (temp.at(0).getVal() != "else") {
        exit(1);
      }
      temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());

      separated =
          isolateFirstBlock(temp, TokenEnum::OPEN_B, TokenEnum::CLOSE_B);
      CODE_CONTENT elseStatementlistBlock = separated.first;
      temp = separated.second;
      IfStatementParser *parser =
          new IfStatementParser(conditionBlock, ifStatementlistBlock,
                                elseStatementlistBlock, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    exit(1);
  }
}

void StatementListParser::parse(LineNumberCounter *lineCounter, PKB *pkb) {
  extractStatements(stmtlistContent);

  for (size_t i = 0; i < statementParsers.size(); i++) {
    StatementParser *st = statementParsers.at(i);
    st->parse(lineCounter, pkb);
    std::unordered_set<PROC> procsToBeAppended = st->getProcsUsed();
    procsUsed.insert(procsToBeAppended.begin(), procsToBeAppended.end());
  }
};

void StatementListParser::populate(PKB *pkb) {
  for (StatementParser *st : statementParsers) {
    // populate each statement
    st->populate(pkb);
    // union the statements' vars table information.
    unionSet<>(&(st->getVarsModified()), &varsModified);
    unionSet<>(&(st->getVarsUsed()), &varsUsed);
  }

  // populate follow table
  LINE_NO rollingLineNo = "-1";
  for (StatementParser *st : statementParsers) {
    if (rollingLineNo != "-1") {
      pkb->addFollow(rollingLineNo, st->getLineNumber());
    }
    rollingLineNo = st->getLineNumber();
  }
};

std::vector<LINE_NO> StatementListParser::getStatementsLineNo() {
  std::vector<LINE_NO> output;
  for (StatementParser *st : statementParsers) {
    output.push_back(st->getLineNumber());
  }
  return output;
}

// ProcedureParser
ProcedureParser::ProcedureParser(PROC name, CODE_CONTENT content) {
  procedureName = name;
  procedureContent = content;
}

void ProcedureParser::parse(LineNumberCounter *lineCounter, PKB *pkb) {
  statementListParser =
      new StatementListParser(procedureContent, procedureName);
  statementListParser->parse(lineCounter, pkb);
  std::unordered_set<PROC> procs_to_be_appended =
      statementListParser->getProcsUsed();
  unionSet<>(&(statementListParser->getProcsUsed()), &procsUsed);
};

void ProcedureParser::populate(PKB *pkb) {
  statementListParser->populate(pkb);
  unionSet<>(&(statementListParser->getVarsModified()), &varsModified);
  unionSet<>(&(statementListParser->getVarsUsed()), &varsUsed);
  PROC_TABLE procTable = pkb->getProcTable();
  PROC_TABLE_INDEX index = procTable.map[procedureName];
  pkb->addModifiesProc(index, varsModified);
  pkb->addUsesProc(index, varsUsed);
}

// others

// isolating first block contained by the specified open and clode brackets.
// Note the first CODE_CONTENT returned has the brackets removed already.
std::pair<CODE_CONTENT, CODE_CONTENT>
isolateFirstBlock(CODE_CONTENT p, TokenEnum open, TokenEnum close) {

  if (p.front().getTokenEnum() != open) {
    exit(1);
  }

  int curserPos = 1;

  int bracketCount = 1;

  while (bracketCount != 0) {
    if (p.at(curserPos).getTokenEnum() == open) {
      bracketCount++;
    } else if (p.at(curserPos).getTokenEnum() == close) {
      bracketCount--;
    }

    curserPos++;
  }
  CODE_CONTENT body(p.cbegin() + 1, p.cbegin() + curserPos - 1);
  CODE_CONTENT rest(p.cbegin() + curserPos, p.cend());
  return std::pair<CODE_CONTENT, CODE_CONTENT>(body, rest);
}

/** @brief Union the from set into the target set
@param from the set to provide the new elements for union
@param target the set that will contain the unioned set of elements after union.
*/
template <class T>
void unionSet(std::unordered_set<T> *from, std::unordered_set<T> *target) {
  target->insert(from->begin(), from->end());
}

/** @brief Add varsUsed or varsModified table into the given pkb.
 */
void addSetToPkb(VAR_TABLE_INDEXES *vars, LINE_NO line,
                 std::string relationship, PKB *pkb) {
  if (relationship == "uses") {
    pkb->addUses(line, *vars);
  }

  if (relationship == "modifies") {
    pkb->addModifies(line, *vars);
  }
}
