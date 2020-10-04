#include "SimpleParser.h"
#include <iostream>
#include <stdio.h>

template <typename T>
void unionSet(std::unordered_set<T> *from, std::unordered_set<T> *target);
void addSetToPkbTables(PkbTables::VARS *vars, PkbTables::LINE_NO line,
                       std::string relationship, PkbTables *pkbTables);
int Parse() { return 0; }

// Parser

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
  // initial parsing
  std::vector<std::pair<PkbTables::PROC, CODE_CONTENT>>
      procedureNameAndBodyList = extractProcedures(tokens);
  int noOfProcedures = procedureNameAndBodyList.size();

  if (noOfProcedures == 0) {
    throw EmptyProgramException();
  }

  for (int i = 0; i < noOfProcedures; i++) {
    PkbTables::PROC next = procedureNameAndBodyList[i].first;
    if (std::count(procs.begin(), procs.end(), next)) {
      throw RepeatedProcedureException(next);
    }
    procs.push_back(next);
    pkbTables->addProc(next);
  }

  // kicks off initial parse. By the end of this step all vars are populated
  // into PkbTables.
  for (int i = 0; i < noOfProcedures; i++) {
    std::pair<PkbTables::PROC, CODE_CONTENT> procedure =
        procedureNameAndBodyList[i];
    ProcedureParser *temp =
        new ProcedureParser(procedure.first, procedure.second);
    temp->parse(&lineNo, pkbTables);
    procedureParsers.push_back(temp);
  }

  // Perform reverse topological sort on procedure parsers. TODO: Error handling
  // for cycles. Algorithm already implemented but error handling not done.
  procedureParsers = topologicalSortProcedures();

  // populating tables.
  std::vector<ProcedureParser *>::iterator it;
  for (it = procedureParsers.begin(); it != procedureParsers.end(); ++it) {
    (*it)->populate(pkbTables);
  }
  pkbTables->deriveTables();
}

std::vector<std::pair<PkbTables::PROC, CODE_CONTENT>>
Parser::extractProcedures(CODE_CONTENT tokens) {
  std::vector<std::pair<PkbTables::PROC, CODE_CONTENT>> output;
  CODE_CONTENT temp = tokens;
  while (!temp.empty()) {
    SimpleToken procedureKeyWord = temp.front();
    if (procedureKeyWord.getVal() != "procedure") {
      throw InvalidProcedureDeclarationException("missing token: 'procedure'");
    }
    temp.erase(temp.begin());
    if (temp.empty()) {
      throw InvalidProcedureDeclarationException(
          "miss token: 'procedure name'");
    }

    SimpleToken procName = temp.front();
    temp.erase(temp.begin());
    if (procName.getTokenType() != SimpleToken::TokenType::WORD) {
      throw InvalidProcedureDeclarationException("procedure name is invalid: " +
                                                 procName.getVal());
    }
    std::pair<CODE_CONTENT, CODE_CONTENT> separatedBlocks;
    try {
      separatedBlocks = isolateFirstBlock(temp, SimpleToken::TokenType::OPEN_B,
                                          SimpleToken::TokenType::CLOSE_B);
    } catch (std::exception &e) {
      ignore(e);
      throw InvalidProcedureDeclarationException(
          "invalid procedure body identified");
    };

    CODE_CONTENT body = separatedBlocks.first;
    temp = separatedBlocks.second;

    output.push_back(make_pair(procName.getVal(), body));
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
        throw CyclicalProcedureCallException();
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
  std::unordered_map<PkbTables::PROC, int> a(procs.size());
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
    std::unordered_set<PkbTables::PROC> temp =
        procedureParsers[i]->getProcsUsed();
    std::unordered_set<PkbTables::PROC>::iterator it = temp.begin();
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

void StatementParser::populateRelationshipTables(PkbTables *pkbTables) {
  addSetToPkbTables(&varsUsed, lineNo, "uses", pkbTables);
  addSetToPkbTables(&varsModified, lineNo, "modifies", pkbTables);
}

void StatementParser::populateStatementTables(PkbTables *pkbTables) {
  pkbTables->addStatementProc(lineNo, parentProcedure);
  pkbTables->addStatementType(lineNo, statementType);
}

// AssignmentStatementParser

AssignmentStatementParser::AssignmentStatementParser(PkbTables::VAR name,
                                                     CODE_CONTENT expression,
                                                     PkbTables::PROC parent) {
  left = name;
  right = expression;
  parentProcedure = parent;
  statementType = PkbTables::StatementType::Assign;
};

void AssignmentStatementParser::parse(LineNumberCounter *lineCounter,
                                      PkbTables *pkbTables) {
  lineNo = lineCounter->get();
  populateStatementTables(pkbTables);
  PkbTables::AST *root = new PkbTables::AST();
  rightParser = new SimpleExprParserWrapper(right, lineNo, root);
  rightParser->parse();

  // union the variables used in the expression with the statement's varsUsed
  std::unordered_set<SimpleToken> tempVarsUsed = rightParser->getUsedVar();
  std::unordered_set<SimpleToken>::iterator it = tempVarsUsed.begin();
  while (it != tempVarsUsed.end()) {
    PkbTables::VAR value = it->getVal();
    pkbTables->addVar(value);
    varsUsed.insert(value);
    it++;
  };
  pkbTables->addVar(left);
  varsModified.insert(left);

  // add the constants used in the expression with the statement's Constants
  // used
  std::unordered_set<SimpleToken> tempConstantsUsed =
      rightParser->getUsedConstants();
  it = tempConstantsUsed.begin();
  while (it != tempConstantsUsed.end()) {
    PkbTables::CONSTANT value = it->getVal();
    pkbTables->addConstant(value);
    it++;
  }

  // pass the completed PkbTables::AST of the current assignment
  pkbTables->addAssignAst(lineNo, *root);
};

void AssignmentStatementParser::populate(PkbTables *pkbTables) {
  populateRelationshipTables(pkbTables);
}

// CallStatementParser

CallStatementParser::CallStatementParser(PkbTables::PROC name,
                                         PkbTables::PROC parent) {
  proc = name;
  parentProcedure = parent;
  statementType = PkbTables::StatementType::Call;
};

void CallStatementParser::parse(LineNumberCounter *lineCounter,
                                PkbTables *pkbTables) {
  lineNo = lineCounter->get();
  populateStatementTables(pkbTables);

  PkbTables::PROC_TABLE procTable = pkbTables->getProcTable();
  if (procTable.find(proc) == procTable.end()) {
    throw NoProcedureException(lineNo, proc);
  }

  procsUsed.insert(proc);
};

void CallStatementParser::populate(PkbTables *pkbTables) {
  PkbTables::PROC_TABLE procTable = pkbTables->getProcTable();

  PkbTables::MODIFIES_PROC_TABLE modifiesProcTable =
      pkbTables->getModifiesProcTable();

  PkbTables::USES_PROC_TABLE usesProcTable = pkbTables->getUsesProcTable();
  unionSet<>(&(usesProcTable.map[proc]), &varsUsed);
  unionSet<>(&(modifiesProcTable.map[proc]), &varsModified);

  // populateRelationshipTables(pkbTables);
  pkbTables->addModifies(lineNo, proc);
  pkbTables->addUses(lineNo, proc);
};

// PrintStatementParser

PrintStatementParser::PrintStatementParser(PkbTables::VAR name,
                                           PkbTables::PROC parent) {
  var = name;
  parentProcedure = parent;
  statementType = PkbTables::StatementType::Print;
}

void PrintStatementParser::parse(LineNumberCounter *lineCounter,
                                 PkbTables *pkbTables) {
  lineNo = lineCounter->get();
  populateStatementTables(pkbTables);
  pkbTables->addVar(var);
  varsUsed.insert(var);
};

void PrintStatementParser::populate(PkbTables *pkbTables) {
  populateRelationshipTables(pkbTables);
};

// ReadStatementParser
ReadStatementParser::ReadStatementParser(PkbTables::VAR name,
                                         PkbTables::PROC parent) {
  var = name;
  parentProcedure = parent;
  statementType = PkbTables::StatementType::Read;
}

void ReadStatementParser::parse(LineNumberCounter *lineCounter,
                                PkbTables *pkbTables) {
  lineNo = lineCounter->get();
  populateStatementTables(pkbTables);
  pkbTables->addVar(var);
  varsModified.insert(var);
};

void ReadStatementParser::populate(PkbTables *pkbTables) {
  populateRelationshipTables(pkbTables);
};

// WhileStatementParser

WhileStatementParser::WhileStatementParser(CODE_CONTENT condition,
                                           CODE_CONTENT content,
                                           PkbTables::PROC parent) {
  conditionContent = condition;
  stmtlistContent = content;
  parentProcedure = parent;
  statementType = PkbTables::StatementType::While;
};

void WhileStatementParser::parse(LineNumberCounter *lineCounter,
                                 PkbTables *pkbTables) {
  lineNo = lineCounter->get();
  populateStatementTables(pkbTables);
  conditionParser = new SimpleCondParserWrapper(conditionContent, lineNo);
  stmtlistParser = new StatementListParser(stmtlistContent, parentProcedure);
  conditionParser->parse();
  stmtlistParser->parse(lineCounter, pkbTables);
  std::unordered_set<SimpleToken> a = conditionParser->getUsedVar();
  std::unordered_set<SimpleToken>::iterator it = a.begin();
  // union vars used in the condition
  while (it != a.end()) {
    PkbTables::VAR value = it->getVal();
    pkbTables->addVar(value);
    varsUsed.insert(value);
    it++;
  };

  // populate the constants used in the condition
  std::unordered_set<SimpleToken> tempConstantsUsed =
      conditionParser->getUsedConstants();
  it = tempConstantsUsed.begin();
  while (it != tempConstantsUsed.end()) {
    PkbTables::CONSTANT value = it->getVal();
    pkbTables->addConstant(value);
    it++;
  }

  // populate the condition table
  pkbTables->addConditionVars(lineNo, varsUsed);

  unionSet<PkbTables::PROC>(&(stmtlistParser->getProcsUsed()), &procsUsed);
};
void WhileStatementParser::populate(PkbTables *pkbTables) {
  stmtlistParser->populate(pkbTables);
  unionSet<>(&(stmtlistParser->getVarsUsed()), &varsUsed);
  unionSet<>(&(stmtlistParser->getVarsModified()), &varsModified);

  // populate parent table
  std::vector<PkbTables::LINE_NO> temp = stmtlistParser->getStatementsLineNo();
  for (size_t i = 0; i < temp.size(); i++) {
    pkbTables->addParent(temp.at(i), lineNo);
  }

  // populate varsUsed/varsModified to pkbTables
  populateRelationshipTables(pkbTables);
};

// IfStatementParser
IfStatementParser::IfStatementParser(CODE_CONTENT condition,
                                     CODE_CONTENT ifContent,
                                     CODE_CONTENT elseContent,
                                     PkbTables::PROC parent) {
  conditionContent = condition;
  ifStmtlistContent = ifContent;
  elseStmtlistContent = elseContent;
  parentProcedure = parent;
  statementType = PkbTables::StatementType::If;
};

void IfStatementParser::parse(LineNumberCounter *lineCounter,
                              PkbTables *pkbTables) {
  lineNo = lineCounter->get();
  populateStatementTables(pkbTables);
  conditionParser = new SimpleCondParserWrapper(conditionContent, lineNo);
  ifStmtlistParser =
      new StatementListParser(ifStmtlistContent, parentProcedure);
  elseStmtlistParser =
      new StatementListParser(elseStmtlistContent, parentProcedure);
  conditionParser->parse();
  ifStmtlistParser->parse(lineCounter, pkbTables);
  elseStmtlistParser->parse(lineCounter, pkbTables);

  // union vars used in the condition and populate
  std::unordered_set<SimpleToken> a = conditionParser->getUsedVar();
  std::unordered_set<SimpleToken>::iterator it = a.begin();
  while (it != a.end()) {
    PkbTables::VAR value = it->getVal();
    pkbTables->addVar(value);
    varsUsed.insert(value);
    it++;
  };

  // populate the constants used in the condition
  std::unordered_set<SimpleToken> tempConstantsUsed =
      conditionParser->getUsedConstants();
  it = tempConstantsUsed.begin();
  while (it != tempConstantsUsed.end()) {
    PkbTables::CONSTANT value = it->getVal();
    pkbTables->addConstant(value);
    it++;
  }

  // populate the condition table
  pkbTables->addConditionVars(lineNo, varsUsed);

  unionSet<PkbTables::PROC>(&(ifStmtlistParser->getProcsUsed()), &procsUsed);
  unionSet<PkbTables::PROC>(&(elseStmtlistParser->getProcsUsed()), &procsUsed);
}

void IfStatementParser::populate(PkbTables *pkbTables) {
  ifStmtlistParser->populate(pkbTables);
  elseStmtlistParser->populate(pkbTables);
  unionSet<>(&(ifStmtlistParser->getVarsModified()), &varsModified);
  unionSet<>(&(ifStmtlistParser->getVarsUsed()), &varsUsed);
  unionSet<>(&(elseStmtlistParser->getVarsModified()), &varsModified);
  unionSet<>(&(elseStmtlistParser->getVarsUsed()), &varsUsed);

  // populate Parent Table
  std::vector<PkbTables::LINE_NO> temp =
      ifStmtlistParser->getStatementsLineNo();
  for (size_t i = 0; i < temp.size(); i++) {
    pkbTables->addParent(temp.at(i), lineNo);
  }
  temp = elseStmtlistParser->getStatementsLineNo();

  for (size_t i = 0; i < temp.size(); i++) {
    pkbTables->addParent(temp.at(i), lineNo);
  }

  // populate to PkbTables
  populateRelationshipTables(pkbTables);
};

// StatementListParser

StatementListParser::StatementListParser(CODE_CONTENT content,
                                         PkbTables::PROC parent) {
  stmtlistContent = content;
  parentProcedure = parent;
}

void StatementListParser::extractStatements(CODE_CONTENT content) {
  CODE_CONTENT temp = content;
  while (!temp.empty()) {
    int curserPos = 0;
    if (temp.size() < 3) {
      throw InvalidStatementSyntaxException(
          "Statement length is too small to be possibly valid in procedure " +
          parentProcedure);
    }

    SimpleToken initial = temp.at(0);
    SimpleToken second = temp.at(1);

    // assignment
    if ((initial.getTokenType() == SimpleToken::TokenType::WORD) &&
        (temp.at(1).getTokenType() == SimpleToken::TokenType::ASSIGN)) {
      try {
        while (temp.at(curserPos).getTokenType() !=
               SimpleToken::TokenType::SEMI_COLON) {
          curserPos++;
        }
        CODE_CONTENT currentLine(temp.cbegin() + 2, temp.cbegin() + curserPos);
        temp = CODE_CONTENT(temp.cbegin() + curserPos + 1, temp.cend());
        AssignmentStatementParser *parser = new AssignmentStatementParser(
            initial.getVal(), currentLine, parentProcedure);
        statementParsers.push_back(parser);
      } catch (ParseException &p) {
        ignore(p);
        throw;
      } catch (std::exception &e) {
        ignore(e);
        throw InvalidStatementSyntaxException(
            "Error in declaring assignment statement in procedure " +
            parentProcedure);
      }
      continue;
    }

    // call
    if (initial.getVal() == "call") {
      PkbTables::PROC proc = second.getVal();
      if (temp.at(2).getTokenType() != SimpleToken::TokenType::SEMI_COLON ||
          temp.at(1).getTokenType() != SimpleToken::TokenType::WORD) {
        throw InvalidStatementSyntaxException(
            "Error in declaring call statement in procedure " +
            parentProcedure);
      }
      temp = CODE_CONTENT(temp.cbegin() + 3, temp.cend());
      CallStatementParser *parser =
          new CallStatementParser(proc, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // print
    if (initial.getVal() == "print") {
      PkbTables::VAR var = second.getVal();
      if (temp.at(2).getTokenType() != SimpleToken::TokenType::SEMI_COLON ||
          temp.at(1).getTokenType() != SimpleToken::TokenType::WORD) {
        throw InvalidStatementSyntaxException(
            "Error in declaring print statement in procedure " +
            parentProcedure);
      }
      temp = CODE_CONTENT(temp.cbegin() + 3, temp.cend());
      PrintStatementParser *parser =
          new PrintStatementParser(var, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // read
    if (initial.getVal() == "read") {
      PkbTables::VAR var = second.getVal();
      if (temp.at(2).getTokenType() != SimpleToken::TokenType::SEMI_COLON ||
          temp.at(1).getTokenType() != SimpleToken::TokenType::WORD) {
        throw InvalidStatementSyntaxException(
            "Error in declaring read statement in procedure " +
            parentProcedure);
      }
      temp = CODE_CONTENT(temp.cbegin() + 3, temp.cend());
      ReadStatementParser *parser =
          new ReadStatementParser(var, parentProcedure);
      statementParsers.push_back(parser);
      continue;
    }

    // while
    if (initial.getVal() == "while") {
      try {
        temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());
        std::pair<CODE_CONTENT, CODE_CONTENT> separated =
            isolateFirstBlock(temp, SimpleToken::TokenType::OPEN_P,
                              SimpleToken::TokenType::CLOSE_P);

        CODE_CONTENT conditionBlock = separated.first;
        temp = separated.second;
        separated = isolateFirstBlock(temp, SimpleToken::TokenType::OPEN_B,
                                      SimpleToken::TokenType::CLOSE_B);

        CODE_CONTENT statementListBlock = separated.first;
        temp = separated.second;
        WhileStatementParser *parser = new WhileStatementParser(
            conditionBlock, statementListBlock, parentProcedure);
        statementParsers.push_back(parser);
      } catch (ParseException &p) {
        ignore(p);
        throw;
      } catch (std::exception &e) {
        ignore(e);
        throw InvalidStatementSyntaxException(
            "syntax error in declaring while statement in procedure " +
            parentProcedure);
      }
      continue;
    }

    // if
    if (initial.getVal() == "if") {
      temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());
      try {
        std::pair<CODE_CONTENT, CODE_CONTENT> separated =
            isolateFirstBlock(temp, SimpleToken::TokenType::OPEN_P,
                              SimpleToken::TokenType::CLOSE_P);

        CODE_CONTENT conditionBlock = separated.first;
        temp = separated.second;

        if (temp.at(0).getVal() != "then") {
          throw InvalidStatementSyntaxException(
              "'Then' keyword missing in if statement in procedure " +
              parentProcedure);
        }
        temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());
        separated = isolateFirstBlock(temp, SimpleToken::TokenType::OPEN_B,
                                      SimpleToken::TokenType::CLOSE_B);

        CODE_CONTENT ifStatementlistBlock = separated.first;
        temp = separated.second;
        if (temp.at(0).getVal() != "else") {
          throw InvalidStatementSyntaxException(
              "'else' keyword missing in if statement in procedure " +
              parentProcedure);
        }
        temp = CODE_CONTENT(temp.cbegin() + 1, temp.cend());
        separated = isolateFirstBlock(temp, SimpleToken::TokenType::OPEN_B,
                                      SimpleToken::TokenType::CLOSE_B);
        CODE_CONTENT elseStatementlistBlock = separated.first;
        temp = separated.second;
        IfStatementParser *parser =
            new IfStatementParser(conditionBlock, ifStatementlistBlock,
                                  elseStatementlistBlock, parentProcedure);
        statementParsers.push_back(parser);
      } catch (InvalidStatementSyntaxException &i) {
        ignore(i);
        throw;
        throw i;
      } catch (ParseException &p) {
        ignore(p);
        throw;
      } catch (std::exception &e) {
        ignore(e);
        throw InvalidStatementSyntaxException(
            "syntax error in if statement in procedure " + parentProcedure);
      }
      continue;
    }

    throw InvalidStatementSyntaxException(
        "Failed to detect statement type in procedure " + parentProcedure);
  }
}

void StatementListParser::parse(LineNumberCounter *lineCounter,
                                PkbTables *pkbTables) {
  if (stmtlistContent.size() == 0) {
    throw EmptyStatementListException(parentProcedure);
  }

  extractStatements(stmtlistContent);

  for (size_t i = 0; i < statementParsers.size(); i++) {
    StatementParser *st = statementParsers.at(i);
    st->parse(lineCounter, pkbTables);
    std::unordered_set<PkbTables::PROC> procsToBeAppended = st->getProcsUsed();
    procsUsed.insert(procsToBeAppended.begin(), procsToBeAppended.end());
  }
};

void StatementListParser::populate(PkbTables *pkbTables) {
  for (StatementParser *st : statementParsers) {
    // populate each statement
    st->populate(pkbTables);
    // union the statements' vars table information.
    unionSet<>(&(st->getVarsModified()), &varsModified);
    unionSet<>(&(st->getVarsUsed()), &varsUsed);
  }

  // populate follow table
  PkbTables::LINE_NO rollingLineNo = 0;
  for (StatementParser *st : statementParsers) {
    if (rollingLineNo != 0) {
      pkbTables->addFollow(rollingLineNo, st->getLineNumber());
    }
    rollingLineNo = st->getLineNumber();
  }
};

std::vector<PkbTables::LINE_NO> StatementListParser::getStatementsLineNo() {
  std::vector<PkbTables::LINE_NO> output;
  for (StatementParser *st : statementParsers) {
    output.push_back(st->getLineNumber());
  }
  return output;
}

// ProcedureParser
ProcedureParser::ProcedureParser(PkbTables::PROC name, CODE_CONTENT content) {
  procedureName = name;
  procedureContent = content;
}

void ProcedureParser::parse(LineNumberCounter *lineCounter,
                            PkbTables *pkbTables) {
  statementListParser =
      new StatementListParser(procedureContent, procedureName);
  statementListParser->parse(lineCounter, pkbTables);
  std::unordered_set<PkbTables::PROC> procs_to_be_appended =
      statementListParser->getProcsUsed();
  unionSet<>(&(statementListParser->getProcsUsed()), &procsUsed);
};

void ProcedureParser::populate(PkbTables *pkbTables) {
  statementListParser->populate(pkbTables);
  unionSet<>(&(statementListParser->getVarsModified()), &varsModified);
  unionSet<>(&(statementListParser->getVarsUsed()), &varsUsed);

  pkbTables->addModifiesProc(procedureName, varsModified);
  pkbTables->addUsesProc(procedureName, varsUsed);
}

// others

// isolating first block contained by the specified open and clode brackets.
// Note the first CODE_CONTENT returned has the brackets removed already.
std::pair<CODE_CONTENT, CODE_CONTENT>
isolateFirstBlock(CODE_CONTENT p, SimpleToken::TokenType open,
                  SimpleToken::TokenType close) {

  if (p.front().getTokenType() != open) {
    throw std::exception();
  }

  int curserPos = 1;

  int bracketCount = 1;

  while (bracketCount != 0) {
    if (p.at(curserPos).getTokenType() == open) {
      bracketCount++;
    } else if (p.at(curserPos).getTokenType() == close) {
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

/** @brief Add varsUsed or varsModified table into the given pkbTables.
 */
void addSetToPkbTables(PkbTables::VARS *vars, PkbTables::LINE_NO line,
                       std::string relationship, PkbTables *pkbTables) {
  if (relationship == "uses") {
    pkbTables->addUses(line, *vars);
  }

  if (relationship == "modifies") {
    pkbTables->addModifies(line, *vars);
  }
}
