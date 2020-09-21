#include "CppUnitTest.h"
#include "stdafx.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestStatementParsers) {

public:
  Pkb pkb;
  PkbTables *pkbTables = pkb.getTables();

  TEST_METHOD(TestisolateFirstBlock) {
    CODE_CONTENT aux;
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
    CODE_CONTENT body(aux.cbegin() + 2, aux.cend());
    std::pair<CODE_CONTENT, CODE_CONTENT> result = isolateFirstBlock(
        body, SimpleToken::TokenType::OPEN_B, SimpleToken::TokenType::CLOSE_B);
    Assert::IsTrue(result.second.size() == 0);
    Assert::IsTrue(result.first.at(0).getVal() == "read");

  } // namespace UnitTesting

  TEST_METHOD(TestSimpleStatementParsers) {
    LineNumberCounter lc;

    std::string name = "x";

    pkbTables->addProc("aux");

    // Read Statement
    ReadStatementParser a(name, "aux");
    a.parse(&lc, pkbTables);
    a.populate(pkbTables);
    PkbTables::MODIFIES_TABLE temp = pkbTables->getModifiesTable();

    Assert::IsTrue(a.getLineNumber() == "1");
    Assert::IsTrue(a.getProcsUsed().size() == 0);
    Assert::IsTrue(a.getVarsModified().size() == 1);
    Assert::IsTrue(a.getVarsUsed().size() == 0);

    // Print Statement
    PrintStatementParser p("y", "aux");
    p.parse(&lc, pkbTables);
    p.populate(pkbTables);
    Assert::IsTrue(p.getLineNumber() == "2");
    Assert::IsTrue(p.getProcsUsed().size() == 0);
    Assert::IsTrue(p.getVarsModified().size() == 0);
    Assert::IsTrue(p.getVarsUsed().size() == 1);

    // Assign Statement: (x + y/ z) * a % ((2 + 3) + 1 - 2 * k) + 1

    CODE_CONTENT assignment;
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
    assignmengStatement1.populate(pkbTables);
    Assert::IsTrue(assignmengStatement1.getLineNumber() == "3");
    Assert::IsTrue(assignmengStatement1.getProcsUsed().size() == 0);
    Assert::IsTrue(assignmengStatement1.getVarsModified().size() == 1);
    Assert::IsTrue(assignmengStatement1.getVarsUsed().size() == 5);

    // Call statement
    CODE_CONTENT second;
    pkbTables->addProc("second");
    second.push_back(SimpleToken("print"));
    second.push_back(SimpleToken("lalala"));
    second.push_back(SimpleToken(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables);

    CallStatementParser c("second", "aux");
    c.parse(&lc, pkbTables);
    c.populate(pkbTables);
    Assert::IsTrue(c.getProcsUsed().count("second") == 1);
    Assert::IsTrue(c.getVarsUsed().size() == 1);

    // while statement
    CODE_CONTENT condition;
    condition.push_back(SimpleToken("x"));
    condition.push_back(SimpleToken("!="));
    condition.push_back(SimpleToken("100"));

    CODE_CONTENT nestedList;
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
    whileStatement.populate(pkbTables);

    Assert::IsTrue(whileStatement.getLineNumber() == "6");
    Assert::IsTrue(whileStatement.getProcsUsed().count("second") == 1);
    Assert::IsTrue(whileStatement.getVarsUsed().size() == 3);
    Assert::IsTrue(whileStatement.getVarsModified().size() == 2);

    // if statement
    CODE_CONTENT condition_if;
    condition_if.push_back(SimpleToken("x"));
    condition_if.push_back(SimpleToken("!="));
    condition_if.push_back(SimpleToken("1"));

    CODE_CONTENT ifNestedList;
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

    CODE_CONTENT elseNestedList;
    elseNestedList.push_back(SimpleToken("k"));
    elseNestedList.push_back(SimpleToken("="));
    elseNestedList.push_back(SimpleToken("1"));
    elseNestedList.push_back(SimpleToken(";"));

    IfStatementParser ifStatement(condition_if, ifNestedList, elseNestedList,
                                  "aux");
    ifStatement.parse(&lc, pkbTables);
    ifStatement.populate(pkbTables);

    Assert::IsTrue(ifStatement.getLineNumber() == "11");
    Assert::IsTrue(ifStatement.getProcsUsed().count("second") == 1);
    Assert::IsTrue(ifStatement.getVarsUsed().size() == 3);
    Assert::IsTrue(ifStatement.getVarsModified().size() == 3);
  }

  TEST_METHOD(TestStatementListParser) {
    LineNumberCounter lc;
    CODE_CONTENT statement_list;
    PkbTables::PROC proc = "aux";
    pkbTables->addProc(proc);
    CODE_CONTENT second;
    pkbTables->addProc("second");
    second.push_back(SimpleToken("print"));
    second.push_back(SimpleToken("lalala"));
    second.push_back(SimpleToken(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables);

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
    slp.populate(pkbTables);
    Assert::IsTrue(slp.getProcsUsed().size() == 1);
    Assert::IsTrue(slp.getVarsModified().size() == 3);
    Assert::IsTrue(slp.getVarsUsed().size() == 6);
  }

  TEST_METHOD(TestSimpleProcedureParser) {
    LineNumberCounter lc;
    PkbTables::PROC proc = "aux";
    pkbTables->addProc(proc);
    CODE_CONTENT second;
    pkbTables->addProc("second");
    second.push_back(SimpleToken("print"));
    second.push_back(SimpleToken("lalala"));
    second.push_back(SimpleToken(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables);

    CODE_CONTENT procedure;
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
    pp.populate(pkbTables);

    Assert::IsTrue(pp.getVarsModified().size() == 3);
    Assert::IsTrue(pp.getVarsUsed().size() == 6);
    Assert::IsTrue(pp.getProcsUsed().count("second"));
  }

  TEST_METHOD(TestProgramParser) {
    std::string input =
        "procedure aux { read x; read y; print y; x = (x + y/ z) "
        "* a % ((2 + 3) + 1 - 2 * k) + 1; while (x!=1) { read x; read y; print "
        "y; call second; } print y; if (x != 1) then { read x;} else {read y; "
        "}} "
        "procedure second { read lalala;}";
    Parser p(input, pkbTables);
    p.parse();
  }

  TEST_METHOD(TestTopologicalSort) {
    std::string validInput =
        "procedure a { call d;}  procedure b { call c; call d;} procedure c { "
        "call a; call d;} procedure d { print as;}";
    Parser p(validInput, pkbTables);
    p.parse();
    PkbTables::PROC_TABLE procTable = pkbTables->getProcTable();
    const PkbTables::USES_PROC_TABLE &t = pkbTables->getUsesProcTable();
    PkbTables::VAR_TABLE_INDEXES tempa = t.map.at(procTable.map["a"]);
    PkbTables::VAR_TABLE_INDEXES tempb = t.map.at(procTable.map["b"]);
    PkbTables::VAR_TABLE_INDEXES tempc = t.map.at(procTable.map["c"]);
    PkbTables::VAR_TABLE_INDEXES tempd = t.map.at(procTable.map["d"]);
    Assert::IsTrue(tempa.size() == 1);
    Assert::IsTrue(tempb.size() == 1);
    Assert::IsTrue(tempc.size() == 1);
    Assert::IsTrue(tempd.size() == 1);
  }

  // exceptions
  TEST_METHOD(TestCyclicalCall) {
    std::string invalidInput =
        "procedure a { call d;}  procedure b { call c; call d;} procedure c { "
        "call a; call d;} procedure d { call a;}";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (CyclicalProcedureCallException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }
  }

  TEST_METHOD(TestEmptyProgram) {
    std::string invalidInput = "";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (EmptyProgramException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }
  }

  TEST_METHOD(TestEmptyStatementListException) {
    std::string invalidInput = "procedure main {}";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (EmptyStatementListException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }
  }

  TEST_METHOD(TestNoProcedureException) {
    std::string invalidInput = "procedure main {call nonExist; }";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (NoProcedureException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }
  }

  TEST_METHOD(TestRepeatedProcedureException) {
    std::string invalidInput =
        "procedure main {read a; } procedure main { read b;}";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (RepeatedProcedureException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }
  }

  TEST_METHOD(TestSyntaxErrors) {
    // procedure has no name
    try {
      std::string invalidInput =
          "procedure main { read b; } procedure { read a;}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidProcedureDeclarationException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    // procedure level missing { or }
    try {
      std::string invalidInput =
          "procedure main { read b; procedure { read a;}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidProcedureDeclarationException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main read b; } procedure { read a;}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidProcedureDeclarationException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main { read b; } read c; }";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidProcedureDeclarationException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    // statement level syntax disasters
    try {
      std::string invalidInput = "procedure main {read b}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {print b}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {call b}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {x = x + 1}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {x = x - 4}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main {if (x == 2) { read a;} else {read b;}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {if (x == 2) then { read a;}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main {while (x == 2) then { read a;}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main {while (x == 2) then { read a}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException &c) {
      ignore(c);
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }
  }

  TEST_METHOD(TestComplexProgram) {
    try {
      std::ifstream ifs("../Tests/Sample_source.txt");

      std::string input((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
      Parser p(input, pkbTables);
      p.parse();
    } catch (std::exception &e) {
      ignore(e);
      Assert::Fail();
    }
  }
};
} // namespace UnitTesting
