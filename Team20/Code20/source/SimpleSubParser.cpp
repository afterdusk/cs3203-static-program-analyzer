#include "SimpleSubParser.h"
#include <iostream>
#include <stdio.h>

// SubParser
template <typename T>
void SubParser::unionSet(std::unordered_set<T> *from,
                         std::unordered_set<T> *target) {
  target->insert(from->begin(), from->end());
}
template <class T>
void SubParser::unionSet(std::set<T> *from, std::set<T> *target) {
  target->insert(from->begin(), from->end());
}

void SubParser::addNextBips(PkbTables *pkbTables, NEXT_BIPS nextBips,
                            PkbTables::LINE_NO prev) {
  NEXT_BIPS::iterator it = nextBips.begin();
  while (it != nextBips.end()) {
    // In case it is a normal line
    if (it->index() == 0) {
      LINE_NEXT_BIP val = std::get<LINE_NEXT_BIP>(*it);
      pkbTables->addNextBip(val, prev);
    }
    // In case it returns from another branch
    else {
      RETURN_NEXT_BIP val = std::get<RETURN_NEXT_BIP>(*it);
      PkbTables::LINE_NO returnLine = std::get<0>(val);
      PkbTables::CALL_BRANCH_LABEL label = std::get<1>(val);
      PkbTables::NEXT_BIP toAdd =
          std::make_tuple(prev, PkbTables::CallBranch::ReturnFrom, label);
      pkbTables->addNextBip(returnLine, toAdd);
    }
    it++;
  }
}

// isolating first block contained by the specified open and clode brackets.
// Note the first CODE_CONTENT returned has the brackets removed already.
std::pair<SubParser::CODE_CONTENT, SubParser::CODE_CONTENT>
SubParser::isolateFirstBlock(CODE_CONTENT p, SimpleToken::TokenType open,
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

// StatementParser

void StatementParser::populateModifiesUsesTables(PkbTables *pkbTables) {
  pkbTables->addUses(lineNo, varsUsed);
  pkbTables->addModifies(lineNo, varsModified);
}

void StatementParser::populateStatementTables(PkbTables *pkbTables) {
  pkbTables->addStatementProc(lineNo, parentProcedure);
  pkbTables->addStatementType(lineNo, statementType);
}

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
    PkbTables::PROCS procsToBeAppended = st->getProcsUsed();
    procsUsed.insert(procsToBeAppended.begin(), procsToBeAppended.end());
  }
};

void StatementListParser::populate(PkbTables *pkbTables,
                                   ProcedureUtil *procedureUtil) {
  for (StatementParser *st : statementParsers) {
    // populate each statement
    st->populate(pkbTables, procedureUtil);
    // union the statements' vars table information.
    unionSet<>(&(st->getVarsModified()), &varsModified);
    unionSet<>(&(st->getVarsUsed()), &varsUsed);
  }

  // populate follow table
  populateFollowsTable(pkbTables);

  // populate Next and NextBip table
  populateNextTable(pkbTables);
  populateNextBipTable(pkbTables);
};

SubParser::LINE_NOS_VECTOR StatementListParser::getStatementsLineNo() {
  LINE_NOS_VECTOR output;
  for (StatementParser *st : statementParsers) {
    output.push_back(st->getLineNumber());
  }
  return output;
}

void StatementListParser::populateFollowsTable(PkbTables *pkbTables) {
  PkbTables::LINE_NO rollingLineNo = 0;
  for (StatementParser *st : statementParsers) {
    if (rollingLineNo != 0) {
      pkbTables->addFollow(rollingLineNo, st->getLineNumber());
    }
    rollingLineNo = st->getLineNumber();
  }
}

void StatementListParser::populateNextTable(PkbTables *pkbTables) {
  StatementParser *prev = NULL;
  PkbTables::NEXTS tempExits;
  for (StatementParser *st : statementParsers) {
    if (prev != NULL) {
      if (!tempExits.empty()) {
        PkbTables::NEXTS::iterator it = tempExits.begin();
        while (it != tempExits.end()) {
          pkbTables->addNext(*it, st->getLineNumber());
          it++;
        }
        tempExits.clear();
      } else {
        pkbTables->addNext(prev->getLineNumber(), st->getLineNumber());
      }
    }

    // if the statement is if statement, we need to get exit of if statement
    if (st->getType() == PkbTables::StatementType::If) {
      tempExits = st->getExits();
    }
    prev = st;
  }
  if (!tempExits.empty()) {
    exits = tempExits;
  } else {
    exits.insert(prev->getLineNumber());
  }
}

void StatementListParser::populateNextBipTable(PkbTables *pkbTables) {
  StatementParser *prev = NULL;
  NEXT_BIPS tempBipExits;
  for (StatementParser *st : statementParsers) {
    if (prev != NULL) {
      if (!tempBipExits.empty()) {
        addNextBips(pkbTables, tempBipExits, st->getLineNumber());
        tempBipExits.clear();
      } else {
        pkbTables->addNextBip(prev->getLineNumber(), st->getLineNumber());
      }
    }
    // if the statement is if statement, we need to get exit of if statement
    if (st->getType() == PkbTables::StatementType::If) {
      tempBipExits = st->getBipExits();
    }
    // if the statement is call statement, we need to get exit of call statement
    // for NextBip
    if (st->getType() == PkbTables::StatementType::Call) {
      tempBipExits = st->getBipExits();
    }
    prev = st;
  }
  if (!tempBipExits.empty()) {
    bipExits = tempBipExits;
  } else {
    bipExits.insert(prev->getLineNumber());
  }
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
  rightParser = new SimpleExprParserWrapper(right, lineNo);
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

  pkbTables->addAssignAst(lineNo, std::move(rightParser->getRootNodePtr()));
};

void AssignmentStatementParser::populate(PkbTables *pkbTables,
                                         ProcedureUtil *procedureUtil) {
  populateModifiesUsesTables(pkbTables);
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

void CallStatementParser::populate(PkbTables *pkbTables,
                                   ProcedureUtil *procedureUtil) {
  PkbTables::MODIFIES_PROC_TABLE modifiesProcTable =
      pkbTables->getModifiesProcTable();
  PkbTables::USES_PROC_TABLE usesProcTable = pkbTables->getUsesProcTable();
  unionSet<>(&(usesProcTable.map[proc]), &varsUsed);
  unionSet<>(&(modifiesProcTable.map[proc]), &varsModified);

  pkbTables->addModifies(lineNo, proc);
  pkbTables->addUses(lineNo, proc);

  handleAndPopulateNextBipTable(pkbTables, procedureUtil);
};

void CallStatementParser::handleAndPopulateNextBipTable(
    PkbTables *pkbTables, ProcedureUtil *procedureUtil) {
  // populate NextBip information
  std::pair<PkbTables::LINE_NO, NEXT_BIPS> p = procedureUtil->get(proc);
  // add JumpTo item
  PkbTables::CALL_BRANCH_LABEL from;
  from.push_back(lineNo);
  PkbTables::NEXT_BIP item =
      std::make_tuple(p.first, PkbTables::CallBranch::JumpTo, from);
  pkbTables->addNextBip(lineNo, item);

  // label return items
  NEXT_BIPS returnBips = p.second;
  NEXT_BIPS::iterator it = returnBips.begin();
  while (it != returnBips.end()) {
    PkbTables::CALL_BRANCH_LABEL label;
    RETURN_NEXT_BIP returnItem;
    if (it->index() == 0) {
      LINE_NEXT_BIP l = std::get<LINE_NEXT_BIP>(*it);
      label.push_back(lineNo);
      returnItem = std::make_tuple(l, label);
    } else {
      RETURN_NEXT_BIP l = std::get<RETURN_NEXT_BIP>(*it);
      label = std::get<1>(l);
      label.push_back(lineNo);
      returnItem = std::make_tuple(std::get<0>(l), label);
    }
    bipExits.insert(returnItem);
    it++;
  }
}

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

void PrintStatementParser::populate(PkbTables *pkbTables,
                                    ProcedureUtil *procedureUtil) {
  populateModifiesUsesTables(pkbTables);
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

void ReadStatementParser::populate(PkbTables *pkbTables,
                                   ProcedureUtil *procedureUtil) {
  populateModifiesUsesTables(pkbTables);
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
void WhileStatementParser::populate(PkbTables *pkbTables,
                                    ProcedureUtil *procedureUtil) {
  stmtlistParser->populate(pkbTables, procedureUtil);
  unionSet<>(&(stmtlistParser->getVarsUsed()), &varsUsed);
  unionSet<>(&(stmtlistParser->getVarsModified()), &varsModified);

  populateParentTable(pkbTables);
  populateModifiesUsesTables(pkbTables);
  populateNextTable(pkbTables);
  populateNextBipTable(pkbTables);
};

void WhileStatementParser::populateParentTable(PkbTables *pkbTables) {
  LINE_NOS_VECTOR temp = stmtlistParser->getStatementsLineNo();
  for (size_t i = 0; i < temp.size(); i++) {
    pkbTables->addParent(temp.at(i), lineNo);
  }
}

void WhileStatementParser::populateNextTable(PkbTables *pkbTables) {
  pkbTables->addNext(lineNo, stmtlistParser->getStatementsLineNo()[0]);

  PkbTables::LINE_NOS tempExits = stmtlistParser->getExits();
  PkbTables::LINE_NOS::iterator it = tempExits.begin();
  while (it != tempExits.end()) {
    pkbTables->addNext(*it, lineNo);
    it++;
  }
}

void WhileStatementParser::populateNextBipTable(PkbTables *pkbTables) {
  pkbTables->addNextBip(lineNo, stmtlistParser->getStatementsLineNo()[0]);
  NEXT_BIPS tempBipExits = stmtlistParser->getBipExits();
  addNextBips(pkbTables, tempBipExits, lineNo);
}

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

void IfStatementParser::populate(PkbTables *pkbTables,
                                 ProcedureUtil *procedureUtil) {
  ifStmtlistParser->populate(pkbTables, procedureUtil);
  elseStmtlistParser->populate(pkbTables, procedureUtil);
  // union varsUsed/Modified
  unionSet<>(&(ifStmtlistParser->getVarsModified()), &varsModified);
  unionSet<>(&(ifStmtlistParser->getVarsUsed()), &varsUsed);
  unionSet<>(&(elseStmtlistParser->getVarsModified()), &varsModified);
  unionSet<>(&(elseStmtlistParser->getVarsUsed()), &varsUsed);
  // union Next and NextBip exits
  unionSet<>(&(ifStmtlistParser->getExits()), &exits);
  unionSet<>(&(elseStmtlistParser->getExits()), &exits);
  unionSet<>(&(ifStmtlistParser->getBipExits()), &bipExits);
  unionSet<>(&(elseStmtlistParser->getBipExits()), &bipExits);

  populateParentTable(pkbTables);
  populateModifiesUsesTables(pkbTables);
  populateNextTable(pkbTables);
  populateNextBipTable(pkbTables);
};

void IfStatementParser::populateParentTable(PkbTables *pkbTables) {
  LINE_NOS_VECTOR temp = ifStmtlistParser->getStatementsLineNo();
  for (size_t i = 0; i < temp.size(); i++) {
    pkbTables->addParent(temp.at(i), lineNo);
  }
  temp = elseStmtlistParser->getStatementsLineNo();

  for (size_t i = 0; i < temp.size(); i++) {
    pkbTables->addParent(temp.at(i), lineNo);
  }
}

void IfStatementParser::populateNextTable(PkbTables *pkbTables) {
  PkbTables::LINE_NO ifFirstLine = ifStmtlistParser->getStatementsLineNo()[0];
  pkbTables->addNext(lineNo, ifFirstLine);
  PkbTables::LINE_NO elseFirstLine =
      elseStmtlistParser->getStatementsLineNo()[0];
  pkbTables->addNext(lineNo, elseFirstLine);
}

void IfStatementParser::populateNextBipTable(PkbTables *pkbTables) {
  PkbTables::LINE_NO ifFirstLine = ifStmtlistParser->getStatementsLineNo()[0];
  pkbTables->addNextBip(lineNo, ifFirstLine);
  PkbTables::LINE_NO elseFirstLine =
      elseStmtlistParser->getStatementsLineNo()[0];
  pkbTables->addNextBip(lineNo, elseFirstLine);
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
  PkbTables::PROCS procs_to_be_appended = statementListParser->getProcsUsed();
  unionSet<>(&(statementListParser->getProcsUsed()), &procsUsed);
};

void ProcedureParser::populate(PkbTables *pkbTables,
                               ProcedureUtil *procedureUtil) {
  statementListParser->populate(pkbTables, procedureUtil);
  // union varsUsed/Modified
  unionSet<>(&(statementListParser->getVarsModified()), &varsModified);
  unionSet<>(&(statementListParser->getVarsUsed()), &varsUsed);
  // union exits for Next and NextBip
  unionSet<>(&(statementListParser->getExits()), &exits);
  unionSet<>(&(statementListParser->getBipExits()), &bipExits);

  populateUsesModifiesTables(pkbTables);
  populateCallsTable(pkbTables);

  // add this procedure's information of entry and exits to procedureUtil
  procedureUtil->put(procedureName,
                     statementListParser->getStatementsLineNo().at(0),
                     bipExits);
}

void ProcedureParser::populateUsesModifiesTables(PkbTables *pkbTables) {
  pkbTables->addModifiesProc(procedureName, varsModified);
  pkbTables->addUsesProc(procedureName, varsUsed);
}

void ProcedureParser::populateCallsTable(PkbTables *pkbTables) {
  PkbTables::PROCS::iterator it = procsUsed.begin();
  while (it != procsUsed.end()) {
    pkbTables->addCall(procedureName, *it);
    it++;
  }
}

// ProgramParser

ProgramParser::ProgramParser(CODE_CONTENT content) { programContent = content; }

void ProgramParser::parse(LineNumberCounter *lineCounter,
                          PkbTables *pkbTables) {
  std::vector<std::pair<PkbTables::PROC, CODE_CONTENT>>
      procedureNameAndBodyList = extractProcedures(programContent);
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

  for (int i = 0; i < noOfProcedures; i++) {
    std::pair<PkbTables::PROC, CODE_CONTENT> procedure =
        procedureNameAndBodyList[i];
    ProcedureParser *temp =
        new ProcedureParser(procedure.first, procedure.second);
    temp->parse(lineCounter, pkbTables);
    procedureParsers.push_back(temp);
  }
}

void ProgramParser::populate(PkbTables *pkbTables,
                             ProcedureUtil *procedureUtil) {
  std::vector<ProcedureParser *>::iterator it;
  for (it = procedureParsers.begin(); it != procedureParsers.end(); ++it) {
    (*it)->populate(pkbTables, procedureUtil);
  }
}

void ProgramParser::topologicalSortProcedures() {
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
    PkbTables::PROCS temp = procedureParsers[i]->getProcsUsed();
    PkbTables::PROCS::iterator it = temp.begin();
    while (it != temp.end()) {
      adjMatrix[i].insert(std::make_pair<>(a[*it], true));
      it++;
    }
  }

  for (size_t i = 0; i < procs.size(); i++) {
    if (visited[i] == 0) {

      DFSrec(i, &visited, &prev, &adjMatrix, procs.size(), &output, recStack);
    }
  }

  std::vector<ProcedureParser *> newSequence;
  for (size_t i = 0; i < procs.size(); i++) {
    newSequence.push_back(procedureParsers.at(output[i]));
  }
  procedureParsers = newSequence;
}

void ProgramParser::DFSrec(
    int u, std::vector<int> *visited, std::vector<int> *prev,
    std::unordered_map<int, std::unordered_map<int, bool>> *m, int n,
    std::vector<int> *output, std::vector<int> recStack) {
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

std::vector<std::pair<PkbTables::PROC, SubParser::CODE_CONTENT>>
ProgramParser::extractProcedures(CODE_CONTENT tokens) {
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
