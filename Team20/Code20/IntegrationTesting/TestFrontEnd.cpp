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
    std::ifstream program("../../Tests20/system_test_1_source.txt");
    std::string input((std::istreambuf_iterator<char>(program)),
                      (std::istreambuf_iterator<char>()));
    Parser parser(input, pkbTables);
    parser.parse();
  }

  TEST_METHOD(TestVariableTable) {
    std::unordered_set<PkbTables::VAR> vars;

    vars.insert("x");
    vars.insert("y");
    vars.insert("length");
    vars.insert("t");
    vars.insert("tan");
    vars.insert("sin");
    vars.insert("cos");
    vars.insert("pi");
    vars.insert("area");

    Assert::IsTrue(vars == pkbTables->getVarTable());
  }

  TEST_METHOD(TestProcedureTable) {
    std::unordered_set<PkbTables::PROC> procs;
    procs.insert("foo");
    procs.insert("bar");
    procs.insert("ellipse");

    Assert::IsTrue(procs == pkbTables->getProcTable());
  }

  TEST_METHOD(TestUsedVariables) {
    PkbTables::VARS usedVars;
    usedVars.insert("x");
    usedVars.insert("y");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(3)) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("y");
    usedVars.insert("length");
    usedVars.insert("pi");
    usedVars.insert("area");
    usedVars.insert("sin");
    usedVars.insert("cos");
    usedVars.insert("tan");

    Assert::IsTrue(std::get<PkbTables::PROC>(
                       pkbTables->getUsesTable().map.at(5)) == "bar");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(7)) == usedVars);
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(8)) == usedVars);

    usedVars.clear();
    usedVars.insert("length");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(6)) == usedVars);

    usedVars.clear();
    usedVars.insert("y");
    usedVars.insert("x");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(9)) == usedVars);

    usedVars.clear();
    usedVars.insert("y");
    usedVars.insert("length");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(10)) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("length");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(11)) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("pi");
    usedVars.insert("y");
    usedVars.insert("area");
    Assert::IsTrue(std::get<PkbTables::PROC>(
                       pkbTables->getUsesTable().map.at(12)) == "ellipse");

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("y");
    usedVars.insert("sin");
    usedVars.insert("cos");
    usedVars.insert("tan");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(13)) == usedVars);

    usedVars.clear();
    usedVars.insert("y");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(14)) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(15)) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("y");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(17)) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("y");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(20)) == usedVars);

    usedVars.clear();
    usedVars.insert("pi");
    usedVars.insert("x");
    usedVars.insert("y");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(22)) == usedVars);

    usedVars.clear();
    usedVars.insert("area");
    usedVars.insert("x");
    usedVars.insert("y");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(23)) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(24)) == usedVars);

    usedVars.clear();
    usedVars.insert("y");
    usedVars.insert("x");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(25)) == usedVars);

    usedVars.clear();
    usedVars.insert("area");
    Assert::IsTrue(std::get<PkbTables::VARS>(
                       pkbTables->getUsesTable().map.at(26)) == usedVars);

    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getUsesTable().map.at(1)).empty());
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getUsesTable().map.at(2)).empty());
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getUsesTable().map.at(4)).empty());
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getUsesTable().map.at(16))
            .empty());
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getUsesTable().map.at(18))
            .empty());
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getUsesTable().map.at(19))
            .empty());
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getUsesTable().map.at(21))
            .empty());

    PkbTables::PROC procName;

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("y");
    usedVars.insert("length");
    usedVars.insert("pi");
    usedVars.insert("area");
    usedVars.insert("sin");
    usedVars.insert("cos");
    usedVars.insert("tan");
    procName = "foo";
    Assert::IsTrue(pkbTables->getUsesProcTable().map.at(procName) == usedVars);
    procName = "bar";
    Assert::IsTrue(pkbTables->getUsesProcTable().map.at(procName) == usedVars);

    usedVars.clear();
    usedVars.insert("x");
    usedVars.insert("y");
    usedVars.insert("pi");
    usedVars.insert("area");
    procName = "ellipse";
    Assert::IsTrue(pkbTables->getUsesProcTable().map.at(procName) == usedVars);
  }

  TEST_METHOD(TestModifieVriables) {
    PkbTables::VARS modifiedVars;

    modifiedVars.insert("x");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(1)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(14)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(16)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(18)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(24)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("y");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(2)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(15)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(19)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(25)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("length");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(3)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(17)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(20)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("t");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(4)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("tan");
    modifiedVars.insert("sin");
    modifiedVars.insert("cos");
    modifiedVars.insert("pi");
    modifiedVars.insert("area");
    modifiedVars.insert("x");
    modifiedVars.insert("y");
    modifiedVars.insert("length");
    Assert::IsTrue(std::get<PkbTables::PROC>(
                       pkbTables->getModifiesTable().map.at(5)) == "bar");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(7)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(8)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("tan");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(9)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("sin");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(10)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("cos");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(11)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("pi");
    modifiedVars.insert("area");
    modifiedVars.insert("x");
    modifiedVars.insert("y");
    Assert::IsTrue(std::get<PkbTables::PROC>(
                       pkbTables->getModifiesTable().map.at(12)) == "ellipse");

    modifiedVars.clear();
    modifiedVars.insert("x");
    modifiedVars.insert("y");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(13)) ==
        modifiedVars);
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(23)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("pi");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(21)) ==
        modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("area");
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(22)) ==
        modifiedVars);

    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(6))
            .empty());
    Assert::IsTrue(
        std::get<PkbTables::VARS>(pkbTables->getModifiesTable().map.at(26))
            .empty());

    PkbTables::PROC procName;

    modifiedVars.clear();
    modifiedVars.insert("tan");
    modifiedVars.insert("sin");
    modifiedVars.insert("cos");
    modifiedVars.insert("pi");
    modifiedVars.insert("area");
    modifiedVars.insert("x");
    modifiedVars.insert("y");
    modifiedVars.insert("length");
    modifiedVars.insert("t");
    procName = "foo";
    Assert::IsTrue(pkbTables->getModifiesProcTable().map.at(procName) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("tan");
    modifiedVars.insert("sin");
    modifiedVars.insert("cos");
    modifiedVars.insert("pi");
    modifiedVars.insert("area");
    modifiedVars.insert("x");
    modifiedVars.insert("y");
    modifiedVars.insert("length");
    procName = "bar";
    Assert::IsTrue(pkbTables->getModifiesProcTable().map.at(procName) ==
                   modifiedVars);

    modifiedVars.clear();
    modifiedVars.insert("pi");
    modifiedVars.insert("area");
    modifiedVars.insert("x");
    modifiedVars.insert("y");
    procName = "ellipse";
    Assert::IsTrue(pkbTables->getModifiesProcTable().map.at(procName) ==
                   modifiedVars);
  }

  TEST_METHOD(TestFollowTable) {
    Assert::IsTrue(pkbTables->getFollowTable().map.size() == 16);
    // procedure foo
    Assert::IsTrue(pkbTables->getFollowTable().map.at(1) == 2);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(2) == 3);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(3) == 4);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(4) == 5);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(5) == 6);

    // procedure bar
    Assert::IsTrue(pkbTables->getFollowTable().map.at(9) == 10);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(10) == 11);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(11) == 12);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(12) == 13);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(13) == 17);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(14) == 15);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(18) == 19);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(19) == 20);

    // procedure ellipse
    Assert::IsTrue(pkbTables->getFollowTable().map.at(21) == 22);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(22) == 23);
    Assert::IsTrue(pkbTables->getFollowTable().map.at(24) == 25);
  }

  TEST_METHOD(TestParentTable) {
    Assert::IsTrue(pkbTables->getParentTable().map.size() == 16);

    // procedure bar
    Assert::IsTrue(pkbTables->getParentTable().map.at(8) == 7);
    Assert::IsTrue(pkbTables->getParentTable().map.at(9) == 8);
    Assert::IsTrue(pkbTables->getParentTable().map.at(10) == 8);
    Assert::IsTrue(pkbTables->getParentTable().map.at(11) == 8);
    Assert::IsTrue(pkbTables->getParentTable().map.at(12) == 8);
    Assert::IsTrue(pkbTables->getParentTable().map.at(13) == 8);
    Assert::IsTrue(pkbTables->getParentTable().map.at(14) == 13);
    Assert::IsTrue(pkbTables->getParentTable().map.at(15) == 13);
    Assert::IsTrue(pkbTables->getParentTable().map.at(16) == 13);
    Assert::IsTrue(pkbTables->getParentTable().map.at(17) == 8);
    Assert::IsTrue(pkbTables->getParentTable().map.at(18) == 7);
    Assert::IsTrue(pkbTables->getParentTable().map.at(19) == 7);
    Assert::IsTrue(pkbTables->getParentTable().map.at(20) == 7);

    // procedure ellipse
    Assert::IsTrue(pkbTables->getParentTable().map.at(24) == 23);
    Assert::IsTrue(pkbTables->getParentTable().map.at(25) == 23);
    Assert::IsTrue(pkbTables->getParentTable().map.at(26) == 23);
  }

  TEST_METHOD(TestStmtProcTable) {
    for (PkbTables::LINE_NO i = 1; i <= 6; i++) {
      Assert::IsTrue(pkbTables->getStatementProcTable().map.at(i) == "foo");
    }
    for (PkbTables::LINE_NO i = 7; i <= 20; i++) {
      Assert::IsTrue(pkbTables->getStatementProcTable().map.at(i) == "bar");
    }
    for (PkbTables::LINE_NO i = 21; i <= 26; i++) {
      Assert::IsTrue(pkbTables->getStatementProcTable().map.at(i) == "ellipse");
    }
  }

  TEST_METHOD(TestStatementTypeTable) {
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(1) ==
                   PkbTables::StatementType::Read);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(2) ==
                   PkbTables::StatementType::Read);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(3) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(4) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(5) ==
                   PkbTables::StatementType::Call);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(6) ==
                   PkbTables::StatementType::Print);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(7) ==
                   PkbTables::StatementType::If);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(8) ==
                   PkbTables::StatementType::While);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(9) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(10) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(11) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(12) ==
                   PkbTables::StatementType::Call);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(13) ==
                   PkbTables::StatementType::If);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(14) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(15) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(16) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(17) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(18) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(19) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(20) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(21) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(22) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(23) ==
                   PkbTables::StatementType::If);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(24) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(25) ==
                   PkbTables::StatementType::Assign);
    Assert::IsTrue(pkbTables->getStatementTypeTable().map.at(26) ==
                   PkbTables::StatementType::Print);
  }

  TEST_METHOD(TestAST) {
    const PkbTables::AST *root;
    root = &(pkbTables->getAssignAstTable().map.at(3));
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

    root = &(pkbTables->getAssignAstTable().map.at(4));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "30");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at(9));
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

    root = &(pkbTables->getAssignAstTable().map.at(10));
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

    root = &(pkbTables->getAssignAstTable().map.at(11));
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

    root = &(pkbTables->getAssignAstTable().map.at(14));
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

    root = &(pkbTables->getAssignAstTable().map.at(15));
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

    root = &(pkbTables->getAssignAstTable().map.at(16));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "0");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    Assert::IsTrue(pkbTables->getAssignAstTable().map.at(17) ==
                   pkbTables->getAssignAstTable().map.at(3));

    root = &(pkbTables->getAssignAstTable().map.at(18));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "10");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at(19));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "10");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    Assert::IsTrue(pkbTables->getAssignAstTable().map.at(20) ==
                   pkbTables->getAssignAstTable().map.at(3));

    root = &(pkbTables->getAssignAstTable().map.at(21));
    Assert::IsTrue(root->op == TNode::Op::Unknown);
    Assert::IsTrue(root->value == "3");
    Assert::IsTrue(root->left == nullptr);
    Assert::IsTrue(root->right == nullptr);

    root = &(pkbTables->getAssignAstTable().map.at(22));
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

    root = &(pkbTables->getAssignAstTable().map.at(24));
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

    root = &(pkbTables->getAssignAstTable().map.at(25));
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

  TEST_METHOD(TestConditionVarsTable) {
    std::unordered_set<PkbTables::CONSTANT> vars;
    vars.insert("x");
    vars.insert("y");
    Assert::IsTrue(pkbTables->getConditionVarsTable().map.at(7) == vars);

    vars.clear();
    vars.insert("x");
    Assert::IsTrue(pkbTables->getConditionVarsTable().map.at(8) == vars);

    vars.clear();
    vars.insert("sin");
    vars.insert("cos");
    vars.insert("tan");
    Assert::IsTrue(pkbTables->getConditionVarsTable().map.at(13) == vars);

    vars.clear();
    vars.insert("area");
    Assert::IsTrue(pkbTables->getConditionVarsTable().map.at(23) == vars);
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
