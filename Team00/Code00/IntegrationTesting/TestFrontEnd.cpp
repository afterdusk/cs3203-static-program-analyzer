#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestFrontEnd) {
public:
  Pkb pkb;
  PkbTables *pkbTables;

  TEST_METHOD_INITIALIZE(SetUp) {
    pkbTables = pkb.getTables();
    std::ifstream program("../Tests/Simple_1.txt");
    if (!(program.is_open())) {
      std::cout << "Unable to open SIMPLE program file." << std::endl;
    }
    std::string input((std::istreambuf_iterator<char>(program)),
                      (std::istreambuf_iterator<char>()));
    Parser parser(input, pkbTables);
    parser.parse();
  }

  TEST_METHOD(TestVariableTable) {
    std::vector<PkbTables::VAR> vars;

    vars.push_back("x");
    vars.push_back("y");
    vars.push_back("length");
    vars.push_back("t");
    vars.push_back("tan");
    vars.push_back("sin");
    vars.push_back("cos");
    vars.push_back("pi");
    vars.push_back("area");

    Assert::IsTrue(vars == pkbTables->getVarTable().keys);
  }

  TEST_METHOD(TestProcedureTable) {
    std::vector<PkbTables::PROC> procs;
    procs.push_back("foo");
    procs.push_back("bar");
    procs.push_back("ellipse");

    Assert::IsTrue(procs == pkbTables->getProcTable().keys);
  }

  TEST_METHOD(TestUsedVariables) {
    PkbTables::VAR_TABLE_INDEXES usedVars;
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("3")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("length"));
    usedVars.insert(pkbTables->getVarTable().map.at("pi"));
    usedVars.insert(pkbTables->getVarTable().map.at("area"));
    usedVars.insert(pkbTables->getVarTable().map.at("sin"));
    usedVars.insert(pkbTables->getVarTable().map.at("cos"));
    usedVars.insert(pkbTables->getVarTable().map.at("tan"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("5")) == usedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("7")) == usedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("8")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("length"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("6")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("9")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("length"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("10")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("length"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("11")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("pi"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("area"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("12")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("sin"));
    usedVars.insert(pkbTables->getVarTable().map.at("cos"));
    usedVars.insert(pkbTables->getVarTable().map.at("tan"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("13")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("14")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("15")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("17")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("20")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("pi"));
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("22")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("area"));
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("23")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("24")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("25")) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("area"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("26")) == usedVars);

    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("1"))
                       .empty());
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("2"))
                       .empty());
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("4"))
                       .empty());
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("16"))
                       .empty());
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("18"))
                       .empty());
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("19"))
                       .empty());
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getUsesTable().map.at("21"))
                       .empty());

    PkbTables::PROC_TABLE_INDEX index;

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("length"));
    usedVars.insert(pkbTables->getVarTable().map.at("pi"));
    usedVars.insert(pkbTables->getVarTable().map.at("area"));
    usedVars.insert(pkbTables->getVarTable().map.at("sin"));
    usedVars.insert(pkbTables->getVarTable().map.at("cos"));
    usedVars.insert(pkbTables->getVarTable().map.at("tan"));
    index = pkbTables->getProcTable().map.at("foo");
    Assert::IsTrue(pkbTables->getUsesProcTable().map.at(index) == usedVars);
    index = pkbTables->getProcTable().map.at("bar");
    Assert::IsTrue(pkbTables->getUsesProcTable().map.at(index) == usedVars);

    usedVars.clear();
    usedVars.insert(pkbTables->getVarTable().map.at("x"));
    usedVars.insert(pkbTables->getVarTable().map.at("y"));
    usedVars.insert(pkbTables->getVarTable().map.at("pi"));
    usedVars.insert(pkbTables->getVarTable().map.at("area"));
    index = pkbTables->getProcTable().map.at("ellipse");
    Assert::IsTrue(pkbTables->getUsesProcTable().map.at(index) == usedVars);
  }

  TEST_METHOD(TestModifieVriables) {
    PkbTables::VAR_TABLE_INDEXES modifiedVars;

    modifiedVars.insert(pkbTables->getVarTable().map.at("x"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("1")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("14")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("16")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("18")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("24")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("2")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("15")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("19")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("25")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("length"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("3")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("17")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("20")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("t"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("4")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("tan"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("sin"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("cos"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("pi"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("area"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("x"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("y"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("length"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("5")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("7")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("8")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("tan"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("9")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("sin"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("10")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("cos"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("11")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("pi"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("area"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("x"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("12")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("x"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("y"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("13")) ==
                   modifiedVars);
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("23")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("pi"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("21")) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("area"));
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("22")) ==
                   modifiedVars);

    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("6"))
                       .empty());
    Assert::IsTrue(std::get<PkbTables::VAR_TABLE_INDEXES>(
                       pkbTables->getModifiesTable().map.at("26"))
                       .empty());

    PkbTables::PROC_TABLE_INDEX index;

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("tan"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("sin"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("cos"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("pi"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("area"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("x"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("y"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("length"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("t"));
    index = pkbTables->getProcTable().map.at("foo");
    Assert::IsTrue(pkbTables->getModifiesProcTable().map.at(index) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("tan"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("sin"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("cos"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("pi"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("area"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("x"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("y"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("length"));
    index = pkbTables->getProcTable().map.at("bar");
    Assert::IsTrue(pkbTables->getModifiesProcTable().map.at(index) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert(pkbTables->getVarTable().map.at("pi"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("area"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("x"));
    modifiedVars.insert(pkbTables->getVarTable().map.at("y"));
    index = pkbTables->getProcTable().map.at("ellipse");
    Assert::IsTrue(pkbTables->getModifiesProcTable().map.at(index) ==
                   modifiedVars);
  }

  TEST_METHOD(TestFollowTable) {
    Assert::IsTrue(pkbTables->getFollowTable().map.size() == 16);
    // procedure foo
    Assert::IsTrue(pkbTables->getFollowTable().map.at("1") == "2");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("2") == "3");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("3") == "4");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("4") == "5");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("5") == "6");

    // procedure bar
    Assert::IsTrue(pkbTables->getFollowTable().map.at("9") == "10");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("10") == "11");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("11") == "12");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("12") == "13");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("13") == "17");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("14") == "15");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("18") == "19");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("19") == "20");

    // procedure ellipse
    Assert::IsTrue(pkbTables->getFollowTable().map.at("21") == "22");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("22") == "23");
    Assert::IsTrue(pkbTables->getFollowTable().map.at("24") == "25");
  }

  TEST_METHOD(TestParentTable) {
    Assert::IsTrue(pkbTables->getParentTable().map.size() == 16);

    // procedure bar
    Assert::IsTrue(pkbTables->getParentTable().map.at("8") == "7");
    Assert::IsTrue(pkbTables->getParentTable().map.at("9") == "8");
    Assert::IsTrue(pkbTables->getParentTable().map.at("10") == "8");
    Assert::IsTrue(pkbTables->getParentTable().map.at("11") == "8");
    Assert::IsTrue(pkbTables->getParentTable().map.at("12") == "8");
    Assert::IsTrue(pkbTables->getParentTable().map.at("13") == "8");
    Assert::IsTrue(pkbTables->getParentTable().map.at("14") == "13");
    Assert::IsTrue(pkbTables->getParentTable().map.at("15") == "13");
    Assert::IsTrue(pkbTables->getParentTable().map.at("16") == "13");
    Assert::IsTrue(pkbTables->getParentTable().map.at("17") == "8");
    Assert::IsTrue(pkbTables->getParentTable().map.at("18") == "7");
    Assert::IsTrue(pkbTables->getParentTable().map.at("19") == "7");
    Assert::IsTrue(pkbTables->getParentTable().map.at("20") == "7");

    // procedure ellipse
    Assert::IsTrue(pkbTables->getParentTable().map.at("24") == "23");
    Assert::IsTrue(pkbTables->getParentTable().map.at("25") == "23");
    Assert::IsTrue(pkbTables->getParentTable().map.at("26") == "23");
  }

  TEST_METHOD(TestStmtProcTable) {
    for (int i = 1; i <= 6; i++) {
      Assert::IsTrue(pkbTables->getStatementProcTable().map.at(
                         std::to_string(i)) == "foo");
    }
    for (int i = 7; i <= 20; i++) {
      Assert::IsTrue(pkbTables->getStatementProcTable().map.at(
                         std::to_string(i)) == "bar");
    }
    for (int i = 21; i <= 26; i++) {
      Assert::IsTrue(pkbTables->getStatementProcTable().map.at(
                         std::to_string(i)) == "ellipse");
    }
  }

  TEST_METHOD(TestStatementTypeTable) {
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("1") ==
                   PkbTables::StatementType::READ);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("2") ==
                   PkbTables::StatementType::READ);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("3") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("4") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("5") ==
                   PkbTables::StatementType::CALL);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("6") ==
                   PkbTables::StatementType::PRINT);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("7") ==
                   PkbTables::StatementType::IF);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("8") ==
                   PkbTables::StatementType::WHILE);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("9") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("10") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("11") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("12") ==
                   PkbTables::StatementType::CALL);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("13") ==
                   PkbTables::StatementType::IF);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("14") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("15") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("16") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("17") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("18") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("19") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("20") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("21") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("22") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("23") ==
                   PkbTables::StatementType::IF);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("24") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("25") ==
                   PkbTables::StatementType::ASSIGN);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at("26") ==
                   PkbTables::StatementType::PRINT);
  }

  TEST_METHOD(TestAST) {
    const PkbTables::AST *root;
    root = &(pkbTables->getAssignAstTable().map.at("3"));
    Assert::IsTrue(root->op == TNode::Op::Plus);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Times);
    Assert::IsTrue(root->left->value == "");
    Assert::IsTrue(root->right->op == TNode::Op::Times);
    Assert::IsTrue(root->right->value == "");
    Assert::IsTrue(root->left->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->left->value == "x");
    Assert::IsTrue(root->left->left->left == nullptr);
    Assert::IsTrue(root->left->left->right == nullptr);
    Assert::IsTrue(root->left->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->right->value == "x");
    Assert::IsTrue(root->left->right->left == nullptr);
    Assert::IsTrue(root->left->right->right == nullptr);
    Assert::IsTrue(root->right->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->left->value == "y");
    Assert::IsTrue(root->right->left->left == nullptr);
    Assert::IsTrue(root->right->left->right == nullptr);
    Assert::IsTrue(root->right->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->right->value == "y");
    Assert::IsTrue(root->right->right->left == nullptr);
    Assert::IsTrue(root->right->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("4"));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "30");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("9"));
    Assert::IsTrue(root->op == TNode::Op::Divide);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->value == "y");
    Assert::IsTrue(root->left->left == nullptr);
    Assert::IsTrue(root->left->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "x");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("10"));
    Assert::IsTrue(root->op == TNode::Op::Divide);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->value == "y");
    Assert::IsTrue(root->left->left == nullptr);
    Assert::IsTrue(root->left->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "length");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("11"));
    Assert::IsTrue(root->op == TNode::Op::Divide);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->value == "x");
    Assert::IsTrue(root->left->left == nullptr);
    Assert::IsTrue(root->left->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "length");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("14"));
    Assert::IsTrue(root->op == TNode::Op::Divide);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->value == "y");
    Assert::IsTrue(root->left->left == nullptr);
    Assert::IsTrue(root->left->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "2");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("15"));
    Assert::IsTrue(root->op == TNode::Op::Divide);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->value == "x");
    Assert::IsTrue(root->left->left == nullptr);
    Assert::IsTrue(root->left->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "2");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("16"));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "0");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    Assert::IsTrue(pkbTables->getAssignAstTable().map.at("17") ==
                   pkbTables->getAssignAstTable().map.at("3"));

    root = &(pkbTables->getAssignAstTable().map.at("18"));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "10");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("19"));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "10");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    Assert::IsTrue(pkbTables->getAssignAstTable().map.at("20") ==
                   pkbTables->getAssignAstTable().map.at("3"));

    root = &(pkbTables->getAssignAstTable().map.at("21"));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "3");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("22"));
    Assert::IsTrue(root->op == TNode::Op::Times);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Times);
    Assert::IsTrue(root->left->value == "");
    Assert::IsTrue(root->left->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->left->value == "pi");
    Assert::IsTrue(root->left->left->left == nullptr);
    Assert::IsTrue(root->left->left->right == nullptr);
    Assert::IsTrue(root->left->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->right->value == "x");
    Assert::IsTrue(root->left->right->left == nullptr);
    Assert::IsTrue(root->left->right->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "y");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("24"));
    Assert::IsTrue(root->op == TNode::Op::Plus);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->value == "x");
    Assert::IsTrue(root->left->left == nullptr);
    Assert::IsTrue(root->left->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "1");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at("25"));
    Assert::IsTrue(root->op == TNode::Op::Plus);
    Assert::IsTrue(root->value == "");
    Assert::IsTrue(root->left->op == TNode::Op::Plus);
    Assert::IsTrue(root->left->value == "");
    Assert::IsTrue(root->left->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->left->value == "y");
    Assert::IsTrue(root->left->left->left == nullptr);
    Assert::IsTrue(root->left->left->right == nullptr);
    Assert::IsTrue(root->left->right->op == TNode::Op::Divide);
    Assert::IsTrue(root->left->right->value == "");
    Assert::IsTrue(root->left->right->left->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->right->left->value == "y");
    Assert::IsTrue(root->left->right->left->left == nullptr);
    Assert::IsTrue(root->left->right->left->right == nullptr);
    Assert::IsTrue(root->left->right->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->left->right->right->value == "x");
    Assert::IsTrue(root->left->right->right->left == nullptr);
    Assert::IsTrue(root->left->right->right->right == nullptr);
    Assert::IsTrue(root->right->op == TNode::Op::Unknown);
    Assert::IsTrue(root->right->value == "1");
    Assert::IsTrue(root->right->left == nullptr);
    Assert::IsTrue(root->right->right == nullptr);
  }

  TEST_METHOD(TestConstantTable) {
    std::unordered_set<PkbTables::CONSTANT> constants;
    constants.insert("30");
    constants.insert("0");
    constants.insert("1");
    constants.insert("2");
    constants.insert("10");
    constants.insert("3");
    Assert::IsTrue(pkbTables->getConstantTable() == constants);
  }

  /*
  * This test method test on call relationship between procedures,
  * which will only be useful fot iteration 2 & 3.
  TEST_METHOD(TestCallTable) {
    PkbTables::CALLS calledProcs;
    PkbTables::PROC_TABLE_INDEX index;
    calledProcs.insert(pkbTables->getProcTable().map.at("bar"));
    index = pkbTables->getProcTable().map.at("foo");
    Assert::IsTrue(pkbTables->getCallsTable().map.at(index) == calledProcs);

    calledProcs.clear();
    calledProcs.insert(pkbTables->getProcTable().map.at("ellipse"));
    Assert::IsTrue(pkbTables->getCallsTable().map.at(
                       pkbTables->getProcTable().map.at("bar")) == calledProcs);

    Assert::IsTrue(pkbTables->getCallsTable().map.size() == 2);
  }*/
};
} // namespace IntegrationTesting
