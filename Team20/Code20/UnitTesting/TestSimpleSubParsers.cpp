#include "CppUnitTest.h"
#include "stdafx.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTesting {
TEST_CLASS(TestStatementParsers) {

public:
  Pkb pkb;
  PkbTables *pkbTables = pkb.getTables();
  SubParser::ProcedureUtil *procedureUtil = new SubParser::ProcedureUtil();

  TEST_METHOD(TestisolateFirstBlock) {
    SubParser::CODE_CONTENT aux;
    aux.push_back(SimpleToken("procedure"));
    aux.push_back(SimpleToken("aux"));
    aux.push_back(SimpleToken("{"));
    aux.push_back(SimpleToken("read"));
    aux.push_back(SimpleToken("x"));
    aux.push_back(SimpleToken(";"));
    aux.push_back(SimpleToken("read"));
    aux.push_back(SimpleToken("y"));
    aux.push_back(SimpleToken(";"));
    aux.push_back(SimpleToken("while"));
    aux.push_back(SimpleToken("("));
    aux.push_back(SimpleToken("y"));
    aux.push_back(SimpleToken("!="));
    aux.push_back(SimpleToken("0"));
    aux.push_back(SimpleToken(")"));
    aux.push_back(SimpleToken("{"));
    aux.push_back(SimpleToken("y"));
    aux.push_back(SimpleToken("="));
    aux.push_back(SimpleToken("y"));
    aux.push_back(SimpleToken("/"));
    aux.push_back(SimpleToken("x"));
    aux.push_back(SimpleToken(";"));
    aux.push_back(SimpleToken("read"));
    aux.push_back(SimpleToken("y"));
    aux.push_back(SimpleToken(";"));
    aux.push_back(SimpleToken("}"));
    aux.push_back(SimpleToken("print"));
    aux.push_back(SimpleToken("x"));
    aux.push_back(SimpleToken(";"));
    aux.push_back(SimpleToken("}"));
    SimpleToken name = aux.at(1);
    SubParser::CODE_CONTENT body(aux.cbegin() + 2, aux.cend());
    std::pair<SubParser::CODE_CONTENT, SubParser::CODE_CONTENT> result =
        SubParser::isolateFirstBlock(body, SimpleToken::TokenType::OPEN_B,
                                     SimpleToken::TokenType::CLOSE_B);
    Assert::IsTrue(result.second.size() == 0);
    Assert::IsTrue(result.first.at(0).getVal() == "read");

  } // namespace UnitTesting

  TEST_METHOD(TestSimpleStatementParsers) {
    SubParser::LineNumberCounter lc;

    std::string name = "x";

    pkbTables->addProc("aux");
    SubParser::NEXT_BIPS l;
    l.insert(100);
    procedureUtil->put("aux", 100, l);

    // Read Statement
    ReadStatementParser a(name, "aux");
    a.parse(&lc, pkbTables);
    a.populate(pkbTables, procedureUtil);
    PkbTables::MODIFIES_TABLE temp = pkbTables->getModifiesTable();

    Assert::IsTrue(a.getLineNumber() == 1);
    Assert::IsTrue(a.getProcsUsed().size() == 0);
    Assert::IsTrue(a.getVarsModified().size() == 1);
    Assert::IsTrue(a.getVarsUsed().size() == 0);

    // Print Statement
    PrintStatementParser p("y", "aux");
    p.parse(&lc, pkbTables);
    p.populate(pkbTables, procedureUtil);
    Assert::IsTrue(p.getLineNumber() == 2);
    Assert::IsTrue(p.getProcsUsed().size() == 0);
    Assert::IsTrue(p.getVarsModified().size() == 0);
    Assert::IsTrue(p.getVarsUsed().size() == 1);

    // Assign Statement: (x + y/ z) * a % ((2 + 3) + 1 - 2 * k) + 1

    SubParser::CODE_CONTENT assignment;
    assignment.push_back(SimpleToken("("));
    assignment.push_back(SimpleToken("x"));
    assignment.push_back(SimpleToken("+"));
    assignment.push_back(SimpleToken("y"));
    assignment.push_back(SimpleToken("/"));
    assignment.push_back(SimpleToken("z"));
    assignment.push_back(SimpleToken(")"));
    assignment.push_back(SimpleToken("*"));
    assignment.push_back(SimpleToken("a"));
    assignment.push_back(SimpleToken("%"));
    assignment.push_back(SimpleToken("("));
    assignment.push_back(SimpleToken("("));
    assignment.push_back(SimpleToken("2"));
    assignment.push_back(SimpleToken("+"));
    assignment.push_back(SimpleToken("3"));
    assignment.push_back(SimpleToken(")"));
    assignment.push_back(SimpleToken("+"));
    assignment.push_back(SimpleToken("1"));
    assignment.push_back(SimpleToken("-"));
    assignment.push_back(SimpleToken("2"));
    assignment.push_back(SimpleToken("*"));
    assignment.push_back(SimpleToken("k"));
    assignment.push_back(SimpleToken(")"));
    assignment.push_back(SimpleToken("+"));
    assignment.push_back(SimpleToken("1"));

    AssignmentStatementParser assignmengStatement1("x", assignment, "aux");
    assignmengStatement1.parse(&lc, pkbTables);
    assignmengStatement1.populate(pkbTables, procedureUtil);
    Assert::IsTrue(assignmengStatement1.getLineNumber() == 3);
    Assert::IsTrue(assignmengStatement1.getProcsUsed().size() == 0);
    Assert::IsTrue(assignmengStatement1.getVarsModified().size() == 1);
    Assert::IsTrue(assignmengStatement1.getVarsUsed().size() == 5);

    // Call statement
    SubParser::CODE_CONTENT second;
    pkbTables->addProc("second");
    second.push_back(SimpleToken("print"));
    second.push_back(SimpleToken("lalala"));
    second.push_back(SimpleToken(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables, procedureUtil);

    CallStatementParser c("second", "aux");
    c.parse(&lc, pkbTables);
    c.populate(pkbTables, procedureUtil);
    Assert::IsTrue(c.getProcsUsed().count("second") == 1);
    Assert::IsTrue(c.getVarsUsed().size() == 1);

    // while statement
    SubParser::CODE_CONTENT condition;
    condition.push_back(SimpleToken("x"));
    condition.push_back(SimpleToken("!="));
    condition.push_back(SimpleToken("100"));

    SubParser::CODE_CONTENT nestedList;
    nestedList.push_back(SimpleToken("read"));
    nestedList.push_back(SimpleToken("x"));
    nestedList.push_back(SimpleToken(";"));
    nestedList.push_back(SimpleToken("read"));
    nestedList.push_back(SimpleToken("y"));
    nestedList.push_back(SimpleToken(";"));
    nestedList.push_back(SimpleToken("print"));
    nestedList.push_back(SimpleToken("y"));
    nestedList.push_back(SimpleToken(";"));
    nestedList.push_back(SimpleToken("call"));
    nestedList.push_back(SimpleToken("second"));
    nestedList.push_back(SimpleToken(";"));

    WhileStatementParser whileStatement(condition, nestedList, "aux");
    whileStatement.parse(&lc, pkbTables);
    whileStatement.populate(pkbTables, procedureUtil);

    Assert::IsTrue(whileStatement.getLineNumber() == 6);
    Assert::IsTrue(whileStatement.getProcsUsed().count("second") == 1);
    Assert::IsTrue(whileStatement.getVarsUsed().size() == 3);
    Assert::IsTrue(whileStatement.getVarsModified().size() == 2);

    // if statement
    SubParser::CODE_CONTENT condition_if;
    condition_if.push_back(SimpleToken("x"));
    condition_if.push_back(SimpleToken("!="));
    condition_if.push_back(SimpleToken("1"));

    SubParser::CODE_CONTENT ifNestedList;
    ifNestedList.push_back(SimpleToken("read"));
    ifNestedList.push_back(SimpleToken("x"));
    ifNestedList.push_back(SimpleToken(";"));
    ifNestedList.push_back(SimpleToken("read"));
    ifNestedList.push_back(SimpleToken("y"));
    ifNestedList.push_back(SimpleToken(";"));
    ifNestedList.push_back(SimpleToken("print"));
    ifNestedList.push_back(SimpleToken("y"));
    ifNestedList.push_back(SimpleToken(";"));
    ifNestedList.push_back(SimpleToken("call"));
    ifNestedList.push_back(SimpleToken("second"));
    ifNestedList.push_back(SimpleToken(";"));

    SubParser::CODE_CONTENT elseNestedList;
    elseNestedList.push_back(SimpleToken("k"));
    elseNestedList.push_back(SimpleToken("="));
    elseNestedList.push_back(SimpleToken("1"));
    elseNestedList.push_back(SimpleToken(";"));

    IfStatementParser ifStatement(condition_if, ifNestedList, elseNestedList,
                                  "aux");
    ifStatement.parse(&lc, pkbTables);
    ifStatement.populate(pkbTables, procedureUtil);

    Assert::IsTrue(ifStatement.getLineNumber() == 11);
    Assert::IsTrue(ifStatement.getProcsUsed().count("second") == 1);
    Assert::IsTrue(ifStatement.getVarsUsed().size() == 3);
    Assert::IsTrue(ifStatement.getVarsModified().size() == 3);
    PkbTables::LINE_NOS nextExits = {15, 16};
    Assert::IsTrue(ifStatement.getExits() == nextExits);
    SubParser::NEXT_BIPS bipExits;
    PkbTables::CALL_BRANCH_LABEL label;
    label.push_back(15);
    SubParser::RETURN_NEXT_BIP next = std::make_tuple(4, label);
    bipExits.insert(next);
    bipExits.insert(16);
    Assert::IsTrue(ifStatement.getBipExits() == bipExits);
  }

  TEST_METHOD(TestStatementListParser) {
    SubParser::LineNumberCounter lc;
    SubParser::CODE_CONTENT statement_list;
    PkbTables::PROC proc = "aux";
    pkbTables->addProc(proc);
    SubParser::CODE_CONTENT second;
    pkbTables->addProc("second");
    second.push_back(SimpleToken("print"));
    second.push_back(SimpleToken("lalala"));
    second.push_back(SimpleToken(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables, procedureUtil);

    statement_list.push_back(SimpleToken("read"));
    statement_list.push_back(SimpleToken("x"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("read"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("print"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("x"));
    statement_list.push_back(SimpleToken("="));
    statement_list.push_back(SimpleToken("("));
    statement_list.push_back(SimpleToken("x"));
    statement_list.push_back(SimpleToken("+"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken("/"));
    statement_list.push_back(SimpleToken("z"));
    statement_list.push_back(SimpleToken(")"));
    statement_list.push_back(SimpleToken("*"));
    statement_list.push_back(SimpleToken("a"));
    statement_list.push_back(SimpleToken("%"));
    statement_list.push_back(SimpleToken("("));
    statement_list.push_back(SimpleToken("("));
    statement_list.push_back(SimpleToken("2"));
    statement_list.push_back(SimpleToken("+"));
    statement_list.push_back(SimpleToken("3"));
    statement_list.push_back(SimpleToken(")"));
    statement_list.push_back(SimpleToken("+"));
    statement_list.push_back(SimpleToken("1"));
    statement_list.push_back(SimpleToken("-"));
    statement_list.push_back(SimpleToken("2"));
    statement_list.push_back(SimpleToken("*"));
    statement_list.push_back(SimpleToken("k"));
    statement_list.push_back(SimpleToken(")"));
    statement_list.push_back(SimpleToken("+"));
    statement_list.push_back(SimpleToken("1"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("call"));
    statement_list.push_back(SimpleToken("second"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("while"));
    statement_list.push_back(SimpleToken("("));
    statement_list.push_back(SimpleToken("x"));
    statement_list.push_back(SimpleToken("!="));
    statement_list.push_back(SimpleToken("1"));
    statement_list.push_back(SimpleToken(")"));
    statement_list.push_back(SimpleToken("{"));
    statement_list.push_back(SimpleToken("read"));
    statement_list.push_back(SimpleToken("x"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("read"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("print"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("call"));
    statement_list.push_back(SimpleToken("second"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("}"));
    statement_list.push_back(SimpleToken("print"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("if"));
    statement_list.push_back(SimpleToken("("));
    statement_list.push_back(SimpleToken("x"));
    statement_list.push_back(SimpleToken("!="));
    statement_list.push_back(SimpleToken("1"));
    statement_list.push_back(SimpleToken(")"));
    statement_list.push_back(SimpleToken("then"));
    statement_list.push_back(SimpleToken("{"));
    statement_list.push_back(SimpleToken("read"));
    statement_list.push_back(SimpleToken("x"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("read"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("print"));
    statement_list.push_back(SimpleToken("y"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("call"));
    statement_list.push_back(SimpleToken("second"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("}"));
    statement_list.push_back(SimpleToken("else"));
    statement_list.push_back(SimpleToken("{"));
    statement_list.push_back(SimpleToken("k"));
    statement_list.push_back(SimpleToken("="));
    statement_list.push_back(SimpleToken("1"));
    statement_list.push_back(SimpleToken(";"));
    statement_list.push_back(SimpleToken("}"));

    StatementListParser slp(statement_list, proc);
    slp.parse(&lc, pkbTables);
    slp.populate(pkbTables, procedureUtil);
    Assert::IsTrue(slp.getProcsUsed().size() == 1);
    Assert::IsTrue(slp.getVarsModified().size() == 3);
    Assert::IsTrue(slp.getVarsUsed().size() == 6);
    PkbTables::LINE_NOS nextExits = {17, 18};
    Assert::IsTrue(slp.getExits() == nextExits);
    SubParser::NEXT_BIPS bipExits;
    PkbTables::CALL_BRANCH_LABEL label;
    label.push_back(17);
    SubParser::RETURN_NEXT_BIP next = std::make_tuple(1, label);
    bipExits.insert(next);
    bipExits.insert(18);
    Assert::IsTrue(slp.getBipExits() == bipExits);
  }

  TEST_METHOD(TestSimpleProcedureParser) {
    SubParser::LineNumberCounter lc;
    PkbTables::PROC proc = "aux";
    pkbTables->addProc(proc);
    SubParser::CODE_CONTENT second;
    pkbTables->addProc("second");
    second.push_back(SimpleToken("print"));
    second.push_back(SimpleToken("lalala"));
    second.push_back(SimpleToken(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables, procedureUtil);

    SubParser::CODE_CONTENT procedure;
    procedure.push_back(SimpleToken("read"));
    procedure.push_back(SimpleToken("x"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("read"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("print"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("x"));
    procedure.push_back(SimpleToken("="));
    procedure.push_back(SimpleToken("("));
    procedure.push_back(SimpleToken("x"));
    procedure.push_back(SimpleToken("+"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken("/"));
    procedure.push_back(SimpleToken("z"));
    procedure.push_back(SimpleToken(")"));
    procedure.push_back(SimpleToken("*"));
    procedure.push_back(SimpleToken("a"));
    procedure.push_back(SimpleToken("%"));
    procedure.push_back(SimpleToken("("));
    procedure.push_back(SimpleToken("("));
    procedure.push_back(SimpleToken("2"));
    procedure.push_back(SimpleToken("+"));
    procedure.push_back(SimpleToken("3"));
    procedure.push_back(SimpleToken(")"));
    procedure.push_back(SimpleToken("+"));
    procedure.push_back(SimpleToken("1"));
    procedure.push_back(SimpleToken("-"));
    procedure.push_back(SimpleToken("2"));
    procedure.push_back(SimpleToken("*"));
    procedure.push_back(SimpleToken("k"));
    procedure.push_back(SimpleToken(")"));
    procedure.push_back(SimpleToken("+"));
    procedure.push_back(SimpleToken("1"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("call"));
    procedure.push_back(SimpleToken("second"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("while"));
    procedure.push_back(SimpleToken("("));
    procedure.push_back(SimpleToken("x"));
    procedure.push_back(SimpleToken("!="));
    procedure.push_back(SimpleToken("1"));
    procedure.push_back(SimpleToken(")"));
    procedure.push_back(SimpleToken("{"));
    procedure.push_back(SimpleToken("read"));
    procedure.push_back(SimpleToken("x"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("read"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("print"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("call"));
    procedure.push_back(SimpleToken("second"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("}"));
    procedure.push_back(SimpleToken("print"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("if"));
    procedure.push_back(SimpleToken("("));
    procedure.push_back(SimpleToken("x"));
    procedure.push_back(SimpleToken("!="));
    procedure.push_back(SimpleToken("1"));
    procedure.push_back(SimpleToken(")"));
    procedure.push_back(SimpleToken("then"));
    procedure.push_back(SimpleToken("{"));
    procedure.push_back(SimpleToken("read"));
    procedure.push_back(SimpleToken("x"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("read"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("print"));
    procedure.push_back(SimpleToken("y"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("call"));
    procedure.push_back(SimpleToken("second"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("}"));
    procedure.push_back(SimpleToken("else"));
    procedure.push_back(SimpleToken("{"));
    procedure.push_back(SimpleToken("k"));
    procedure.push_back(SimpleToken("="));
    procedure.push_back(SimpleToken("1"));
    procedure.push_back(SimpleToken(";"));
    procedure.push_back(SimpleToken("}"));

    ProcedureParser pp(proc, procedure);
    pp.parse(&lc, pkbTables);
    pp.populate(pkbTables, procedureUtil);

    Assert::IsTrue(pp.getVarsModified().size() == 3);
    Assert::IsTrue(pp.getVarsUsed().size() == 6);
    Assert::IsTrue(pp.getProcsUsed().count("second"));

    PkbTables::LINE_NOS nextExits = {17, 18};
    Assert::IsTrue(pp.getExits() == nextExits);
    SubParser::NEXT_BIPS bipExits;
    PkbTables::CALL_BRANCH_LABEL label;
    label.push_back(17);
    SubParser::RETURN_NEXT_BIP next = std::make_tuple(1, label);
    bipExits.insert(next);
    bipExits.insert(18);
    Assert::IsTrue(pp.getBipExits() == bipExits);
  }

  TEST_METHOD(TestSimpleProgramParser) {
    SubParser::LineNumberCounter lc;
    PkbTables::PROC proc = "aux";
    pkbTables->addProc(proc);
    SubParser::CODE_CONTENT second;
    pkbTables->addProc("second");
    second.push_back(SimpleToken("print"));
    second.push_back(SimpleToken("lalala"));
    second.push_back(SimpleToken(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables, procedureUtil);

    SubParser::CODE_CONTENT program;
    program.push_back(SimpleToken("procedure"));
    program.push_back(SimpleToken("second"));
    program.push_back(SimpleToken("{"));
    program.push_back(SimpleToken("print"));
    program.push_back(SimpleToken("lalala"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("}"));
    program.push_back(SimpleToken("procedure"));
    program.push_back(SimpleToken("aux"));
    program.push_back(SimpleToken("{"));
    program.push_back(SimpleToken("read"));
    program.push_back(SimpleToken("x"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("read"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("print"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("x"));
    program.push_back(SimpleToken("="));
    program.push_back(SimpleToken("("));
    program.push_back(SimpleToken("x"));
    program.push_back(SimpleToken("+"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken("/"));
    program.push_back(SimpleToken("z"));
    program.push_back(SimpleToken(")"));
    program.push_back(SimpleToken("*"));
    program.push_back(SimpleToken("a"));
    program.push_back(SimpleToken("%"));
    program.push_back(SimpleToken("("));
    program.push_back(SimpleToken("("));
    program.push_back(SimpleToken("2"));
    program.push_back(SimpleToken("+"));
    program.push_back(SimpleToken("3"));
    program.push_back(SimpleToken(")"));
    program.push_back(SimpleToken("+"));
    program.push_back(SimpleToken("1"));
    program.push_back(SimpleToken("-"));
    program.push_back(SimpleToken("2"));
    program.push_back(SimpleToken("*"));
    program.push_back(SimpleToken("k"));
    program.push_back(SimpleToken(")"));
    program.push_back(SimpleToken("+"));
    program.push_back(SimpleToken("1"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("call"));
    program.push_back(SimpleToken("second"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("while"));
    program.push_back(SimpleToken("("));
    program.push_back(SimpleToken("x"));
    program.push_back(SimpleToken("!="));
    program.push_back(SimpleToken("1"));
    program.push_back(SimpleToken(")"));
    program.push_back(SimpleToken("{"));
    program.push_back(SimpleToken("read"));
    program.push_back(SimpleToken("x"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("read"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("print"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("call"));
    program.push_back(SimpleToken("second"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("}"));
    program.push_back(SimpleToken("print"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("if"));
    program.push_back(SimpleToken("("));
    program.push_back(SimpleToken("x"));
    program.push_back(SimpleToken("!="));
    program.push_back(SimpleToken("1"));
    program.push_back(SimpleToken(")"));
    program.push_back(SimpleToken("then"));
    program.push_back(SimpleToken("{"));
    program.push_back(SimpleToken("read"));
    program.push_back(SimpleToken("x"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("read"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("print"));
    program.push_back(SimpleToken("y"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("call"));
    program.push_back(SimpleToken("second"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("}"));
    program.push_back(SimpleToken("else"));
    program.push_back(SimpleToken("{"));
    program.push_back(SimpleToken("k"));
    program.push_back(SimpleToken("="));
    program.push_back(SimpleToken("1"));
    program.push_back(SimpleToken(";"));
    program.push_back(SimpleToken("}"));
    program.push_back(SimpleToken("}"));

    ProgramParser pp(program);
    pp.parse(&lc, pkbTables);
    pp.topologicalSortProcedures();
    pp.populate(pkbTables, procedureUtil);
    std::vector<PkbTables::PROC> p;
    p.push_back("second");
    p.push_back("aux");
    Assert::IsTrue(pp.procs == p);
  }
};
} // namespace UnitTesting
