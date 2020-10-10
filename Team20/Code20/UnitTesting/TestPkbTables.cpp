#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPkbTables) {

public:
  Pkb pkb;
  PkbTables *pkbTables = pkb.getTables();

  /** @brief Populate Pkb::varTable.
  Add variables "a", "b", ..., "d", so that "a" has
  index 0, "b" has index 1, ...
  */
  TEST_METHOD(TestVarTable) {
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "b";
    Pkb::VAR v2 = "c";
    Pkb::VAR v3 = "d";
    Pkb::VAR v4 = "bad";

    pkbTables->addVar(v0);
    pkbTables->addVar(v1);
    pkbTables->addVar(v2);
    pkbTables->addVar(v3);

    Pkb::VAR_TABLE varTable = pkbTables->getVarTable();

    Assert::IsTrue(*varTable.find(v0) == v0);
    Assert::IsTrue(*varTable.find(v1) == v1);
    Assert::IsTrue(*varTable.find(v2) == v2);
    Assert::IsTrue(*varTable.find(v3) == v3);

    // An unmapped key is not found.
    Assert::IsTrue(varTable.find(v4) == varTable.end());

  } // namespace UnitTesting

  /** @brief Populate Pkb::procTable.
  Add procedures "a", "b", ..., "d", so that "a" has index
  0, "b" has index 1, ...
  */
  TEST_METHOD(TestProcTable) {
    Pkb::PROC p0 = "a";
    Pkb::PROC p1 = "b";
    Pkb::PROC p2 = "c";
    Pkb::PROC p3 = "d";
    Pkb::PROC p4 = "bad";

    pkbTables->addProc(p0);
    pkbTables->addProc(p1);
    pkbTables->addProc(p2);
    pkbTables->addProc(p3);

    Pkb::PROC_TABLE procTable = pkbTables->getProcTable();

    Assert::IsTrue(*procTable.find(p0) == p0);
    Assert::IsTrue(*procTable.find(p1) == p1);
    Assert::IsTrue(*procTable.find(p2) == p2);
    Assert::IsTrue(*procTable.find(p3) == p3);

    // An unmapped key is not found.
    Assert::IsTrue(procTable.find(p4) == procTable.end());

  } // namespace UnitTesting

  /** @brief Populate Pkb::usesTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     a = x + y;
  2     call aux;
      }
      procedure aux {
  3     print z;
  4     read z;
      }
  */
  TEST_METHOD(TestUsesTableAndUsesProcTable) {
    Pkb::PROC p0 = "main";
    Pkb::PROC p1 = "aux";
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "x";
    Pkb::VAR v2 = "y";
    Pkb::VAR v3 = "z";

    pkbTables->addProc(p0);
    pkbTables->addProc(p1);
    pkbTables->addVar(v0);
    pkbTables->addVar(v1);
    pkbTables->addVar(v2);
    pkbTables->addVar(v3);

    /* UsesTable ******/
    // UsesTable may be populated in any order.
    pkbTables->addUses(l1, Pkb::VARS{v1, v2});
    pkbTables->addUses(l2, p1);
    pkbTables->addUses(l3, Pkb::VARS{v3});

    /* UsesProcTable ******/
    // UsesProcTable must be populated in a certain order:
    // First, the procedures that do not contain calls to other procedures.
    // Secondly, the procedures that only contain calls to procedures already in
    // UsesProcTable. Thirdly, the procedures that only contain calls to
    // procedures already in UsesProcTable. Fourthly, ...
    // ...
    // Lastly, the first procedure.
    // The reason is the below line `p0Vars.merge(vs2);` assumes `vs2` is
    // the complete indexes of varTable contained in `p1`.

    // Firstly, we populate UsesProcTable with the procedure "aux" that does not
    // contain calls to other procedures. We populate the `p1` key of
    // UsesProcTable.

    Pkb::USES_TABLE usesTable = pkbTables->getUsesTable();
    const Pkb::USES_PROC_TABLE &usesProcTable = pkbTables->getUsesProcTable();

    // Line 3:
    Pkb::VARS vs3 = std::get<Pkb::VARS>(usesTable.map[l3]);
    Assert::IsTrue(vs3 == Pkb::VARS{v3});

    // There are no more lines in the procedure "aux", so we populate the `p1`
    // key of UsesProcTable with all collected Pkb::VARS.
    Pkb::VARS p1Vars = vs3;
    pkbTables->addUsesProc(p1, p1Vars);

    Assert::IsTrue(usesProcTable.map.at(p1) == Pkb::VARS{v3});

    // Lastly, we populate UsesProcTable with the first procedure "main".
    // We populate the `p0` key of UsesProcTable.

    // Line 1:
    Pkb::VARS vs1 = std::get<Pkb::VARS>(usesTable.map[l1]);
    Assert::IsTrue(vs1 == Pkb::VARS{v1, v2});

    // Line 2:
    // Now that the `p1` key of UsesProcTable has been populated, we can
    // soundly call `pkbTables->getUsesProc`. Note: here we use both UsesTable
    // and UsesProcTable.
    Assert::IsTrue(std::get<Pkb::PROC>(usesTable.map[l2]) == p1);
    Pkb::VARS vs2 = usesProcTable.map.at(p1);
    Assert::IsTrue(vs2 == Pkb::VARS{v3});

    // There are no more lines in the procedure "aux", so we populate the `p0`
    // key of UsesProcTable with all collected Pkb::VARS.
    Pkb::VARS p0Vars = vs1;
    p0Vars.merge(vs2);
    pkbTables->addUsesProc(p0, p0Vars);

    Assert::IsTrue(usesProcTable.map.at(p0) == Pkb::VARS{v1, v2, v3});

    // We assert default values.
    Assert::IsTrue(usesTable.map[l4] == Pkb::USES());

    // We test Pkb::transit.
    KeysTable<Pkb::LINE_NO, Pkb::VARS> transitUsesTable =
        PkbTableTransformers::transit(usesTable, usesProcTable);
    Assert::IsTrue(transitUsesTable.map[l1] == Pkb::VARS{v1, v2});
    Assert::IsTrue(transitUsesTable.map[l2] == Pkb::VARS{v3});
    Assert::IsTrue(transitUsesTable.map[l3] == Pkb::VARS{v3});
    Assert::IsTrue(transitUsesTable.map[l4] == Pkb::VARS());

    // We test Pkb::pseudoinvertFlattenKeys.
    KeysTable<Pkb::VAR, Pkb::LINE_NOS> pseudoinvertFlattenKeysTransitUsesTable =
        PkbTableTransformers::pseudoinvertFlattenKeys<Pkb::LINE_NO, Pkb::VAR>(
            transitUsesTable);
    Assert::IsTrue(pseudoinvertFlattenKeysTransitUsesTable.map[v0] ==
                   Pkb::LINE_NOS());
    Assert::IsTrue(pseudoinvertFlattenKeysTransitUsesTable.map[v1] ==
                   std::unordered_set{l1});
    Assert::IsTrue(pseudoinvertFlattenKeysTransitUsesTable.map[v2] ==
                   std::unordered_set{l1});
    Assert::IsTrue(pseudoinvertFlattenKeysTransitUsesTable.map[v3] ==
                   std::unordered_set{l2, l3});

  } // namespace UnitTesting

  /** @brief Populate Pkb::modifiesTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     a = x + y;
  2     call aux;
      }
      procedure aux {
  3     print z;
  4     read z;
      }
  */
  TEST_METHOD(TestModifiesTableAndModifiesProcTable) {
    Pkb::PROC p0 = "main";
    Pkb::PROC p1 = "aux";
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "x";
    Pkb::VAR v2 = "y";
    Pkb::VAR v3 = "z";

    pkbTables->addProc(p0);
    pkbTables->addProc(p1);
    pkbTables->addVar(v0);
    pkbTables->addVar(v1);
    pkbTables->addVar(v2);
    pkbTables->addVar(v3);

    /* ModifiesTable ******/
    // ModifiesTable may be populated in any order.
    pkbTables->addModifies(l1, Pkb::VARS{v0});
    pkbTables->addModifies(l2, p1);
    pkbTables->addModifies(l4, Pkb::VARS{v3});

    /* ModifiesProcTable ******/
    // ModifiesProcTable must be populated in a certain order:
    // First, the procedures that do not contain calls to other procedures.
    // Secondly, the procedures that only contain calls to procedures already in
    // ModifiesProcTable. Thirdly, the procedures that only contain calls to
    // procedures already in ModifiesProcTable. Fourthly, ...
    // ...
    // Lastly, the first procedure.
    // The reason is the below line `p0Vars.merge(vs2);` assumes `vs2` is
    // the complete indexes of varTable contained in `p1`.

    // Firstly, we populate ModifiesProcTable with the procedure "aux" that does
    // not contain calls to other procedures. We populate the `p1` key of
    // ModifiesProcTable.

    Pkb::MODIFIES_TABLE modifiesTable = pkbTables->getModifiesTable();
    const Pkb::MODIFIES_PROC_TABLE &modifiesProcTable =
        pkbTables->getModifiesProcTable();

    // Line 4:
    Pkb::VARS vs4 = std::get<Pkb::VARS>(modifiesTable.map[l4]);
    Assert::IsTrue(vs4 == Pkb::VARS{v3});

    // There are no more lines in the procedure "aux", so we populate the `p1`
    // key of ModifiesProcTable with all collected Pkb::VARS.
    Pkb::VARS p1Vars = vs4;
    pkbTables->addModifiesProc(p1, p1Vars);

    Assert::IsTrue(modifiesProcTable.map.at(p1) == Pkb::VARS{v3});

    // Lastly, we populate ModifiesProcTable with the first procedure "main".
    // We populate the `p0` key of ModifiesProcTable.

    // Line 1:
    Pkb::VARS vs1 = std::get<Pkb::VARS>(modifiesTable.map[l1]);
    Assert::IsTrue(vs1 == Pkb::VARS{v0});

    // Line 2:
    // Now that the `p1` key of ModifiesProcTable has been populated, we can
    // soundly call `pkbTables->getModifiesProc`. Note: here we use both
    // ModifiesTable and ModifiesProcTable.
    Assert::IsTrue(std::get<Pkb::PROC>(modifiesTable.map[l2]) == p1);
    Pkb::VARS vs2 = modifiesProcTable.map.at(p1);
    Assert::IsTrue(vs2 == Pkb::VARS{v3});

    // There are no more lines in the procedure "aux", so we populate the `p0`
    // key of ModifiesProcTable with all collected Pkb::VARS.
    Pkb::VARS p0Vars = vs1;
    p0Vars.merge(vs2);
    pkbTables->addModifiesProc(p0, p0Vars);

    Assert::IsTrue(modifiesProcTable.map.at(p0) == Pkb::VARS{v0, v3});

    // We assert default values.
    Assert::IsTrue(modifiesTable.map[l3] == Pkb::MODIFIES());

    // We test Pkb::transit.
    KeysTable<Pkb::LINE_NO, Pkb::VARS> transitModifiesTable =
        PkbTableTransformers::transit(modifiesTable, modifiesProcTable);
    Assert::IsTrue(transitModifiesTable.map[l1] == Pkb::VARS{v0});
    Assert::IsTrue(transitModifiesTable.map[l2] == Pkb::VARS{v3});
    Assert::IsTrue(transitModifiesTable.map[l3] == Pkb::VARS());
    Assert::IsTrue(transitModifiesTable.map[l4] == Pkb::VARS{v3});

    // We test Pkb::pseudoinvertFlattenKeys.
    KeysTable<Pkb::VAR, Pkb::LINE_NOS>
        pseudoinvertFlattenKeysTransitModifiesTable =
            PkbTableTransformers::pseudoinvertFlattenKeys<Pkb::LINE_NO,
                                                          Pkb::VAR>(
                transitModifiesTable);
    Assert::IsTrue(pseudoinvertFlattenKeysTransitModifiesTable.map[v0] ==
                   std::unordered_set{l1});
    Assert::IsTrue(pseudoinvertFlattenKeysTransitModifiesTable.map[v1] ==
                   Pkb::LINE_NOS());
    Assert::IsTrue(pseudoinvertFlattenKeysTransitModifiesTable.map[v2] ==
                   Pkb::LINE_NOS());
    Assert::IsTrue(pseudoinvertFlattenKeysTransitModifiesTable.map[v3] ==
                   std::unordered_set{l2, l4});

  } // namespace UnitTesting

  /** @brief Populate Pkb::followTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     a = x;
  2     if (x == y) {
  3       x = y;
  4       while (x == y) {
  5         a = z; }
  6       a = x;
        } else {
  7       call aux; }
  8     a = z;
  9     a = z;
      }
      procedure aux {
  10    while (x == y) {
  11      if (x == y) {
  12        print z;
          } else {
  13        read z; }}
  */
  TEST_METHOD(TestFollowTable) {
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;
    Pkb::LINE_NO l5 = 5;
    Pkb::LINE_NO l6 = 6;
    Pkb::LINE_NO l7 = 7;
    Pkb::LINE_NO l8 = 8;
    Pkb::LINE_NO l9 = 9;
    Pkb::LINE_NO l10 = 10;
    Pkb::LINE_NO l11 = 11;
    Pkb::LINE_NO l12 = 12;
    Pkb::LINE_NO l13 = 13;

    pkbTables->addFollow(l1, l2);
    pkbTables->addFollow(l2, l8);
    pkbTables->addFollow(l3, l4);
    pkbTables->addFollow(l4, l6);
    pkbTables->addFollow(l8, l9);
    pkbTables->addFollow(l10, l11);

    Pkb::FOLLOW_TABLE followTable = pkbTables->getFollowTable();

    Assert::IsTrue(followTable.map[l1] == l2);
    Assert::IsTrue(followTable.map[l2] == l8);
    Assert::IsTrue(followTable.map[l3] == l4);
    Assert::IsTrue(followTable.map[l4] == l6);
    Assert::IsTrue(followTable.map[l5] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l6] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l7] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l8] == l9);
    Assert::IsTrue(followTable.map[l9] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l10] == l11);
    Assert::IsTrue(followTable.map[l11] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l12] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l13] == Pkb::FOLLOW());

    KeysTable<Pkb::FOLLOW, Pkb::LINE_NO> invertFollowTable =
        PkbTableTransformers::invert<Pkb::LINE_NO, Pkb::FOLLOW>(followTable);

    Assert::IsTrue(invertFollowTable.map[l1] == Pkb::LINE_NO());
    Assert::IsTrue(invertFollowTable.map[l2] == l1);
    Assert::IsTrue(invertFollowTable.map[l3] == Pkb::LINE_NO());
    Assert::IsTrue(invertFollowTable.map[l4] == l3);
    Assert::IsTrue(invertFollowTable.map[l5] == Pkb::LINE_NO());
    Assert::IsTrue(invertFollowTable.map[l6] == l4);
    Assert::IsTrue(invertFollowTable.map[l7] == Pkb::LINE_NO());
    Assert::IsTrue(invertFollowTable.map[l8] == l2);
    Assert::IsTrue(invertFollowTable.map[l9] == l8);
    Assert::IsTrue(invertFollowTable.map[l10] == Pkb::LINE_NO());
    Assert::IsTrue(invertFollowTable.map[l11] == l10);
    Assert::IsTrue(invertFollowTable.map[l12] == Pkb::LINE_NO());
    Assert::IsTrue(invertFollowTable.map[l13] == Pkb::LINE_NO());

    // Pkb::close does compute the transitive closure.
    KeysTable<Pkb::LINE_NO, Pkb::FOLLOWS> closeFollowTable =
        PkbTableTransformers::close<Pkb::FOLLOW>(followTable);

    Assert::IsTrue(closeFollowTable.map[l1] == Pkb::FOLLOWS{l2, l8, l9});
    Assert::IsTrue(closeFollowTable.map[l2] == Pkb::FOLLOWS{l8, l9});
    Assert::IsTrue(closeFollowTable.map[l3] == Pkb::FOLLOWS{l4, l6});
    Assert::IsTrue(closeFollowTable.map[l4] == Pkb::FOLLOWS{l6});
    Assert::IsTrue(closeFollowTable.map[l5] == Pkb::FOLLOWS());
    Assert::IsTrue(closeFollowTable.map[l6] == Pkb::FOLLOWS());
    Assert::IsTrue(closeFollowTable.map[l7] == Pkb::FOLLOWS());
    Assert::IsTrue(closeFollowTable.map[l8] == Pkb::FOLLOWS{l9});
    Assert::IsTrue(closeFollowTable.map[l9] == Pkb::FOLLOWS());
    Assert::IsTrue(closeFollowTable.map[l10] == Pkb::FOLLOWS{l11});
    Assert::IsTrue(closeFollowTable.map[l11] == Pkb::FOLLOWS());
    Assert::IsTrue(closeFollowTable.map[l12] == Pkb::FOLLOWS());
    Assert::IsTrue(closeFollowTable.map[l13] == Pkb::FOLLOWS());

  } // namespace UnitTesting

  /** @brief Populate Pkb::parentTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     while (a == x) {
  2       if (x == y) {
  3         while (a == x) {
  4           a = x + y; }
          } else {
  5         call aux; }}
      }
      procedure aux {
  6     print z;
  7     read z;
      }
  */
  TEST_METHOD(TestParentTable) {
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;
    Pkb::LINE_NO l5 = 5;
    Pkb::LINE_NO l6 = 6;
    Pkb::LINE_NO l7 = 7;

    pkbTables->addParent(l2, l1);
    pkbTables->addParent(l3, l2);
    pkbTables->addParent(l4, l3);
    pkbTables->addParent(l5, l2);

    Pkb::PARENT_TABLE parentTable = pkbTables->getParentTable();

    Assert::IsTrue(parentTable.map[l1] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[l2] == l1);
    Assert::IsTrue(parentTable.map[l3] == l2);
    Assert::IsTrue(parentTable.map[l4] == l3);
    Assert::IsTrue(parentTable.map[l5] == l2);
    Assert::IsTrue(parentTable.map[l6] == Pkb::PARENT());
    Assert::IsTrue(parentTable.map[l7] == Pkb::PARENT());

    KeysTable<Pkb::PARENT, Pkb::LINE_NOS> pseudoinvertParentTable =
        PkbTableTransformers::pseudoinvert<Pkb::LINE_NO, Pkb::PARENT>(
            parentTable);

    Assert::IsTrue(pseudoinvertParentTable.map[l1] == Pkb::CHILDREN{l2});
    Assert::IsTrue(pseudoinvertParentTable.map[l2] == Pkb::CHILDREN{l3, l5});
    Assert::IsTrue(pseudoinvertParentTable.map[l3] == Pkb::CHILDREN{l4});
    Assert::IsTrue(pseudoinvertParentTable.map[l4] == Pkb::CHILDREN());
    Assert::IsTrue(pseudoinvertParentTable.map[l5] == Pkb::CHILDREN());
    Assert::IsTrue(pseudoinvertParentTable.map[l6] == Pkb::CHILDREN());
    Assert::IsTrue(pseudoinvertParentTable.map[l7] == Pkb::CHILDREN());

    // Test Pkb::closeFlatten.
    KeysTable<Pkb::PARENT, Pkb::CHILDREN> closeFlattenPseudoinvertParentTable =
        PkbTableTransformers::closeFlatten<Pkb::PARENT>(
            pseudoinvertParentTable);

    Assert::IsTrue(closeFlattenPseudoinvertParentTable.map[l1] ==
                   Pkb::CHILDREN{l2, l3, l5, l4});
    Assert::IsTrue(closeFlattenPseudoinvertParentTable.map[l2] ==
                   Pkb::CHILDREN{l3, l5, l4});
    Assert::IsTrue(closeFlattenPseudoinvertParentTable.map[l3] ==
                   Pkb::CHILDREN{l4});
    Assert::IsTrue(closeFlattenPseudoinvertParentTable.map[l4] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(closeFlattenPseudoinvertParentTable.map[l5] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(closeFlattenPseudoinvertParentTable.map[l6] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(closeFlattenPseudoinvertParentTable.map[l7] ==
                   Pkb::CHILDREN());

    // With reversed keys...
    Pkb::PARENT_TABLE parentTableKeysReversed = pkbTables->getParentTable();
    std::reverse(parentTableKeysReversed.keys.begin(),
                 parentTableKeysReversed.keys.end());

    // ... Pkb::close does compute the transitive closure.
    KeysTable<Pkb::LINE_NO, Pkb::PARENTS> closeParentTableKeysReversed =
        PkbTableTransformers::close<Pkb::PARENT>(parentTableKeysReversed);

    Assert::IsTrue(closeParentTableKeysReversed.map[l1] == Pkb::PARENTS());
    Assert::IsTrue(closeParentTableKeysReversed.map[l2] == Pkb::PARENTS{l1});
    Assert::IsTrue(closeParentTableKeysReversed.map[l3] ==
                   Pkb::PARENTS{l2, l1});
    Assert::IsTrue(closeParentTableKeysReversed.map[l4] ==
                   Pkb::PARENTS{l3, l2, l1});
    Assert::IsTrue(closeParentTableKeysReversed.map[l5] ==
                   Pkb::PARENTS{l2, l1});
    Assert::IsTrue(closeParentTableKeysReversed.map[l6] == Pkb::PARENTS());
    Assert::IsTrue(closeParentTableKeysReversed.map[l7] == Pkb::PARENTS());

  } // namespace UnitTesting

  /** @brief Populate Pkb::statementProcTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     a = x + y;
  2     call aux;
      }
      procedure aux {
  3     print z;
  4     read z;
      }
  */
  TEST_METHOD(TestStatementProcTable) {
    Pkb::PROC p0 = "main";
    Pkb::PROC p1 = "aux";
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;

    pkbTables->addProc(p0);
    pkbTables->addProc(p1);
    pkbTables->addStatementProc(l1, p0);
    pkbTables->addStatementProc(l2, p0);
    pkbTables->addStatementProc(l3, p1);
    pkbTables->addStatementProc(l4, p1);

    Pkb::STATEMENT_PROC_TABLE statementProcTable =
        pkbTables->getStatementProcTable();

    Assert::IsTrue(statementProcTable.map[l1] == p0);
    Assert::IsTrue(statementProcTable.map[l2] == p0);
    Assert::IsTrue(statementProcTable.map[l3] == p1);
    Assert::IsTrue(statementProcTable.map[l4] == p1);

  } // namespace UnitTesting

  /** @brief Populate Pkb::statementTypeTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     while (a == x) {
  2       if (x == y) {
  3         a = x + y;
          } else {
  4         call aux; }}
      }
      procedure aux {
  5     print z;
  6     read z;
      }
  */
  TEST_METHOD(TestStatementTypeTable) {
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;
    Pkb::LINE_NO l5 = 5;
    Pkb::LINE_NO l6 = 6;

    pkbTables->addStatementType(l1, Pkb::StatementType::While);
    pkbTables->addStatementType(l2, Pkb::StatementType::If);
    pkbTables->addStatementType(l3, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l4, Pkb::StatementType::Call);
    pkbTables->addStatementType(l5, Pkb::StatementType::Print);
    pkbTables->addStatementType(l6, Pkb::StatementType::Read);

    Pkb::STATEMENT_TYPE_TABLE statementTypeTable =
        pkbTables->getStatementTypeTable();

    Assert::IsTrue(statementTypeTable.map[l1] == Pkb::StatementType::While);
    Assert::IsTrue(statementTypeTable.map[l2] == Pkb::StatementType::If);
    Assert::IsTrue(statementTypeTable.map[l3] == Pkb::StatementType::Assign);
    Assert::IsTrue(statementTypeTable.map[l4] == Pkb::StatementType::Call);
    Assert::IsTrue(statementTypeTable.map[l5] == Pkb::StatementType::Print);
    Assert::IsTrue(statementTypeTable.map[l6] == Pkb::StatementType::Read);

  } // namespace UnitTesting

  /** @brief Populate Pkb::assignAstTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     a = x + y;
  2     call aux;
      }
      procedure aux {
  3     print z;
  4     read z;
      }
  */
  TEST_METHOD(TestAssignAstTable) {
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "x";
    Pkb::VAR v2 = "y";
    std::shared_ptr<TNode> plus = std::make_shared<TNode>(TNode::Op::Plus);
    plus->left = std::make_shared<TNode>(v1);
    plus->right = std::make_shared<TNode>(v1);

    pkbTables->addAssignAst(l1, plus);

    Pkb::ASSIGN_AST_TABLE assignAstTable = pkbTables->getAssignAstTable();

    Assert::IsTrue(assignAstTable.map[l1] == plus);
    Assert::IsTrue(assignAstTable.map[l2] == Pkb::AST());
    Assert::IsTrue(assignAstTable.map[l3] == Pkb::AST());
    Assert::IsTrue(assignAstTable.map[l4] == Pkb::AST());

  } // namespace UnitTesting

  /** @brief Populate Pkb::conditionVarsTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     while (a == x) {
  2       if (x == y) {
  3         while (a == x) {
  4           a = x + y; }
          } else {
  5         call aux; }}
      }
      procedure aux {
  6     print z;
  7     read z;
      }
  */
  TEST_METHOD(TestConditionVarsTable) {
    Pkb::LINE_NO l1 = 1;
    Pkb::LINE_NO l2 = 2;
    Pkb::LINE_NO l3 = 3;
    Pkb::LINE_NO l4 = 4;
    Pkb::LINE_NO l5 = 5;
    Pkb::LINE_NO l6 = 6;
    Pkb::LINE_NO l7 = 7;
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "x";
    Pkb::VAR v2 = "y";
    Pkb::VAR v3 = "z";

    pkbTables->addVar(v0);
    pkbTables->addVar(v1);
    pkbTables->addVar(v2);
    pkbTables->addVar(v3);

    pkbTables->addConditionVars(l1, {v0, v1});
    pkbTables->addConditionVars(l2, {v1, v2});
    pkbTables->addConditionVars(l3, {v0, v1});

    Pkb::CONDITION_VARS_TABLE conditionVarsTable =
        pkbTables->getConditionVarsTable();
    Assert::IsTrue(conditionVarsTable.map[l1] == Pkb::VARS{v0, v1});
    Assert::IsTrue(conditionVarsTable.map[l2] == Pkb::VARS{v1, v2});
    Assert::IsTrue(conditionVarsTable.map[l3] == Pkb::VARS{v0, v1});
    Assert::IsTrue(conditionVarsTable.map[l4] == Pkb::VARS());
    Assert::IsTrue(conditionVarsTable.map[l5] == Pkb::VARS());
    Assert::IsTrue(conditionVarsTable.map[l6] == Pkb::VARS());
    Assert::IsTrue(conditionVarsTable.map[l7] == Pkb::VARS());

  } // namespace UnitTesting

  /** @brief Populate PKB::callsTable.
  To be tested: SIMPLE Program (line numbers elided):
      procedure main {
        call aux1;
        call aux2;
        call aux3;
      }
      procedure aux1 {
        call aux2;
        call aux3;
      }
      procedure aux2 {
        call aux3;
        call aux4;
      }
      procedure aux3 {
        read aux3;
      }
      procedure aux4 {
        call aux5;
      }
      procedure aux5 {
        read aux5;
      }
  */
  TEST_METHOD(TestCallsTable) {
    Pkb::PROC p0 = "main";
    Pkb::PROC p1 = "aux1";
    Pkb::PROC p2 = "aux2";
    Pkb::PROC p3 = "aux3";
    Pkb::PROC p4 = "aux4";
    Pkb::PROC p5 = "aux5";

    pkbTables->addProc(p0);
    pkbTables->addProc(p1);
    pkbTables->addProc(p2);
    pkbTables->addProc(p3);
    pkbTables->addProc(p4);
    pkbTables->addProc(p5);

    pkbTables->addCall(p0, p1);
    pkbTables->addCall(p0, p2);
    pkbTables->addCall(p0, p3);
    pkbTables->addCall(p1, p2);
    pkbTables->addCall(p1, p3);
    pkbTables->addCall(p2, p3);
    pkbTables->addCall(p2, p4);
    pkbTables->addCall(p4, p5);

    Pkb::CALLS_TABLE callsTable = pkbTables->getCallsTable();

    Assert::IsTrue(callsTable.map[p0] == Pkb::CALLS{p1, p2, p3});
    Assert::IsTrue(callsTable.map[p1] == Pkb::CALLS{p2, p3});
    Assert::IsTrue(callsTable.map[p2] == Pkb::CALLS{p3, p4});
    Assert::IsTrue(callsTable.map[p3] == Pkb::CALLS());
    Assert::IsTrue(callsTable.map[p4] == Pkb::CALLS{p5});
    Assert::IsTrue(callsTable.map[p5] == Pkb::CALLS());

    KeysTable<Pkb::CALL, Pkb::PROCS> pseudoinvertKeysFlattenCallsTable =
        PkbTableTransformers::pseudoinvertFlattenKeys<Pkb::PROC, Pkb::CALL>(
            callsTable);

    Assert::IsTrue(pseudoinvertKeysFlattenCallsTable.map[p1] == Pkb::PROCS{p0});
    Assert::IsTrue(pseudoinvertKeysFlattenCallsTable.map[p2] ==
                   Pkb::PROCS{p0, p1});
    Assert::IsTrue(pseudoinvertKeysFlattenCallsTable.map[p3] ==
                   Pkb::PROCS{p0, p1, p2});
    Assert::IsTrue(pseudoinvertKeysFlattenCallsTable.map[p4] == Pkb::PROCS{p2});
    Assert::IsTrue(pseudoinvertKeysFlattenCallsTable.map[p5] == Pkb::PROCS{p4});

    KeysTable<Pkb::PROC, Pkb::CALLS> closeFlattenCallsTable =
        PkbTableTransformers::closeFlatten<Pkb::CALL>(callsTable);

    Assert::IsTrue(closeFlattenCallsTable.map[p0] ==
                   Pkb::CALLS{p1, p2, p3, p4, p5});
    Assert::IsTrue(closeFlattenCallsTable.map[p1] ==
                   Pkb::CALLS{p2, p3, p4, p5});
    Assert::IsTrue(closeFlattenCallsTable.map[p2] == Pkb::CALLS{p3, p4, p5});
    Assert::IsTrue(closeFlattenCallsTable.map[p3] == Pkb::CALLS());
    Assert::IsTrue(closeFlattenCallsTable.map[p4] == Pkb::CALLS{p5});
    Assert::IsTrue(closeFlattenCallsTable.map[p5] == Pkb::CALLS());

  } // namespace UnitTesting

  /** @brief Populate Pkb::constantTable. */
  TEST_METHOD(TestConstantTable) {
    Pkb::CONSTANT c0 = "-2";
    Pkb::CONSTANT c1 = "-1";
    Pkb::CONSTANT c2 = "0";
    Pkb::CONSTANT c3 = "1";

    pkbTables->addConstant(c0);
    pkbTables->addConstant(c1);
    pkbTables->addConstant(c2);
    pkbTables->addConstant(c3);

    Pkb::CONSTANT_TABLE constantTable = pkbTables->getConstantTable();

    Assert::IsTrue(constantTable == Pkb::CONSTANT_TABLE{c0, c1, c2, c3});

  } // namespace UnitTesting
};
} // namespace UnitTesting
