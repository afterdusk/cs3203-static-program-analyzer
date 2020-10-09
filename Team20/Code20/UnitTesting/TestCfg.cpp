#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestCfg) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::B);
  PkbTables *pkbTables = pkb.getTables();
  PkbTableTransformers pkbTableTransformers = PkbTableTransformers();

  /** @brief Derive nextTable. */
  /*
  TEST_METHOD(TestNextTable) {
    Pkb::STATEMENT_TYPE_TABLE statementTypeTable =
        pkbTables->getStatementTypeTable();

    Pkb::FOLLOW_TABLE followTable = pkbTables->getFollowTable();

    PkbTables::NEXT_TABLE nextTable =
        pkbTableTransformers.deriveNextTable(followTable, statementTypeTable);

    Assert::IsTrue(nextTable.map[setUpTests.l1] ==
                   PkbTables::NEXTS{setUpTests.l2});
    Assert::IsTrue(nextTable.map[setUpTests.l2] ==
                   PkbTables::NEXTS{setUpTests.l3, setUpTests.l7});
    Assert::IsTrue(nextTable.map[setUpTests.l3] ==
                   PkbTables::NEXTS{setUpTests.l4});
    Assert::IsTrue(nextTable.map[setUpTests.l4] ==
                   PkbTables::NEXTS{setUpTests.l5, setUpTests.l6});
    Assert::IsTrue(nextTable.map[setUpTests.l5] ==
                   PkbTables::NEXTS{setUpTests.l4});
    Assert::IsTrue(nextTable.map[setUpTests.l6] ==
                   PkbTables::NEXTS{setUpTests.l8});
    Assert::IsTrue(nextTable.map[setUpTests.l7] ==
                   PkbTables::NEXTS{setUpTests.l8});
    Assert::IsTrue(nextTable.map[setUpTests.l8] ==
                   PkbTables::NEXTS{setUpTests.l9});
    Assert::IsTrue(nextTable.map[setUpTests.l9] == PkbTables::NEXTS());
    Assert::IsTrue(nextTable.map[setUpTests.l10] ==
                   PkbTables::NEXTS{setUpTests.l11});
    Assert::IsTrue(nextTable.map[setUpTests.l11] ==
                   PkbTables::NEXTS{setUpTests.l12, setUpTests.l16});
    Assert::IsTrue(nextTable.map[setUpTests.l12] ==
                   PkbTables::NEXTS{setUpTests.l13, setUpTests.l14});
    Assert::IsTrue(nextTable.map[setUpTests.l13] ==
                   PkbTables::NEXTS{setUpTests.l21});
    Assert::IsTrue(nextTable.map[setUpTests.l14] ==
                   PkbTables::NEXTS{setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(nextTable.map[setUpTests.l15] ==
                   PkbTables::NEXTS{setUpTests.l14});
    Assert::IsTrue(nextTable.map[setUpTests.l16] ==
                   PkbTables::NEXTS{setUpTests.l17, setUpTests.l20});
    Assert::IsTrue(nextTable.map[setUpTests.l17] ==
                   PkbTables::NEXTS{setUpTests.l18, setUpTests.l21});
    Assert::IsTrue(nextTable.map[setUpTests.l18] ==
                   PkbTables::NEXTS{setUpTests.l17, setUpTests.l19});
    Assert::IsTrue(nextTable.map[setUpTests.l19] ==
                   PkbTables::NEXTS{setUpTests.l18});
    Assert::IsTrue(nextTable.map[setUpTests.l20] ==
                   PkbTables::NEXTS{setUpTests.l21});
    Assert::IsTrue(nextTable.map[setUpTests.l21] ==
                   PkbTables::NEXTS{setUpTests.l10});
    Assert::IsTrue(nextTable.map[setUpTests.l22] ==
                   PkbTables::NEXTS{setUpTests.l23, setUpTests.l24});
    Assert::IsTrue(nextTable.map[setUpTests.l23] == PkbTables::NEXTS());
    Assert::IsTrue(nextTable.map[setUpTests.l24] == PkbTables::NEXTS());

    KeysTable<Pkb::LINE_NO, Pkb::NEXTS> closeWarshallNextTable =
        PkbTableTransformers::closeWarshall(nextTable);

    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l1] ==
                   PkbTables::NEXTS{setUpTests.l2, setUpTests.l3, setUpTests.l7,
                                    setUpTests.l8, setUpTests.l9, setUpTests.l4,
                                    setUpTests.l5, setUpTests.l6});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l2] ==
                   PkbTables::NEXTS{setUpTests.l3, setUpTests.l7, setUpTests.l8,
                                    setUpTests.l9, setUpTests.l4, setUpTests.l5,
                                    setUpTests.l6});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l3] ==
                   PkbTables::NEXTS{setUpTests.l4, setUpTests.l5, setUpTests.l6,
                                    setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l4] ==
                   PkbTables::NEXTS{setUpTests.l5, setUpTests.l6, setUpTests.l8,
                                    setUpTests.l9, setUpTests.l4});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l5] ==
                   PkbTables::NEXTS{setUpTests.l5, setUpTests.l6, setUpTests.l8,
                                    setUpTests.l9, setUpTests.l4});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l6] ==
                   PkbTables::NEXTS{setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l7] ==
                   PkbTables::NEXTS{setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l8] ==
                   PkbTables::NEXTS{setUpTests.l9});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l9] ==
                   PkbTables::NEXTS());
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l10] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l11] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l12] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l13] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l14] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l15] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l16] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l17] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l18] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l19] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l20] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(
        closeWarshallNextTable.map[setUpTests.l21] ==
        PkbTables::NEXTS{setUpTests.l11, setUpTests.l12, setUpTests.l16,
                         setUpTests.l17, setUpTests.l20, setUpTests.l10,
                         setUpTests.l18, setUpTests.l19, setUpTests.l13,
                         setUpTests.l14, setUpTests.l15, setUpTests.l21});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l22] ==
                   PkbTables::NEXTS{setUpTests.l23, setUpTests.l24});
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l23] ==
                   PkbTables::NEXTS());
    Assert::IsTrue(closeWarshallNextTable.map[setUpTests.l24] ==
                   PkbTables::NEXTS());

  } // namespace UnitTesting
  */

  /** @brief Derive CfgNode. */
  TEST_METHOD(TestCfgNode) {
    Pkb::STATEMENT_PROC_TABLE statementProcTable =
        pkbTables->getStatementProcTable();

    KeysTable<Pkb::PROC, Pkb::LINE_NOS> statementProcTablePseudoinverted =
        PkbTableTransformers::pseudoinvert<Pkb::LINE_NO, Pkb::PROC>(
            statementProcTable);

    Assert::IsTrue(statementProcTablePseudoinverted.map[setUpTests.p0] ==
                   Pkb::LINE_NOS{setUpTests.l1, setUpTests.l2, setUpTests.l3,
                                 setUpTests.l4, setUpTests.l5, setUpTests.l6,
                                 setUpTests.l7, setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(statementProcTablePseudoinverted.map[setUpTests.p1] ==
                   Pkb::LINE_NOS{setUpTests.l10, setUpTests.l11, setUpTests.l12,
                                 setUpTests.l13, setUpTests.l14, setUpTests.l15,
                                 setUpTests.l16, setUpTests.l17, setUpTests.l18,
                                 setUpTests.l19, setUpTests.l20,
                                 setUpTests.l21});
    Assert::IsTrue(
        statementProcTablePseudoinverted.map[setUpTests.p2] ==
        Pkb::LINE_NOS{setUpTests.l22, setUpTests.l23, setUpTests.l24});

    Pkb::FOLLOW_TABLE followTable = pkbTables->getFollowTable();

    Assert::IsTrue(followTable.map[setUpTests.l1] == setUpTests.l2);
    Assert::IsTrue(followTable.map[setUpTests.l2] == setUpTests.l8);
    Assert::IsTrue(followTable.map[setUpTests.l3] == setUpTests.l4);
    Assert::IsTrue(followTable.map[setUpTests.l4] == setUpTests.l6);
    Assert::IsTrue(followTable.map[setUpTests.l5] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l6] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l7] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l8] == setUpTests.l9);
    Assert::IsTrue(followTable.map[setUpTests.l9] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l10] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l11] == setUpTests.l21);
    Assert::IsTrue(followTable.map[setUpTests.l12] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l13] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l14] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l15] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l16] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l17] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l18] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l19] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l20] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l21] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l22] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l23] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[setUpTests.l24] == Pkb::FOLLOW());

    KeysTable<Pkb::LINE_NO, Pkb::FOLLOWS> followTableClosed =
        PkbTableTransformers::close<Pkb::FOLLOW>(followTable);

    Assert::IsTrue(followTableClosed.map[setUpTests.l1] ==
                   Pkb::FOLLOWS{setUpTests.l2, setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(followTableClosed.map[setUpTests.l2] ==
                   Pkb::FOLLOWS{setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(followTableClosed.map[setUpTests.l3] ==
                   Pkb::FOLLOWS{setUpTests.l4, setUpTests.l6});
    Assert::IsTrue(followTableClosed.map[setUpTests.l4] ==
                   Pkb::FOLLOWS{setUpTests.l6});
    Assert::IsTrue(followTableClosed.map[setUpTests.l5] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l6] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l7] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l8] ==
                   Pkb::FOLLOWS{setUpTests.l9});
    Assert::IsTrue(followTableClosed.map[setUpTests.l9] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l10] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l11] ==
                   Pkb::FOLLOWS{setUpTests.l21});
    Assert::IsTrue(followTableClosed.map[setUpTests.l12] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l13] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l14] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l15] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l16] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l17] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l18] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l19] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l20] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l21] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l22] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l23] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[setUpTests.l24] == Pkb::FOLLOWS());

    Pkb::PARENT_TABLE parentTable = pkbTables->getParentTable();

    Assert::IsTrue(parentTable.map[setUpTests.l1] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[setUpTests.l2] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[setUpTests.l3] == setUpTests.l2);
    Assert::IsTrue(parentTable.map[setUpTests.l4] == setUpTests.l2);
    Assert::IsTrue(parentTable.map[setUpTests.l5] == setUpTests.l4);
    Assert::IsTrue(parentTable.map[setUpTests.l6] == setUpTests.l2);
    Assert::IsTrue(parentTable.map[setUpTests.l7] == setUpTests.l2);
    Assert::IsTrue(parentTable.map[setUpTests.l8] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[setUpTests.l9] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[setUpTests.l10] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[setUpTests.l11] == setUpTests.l10);
    Assert::IsTrue(parentTable.map[setUpTests.l12] == setUpTests.l11);
    Assert::IsTrue(parentTable.map[setUpTests.l13] == setUpTests.l12);
    Assert::IsTrue(parentTable.map[setUpTests.l14] == setUpTests.l12);
    Assert::IsTrue(parentTable.map[setUpTests.l15] == setUpTests.l14);
    Assert::IsTrue(parentTable.map[setUpTests.l16] == setUpTests.l11);
    Assert::IsTrue(parentTable.map[setUpTests.l17] == setUpTests.l16);
    Assert::IsTrue(parentTable.map[setUpTests.l18] == setUpTests.l17);
    Assert::IsTrue(parentTable.map[setUpTests.l19] == setUpTests.l18);
    Assert::IsTrue(parentTable.map[setUpTests.l20] == setUpTests.l16);
    Assert::IsTrue(parentTable.map[setUpTests.l21] == setUpTests.l10);
    Assert::IsTrue(parentTable.map[setUpTests.l22] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[setUpTests.l23] == setUpTests.l22);
    Assert::IsTrue(parentTable.map[setUpTests.l24] == setUpTests.l22);

    KeysTable<Pkb::PARENT, Pkb::LINE_NOS> parentTablePseudoinverted =
        PkbTableTransformers::pseudoinvert<Pkb::LINE_NO, Pkb::PARENT>(
            parentTable);

    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l1] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l2] ==
                   Pkb::CHILDREN{setUpTests.l3, setUpTests.l4, setUpTests.l6,
                                 setUpTests.l7});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l3] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l4] ==
                   Pkb::CHILDREN{setUpTests.l5});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l5] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l6] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l7] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l8] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l9] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l10] ==
                   Pkb::CHILDREN{setUpTests.l11, setUpTests.l21});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l11] ==
                   Pkb::CHILDREN{setUpTests.l12, setUpTests.l16});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l12] ==
                   Pkb::CHILDREN{setUpTests.l13, setUpTests.l14});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l13] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l14] ==
                   Pkb::CHILDREN{setUpTests.l15});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l15] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l16] ==
                   Pkb::CHILDREN{setUpTests.l17, setUpTests.l20});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l17] ==
                   Pkb::CHILDREN{setUpTests.l18});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l18] ==
                   Pkb::CHILDREN{setUpTests.l19});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l19] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l20] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l21] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l22] ==
                   Pkb::CHILDREN{setUpTests.l23, setUpTests.l24});
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l23] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[setUpTests.l24] ==
                   Pkb::CHILDREN());

    Pkb::STATEMENT_TYPE_TABLE statementTypeTable =
        pkbTables->getStatementTypeTable();

    /* CfgNode assertions.
    Variable naming scheme:
    <procedure variable> "Cfg" <entryPoint> <type> <exitPoint>
    */
    std::shared_ptr<CfgNode> p0Cfg;
    CfgNode::deriveCfg(
        p0Cfg, statementProcTablePseudoinverted.map[setUpTests.p0],
        {followTableClosed, parentTablePseudoinverted, statementTypeTable});
    Assert::IsTrue(p0Cfg->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p0Cfg->entryPoint == setUpTests.l1);
    Assert::IsTrue(p0Cfg->exitPoint == setUpTests.l1);
    Assert::IsNull(p0Cfg->nestedBlock.get());
    Assert::IsNull(p0Cfg->basicBlock.get());
    Assert::IsNull(p0Cfg->thenBlock.get());
    Assert::IsNull(p0Cfg->elseBlock.get());
    Assert::IsNull(p0Cfg->whileBlock.get());
    Assert::IsNull(p0Cfg->whileStatement.get());
    Assert::IsNotNull(p0Cfg->ifStatement.get());

    std::shared_ptr<CfgNode> p0Cfg2IfStatement2 = p0Cfg->ifStatement;
    Assert::IsTrue(p0Cfg2IfStatement2->type == CfgNode::Type::IfStatement);
    Assert::IsTrue(p0Cfg2IfStatement2->entryPoint == setUpTests.l2);
    Assert::IsTrue(p0Cfg2IfStatement2->exitPoint == setUpTests.l2);
    Assert::IsNull(p0Cfg2IfStatement2->nestedBlock.get());
    Assert::IsNull(p0Cfg2IfStatement2->whileBlock.get());
    Assert::IsNull(p0Cfg2IfStatement2->ifStatement.get());
    Assert::IsNull(p0Cfg2IfStatement2->whileStatement.get());
    Assert::IsNotNull(p0Cfg2IfStatement2->basicBlock.get());
    Assert::IsNotNull(p0Cfg2IfStatement2->thenBlock.get());
    Assert::IsNotNull(p0Cfg2IfStatement2->elseBlock.get());

    std::shared_ptr<CfgNode> p0Cfg3ThenBlock6 = p0Cfg2IfStatement2->thenBlock;
    Assert::IsTrue(p0Cfg3ThenBlock6->type == CfgNode::Type::ThenBlock);
    Assert::IsTrue(p0Cfg3ThenBlock6->entryPoint == setUpTests.l3);
    Assert::IsTrue(p0Cfg3ThenBlock6->exitPoint == setUpTests.l6);
    Assert::IsNull(p0Cfg3ThenBlock6->basicBlock.get());
    Assert::IsNull(p0Cfg3ThenBlock6->thenBlock.get());
    Assert::IsNull(p0Cfg3ThenBlock6->elseBlock.get());
    Assert::IsNull(p0Cfg3ThenBlock6->whileBlock.get());
    Assert::IsNull(p0Cfg3ThenBlock6->ifStatement.get());
    Assert::IsNull(p0Cfg3ThenBlock6->whileStatement.get());
    Assert::IsNotNull(p0Cfg3ThenBlock6->nestedBlock.get());

    std::shared_ptr<CfgNode> p0Cfg3BasicBlock3 = p0Cfg3ThenBlock6->nestedBlock;
    Assert::IsTrue(p0Cfg3BasicBlock3->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p0Cfg3BasicBlock3->entryPoint == setUpTests.l3);
    Assert::IsTrue(p0Cfg3BasicBlock3->exitPoint == setUpTests.l3);
    Assert::IsNull(p0Cfg3BasicBlock3->nestedBlock.get());
    Assert::IsNull(p0Cfg3BasicBlock3->basicBlock.get());
    Assert::IsNull(p0Cfg3BasicBlock3->thenBlock.get());
    Assert::IsNull(p0Cfg3BasicBlock3->elseBlock.get());
    Assert::IsNull(p0Cfg3BasicBlock3->whileBlock.get());
    Assert::IsNull(p0Cfg3BasicBlock3->ifStatement.get());
    Assert::IsNotNull(p0Cfg3BasicBlock3->whileStatement.get());

    std::shared_ptr<CfgNode> p0Cfg4WhileStatement4 =
        p0Cfg3BasicBlock3->whileStatement;
    Assert::IsTrue(p0Cfg4WhileStatement4->type ==
                   CfgNode::Type::WhileStatement);
    Assert::IsTrue(p0Cfg4WhileStatement4->entryPoint == setUpTests.l4);
    Assert::IsTrue(p0Cfg4WhileStatement4->exitPoint == setUpTests.l4);
    Assert::IsNull(p0Cfg4WhileStatement4->thenBlock.get());
    Assert::IsNull(p0Cfg4WhileStatement4->elseBlock.get());
    Assert::IsNull(p0Cfg4WhileStatement4->ifStatement.get());
    Assert::IsNull(p0Cfg4WhileStatement4->whileStatement.get());
    Assert::IsNull(p0Cfg4WhileStatement4->nestedBlock.get());
    Assert::IsNotNull(p0Cfg4WhileStatement4->basicBlock.get());
    Assert::IsNotNull(p0Cfg4WhileStatement4->whileBlock.get());

    std::shared_ptr<CfgNode> p0Cfg5WhileBlock5 =
        p0Cfg4WhileStatement4->whileBlock;
    Assert::IsTrue(p0Cfg5WhileBlock5->type == CfgNode::Type::WhileBlock);
    Assert::IsTrue(p0Cfg5WhileBlock5->entryPoint == setUpTests.l5);
    Assert::IsTrue(p0Cfg5WhileBlock5->exitPoint == setUpTests.l5);
    Assert::IsNull(p0Cfg5WhileBlock5->basicBlock.get());
    Assert::IsNull(p0Cfg5WhileBlock5->thenBlock.get());
    Assert::IsNull(p0Cfg5WhileBlock5->elseBlock.get());
    Assert::IsNull(p0Cfg5WhileBlock5->whileBlock.get());
    Assert::IsNull(p0Cfg5WhileBlock5->ifStatement.get());
    Assert::IsNull(p0Cfg5WhileBlock5->whileStatement.get());
    Assert::IsNotNull(p0Cfg5WhileBlock5->nestedBlock.get());

    std::shared_ptr<CfgNode> p0Cfg5BasicBlock5 = p0Cfg5WhileBlock5->nestedBlock;
    Assert::IsTrue(p0Cfg5BasicBlock5->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p0Cfg5BasicBlock5->entryPoint == setUpTests.l5);
    Assert::IsTrue(p0Cfg5BasicBlock5->exitPoint == setUpTests.l5);
    Assert::IsNull(p0Cfg5BasicBlock5->nestedBlock.get());
    Assert::IsNull(p0Cfg5BasicBlock5->basicBlock.get());
    Assert::IsNull(p0Cfg5BasicBlock5->thenBlock.get());
    Assert::IsNull(p0Cfg5BasicBlock5->elseBlock.get());
    Assert::IsNull(p0Cfg5BasicBlock5->whileBlock.get());
    Assert::IsNull(p0Cfg5BasicBlock5->ifStatement.get());
    Assert::IsNull(p0Cfg5BasicBlock5->whileStatement.get());

    std::shared_ptr<CfgNode> p0Cfg6BasicBlock6 =
        p0Cfg4WhileStatement4->basicBlock;
    Assert::IsTrue(p0Cfg6BasicBlock6->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p0Cfg6BasicBlock6->entryPoint == setUpTests.l6);
    Assert::IsTrue(p0Cfg6BasicBlock6->exitPoint == setUpTests.l6);
    Assert::IsNull(p0Cfg6BasicBlock6->nestedBlock.get());
    Assert::IsNull(p0Cfg6BasicBlock6->basicBlock.get());
    Assert::IsNull(p0Cfg6BasicBlock6->thenBlock.get());
    Assert::IsNull(p0Cfg6BasicBlock6->elseBlock.get());
    Assert::IsNull(p0Cfg6BasicBlock6->whileBlock.get());
    Assert::IsNull(p0Cfg6BasicBlock6->ifStatement.get());
    Assert::IsNull(p0Cfg6BasicBlock6->whileStatement.get());

    std::shared_ptr<CfgNode> p0Cfg7ElseBlock7 = p0Cfg2IfStatement2->elseBlock;
    Assert::IsTrue(p0Cfg7ElseBlock7->type == CfgNode::Type::ElseBlock);
    Assert::IsTrue(p0Cfg7ElseBlock7->entryPoint == setUpTests.l7);
    Assert::IsTrue(p0Cfg7ElseBlock7->exitPoint == setUpTests.l7);
    Assert::IsNull(p0Cfg7ElseBlock7->basicBlock.get());
    Assert::IsNull(p0Cfg7ElseBlock7->thenBlock.get());
    Assert::IsNull(p0Cfg7ElseBlock7->elseBlock.get());
    Assert::IsNull(p0Cfg7ElseBlock7->whileBlock.get());
    Assert::IsNull(p0Cfg7ElseBlock7->ifStatement.get());
    Assert::IsNull(p0Cfg7ElseBlock7->whileStatement.get());
    Assert::IsNotNull(p0Cfg7ElseBlock7->nestedBlock.get());

    std::shared_ptr<CfgNode> p0Cfg7BasicBlock7 = p0Cfg7ElseBlock7->nestedBlock;
    Assert::IsTrue(p0Cfg7BasicBlock7->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p0Cfg7BasicBlock7->entryPoint == setUpTests.l7);
    Assert::IsTrue(p0Cfg7BasicBlock7->exitPoint == setUpTests.l7);
    Assert::IsNull(p0Cfg7BasicBlock7->nestedBlock.get());
    Assert::IsNull(p0Cfg7BasicBlock7->basicBlock.get());
    Assert::IsNull(p0Cfg7BasicBlock7->thenBlock.get());
    Assert::IsNull(p0Cfg7BasicBlock7->elseBlock.get());
    Assert::IsNull(p0Cfg7BasicBlock7->whileBlock.get());
    Assert::IsNull(p0Cfg7BasicBlock7->ifStatement.get());
    Assert::IsNull(p0Cfg7BasicBlock7->whileStatement.get());

    std::shared_ptr<CfgNode> p0Cfg8BasicBlock9 = p0Cfg2IfStatement2->basicBlock;
    Assert::IsTrue(p0Cfg8BasicBlock9->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p0Cfg8BasicBlock9->entryPoint == setUpTests.l8);
    Assert::IsTrue(p0Cfg8BasicBlock9->exitPoint == setUpTests.l9);
    Assert::IsNull(p0Cfg8BasicBlock9->nestedBlock.get());
    Assert::IsNull(p0Cfg8BasicBlock9->basicBlock.get());
    Assert::IsNull(p0Cfg8BasicBlock9->thenBlock.get());
    Assert::IsNull(p0Cfg8BasicBlock9->elseBlock.get());
    Assert::IsNull(p0Cfg8BasicBlock9->whileBlock.get());
    Assert::IsNull(p0Cfg8BasicBlock9->ifStatement.get());
    Assert::IsNull(p0Cfg8BasicBlock9->whileStatement.get());

    std::shared_ptr<CfgNode> p1Cfg;
    CfgNode::deriveCfg(
        p1Cfg, statementProcTablePseudoinverted.map[setUpTests.p1],
        {followTableClosed, parentTablePseudoinverted, statementTypeTable});
    Assert::IsTrue(p1Cfg->type == CfgNode::Type::WhileStatement);
    Assert::IsTrue(p1Cfg->entryPoint == setUpTests.l10);
    Assert::IsTrue(p1Cfg->exitPoint == setUpTests.l10);
    Assert::IsNull(p1Cfg->basicBlock.get());
    Assert::IsNull(p1Cfg->thenBlock.get());
    Assert::IsNull(p1Cfg->elseBlock.get());
    Assert::IsNull(p1Cfg->ifStatement.get());
    Assert::IsNull(p1Cfg->whileStatement.get());
    Assert::IsNull(p1Cfg->nestedBlock.get());
    Assert::IsNotNull(p1Cfg->whileBlock.get());

    std::shared_ptr<CfgNode> p1Cfg11WhileBlock21 = p1Cfg->whileBlock;
    Assert::IsTrue(p1Cfg11WhileBlock21->type == CfgNode::Type::WhileBlock);
    Assert::IsTrue(p1Cfg11WhileBlock21->entryPoint == setUpTests.l11);
    Assert::IsTrue(p1Cfg11WhileBlock21->exitPoint == setUpTests.l21);
    Assert::IsNull(p1Cfg11WhileBlock21->basicBlock.get());
    Assert::IsNull(p1Cfg11WhileBlock21->thenBlock.get());
    Assert::IsNull(p1Cfg11WhileBlock21->elseBlock.get());
    Assert::IsNull(p1Cfg11WhileBlock21->whileBlock.get());
    Assert::IsNull(p1Cfg11WhileBlock21->ifStatement.get());
    Assert::IsNull(p1Cfg11WhileBlock21->whileStatement.get());
    Assert::IsNotNull(p1Cfg11WhileBlock21->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg11IfStatement11 =
        p1Cfg11WhileBlock21->nestedBlock;
    Assert::IsTrue(p1Cfg11IfStatement11->type == CfgNode::Type::IfStatement);
    Assert::IsTrue(p1Cfg11IfStatement11->entryPoint == setUpTests.l11);
    Assert::IsTrue(p1Cfg11IfStatement11->exitPoint == setUpTests.l11);
    Assert::IsNull(p1Cfg11IfStatement11->nestedBlock.get());
    Assert::IsNull(p1Cfg11IfStatement11->whileBlock.get());
    Assert::IsNull(p1Cfg11IfStatement11->ifStatement.get());
    Assert::IsNull(p1Cfg11IfStatement11->whileStatement.get());
    Assert::IsNotNull(p1Cfg11IfStatement11->basicBlock.get());
    Assert::IsNotNull(p1Cfg11IfStatement11->thenBlock.get());
    Assert::IsNotNull(p1Cfg11IfStatement11->elseBlock.get());

    std::shared_ptr<CfgNode> p1Cfg12ThenBlock15 =
        p1Cfg11IfStatement11->thenBlock;
    Assert::IsTrue(p1Cfg12ThenBlock15->type == CfgNode::Type::ThenBlock);
    Assert::IsTrue(p1Cfg12ThenBlock15->entryPoint == setUpTests.l12);
    Assert::IsTrue(p1Cfg12ThenBlock15->exitPoint == setUpTests.l15);
    Assert::IsNull(p1Cfg12ThenBlock15->basicBlock.get());
    Assert::IsNull(p1Cfg12ThenBlock15->thenBlock.get());
    Assert::IsNull(p1Cfg12ThenBlock15->elseBlock.get());
    Assert::IsNull(p1Cfg12ThenBlock15->whileBlock.get());
    Assert::IsNull(p1Cfg12ThenBlock15->ifStatement.get());
    Assert::IsNull(p1Cfg12ThenBlock15->whileStatement.get());
    Assert::IsNotNull(p1Cfg12ThenBlock15->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg12IfStatement12 =
        p1Cfg12ThenBlock15->nestedBlock;
    Assert::IsTrue(p1Cfg12IfStatement12->type == CfgNode::Type::IfStatement);
    Assert::IsTrue(p1Cfg12IfStatement12->entryPoint == setUpTests.l12);
    Assert::IsTrue(p1Cfg12IfStatement12->exitPoint == setUpTests.l12);
    Assert::IsNull(p1Cfg12IfStatement12->nestedBlock.get());
    Assert::IsNull(p1Cfg12IfStatement12->basicBlock.get());
    Assert::IsNull(p1Cfg12IfStatement12->whileBlock.get());
    Assert::IsNull(p1Cfg12IfStatement12->ifStatement.get());
    Assert::IsNull(p1Cfg12IfStatement12->whileStatement.get());
    Assert::IsNotNull(p1Cfg12IfStatement12->thenBlock.get());
    Assert::IsNotNull(p1Cfg12IfStatement12->elseBlock.get());

    std::shared_ptr<CfgNode> p1Cfg13ThenBlock13 =
        p1Cfg12IfStatement12->thenBlock;
    Assert::IsTrue(p1Cfg13ThenBlock13->type == CfgNode::Type::ThenBlock);
    Assert::IsTrue(p1Cfg13ThenBlock13->entryPoint == setUpTests.l13);
    Assert::IsTrue(p1Cfg13ThenBlock13->exitPoint == setUpTests.l13);
    Assert::IsNull(p1Cfg13ThenBlock13->basicBlock.get());
    Assert::IsNull(p1Cfg13ThenBlock13->thenBlock.get());
    Assert::IsNull(p1Cfg13ThenBlock13->elseBlock.get());
    Assert::IsNull(p1Cfg13ThenBlock13->whileBlock.get());
    Assert::IsNull(p1Cfg13ThenBlock13->ifStatement.get());
    Assert::IsNull(p1Cfg13ThenBlock13->whileStatement.get());
    Assert::IsNotNull(p1Cfg13ThenBlock13->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg13BasicBlock13 =
        p1Cfg13ThenBlock13->nestedBlock;
    Assert::IsTrue(p1Cfg13BasicBlock13->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p1Cfg13BasicBlock13->entryPoint == setUpTests.l13);
    Assert::IsTrue(p1Cfg13BasicBlock13->exitPoint == setUpTests.l13);
    Assert::IsNull(p1Cfg13BasicBlock13->nestedBlock.get());
    Assert::IsNull(p1Cfg13BasicBlock13->basicBlock.get());
    Assert::IsNull(p1Cfg13BasicBlock13->thenBlock.get());
    Assert::IsNull(p1Cfg13BasicBlock13->elseBlock.get());
    Assert::IsNull(p1Cfg13BasicBlock13->whileBlock.get());
    Assert::IsNull(p1Cfg13BasicBlock13->ifStatement.get());
    Assert::IsNull(p1Cfg13BasicBlock13->whileStatement.get());

    std::shared_ptr<CfgNode> p1Cfg14ElseBlock15 =
        p1Cfg12IfStatement12->elseBlock;
    Assert::IsTrue(p1Cfg14ElseBlock15->type == CfgNode::Type::ElseBlock);
    Assert::IsTrue(p1Cfg14ElseBlock15->entryPoint == setUpTests.l14);
    Assert::IsTrue(p1Cfg14ElseBlock15->exitPoint == setUpTests.l15);
    Assert::IsNull(p1Cfg14ElseBlock15->basicBlock.get());
    Assert::IsNull(p1Cfg14ElseBlock15->thenBlock.get());
    Assert::IsNull(p1Cfg14ElseBlock15->elseBlock.get());
    Assert::IsNull(p1Cfg14ElseBlock15->whileBlock.get());
    Assert::IsNull(p1Cfg14ElseBlock15->ifStatement.get());
    Assert::IsNull(p1Cfg14ElseBlock15->whileStatement.get());
    Assert::IsNotNull(p1Cfg14ElseBlock15->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg14WhileStatement14 =
        p1Cfg14ElseBlock15->nestedBlock;
    Assert::IsTrue(p1Cfg14WhileStatement14->type ==
                   CfgNode::Type::WhileStatement);
    Assert::IsTrue(p1Cfg14WhileStatement14->entryPoint == setUpTests.l14);
    Assert::IsTrue(p1Cfg14WhileStatement14->exitPoint == setUpTests.l14);
    Assert::IsNull(p1Cfg14WhileStatement14->basicBlock.get());
    Assert::IsNull(p1Cfg14WhileStatement14->thenBlock.get());
    Assert::IsNull(p1Cfg14WhileStatement14->elseBlock.get());
    Assert::IsNull(p1Cfg14WhileStatement14->ifStatement.get());
    Assert::IsNull(p1Cfg14WhileStatement14->whileStatement.get());
    Assert::IsNull(p1Cfg14WhileStatement14->nestedBlock.get());
    Assert::IsNotNull(p1Cfg14WhileStatement14->whileBlock.get());

    std::shared_ptr<CfgNode> p1Cfg15WhileBlock15 =
        p1Cfg14WhileStatement14->whileBlock;
    Assert::IsTrue(p1Cfg15WhileBlock15->type == CfgNode::Type::WhileBlock);
    Assert::IsTrue(p1Cfg15WhileBlock15->entryPoint == setUpTests.l15);
    Assert::IsTrue(p1Cfg15WhileBlock15->exitPoint == setUpTests.l15);
    Assert::IsNull(p1Cfg15WhileBlock15->basicBlock.get());
    Assert::IsNull(p1Cfg15WhileBlock15->thenBlock.get());
    Assert::IsNull(p1Cfg15WhileBlock15->elseBlock.get());
    Assert::IsNull(p1Cfg15WhileBlock15->whileBlock.get());
    Assert::IsNull(p1Cfg15WhileBlock15->ifStatement.get());
    Assert::IsNull(p1Cfg15WhileBlock15->whileStatement.get());
    Assert::IsNotNull(p1Cfg15WhileBlock15->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg15BasicBlock15 =
        p1Cfg15WhileBlock15->nestedBlock;
    Assert::IsTrue(p1Cfg15BasicBlock15->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p1Cfg15BasicBlock15->entryPoint == setUpTests.l15);
    Assert::IsTrue(p1Cfg15BasicBlock15->exitPoint == setUpTests.l15);
    Assert::IsNull(p1Cfg15BasicBlock15->nestedBlock.get());
    Assert::IsNull(p1Cfg15BasicBlock15->basicBlock.get());
    Assert::IsNull(p1Cfg15BasicBlock15->thenBlock.get());
    Assert::IsNull(p1Cfg15BasicBlock15->elseBlock.get());
    Assert::IsNull(p1Cfg15BasicBlock15->whileBlock.get());
    Assert::IsNull(p1Cfg15BasicBlock15->ifStatement.get());
    Assert::IsNull(p1Cfg15BasicBlock15->whileStatement.get());

    std::shared_ptr<CfgNode> p1Cfg16ElseBlock20 =
        p1Cfg11IfStatement11->elseBlock;
    Assert::IsTrue(p1Cfg16ElseBlock20->type == CfgNode::Type::ElseBlock);
    Assert::IsTrue(p1Cfg16ElseBlock20->entryPoint == setUpTests.l16);
    Assert::IsTrue(p1Cfg16ElseBlock20->exitPoint == setUpTests.l20);
    Assert::IsNull(p1Cfg16ElseBlock20->basicBlock.get());
    Assert::IsNull(p1Cfg16ElseBlock20->thenBlock.get());
    Assert::IsNull(p1Cfg16ElseBlock20->elseBlock.get());
    Assert::IsNull(p1Cfg16ElseBlock20->whileBlock.get());
    Assert::IsNull(p1Cfg16ElseBlock20->ifStatement.get());
    Assert::IsNull(p1Cfg16ElseBlock20->whileStatement.get());
    Assert::IsNotNull(p1Cfg16ElseBlock20->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg16IfStatement16 =
        p1Cfg16ElseBlock20->nestedBlock;
    Assert::IsTrue(p1Cfg16IfStatement16->type == CfgNode::Type::IfStatement);
    Assert::IsTrue(p1Cfg16IfStatement16->entryPoint == setUpTests.l16);
    Assert::IsTrue(p1Cfg16IfStatement16->exitPoint == setUpTests.l16);
    Assert::IsNull(p1Cfg16IfStatement16->nestedBlock.get());
    Assert::IsNull(p1Cfg16IfStatement16->basicBlock.get());
    Assert::IsNull(p1Cfg16IfStatement16->whileBlock.get());
    Assert::IsNull(p1Cfg16IfStatement16->ifStatement.get());
    Assert::IsNull(p1Cfg16IfStatement16->whileStatement.get());
    Assert::IsNotNull(p1Cfg16IfStatement16->thenBlock.get());
    Assert::IsNotNull(p1Cfg16IfStatement16->elseBlock.get());

    std::shared_ptr<CfgNode> p1Cfg17ThenBlock19 =
        p1Cfg16IfStatement16->thenBlock;
    Assert::IsTrue(p1Cfg17ThenBlock19->type == CfgNode::Type::ThenBlock);
    Assert::IsTrue(p1Cfg17ThenBlock19->entryPoint == setUpTests.l17);
    Assert::IsTrue(p1Cfg17ThenBlock19->exitPoint == setUpTests.l19);
    Assert::IsNull(p1Cfg17ThenBlock19->basicBlock.get());
    Assert::IsNull(p1Cfg17ThenBlock19->thenBlock.get());
    Assert::IsNull(p1Cfg17ThenBlock19->elseBlock.get());
    Assert::IsNull(p1Cfg17ThenBlock19->whileBlock.get());
    Assert::IsNull(p1Cfg17ThenBlock19->ifStatement.get());
    Assert::IsNull(p1Cfg17ThenBlock19->whileStatement.get());
    Assert::IsNotNull(p1Cfg17ThenBlock19->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg17WhileStatement17 =
        p1Cfg17ThenBlock19->nestedBlock;
    Assert::IsTrue(p1Cfg17WhileStatement17->type ==
                   CfgNode::Type::WhileStatement);
    Assert::IsTrue(p1Cfg17WhileStatement17->entryPoint == setUpTests.l17);
    Assert::IsTrue(p1Cfg17WhileStatement17->exitPoint == setUpTests.l17);
    Assert::IsNull(p1Cfg17WhileStatement17->basicBlock.get());
    Assert::IsNull(p1Cfg17WhileStatement17->thenBlock.get());
    Assert::IsNull(p1Cfg17WhileStatement17->elseBlock.get());
    Assert::IsNull(p1Cfg17WhileStatement17->ifStatement.get());
    Assert::IsNull(p1Cfg17WhileStatement17->whileStatement.get());
    Assert::IsNull(p1Cfg17WhileStatement17->nestedBlock.get());
    Assert::IsNotNull(p1Cfg17WhileStatement17->whileBlock.get());

    std::shared_ptr<CfgNode> p1Cfg18WhileBlock19 =
        p1Cfg17WhileStatement17->whileBlock;
    Assert::IsTrue(p1Cfg18WhileBlock19->type == CfgNode::Type::WhileBlock);
    Assert::IsTrue(p1Cfg18WhileBlock19->entryPoint == setUpTests.l18);
    Assert::IsTrue(p1Cfg18WhileBlock19->exitPoint == setUpTests.l19);
    Assert::IsNull(p1Cfg18WhileBlock19->basicBlock.get());
    Assert::IsNull(p1Cfg18WhileBlock19->thenBlock.get());
    Assert::IsNull(p1Cfg18WhileBlock19->elseBlock.get());
    Assert::IsNull(p1Cfg18WhileBlock19->whileBlock.get());
    Assert::IsNull(p1Cfg18WhileBlock19->ifStatement.get());
    Assert::IsNull(p1Cfg18WhileBlock19->whileStatement.get());
    Assert::IsNotNull(p1Cfg18WhileBlock19->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg18WhileStatement18 =
        p1Cfg18WhileBlock19->nestedBlock;
    Assert::IsTrue(p1Cfg18WhileStatement18->type ==
                   CfgNode::Type::WhileStatement);
    Assert::IsTrue(p1Cfg18WhileStatement18->entryPoint == setUpTests.l18);
    Assert::IsTrue(p1Cfg18WhileStatement18->exitPoint == setUpTests.l18);
    Assert::IsNull(p1Cfg18WhileStatement18->nestedBlock.get());
    Assert::IsNull(p1Cfg18WhileStatement18->basicBlock.get());
    Assert::IsNull(p1Cfg18WhileStatement18->thenBlock.get());
    Assert::IsNull(p1Cfg18WhileStatement18->elseBlock.get());
    Assert::IsNull(p1Cfg18WhileStatement18->ifStatement.get());
    Assert::IsNull(p1Cfg18WhileStatement18->whileStatement.get());
    Assert::IsNotNull(p1Cfg18WhileStatement18->whileBlock.get());

    std::shared_ptr<CfgNode> p1Cfg19WhileBlock19 =
        p1Cfg18WhileStatement18->whileBlock;
    Assert::IsTrue(p1Cfg19WhileBlock19->type == CfgNode::Type::WhileBlock);
    Assert::IsTrue(p1Cfg19WhileBlock19->entryPoint == setUpTests.l19);
    Assert::IsTrue(p1Cfg19WhileBlock19->exitPoint == setUpTests.l19);
    Assert::IsNull(p1Cfg19WhileBlock19->basicBlock.get());
    Assert::IsNull(p1Cfg19WhileBlock19->thenBlock.get());
    Assert::IsNull(p1Cfg19WhileBlock19->elseBlock.get());
    Assert::IsNull(p1Cfg19WhileBlock19->whileBlock.get());
    Assert::IsNull(p1Cfg19WhileBlock19->ifStatement.get());
    Assert::IsNull(p1Cfg19WhileBlock19->whileStatement.get());
    Assert::IsNotNull(p1Cfg19WhileBlock19->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg19BasicBlock19 =
        p1Cfg19WhileBlock19->nestedBlock;
    Assert::IsTrue(p1Cfg19BasicBlock19->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p1Cfg19BasicBlock19->entryPoint == setUpTests.l19);
    Assert::IsTrue(p1Cfg19BasicBlock19->exitPoint == setUpTests.l19);
    Assert::IsNull(p1Cfg19BasicBlock19->nestedBlock.get());
    Assert::IsNull(p1Cfg19BasicBlock19->basicBlock.get());
    Assert::IsNull(p1Cfg19BasicBlock19->thenBlock.get());
    Assert::IsNull(p1Cfg19BasicBlock19->elseBlock.get());
    Assert::IsNull(p1Cfg19BasicBlock19->whileBlock.get());
    Assert::IsNull(p1Cfg19BasicBlock19->ifStatement.get());
    Assert::IsNull(p1Cfg19BasicBlock19->whileStatement.get());

    std::shared_ptr<CfgNode> p1Cfg20ElseBlock20 =
        p1Cfg16IfStatement16->elseBlock;
    Assert::IsTrue(p1Cfg20ElseBlock20->type == CfgNode::Type::ElseBlock);
    Assert::IsTrue(p1Cfg20ElseBlock20->entryPoint == setUpTests.l20);
    Assert::IsTrue(p1Cfg20ElseBlock20->exitPoint == setUpTests.l20);
    Assert::IsNull(p1Cfg20ElseBlock20->basicBlock.get());
    Assert::IsNull(p1Cfg20ElseBlock20->thenBlock.get());
    Assert::IsNull(p1Cfg20ElseBlock20->elseBlock.get());
    Assert::IsNull(p1Cfg20ElseBlock20->whileBlock.get());
    Assert::IsNull(p1Cfg20ElseBlock20->ifStatement.get());
    Assert::IsNull(p1Cfg20ElseBlock20->whileStatement.get());
    Assert::IsNotNull(p1Cfg20ElseBlock20->nestedBlock.get());

    std::shared_ptr<CfgNode> p1Cfg20BasicBlock20 =
        p1Cfg20ElseBlock20->nestedBlock;
    Assert::IsTrue(p1Cfg20BasicBlock20->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p1Cfg20BasicBlock20->entryPoint == setUpTests.l20);
    Assert::IsTrue(p1Cfg20BasicBlock20->exitPoint == setUpTests.l20);
    Assert::IsNull(p1Cfg20BasicBlock20->nestedBlock.get());
    Assert::IsNull(p1Cfg20BasicBlock20->basicBlock.get());
    Assert::IsNull(p1Cfg20BasicBlock20->thenBlock.get());
    Assert::IsNull(p1Cfg20BasicBlock20->elseBlock.get());
    Assert::IsNull(p1Cfg20BasicBlock20->whileBlock.get());
    Assert::IsNull(p1Cfg20BasicBlock20->ifStatement.get());
    Assert::IsNull(p1Cfg20BasicBlock20->whileStatement.get());

    std::shared_ptr<CfgNode> p1Cfg21BasicBlock21 =
        p1Cfg11IfStatement11->basicBlock;
    Assert::IsTrue(p1Cfg21BasicBlock21->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p1Cfg21BasicBlock21->entryPoint == setUpTests.l21);
    Assert::IsTrue(p1Cfg21BasicBlock21->exitPoint == setUpTests.l21);
    Assert::IsNull(p1Cfg21BasicBlock21->nestedBlock.get());
    Assert::IsNull(p1Cfg21BasicBlock21->basicBlock.get());
    Assert::IsNull(p1Cfg21BasicBlock21->thenBlock.get());
    Assert::IsNull(p1Cfg21BasicBlock21->elseBlock.get());
    Assert::IsNull(p1Cfg21BasicBlock21->whileBlock.get());
    Assert::IsNull(p1Cfg21BasicBlock21->ifStatement.get());
    Assert::IsNull(p1Cfg21BasicBlock21->whileStatement.get());

    std::shared_ptr<CfgNode> p2Cfg;
    CfgNode::deriveCfg(
        p2Cfg, statementProcTablePseudoinverted.map[setUpTests.p2],
        {followTableClosed, parentTablePseudoinverted, statementTypeTable});
    Assert::IsTrue(p2Cfg->type == CfgNode::Type::IfStatement);
    Assert::IsTrue(p2Cfg->entryPoint == setUpTests.l22);
    Assert::IsTrue(p2Cfg->exitPoint == setUpTests.l22);
    Assert::IsNull(p2Cfg->basicBlock.get());
    Assert::IsNull(p2Cfg->whileBlock.get());
    Assert::IsNull(p2Cfg->ifStatement.get());
    Assert::IsNull(p2Cfg->whileStatement.get());
    Assert::IsNull(p2Cfg->nestedBlock.get());
    Assert::IsNotNull(p2Cfg->thenBlock.get());
    Assert::IsNotNull(p2Cfg->elseBlock.get());

    std::shared_ptr<CfgNode> p2Cfg23ThenBlock23 = p2Cfg->thenBlock;
    Assert::IsTrue(p2Cfg23ThenBlock23->type == CfgNode::Type::ThenBlock);
    Assert::IsTrue(p2Cfg23ThenBlock23->entryPoint == setUpTests.l23);
    Assert::IsTrue(p2Cfg23ThenBlock23->exitPoint == setUpTests.l23);
    Assert::IsNull(p2Cfg23ThenBlock23->basicBlock.get());
    Assert::IsNull(p2Cfg23ThenBlock23->thenBlock.get());
    Assert::IsNull(p2Cfg23ThenBlock23->elseBlock.get());
    Assert::IsNull(p2Cfg23ThenBlock23->whileBlock.get());
    Assert::IsNull(p2Cfg23ThenBlock23->ifStatement.get());
    Assert::IsNull(p2Cfg23ThenBlock23->whileStatement.get());
    Assert::IsNotNull(p2Cfg23ThenBlock23->nestedBlock.get());

    std::shared_ptr<CfgNode> p2Cfg23BasicBlock23 =
        p2Cfg23ThenBlock23->nestedBlock;
    Assert::IsTrue(p2Cfg23BasicBlock23->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p2Cfg23BasicBlock23->entryPoint == setUpTests.l23);
    Assert::IsTrue(p2Cfg23BasicBlock23->exitPoint == setUpTests.l23);
    Assert::IsNull(p2Cfg23BasicBlock23->nestedBlock.get());
    Assert::IsNull(p2Cfg23BasicBlock23->basicBlock.get());
    Assert::IsNull(p2Cfg23BasicBlock23->thenBlock.get());
    Assert::IsNull(p2Cfg23BasicBlock23->elseBlock.get());
    Assert::IsNull(p2Cfg23BasicBlock23->whileBlock.get());
    Assert::IsNull(p2Cfg23BasicBlock23->ifStatement.get());
    Assert::IsNull(p2Cfg23BasicBlock23->whileStatement.get());

    std::shared_ptr<CfgNode> p2Cfg24ElseBlock24 = p2Cfg->elseBlock;
    Assert::IsTrue(p2Cfg24ElseBlock24->type == CfgNode::Type::ElseBlock);
    Assert::IsTrue(p2Cfg24ElseBlock24->entryPoint == setUpTests.l24);
    Assert::IsTrue(p2Cfg24ElseBlock24->exitPoint == setUpTests.l24);
    Assert::IsNull(p2Cfg24ElseBlock24->basicBlock.get());
    Assert::IsNull(p2Cfg24ElseBlock24->thenBlock.get());
    Assert::IsNull(p2Cfg24ElseBlock24->elseBlock.get());
    Assert::IsNull(p2Cfg24ElseBlock24->whileBlock.get());
    Assert::IsNull(p2Cfg24ElseBlock24->ifStatement.get());
    Assert::IsNull(p2Cfg24ElseBlock24->whileStatement.get());
    Assert::IsNotNull(p2Cfg24ElseBlock24->nestedBlock.get());

    std::shared_ptr<CfgNode> p2Cfg24BasicBlock24 =
        p2Cfg24ElseBlock24->nestedBlock;
    Assert::IsTrue(p2Cfg24BasicBlock24->type == CfgNode::Type::BasicBlock);
    Assert::IsTrue(p2Cfg24BasicBlock24->entryPoint == setUpTests.l24);
    Assert::IsTrue(p2Cfg24BasicBlock24->exitPoint == setUpTests.l24);
    Assert::IsNull(p2Cfg24BasicBlock24->nestedBlock.get());
    Assert::IsNull(p2Cfg24BasicBlock24->basicBlock.get());
    Assert::IsNull(p2Cfg24BasicBlock24->thenBlock.get());
    Assert::IsNull(p2Cfg24BasicBlock24->elseBlock.get());
    Assert::IsNull(p2Cfg24BasicBlock24->whileBlock.get());
    Assert::IsNull(p2Cfg24BasicBlock24->ifStatement.get());
    Assert::IsNull(p2Cfg24BasicBlock24->whileStatement.get());

  } // namespace UnitTesting
};
} // namespace UnitTesting
