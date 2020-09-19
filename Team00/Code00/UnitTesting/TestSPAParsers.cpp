#include "CppUnitTest.h"
#include "stdafx.h"
#include <fstream>

#include "Parser.h"
#include "Token.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestStatementParsers) {

public:
  Pkb pkb;
  SetUpTests setUpTests = SetUpTests::SetUpTests(pkb);
  PkbTables &pkbTables = pkb.getTables();

  TEST_METHOD(TestisolateFirstBlock) {
    CODE_CONTENT aux;
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

  TEST_METHOD(TestSimpleStatementParsers) {
    LineNumberCounter lc;

    std::string name = "x";

    pkbTables.addProc("aux");

    // Read Statement
    ReadStatementParser a(name, "aux");
    a.parse(&lc, pkbTables);
    a.populate(pkbTables);
    PkbTables::MODIFIES_TABLE temp = pkbTables.getModifiesTable();

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
    assignment.push_back(Token("("));
    assignment.push_back(Token("x"));
    assignment.push_back(Token("+"));
    assignment.push_back(Token("y"));
    assignment.push_back(Token("/"));
    assignment.push_back(Token("z"));
    assignment.push_back(Token(")"));
    assignment.push_back(Token("*"));
    assignment.push_back(Token("a"));
    assignment.push_back(Token("%"));
    assignment.push_back(Token("("));
    assignment.push_back(Token("("));
    assignment.push_back(Token("2"));
    assignment.push_back(Token("+"));
    assignment.push_back(Token("3"));
    assignment.push_back(Token(")"));
    assignment.push_back(Token("+"));
    assignment.push_back(Token("1"));
    assignment.push_back(Token("-"));
    assignment.push_back(Token("2"));
    assignment.push_back(Token("*"));
    assignment.push_back(Token("k"));
    assignment.push_back(Token(")"));
    assignment.push_back(Token("+"));
    assignment.push_back(Token("1"));

    AssignmentStatementParser assignmengStatement1("x", assignment, "aux");
    assignmengStatement1.parse(&lc, pkbTables);
    assignmengStatement1.populate(pkbTables);
    Assert::IsTrue(assignmengStatement1.getLineNumber() == "3");
    Assert::IsTrue(assignmengStatement1.getProcsUsed().size() == 0);
    Assert::IsTrue(assignmengStatement1.getVarsModified().size() == 1);
    Assert::IsTrue(assignmengStatement1.getVarsUsed().size() == 5);

    // Call statement
    CODE_CONTENT second;
    pkbTables.addProc("second");
    second.push_back(Token("print"));
    second.push_back(Token("lalala"));
    second.push_back(Token(";"));
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
    condition.push_back(Token("x"));
    condition.push_back(Token("!="));
    condition.push_back(Token("100"));

    CODE_CONTENT nestedList;
    nestedList.push_back(Token("read"));
    nestedList.push_back(Token("x"));
    nestedList.push_back(Token(";"));
    nestedList.push_back(Token("read"));
    nestedList.push_back(Token("y"));
    nestedList.push_back(Token(";"));
    nestedList.push_back(Token("print"));
    nestedList.push_back(Token("y"));
    nestedList.push_back(Token(";"));
    nestedList.push_back(Token("call"));
    nestedList.push_back(Token("second"));
    nestedList.push_back(Token(";"));

    WhileStatementParser whileStatement(condition, nestedList, "aux");
    whileStatement.parse(&lc, pkbTables);
    whileStatement.populate(pkbTables);

    Assert::IsTrue(whileStatement.getLineNumber() == "6");
    Assert::IsTrue(whileStatement.getProcsUsed().count("second") == 1);
    Assert::IsTrue(whileStatement.getVarsUsed().size() == 3);
    Assert::IsTrue(whileStatement.getVarsModified().size() == 2);

    // if statement
    CODE_CONTENT condition_if;
    condition_if.push_back(Token("x"));
    condition_if.push_back(Token("!="));
    condition_if.push_back(Token("1"));

    CODE_CONTENT ifNestedList;
    ifNestedList.push_back(Token("read"));
    ifNestedList.push_back(Token("x"));
    ifNestedList.push_back(Token(";"));
    ifNestedList.push_back(Token("read"));
    ifNestedList.push_back(Token("y"));
    ifNestedList.push_back(Token(";"));
    ifNestedList.push_back(Token("print"));
    ifNestedList.push_back(Token("y"));
    ifNestedList.push_back(Token(";"));
    ifNestedList.push_back(Token("call"));
    ifNestedList.push_back(Token("second"));
    ifNestedList.push_back(Token(";"));

    CODE_CONTENT elseNestedList;
    elseNestedList.push_back(Token("k"));
    elseNestedList.push_back(Token("="));
    elseNestedList.push_back(Token("1"));
    elseNestedList.push_back(Token(";"));

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
    pkbTables.addProc(proc);
    CODE_CONTENT second;
    pkbTables.addProc("second");
    second.push_back(Token("print"));
    second.push_back(Token("lalala"));
    second.push_back(Token(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables);

    statement_list.push_back(Token("read"));
    statement_list.push_back(Token("x"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("read"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("print"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("x"));
    statement_list.push_back(Token("="));
    statement_list.push_back(Token("("));
    statement_list.push_back(Token("x"));
    statement_list.push_back(Token("+"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token("/"));
    statement_list.push_back(Token("z"));
    statement_list.push_back(Token(")"));
    statement_list.push_back(Token("*"));
    statement_list.push_back(Token("a"));
    statement_list.push_back(Token("%"));
    statement_list.push_back(Token("("));
    statement_list.push_back(Token("("));
    statement_list.push_back(Token("2"));
    statement_list.push_back(Token("+"));
    statement_list.push_back(Token("3"));
    statement_list.push_back(Token(")"));
    statement_list.push_back(Token("+"));
    statement_list.push_back(Token("1"));
    statement_list.push_back(Token("-"));
    statement_list.push_back(Token("2"));
    statement_list.push_back(Token("*"));
    statement_list.push_back(Token("k"));
    statement_list.push_back(Token(")"));
    statement_list.push_back(Token("+"));
    statement_list.push_back(Token("1"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("call"));
    statement_list.push_back(Token("second"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("while"));
    statement_list.push_back(Token("("));
    statement_list.push_back(Token("x"));
    statement_list.push_back(Token("!="));
    statement_list.push_back(Token("1"));
    statement_list.push_back(Token(")"));
    statement_list.push_back(Token("{"));
    statement_list.push_back(Token("read"));
    statement_list.push_back(Token("x"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("read"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("print"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("call"));
    statement_list.push_back(Token("second"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("}"));
    statement_list.push_back(Token("print"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("if"));
    statement_list.push_back(Token("("));
    statement_list.push_back(Token("x"));
    statement_list.push_back(Token("!="));
    statement_list.push_back(Token("1"));
    statement_list.push_back(Token(")"));
    statement_list.push_back(Token("then"));
    statement_list.push_back(Token("{"));
    statement_list.push_back(Token("read"));
    statement_list.push_back(Token("x"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("read"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("print"));
    statement_list.push_back(Token("y"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("call"));
    statement_list.push_back(Token("second"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("}"));
    statement_list.push_back(Token("else"));
    statement_list.push_back(Token("{"));
    statement_list.push_back(Token("k"));
    statement_list.push_back(Token("="));
    statement_list.push_back(Token("1"));
    statement_list.push_back(Token(";"));
    statement_list.push_back(Token("}"));

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
    pkbTables.addProc(proc);
    CODE_CONTENT second;
    pkbTables.addProc("second");
    second.push_back(Token("print"));
    second.push_back(Token("lalala"));
    second.push_back(Token(";"));
    ProcedureParser second_proc("second", second);
    second_proc.parse(&lc, pkbTables);
    second_proc.populate(pkbTables);

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
    procedure.push_back(Token("x"));
    procedure.push_back(Token("="));
    procedure.push_back(Token("("));
    procedure.push_back(Token("x"));
    procedure.push_back(Token("+"));
    procedure.push_back(Token("y"));
    procedure.push_back(Token("/"));
    procedure.push_back(Token("z"));
    procedure.push_back(Token(")"));
    procedure.push_back(Token("*"));
    procedure.push_back(Token("a"));
    procedure.push_back(Token("%"));
    procedure.push_back(Token("("));
    procedure.push_back(Token("("));
    procedure.push_back(Token("2"));
    procedure.push_back(Token("+"));
    procedure.push_back(Token("3"));
    procedure.push_back(Token(")"));
    procedure.push_back(Token("+"));
    procedure.push_back(Token("1"));
    procedure.push_back(Token("-"));
    procedure.push_back(Token("2"));
    procedure.push_back(Token("*"));
    procedure.push_back(Token("k"));
    procedure.push_back(Token(")"));
    procedure.push_back(Token("+"));
    procedure.push_back(Token("1"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("call"));
    procedure.push_back(Token("second"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("while"));
    procedure.push_back(Token("("));
    procedure.push_back(Token("x"));
    procedure.push_back(Token("!="));
    procedure.push_back(Token("1"));
    procedure.push_back(Token(")"));
    procedure.push_back(Token("{"));
    procedure.push_back(Token("read"));
    procedure.push_back(Token("x"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("read"));
    procedure.push_back(Token("y"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("print"));
    procedure.push_back(Token("y"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("call"));
    procedure.push_back(Token("second"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("}"));
    procedure.push_back(Token("print"));
    procedure.push_back(Token("y"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("if"));
    procedure.push_back(Token("("));
    procedure.push_back(Token("x"));
    procedure.push_back(Token("!="));
    procedure.push_back(Token("1"));
    procedure.push_back(Token(")"));
    procedure.push_back(Token("then"));
    procedure.push_back(Token("{"));
    procedure.push_back(Token("read"));
    procedure.push_back(Token("x"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("read"));
    procedure.push_back(Token("y"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("print"));
    procedure.push_back(Token("y"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("call"));
    procedure.push_back(Token("second"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("}"));
    procedure.push_back(Token("else"));
    procedure.push_back(Token("{"));
    procedure.push_back(Token("k"));
    procedure.push_back(Token("="));
    procedure.push_back(Token("1"));
    procedure.push_back(Token(";"));
    procedure.push_back(Token("}"));

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
    PkbTables::PROC_TABLE procTable = pkbTables.getProcTable();
    const PkbTables::USES_PROC_TABLE &t = pkbTables.getUsesProcTable();
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
    } catch (CyclicalProcedureCallException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }
  }

  TEST_METHOD(TestEmptyProgram) {
    std::string invalidInput = "";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (EmptyProgramException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }
  }

  TEST_METHOD(TestEmptyStatementListException) {
    std::string invalidInput = "procedure main {}";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (EmptyStatementListException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }
  }

  TEST_METHOD(TestNoProcedureException) {
    std::string invalidInput = "procedure main {call nonExist; }";
    Parser p(invalidInput, pkbTables);
    try {
      p.parse();
      Assert::Fail();
    } catch (NoProcedureException c) {
    } catch (std::exception e) {
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
    } catch (RepeatedProcedureException c) {
    } catch (std::exception e) {
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
    } catch (InvalidProcedureDeclarationException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    // procedure level missing { or }
    try {
      std::string invalidInput =
          "procedure main { read b; procedure { read a;}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidProcedureDeclarationException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main read b; } procedure { read a;}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidProcedureDeclarationException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main { read b; } read c; }";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidProcedureDeclarationException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    // statement level syntax disasters
    try {
      std::string invalidInput = "procedure main {read b}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {print b}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {call b}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {x = x + 1}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {x = x - 4}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main {if (x == 2) { read a;} else {read b;}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput = "procedure main {if (x == 2) then { read a;}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main {while (x == 2) then { read a;}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }

    try {
      std::string invalidInput =
          "procedure main {while (x == 2) then { read a}}";
      Parser p(invalidInput, pkbTables);
      p.parse();
      Assert::Fail();
    } catch (InvalidStatementSyntaxException c) {
    } catch (std::exception e) {
      Assert::Fail();
    }
  }

  TEST_METHOD(TestComplexProgram) {
    /*
      std::ifstream ifs("C:/Users/admin/source/repos/nus-cs3203/"
                        "team20-win-spa-20s1/Team00/Tests00/Sample_source.txt");

      std::string input((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));

      Parser p(input, pkbTables);
      p.parse();
      */
  }
};
} // namespace UnitTesting
