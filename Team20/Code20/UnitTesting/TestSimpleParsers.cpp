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

  TEST_METHOD(TestProgramParser) {
    std::string input =
        "procedure aux { read x; read y; print y; x = (x + y/ z) "
        "* a % ((2 + 3) + 1 - 2 * k) + 1; while (x!=1) { read x; read y; print "
        "y; call second; if (x != 1) then { read x;} else {read y;}}print y;  "
        "} "
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
    PkbTables::VARS tempa = t.map.at("a");
    PkbTables::VARS tempb = t.map.at("b");
    PkbTables::VARS tempc = t.map.at("c");
    PkbTables::VARS tempd = t.map.at("d");
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
