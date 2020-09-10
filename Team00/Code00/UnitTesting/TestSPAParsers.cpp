#include "CppUnitTest.h"
#include "stdafx.h"

#include "Parser.h"
#include "Token.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestParser){

  public :

      TEST_METHOD(TestisolateFirstBlock){CODE_CONTENT aux;
PKB pkb;
aux.push_back(Token("procedure"));
aux.push_back(Token("aux"));
aux.push_back(Token("{"));
aux.push_back(Token("read"));
aux.push_back(Token("x"));
aux.push_back(Token(";"));
aux.push_back(Token("read"));
aux.push_back(Token("y"));
aux.push_back(Token(";"));
aux.push_back(Token("while"));
aux.push_back(Token("("));
aux.push_back(Token("y"));
aux.push_back(Token("!="));
aux.push_back(Token("0"));
aux.push_back(Token(")"));
aux.push_back(Token("{"));
aux.push_back(Token("y"));
aux.push_back(Token("="));
aux.push_back(Token("y"));
aux.push_back(Token("/"));
aux.push_back(Token("x"));
aux.push_back(Token(";"));
aux.push_back(Token("read"));
aux.push_back(Token("y"));
aux.push_back(Token(";"));
aux.push_back(Token("}"));
aux.push_back(Token("print"));
aux.push_back(Token("x"));
aux.push_back(Token(";"));
aux.push_back(Token("}"));
Token name = aux.at(1);
CODE_CONTENT body(aux.cbegin() + 2, aux.cend());
std::pair<CODE_CONTENT, CODE_CONTENT> result =
    isolateFirstBlock(body, TokenEnum::OPEN_B, TokenEnum::CLOSE_B);
Assert::IsTrue(result.second.size() == 0);
Assert::IsTrue(result.first.at(0).getVal() == "read");

} // namespace UnitTesting

TEST_METHOD(TestSimpleStatementParsersAndStatementListParser) {
  PKB pkb;
  LineNumberCounter lc;

  std::string name = "x";

  pkb.addProc("aux");

  ReadStatementParser a(name, "aux");
  a.parse(&lc, &pkb);
  a.populate(&pkb);
  MODIFIES_TABLE temp = pkb.getModifiesTable();

  Assert::IsTrue(a.getLineNumber() == "1");
  Assert::IsTrue(a.getProcsUsed().size() == 0);
  Assert::IsTrue(a.getVarsModified().size() == 1);
  Assert::IsTrue(a.getVarsUsed().size() == 0);

  PrintStatementParser p("y", "aux");

  pkb.addProc("aux");
  p.parse(&lc, &pkb);
  p.populate(&pkb);
  Assert::IsTrue(p.getLineNumber() == "2");
  Assert::IsTrue(p.getProcsUsed().size() == 0);
  Assert::IsTrue(p.getVarsModified().size() == 0);
  Assert::IsTrue(p.getVarsUsed().size() == 1);

  PKB newpkb;
  LineNumberCounter newlc;
  CODE_CONTENT statement_list;
  PROC proc = "aux";
  newpkb.addProc(proc);
  statement_list.push_back(Token("read"));
  statement_list.push_back(Token("x"));
  statement_list.push_back(Token(";"));
  statement_list.push_back(Token("read"));
  statement_list.push_back(Token("y"));
  statement_list.push_back(Token(";"));
  statement_list.push_back(Token("print"));
  statement_list.push_back(Token("y"));
  statement_list.push_back(Token(";"));

  StatementListParser slp(statement_list, proc);
  slp.parse(&newlc, &newpkb);
  slp.populate(&newpkb);
  Assert::IsTrue(slp.getProcsUsed().size() == 0);
  Assert::IsTrue(slp.getVarsModified().size() == 2);
  Assert::IsTrue(slp.getVarsUsed().size() == 1);
}

TEST_METHOD(TestSimpleProcedureParser) {
  PKB pkb;
  LineNumberCounter lc;
  PROC proc = "aux";
  pkb.addProc(proc);
  CODE_CONTENT procedure;
  procedure.push_back(Token("read"));
  procedure.push_back(Token("x"));
  procedure.push_back(Token(";"));
  procedure.push_back(Token("read"));
  procedure.push_back(Token("y"));
  procedure.push_back(Token(";"));
  procedure.push_back(Token("print"));
  procedure.push_back(Token("y"));
  procedure.push_back(Token(";"));

  ProcedureParser pp(proc, procedure);
  pp.parse(&lc, &pkb);
  pp.populate(&pkb);

  Assert::IsTrue(pp.getVarsModified().size() == 2);
  Assert::IsTrue(pp.getVarsUsed().size() == 1);
}
}
;
}
