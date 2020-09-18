#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPkbTables) {

public:
  Pkb pkb;
  PkbTables *pkbTables;

  TEST_METHOD_INITIALIZE(SetUpPkbTables) {
    pkbTables = pkb.getTables();
  } // namespace UnitTesting

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

    Assert::IsTrue(pkbTables->addVar(v0) == 1);
    Assert::IsTrue(pkbTables->addVar(v1) == 2);
    Assert::IsTrue(pkbTables->addVar(v2) == 3);
    Assert::IsTrue(pkbTables->addVar(v3) == 4);

    // Adding an existing variable returns the existing index.
    Assert::IsTrue(pkbTables->addVar(v2) == 3);

    Pkb::VAR_TABLE varTable = pkbTables->getVarTable();

    Assert::IsTrue(varTable.map[v0] == 1);
    Assert::IsTrue(varTable.map[v1] == 2);
    Assert::IsTrue(varTable.map[v2] == 3);
    Assert::IsTrue(varTable.map[v3] == 4);

    // Getting the value of an unmapped key returns the default constructor.
    Assert::IsTrue(varTable.map[v4] == Pkb::VAR_TABLE_INDEX());

    KeysTable<Pkb::VAR_TABLE_INDEX, Pkb::VAR> varTableInverted =
        pkbTables->invert<Pkb::VAR, Pkb::VAR_TABLE_INDEX>(varTable);

    Assert::IsTrue(varTableInverted.map[1] == v0);
    Assert::IsTrue(varTableInverted.map[2] == v1);
    Assert::IsTrue(varTableInverted.map[3] == v2);
    Assert::IsTrue(varTableInverted.map[4] == v3);

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

    Assert::IsTrue(pkbTables->addProc(p0) == 1);
    Assert::IsTrue(pkbTables->addProc(p1) == 2);
    Assert::IsTrue(pkbTables->addProc(p2) == 3);
    Assert::IsTrue(pkbTables->addProc(p3) == 4);

    Pkb::PROC_TABLE procTable = pkbTables->getProcTable();

    Assert::IsTrue(procTable.map[p0] == 1);
    Assert::IsTrue(procTable.map[p1] == 2);
    Assert::IsTrue(procTable.map[p2] == 3);
    Assert::IsTrue(procTable.map[p3] == 4);

    // Getting the value of an unmapped key returns the default constructor.
    Assert::IsTrue(procTable.map[p4] == Pkb::PROC_TABLE_INDEX());

    KeysTable<Pkb::PROC_TABLE_INDEX, Pkb::PROC> procTableInverted =
        pkbTables->invert<Pkb::PROC, Pkb::PROC_TABLE_INDEX>(procTable);

    Assert::IsTrue(procTableInverted.map[1] == p0);
    Assert::IsTrue(procTableInverted.map[2] == p1);
    Assert::IsTrue(procTableInverted.map[3] == p2);
    Assert::IsTrue(procTableInverted.map[4] == p3);

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
    Pkb::PROC_TABLE_INDEX pti0 = pkbTables->addProc(p0);
    Pkb::PROC_TABLE_INDEX pti1 = pkbTables->addProc(p1);
    Pkb::LINE_NO l1 = "1";
    Pkb::LINE_NO l2 = "2";
    Pkb::LINE_NO l3 = "3";
    Pkb::LINE_NO l4 = "4";
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "x";
    Pkb::VAR v2 = "y";
    Pkb::VAR v3 = "z";

    Pkb::VAR_TABLE_INDEX vti0 = pkbTables->addVar(v0);
    Pkb::VAR_TABLE_INDEX vti1 = pkbTables->addVar(v1);
    Pkb::VAR_TABLE_INDEX vti2 = pkbTables->addVar(v2);
    Pkb::VAR_TABLE_INDEX vti3 = pkbTables->addVar(v3);

    /* UsesTable ******/
    // UsesTable may be populated in any order.
    pkbTables->addUses(l1, Pkb::VAR_TABLE_INDEXES{vti1, vti2});
    pkbTables->addUses(l2, pti1);
    pkbTables->addUses(l3, Pkb::VAR_TABLE_INDEXES{vti3});

    /* UsesProcTable ******/
    // UsesProcTable must be populated in a certain order:
    // First, the procedures that do not contain calls to other procedures.
    // Secondly, the procedures that only contain calls to procedures already in
    // UsesProcTable. Thirdly, the procedures that only contain calls to
    // procedures already in UsesProcTable. Fourthly, ...
    // ...
    // Lastly, the first procedure.
    // The reason is the below line `pti0Vars.merge(vtis2);` assumes `vtis2` is
    // the complete indexes of varTable contained in `pti1`.

    // Firstly, we populate UsesProcTable with the procedure "aux" that does not
    // contain calls to other procedures. We populate the `pti1` key of
    // UsesProcTable.

    Pkb::USES_TABLE usesTable = pkbTables->getUsesTable();
    const Pkb::USES_PROC_TABLE &usesProcTable = pkbTables->getUsesProcTable();

    // Line 3:
    Pkb::VAR_TABLE_INDEXES vtis3 =
        std::get<Pkb::VAR_TABLE_INDEXES>(usesTable.map[l3]);
    Assert::IsTrue(vtis3 == Pkb::VAR_TABLE_INDEXES{vti3});

    // There are no more lines in the procedure "aux", so we populate the `pti1`
    // key of UsesProcTable with all collected Pkb::VAR_TABLE_INDEXES.
    Pkb::VAR_TABLE_INDEXES pti1Vars = vtis3;
    pkbTables->addUsesProc(pti1, pti1Vars);

    Assert::IsTrue(usesProcTable.map.at(pti1) == Pkb::VAR_TABLE_INDEXES{vti3});

    // Lastly, we populate UsesProcTable with the first procedure "main".
    // We populate the `pti0` key of UsesProcTable.

    // Line 1:
    Pkb::VAR_TABLE_INDEXES vtis1 =
        std::get<Pkb::VAR_TABLE_INDEXES>(usesTable.map[l1]);
    Assert::IsTrue(vtis1 == Pkb::VAR_TABLE_INDEXES{vti1, vti2});

    // Line 2:
    // Now that the `pti1` key of UsesProcTable has been populated, we can
    // soundly call `pkbTables->getUsesProc`. Note: here we use both UsesTable
    // and UsesProcTable.
    Assert::IsTrue(std::get<Pkb::PROC_TABLE_INDEX>(usesTable.map[l2]) == pti1);
    Pkb::VAR_TABLE_INDEXES vtis2 = usesProcTable.map.at(pti1);
    Assert::IsTrue(vtis2 == Pkb::VAR_TABLE_INDEXES{vti3});

    // There are no more lines in the procedure "aux", so we populate the `pti0`
    // key of UsesProcTable with all collected Pkb::VAR_TABLE_INDEXES.
    Pkb::VAR_TABLE_INDEXES pti0Vars = vtis1;
    pti0Vars.merge(vtis2);
    pkbTables->addUsesProc(pti0, pti0Vars);

    Assert::IsTrue(usesProcTable.map.at(pti0) ==
                   Pkb::VAR_TABLE_INDEXES{vti1, vti2, vti3});

    // We assert default values.
    Assert::IsTrue(usesTable.map[l4] == Pkb::USES());

    // We test Pkb::transit.
    KeysTable<Pkb::LINE_NO, Pkb::VAR_TABLE_INDEXES> usesTableTransited =
        pkbTables->transit(usesTable, usesProcTable);
    Assert::IsTrue(usesTableTransited.map[l1] ==
                   Pkb::VAR_TABLE_INDEXES{vti1, vti2});
    Assert::IsTrue(usesTableTransited.map[l2] == Pkb::VAR_TABLE_INDEXES{vti3});
    Assert::IsTrue(usesTableTransited.map[l3] == Pkb::VAR_TABLE_INDEXES{vti3});
    Assert::IsTrue(usesTableTransited.map[l4] == Pkb::VAR_TABLE_INDEXES());

    // We test Pkb::pseudoinvertFlattenKeys.
    KeysTable<Pkb::VAR_TABLE_INDEX, std::unordered_set<Pkb::LINE_NO>>
        usesTableTransitedPseudoinvertedKeysFlattened =
            pkbTables
                ->pseudoinvertFlattenKeys<Pkb::LINE_NO, Pkb::VAR_TABLE_INDEX>(
                    usesTableTransited);
    Assert::IsTrue(usesTableTransitedPseudoinvertedKeysFlattened.map[vti0] ==
                   std::unordered_set<Pkb::LINE_NO>());
    Assert::IsTrue(usesTableTransitedPseudoinvertedKeysFlattened.map[vti1] ==
                   std::unordered_set{l1});
    Assert::IsTrue(usesTableTransitedPseudoinvertedKeysFlattened.map[vti2] ==
                   std::unordered_set{l1});
    Assert::IsTrue(usesTableTransitedPseudoinvertedKeysFlattened.map[vti3] ==
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
    Pkb::PROC_TABLE_INDEX pti0 = pkbTables->addProc(p0);
    Pkb::PROC_TABLE_INDEX pti1 = pkbTables->addProc(p1);
    Pkb::LINE_NO l1 = "1";
    Pkb::LINE_NO l2 = "2";
    Pkb::LINE_NO l3 = "3";
    Pkb::LINE_NO l4 = "4";
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "x";
    Pkb::VAR v2 = "y";
    Pkb::VAR v3 = "z";

    Pkb::VAR_TABLE_INDEX vti0 = pkbTables->addVar(v0);
    Pkb::VAR_TABLE_INDEX vti1 = pkbTables->addVar(v1);
    Pkb::VAR_TABLE_INDEX vti2 = pkbTables->addVar(v2);
    Pkb::VAR_TABLE_INDEX vti3 = pkbTables->addVar(v3);

    /* ModifiesTable ******/
    // ModifiesTable may be populated in any order.
    pkbTables->addModifies(l1, Pkb::VAR_TABLE_INDEXES{vti0});
    pkbTables->addModifies(l2, pti1);
    pkbTables->addModifies(l4, Pkb::VAR_TABLE_INDEXES{vti3});

    /* ModifiesProcTable ******/
    // ModifiesProcTable must be populated in a certain order:
    // First, the procedures that do not contain calls to other procedures.
    // Secondly, the procedures that only contain calls to procedures already in
    // ModifiesProcTable. Thirdly, the procedures that only contain calls to
    // procedures already in ModifiesProcTable. Fourthly, ...
    // ...
    // Lastly, the first procedure.
    // The reason is the below line `pti0Vars.merge(vtis2);` assumes `vtis2` is
    // the complete indexes of varTable contained in `pti1`.

    // Firstly, we populate ModifiesProcTable with the procedure "aux" that does
    // not contain calls to other procedures. We populate the `pti1` key of
    // ModifiesProcTable.

    Pkb::MODIFIES_TABLE modifiesTable = pkbTables->getModifiesTable();
    const Pkb::MODIFIES_PROC_TABLE &modifiesProcTable =
        pkbTables->getModifiesProcTable();

    // Line 4:
    Pkb::VAR_TABLE_INDEXES vtis4 =
        std::get<Pkb::VAR_TABLE_INDEXES>(modifiesTable.map[l4]);
    Assert::IsTrue(vtis4 == Pkb::VAR_TABLE_INDEXES{vti3});

    // There are no more lines in the procedure "aux", so we populate the `pti1`
    // key of ModifiesProcTable with all collected Pkb::VAR_TABLE_INDEXES.
    Pkb::VAR_TABLE_INDEXES pti1Vars = vtis4;
    pkbTables->addModifiesProc(pti1, pti1Vars);

    Assert::IsTrue(modifiesProcTable.map.at(pti1) ==
                   Pkb::VAR_TABLE_INDEXES{vti3});

    // Lastly, we populate ModifiesProcTable with the first procedure "main".
    // We populate the `pti0` key of ModifiesProcTable.

    // Line 1:
    Pkb::VAR_TABLE_INDEXES vtis1 =
        std::get<Pkb::VAR_TABLE_INDEXES>(modifiesTable.map[l1]);
    Assert::IsTrue(vtis1 == Pkb::VAR_TABLE_INDEXES{vti0});

    // Line 2:
    // Now that the `pti1` key of ModifiesProcTable has been populated, we can
    // soundly call `pkbTables->getModifiesProc`. Note: here we use both
    // ModifiesTable and ModifiesProcTable.
    Assert::IsTrue(std::get<Pkb::PROC_TABLE_INDEX>(modifiesTable.map[l2]) ==
                   pti1);
    Pkb::VAR_TABLE_INDEXES vtis2 = modifiesProcTable.map.at(pti1);
    Assert::IsTrue(vtis2 == Pkb::VAR_TABLE_INDEXES{vti3});

    // There are no more lines in the procedure "aux", so we populate the `pti0`
    // key of ModifiesProcTable with all collected Pkb::VAR_TABLE_INDEXES.
    Pkb::VAR_TABLE_INDEXES pti0Vars = vtis1;
    pti0Vars.merge(vtis2);
    pkbTables->addModifiesProc(pti0, pti0Vars);

    Assert::IsTrue(modifiesProcTable.map.at(pti0) ==
                   Pkb::VAR_TABLE_INDEXES{vti0, vti3});

    // We assert default values.
    Assert::IsTrue(modifiesTable.map[l3] == Pkb::MODIFIES());

    // We test Pkb::transit.
    KeysTable<Pkb::LINE_NO, Pkb::VAR_TABLE_INDEXES> modifiesTableTransited =
        pkbTables->transit(modifiesTable, modifiesProcTable);
    Assert::IsTrue(modifiesTableTransited.map[l1] ==
                   Pkb::VAR_TABLE_INDEXES{vti0});
    Assert::IsTrue(modifiesTableTransited.map[l2] ==
                   Pkb::VAR_TABLE_INDEXES{vti3});
    Assert::IsTrue(modifiesTableTransited.map[l3] == Pkb::VAR_TABLE_INDEXES());
    Assert::IsTrue(modifiesTableTransited.map[l4] ==
                   Pkb::VAR_TABLE_INDEXES{vti3});

    // We test Pkb::pseudoinvertFlattenKeys.
    KeysTable<Pkb::VAR_TABLE_INDEX, std::unordered_set<Pkb::LINE_NO>>
        modifiesTableTransitedPseudoinvertedKeysFlattened =
            pkbTables
                ->pseudoinvertFlattenKeys<Pkb::LINE_NO, Pkb::VAR_TABLE_INDEX>(
                    modifiesTableTransited);
    Assert::IsTrue(
        modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti0] ==
        std::unordered_set{l1});
    Assert::IsTrue(
        modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti1] ==
        std::unordered_set<Pkb::LINE_NO>());
    Assert::IsTrue(
        modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti2] ==
        std::unordered_set<Pkb::LINE_NO>());
    Assert::IsTrue(
        modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti3] ==
        std::unordered_set{l2, l4});

  } // namespace UnitTesting

  /** @brief Populate Pkb::followTable.
  To be tested: SIMPLE Program:
      procedure main {
  1     a = x;
  2     if (x == y) {
  3       x = y;
  4       a = x;
        } else {
  5       call aux; }
  6     a = z;
  7     a = z;
      }
      procedure aux {
  8     print z;
  9     read z;
      }
  */
  TEST_METHOD(TestFollowTable) {
    Pkb::LINE_NO l1 = "1";
    Pkb::LINE_NO l2 = "2";
    Pkb::LINE_NO l3 = "3";
    Pkb::LINE_NO l4 = "4";
    Pkb::LINE_NO l5 = "5";
    Pkb::LINE_NO l6 = "6";
    Pkb::LINE_NO l7 = "7";
    Pkb::LINE_NO l8 = "8";
    Pkb::LINE_NO l9 = "9";

    pkbTables->addFollow(l1, l2);
    pkbTables->addFollow(l2, l6);
    pkbTables->addFollow(l3, l4);
    pkbTables->addFollow(l6, l7);
    pkbTables->addFollow(l8, l9);

    Pkb::FOLLOW_TABLE followTable = pkbTables->getFollowTable();

    Assert::IsTrue(followTable.map[l1] == l2);
    Assert::IsTrue(followTable.map[l2] == l6);
    Assert::IsTrue(followTable.map[l3] == l4);
    Assert::IsTrue(followTable.map[l4] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l5] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l6] == l7);
    Assert::IsTrue(followTable.map[l7] == Pkb::FOLLOW());
    Assert::IsTrue(followTable.map[l8] == l9);
    Assert::IsTrue(followTable.map[l9] == Pkb::FOLLOW());

    KeysTable<Pkb::FOLLOW, Pkb::LINE_NO> followTableInverted =
        pkbTables->invert<Pkb::LINE_NO, Pkb::FOLLOW>(followTable);

    Assert::IsTrue(followTableInverted.map[l1] == Pkb::LINE_NO());
    Assert::IsTrue(followTableInverted.map[l2] == l1);
    Assert::IsTrue(followTableInverted.map[l3] == Pkb::LINE_NO());
    Assert::IsTrue(followTableInverted.map[l4] == l3);
    Assert::IsTrue(followTableInverted.map[l5] == Pkb::LINE_NO());
    Assert::IsTrue(followTableInverted.map[l6] == l2);
    Assert::IsTrue(followTableInverted.map[l7] == l6);
    Assert::IsTrue(followTableInverted.map[l8] == Pkb::LINE_NO());
    Assert::IsTrue(followTableInverted.map[l9] == l8);

    // Pkb::closeOnce does not compute the transitive closure.
    KeysTable<Pkb::LINE_NO, Pkb::FOLLOWS> followTableOnceClosed =
        pkbTables->closeOnce<Pkb::FOLLOW>(followTable);

    Assert::IsTrue(followTableOnceClosed.map[l1] == Pkb::FOLLOWS{l2, l6});
    Assert::IsTrue(followTableOnceClosed.map[l2] == Pkb::FOLLOWS{l6, l7});
    Assert::IsTrue(followTableOnceClosed.map[l3] == Pkb::FOLLOWS{l4});
    Assert::IsTrue(followTableOnceClosed.map[l4] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableOnceClosed.map[l5] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableOnceClosed.map[l6] == Pkb::FOLLOWS{l7});
    Assert::IsTrue(followTableOnceClosed.map[l7] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableOnceClosed.map[l8] == Pkb::FOLLOWS{l9});
    Assert::IsTrue(followTableOnceClosed.map[l9] == Pkb::FOLLOWS());

    // Pkb::close does compute the transitive closure.
    KeysTable<Pkb::LINE_NO, Pkb::FOLLOWS> followTableClosed =
        pkbTables->close<Pkb::FOLLOW>(followTable);

    Assert::IsTrue(followTableClosed.map[l1] == Pkb::FOLLOWS{l2, l6, l7});
    Assert::IsTrue(followTableClosed.map[l2] == Pkb::FOLLOWS{l6, l7});
    Assert::IsTrue(followTableClosed.map[l3] == Pkb::FOLLOWS{l4});
    Assert::IsTrue(followTableClosed.map[l4] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[l5] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[l6] == Pkb::FOLLOWS{l7});
    Assert::IsTrue(followTableClosed.map[l7] == Pkb::FOLLOWS());
    Assert::IsTrue(followTableClosed.map[l8] == Pkb::FOLLOWS{l9});
    Assert::IsTrue(followTableClosed.map[l9] == Pkb::FOLLOWS());

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
    Pkb::LINE_NO l1 = "1";
    Pkb::LINE_NO l2 = "2";
    Pkb::LINE_NO l3 = "3";
    Pkb::LINE_NO l4 = "4";
    Pkb::LINE_NO l5 = "5";
    Pkb::LINE_NO l6 = "6";
    Pkb::LINE_NO l7 = "7";

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

    KeysTable<Pkb::PARENT, std::unordered_set<Pkb::LINE_NO>>
        parentTablePseudoinverted =
            pkbTables->pseudoinvert<Pkb::LINE_NO, Pkb::PARENT>(parentTable);

    Assert::IsTrue(parentTablePseudoinverted.map[l1] == Pkb::CHILDREN{l2});
    Assert::IsTrue(parentTablePseudoinverted.map[l2] == Pkb::CHILDREN{l3, l5});
    Assert::IsTrue(parentTablePseudoinverted.map[l3] == Pkb::CHILDREN{l4});
    Assert::IsTrue(parentTablePseudoinverted.map[l4] == Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[l5] == Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[l6] == Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinverted.map[l7] == Pkb::CHILDREN());

    // Pkb::closeOnce does compute the transitive closure.
    KeysTable<Pkb::LINE_NO, Pkb::PARENTS> parentTableOnceClosed =
        pkbTables->closeOnce<Pkb::PARENT>(parentTable);

    Assert::IsTrue(parentTableOnceClosed.map[l1] == Pkb::PARENTS());
    Assert::IsTrue(parentTableOnceClosed.map[l2] == Pkb::PARENTS{l1});
    Assert::IsTrue(parentTableOnceClosed.map[l3] == Pkb::PARENTS{l2, l1});
    Assert::IsTrue(parentTableOnceClosed.map[l4] == Pkb::PARENTS{l3, l2, l1});
    Assert::IsTrue(parentTableOnceClosed.map[l5] == Pkb::PARENTS{l2, l1});
    Assert::IsTrue(parentTableOnceClosed.map[l6] == Pkb::PARENTS());
    Assert::IsTrue(parentTableOnceClosed.map[l7] == Pkb::PARENTS());

    // Test Pkb::closeFlatten.
    KeysTable<Pkb::PARENT, Pkb::CHILDREN>
        parentTablePseudoinvertedCloseFlattened =
            pkbTables->closeFlatten<Pkb::PARENT>(parentTablePseudoinverted);

    Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l1] ==
                   Pkb::CHILDREN{l2, l3, l5, l4});
    Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l2] ==
                   Pkb::CHILDREN{l3, l5, l4});
    Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l3] ==
                   Pkb::CHILDREN{l4});
    Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l4] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l5] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l6] ==
                   Pkb::CHILDREN());
    Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l7] ==
                   Pkb::CHILDREN());

    // With reversed keys...
    Pkb::PARENT_TABLE parentTableKeysReversed = pkbTables->getParentTable();
    std::reverse(parentTableKeysReversed.keys.begin(),
                 parentTableKeysReversed.keys.end());

    // ... Pkb::closeOnce does not compute the transitive closure.
    KeysTable<Pkb::LINE_NO, Pkb::PARENTS> parentTableKeysReversedOnceClosed =
        pkbTables->closeOnce<Pkb::PARENT>(parentTableKeysReversed);

    Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l1] == Pkb::PARENTS());
    Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l2] ==
                   Pkb::PARENTS{l1});
    Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l3] ==
                   Pkb::PARENTS{l2, l1});
    Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l4] ==
                   Pkb::PARENTS{l3, l2});
    Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l5] ==
                   Pkb::PARENTS{l2, l1});
    Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l6] == Pkb::PARENTS());
    Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l7] == Pkb::PARENTS());

    // ... Pkb::close does compute the transitive closure.
    KeysTable<Pkb::LINE_NO, Pkb::PARENTS> parentTableKeysReversedClosed =
        pkbTables->close<Pkb::PARENT>(parentTableKeysReversed);

    Assert::IsTrue(parentTableKeysReversedClosed.map[l1] == Pkb::PARENTS());
    Assert::IsTrue(parentTableKeysReversedClosed.map[l2] == Pkb::PARENTS{l1});
    Assert::IsTrue(parentTableKeysReversedClosed.map[l3] ==
                   Pkb::PARENTS{l2, l1});
    Assert::IsTrue(parentTableKeysReversedClosed.map[l4] ==
                   Pkb::PARENTS{l3, l2, l1});
    Assert::IsTrue(parentTableKeysReversedClosed.map[l5] ==
                   Pkb::PARENTS{l2, l1});
    Assert::IsTrue(parentTableKeysReversedClosed.map[l6] == Pkb::PARENTS());
    Assert::IsTrue(parentTableKeysReversedClosed.map[l7] == Pkb::PARENTS());

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
    Pkb::PROC_TABLE_INDEX pti0 = pkbTables->addProc(p0);
    Pkb::PROC_TABLE_INDEX pti1 = pkbTables->addProc(p1);
    Pkb::LINE_NO l1 = "1";
    Pkb::LINE_NO l2 = "2";
    Pkb::LINE_NO l3 = "3";
    Pkb::LINE_NO l4 = "4";

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
    Pkb::LINE_NO l1 = "1";
    Pkb::LINE_NO l2 = "2";
    Pkb::LINE_NO l3 = "3";
    Pkb::LINE_NO l4 = "4";
    Pkb::LINE_NO l5 = "5";
    Pkb::LINE_NO l6 = "6";

    pkbTables->addStatementType(l1, Pkb::StatementType::WHILE);
    pkbTables->addStatementType(l2, Pkb::StatementType::IF);
    pkbTables->addStatementType(l3, Pkb::StatementType::ASSIGN);
    pkbTables->addStatementType(l4, Pkb::StatementType::CALL);
    pkbTables->addStatementType(l5, Pkb::StatementType::PRINT);
    pkbTables->addStatementType(l6, Pkb::StatementType::READ);

    Pkb::STATEMENT_TYPE_TABLE statementTypeTable =
        pkbTables->getStatementTypeTable();

    Assert::IsTrue(statementTypeTable.map[l1] == Pkb::StatementType::WHILE);
    Assert::IsTrue(statementTypeTable.map[l2] == Pkb::StatementType::IF);
    Assert::IsTrue(statementTypeTable.map[l3] == Pkb::StatementType::ASSIGN);
    Assert::IsTrue(statementTypeTable.map[l4] == Pkb::StatementType::CALL);
    Assert::IsTrue(statementTypeTable.map[l5] == Pkb::StatementType::PRINT);
    Assert::IsTrue(statementTypeTable.map[l6] == Pkb::StatementType::READ);

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
    Pkb::LINE_NO l1 = "1";
    Pkb::LINE_NO l2 = "2";
    Pkb::LINE_NO l3 = "3";
    Pkb::LINE_NO l4 = "4";
    Pkb::VAR v0 = "a";
    Pkb::VAR v1 = "x";
    Pkb::VAR v2 = "y";
    TNode plus = TNode(TNode::Op::Plus);
    plus.left = new TNode(v1);
    plus.right = new TNode(v2);

    pkbTables->addAssignAst(l1, plus);

    Pkb::ASSIGN_AST_TABLE assignAstTable = pkbTables->getAssignAstTable();

    Assert::IsTrue(assignAstTable.map[l1] == plus);
    Assert::IsTrue(assignAstTable.map[l2] == Pkb::AST());
    Assert::IsTrue(assignAstTable.map[l3] == Pkb::AST());
    Assert::IsTrue(assignAstTable.map[l4] == Pkb::AST());

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

    Pkb::PROC_TABLE_INDEX pti0 = pkbTables->addProc(p0);
    Pkb::PROC_TABLE_INDEX pti1 = pkbTables->addProc(p1);
    Pkb::PROC_TABLE_INDEX pti2 = pkbTables->addProc(p2);
    Pkb::PROC_TABLE_INDEX pti3 = pkbTables->addProc(p3);
    Pkb::PROC_TABLE_INDEX pti4 = pkbTables->addProc(p4);
    Pkb::PROC_TABLE_INDEX pti5 = pkbTables->addProc(p5);

    pkbTables->addCall(pti0, pti1);
    pkbTables->addCall(pti0, pti2);
    pkbTables->addCall(pti0, pti3);
    pkbTables->addCall(pti1, pti2);
    pkbTables->addCall(pti1, pti3);
    pkbTables->addCall(pti2, pti3);
    pkbTables->addCall(pti2, pti4);
    pkbTables->addCall(pti4, pti5);

    Pkb::CALLS_TABLE callsTable = pkbTables->getCallsTable();

    Assert::IsTrue(callsTable.map[pti0] == Pkb::CALLS{pti1, pti2, pti3});
    Assert::IsTrue(callsTable.map[pti1] == Pkb::CALLS{pti2, pti3});
    Assert::IsTrue(callsTable.map[pti2] == Pkb::CALLS{pti3, pti4});
    Assert::IsTrue(callsTable.map[pti3] == Pkb::CALLS());
    Assert::IsTrue(callsTable.map[pti4] == Pkb::CALLS{pti5});
    Assert::IsTrue(callsTable.map[pti5] == Pkb::CALLS());

    KeysTable<Pkb::CALL, Pkb::PROC_TABLE_INDEXES>
        callsTablePseudoinvertKeysFlattened =
            pkbTables
                ->pseudoinvertFlattenKeys<Pkb::PROC_TABLE_INDEX, Pkb::CALL>(
                    callsTable);

    Assert::IsTrue(callsTablePseudoinvertKeysFlattened.map[pti1] ==
                   Pkb::PROC_TABLE_INDEXES{pti0});
    Assert::IsTrue(callsTablePseudoinvertKeysFlattened.map[pti2] ==
                   Pkb::PROC_TABLE_INDEXES{pti0, pti1});
    Assert::IsTrue(callsTablePseudoinvertKeysFlattened.map[pti3] ==
                   Pkb::PROC_TABLE_INDEXES{pti0, pti1, pti2});
    Assert::IsTrue(callsTablePseudoinvertKeysFlattened.map[pti4] ==
                   Pkb::PROC_TABLE_INDEXES{pti2});
    Assert::IsTrue(callsTablePseudoinvertKeysFlattened.map[pti5] ==
                   Pkb::PROC_TABLE_INDEXES{pti4});

    KeysTable<Pkb::PROC_TABLE_INDEX, Pkb::CALLS> callsTableCloseFlattened =
        pkbTables->closeFlatten<Pkb::CALL>(callsTable);

    Assert::IsTrue(callsTableCloseFlattened.map[pti0] ==
                   Pkb::CALLS{pti1, pti2, pti3, pti4, pti5});
    Assert::IsTrue(callsTableCloseFlattened.map[pti1] ==
                   Pkb::CALLS{pti2, pti3, pti4, pti5});
    Assert::IsTrue(callsTableCloseFlattened.map[pti2] ==
                   Pkb::CALLS{pti3, pti4, pti5});
    Assert::IsTrue(callsTableCloseFlattened.map[pti3] == Pkb::CALLS());
    Assert::IsTrue(callsTableCloseFlattened.map[pti4] == Pkb::CALLS{pti5});
    Assert::IsTrue(callsTableCloseFlattened.map[pti5] == Pkb::CALLS());

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
