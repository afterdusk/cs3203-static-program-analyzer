#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPkb){public :
                        /** @brief Populate Pkb::varTable.
                        Add variables "a", "b", ..., "d", so that "a" has index
                        0, "b" has index 1, ...
                        */
                        TEST_METHOD(TestVarTable){Pkb pkb;
VAR v0 = "a";
VAR v1 = "b";
VAR v2 = "c";
VAR v3 = "d";
VAR v4 = "bad";

Assert::IsTrue(pkb.addVar(v0) == 1);
Assert::IsTrue(pkb.addVar(v1) == 2);
Assert::IsTrue(pkb.addVar(v2) == 3);
Assert::IsTrue(pkb.addVar(v3) == 4);

// Adding an existing variable returns the existing index.
Assert::IsTrue(pkb.addVar(v2) == 3);

VAR_TABLE varTable = pkb.getVarTable();

Assert::IsTrue(varTable.map[v0] == 1);
Assert::IsTrue(varTable.map[v1] == 2);
Assert::IsTrue(varTable.map[v2] == 3);
Assert::IsTrue(varTable.map[v3] == 4);

// Getting the value of an unmapped key returns the default constructor.
Assert::IsTrue(varTable.map[v4] == VAR_TABLE_INDEX());

KeysTable<VAR_TABLE_INDEX, VAR> varTableInverted =
    pkb.invert<VAR, VAR_TABLE_INDEX>(varTable);

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
  Pkb pkb;
  PROC p0 = "a";
  PROC p1 = "b";
  PROC p2 = "c";
  PROC p3 = "d";
  PROC p4 = "bad";

  Assert::IsTrue(pkb.addProc(p0) == 1);
  Assert::IsTrue(pkb.addProc(p1) == 2);
  Assert::IsTrue(pkb.addProc(p2) == 3);
  Assert::IsTrue(pkb.addProc(p3) == 4);

  PROC_TABLE procTable = pkb.getProcTable();

  Assert::IsTrue(procTable.map[p0] == 1);
  Assert::IsTrue(procTable.map[p1] == 2);
  Assert::IsTrue(procTable.map[p2] == 3);
  Assert::IsTrue(procTable.map[p3] == 4);

  // Getting the value of an unmapped key returns the default constructor.
  Assert::IsTrue(procTable.map[p4] == PROC_TABLE_INDEX());

  KeysTable<PROC_TABLE_INDEX, PROC> procTableInverted =
      pkb.invert<PROC, PROC_TABLE_INDEX>(procTable);

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
  Pkb pkb;

  PROC p0 = "main";
  PROC p1 = "aux";
  PROC_TABLE_INDEX pti0 = pkb.addProc(p0);
  PROC_TABLE_INDEX pti1 = pkb.addProc(p1);
  LINE_NO l1 = "1";
  LINE_NO l2 = "2";
  LINE_NO l3 = "3";
  LINE_NO l4 = "4";
  VAR v0 = "a";
  VAR v1 = "x";
  VAR v2 = "y";
  VAR v3 = "z";

  VAR_TABLE_INDEX vti0 = pkb.addVar(v0);
  VAR_TABLE_INDEX vti1 = pkb.addVar(v1);
  VAR_TABLE_INDEX vti2 = pkb.addVar(v2);
  VAR_TABLE_INDEX vti3 = pkb.addVar(v3);

  /* UsesTable ******/
  // UsesTable may be populated in any order.
  pkb.addUses(l1, VAR_TABLE_INDEXES{vti1, vti2});
  pkb.addUses(l2, pti1);
  pkb.addUses(l3, VAR_TABLE_INDEXES{vti3});

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

  USES_TABLE usesTable = pkb.getUsesTable();
  const USES_PROC_TABLE &usesProcTable = pkb.getUsesProcTable();

  // Line 3:
  VAR_TABLE_INDEXES vtis3 = std::get<VAR_TABLE_INDEXES>(usesTable.map[l3]);
  Assert::IsTrue(vtis3 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti1`
  // key of UsesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti1Vars = vtis3;
  pkb.addUsesProc(pti1, pti1Vars);

  Assert::IsTrue(usesProcTable.map.at(pti1) == VAR_TABLE_INDEXES{vti3});

  // Lastly, we populate UsesProcTable with the first procedure "main".
  // We populate the `pti0` key of UsesProcTable.

  // Line 1:
  VAR_TABLE_INDEXES vtis1 = std::get<VAR_TABLE_INDEXES>(usesTable.map[l1]);
  Assert::IsTrue(vtis1 == VAR_TABLE_INDEXES{vti1, vti2});

  // Line 2:
  // Now that the `pti1` key of UsesProcTable has been populated, we can soundly
  // call `pkb.getUsesProc`. Note: here we use both UsesTable and UsesProcTable.
  Assert::IsTrue(std::get<PROC_TABLE_INDEX>(usesTable.map[l2]) == pti1);
  VAR_TABLE_INDEXES vtis2 = usesProcTable.map.at(pti1);
  Assert::IsTrue(vtis2 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti0`
  // key of UsesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti0Vars = vtis1;
  pti0Vars.merge(vtis2);
  pkb.addUsesProc(pti0, pti0Vars);

  Assert::IsTrue(usesProcTable.map.at(pti0) ==
                 VAR_TABLE_INDEXES{vti1, vti2, vti3});

  // We assert default values.
  Assert::IsTrue(usesTable.map[l4] == USES());

  // We test Pkb::transit.
  KeysTable<LINE_NO, VAR_TABLE_INDEXES> usesTableTransited =
      pkb.transit(usesTable, usesProcTable);
  Assert::IsTrue(usesTableTransited.map[l1] == VAR_TABLE_INDEXES{vti1, vti2});
  Assert::IsTrue(usesTableTransited.map[l2] == VAR_TABLE_INDEXES{vti3});
  Assert::IsTrue(usesTableTransited.map[l3] == VAR_TABLE_INDEXES{vti3});
  Assert::IsTrue(usesTableTransited.map[l4] == VAR_TABLE_INDEXES());

  // We test Pkb::pseudoinvertFlattenKeys.
  KeysTable<VAR_TABLE_INDEX, std::unordered_set<LINE_NO>>
      usesTableTransitedPseudoinvertedKeysFlattened =
          pkb.pseudoinvertFlattenKeys<LINE_NO, VAR_TABLE_INDEX>(
              usesTableTransited);
  Assert::IsTrue(usesTableTransitedPseudoinvertedKeysFlattened.map[vti0] ==
                 std::unordered_set<LINE_NO>());
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
  Pkb pkb;

  PROC p0 = "main";
  PROC p1 = "aux";
  PROC_TABLE_INDEX pti0 = pkb.addProc(p0);
  PROC_TABLE_INDEX pti1 = pkb.addProc(p1);
  LINE_NO l1 = "1";
  LINE_NO l2 = "2";
  LINE_NO l3 = "3";
  LINE_NO l4 = "4";
  VAR v0 = "a";
  VAR v1 = "x";
  VAR v2 = "y";
  VAR v3 = "z";

  VAR_TABLE_INDEX vti0 = pkb.addVar(v0);
  VAR_TABLE_INDEX vti1 = pkb.addVar(v1);
  VAR_TABLE_INDEX vti2 = pkb.addVar(v2);
  VAR_TABLE_INDEX vti3 = pkb.addVar(v3);

  /* ModifiesTable ******/
  // ModifiesTable may be populated in any order.
  pkb.addModifies(l1, VAR_TABLE_INDEXES{vti0});
  pkb.addModifies(l2, pti1);
  pkb.addModifies(l4, VAR_TABLE_INDEXES{vti3});

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

  MODIFIES_TABLE modifiesTable = pkb.getModifiesTable();
  const MODIFIES_PROC_TABLE &modifiesProcTable = pkb.getModifiesProcTable();

  // Line 4:
  VAR_TABLE_INDEXES vtis4 = std::get<VAR_TABLE_INDEXES>(modifiesTable.map[l4]);
  Assert::IsTrue(vtis4 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti1`
  // key of ModifiesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti1Vars = vtis4;
  pkb.addModifiesProc(pti1, pti1Vars);

  Assert::IsTrue(modifiesProcTable.map.at(pti1) == VAR_TABLE_INDEXES{vti3});

  // Lastly, we populate ModifiesProcTable with the first procedure "main".
  // We populate the `pti0` key of ModifiesProcTable.

  // Line 1:
  VAR_TABLE_INDEXES vtis1 = std::get<VAR_TABLE_INDEXES>(modifiesTable.map[l1]);
  Assert::IsTrue(vtis1 == VAR_TABLE_INDEXES{vti0});

  // Line 2:
  // Now that the `pti1` key of ModifiesProcTable has been populated, we can
  // soundly call `pkb.getModifiesProc`. Note: here we use both ModifiesTable
  // and ModifiesProcTable.
  Assert::IsTrue(std::get<PROC_TABLE_INDEX>(modifiesTable.map[l2]) == pti1);
  VAR_TABLE_INDEXES vtis2 = modifiesProcTable.map.at(pti1);
  Assert::IsTrue(vtis2 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti0`
  // key of ModifiesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti0Vars = vtis1;
  pti0Vars.merge(vtis2);
  pkb.addModifiesProc(pti0, pti0Vars);

  Assert::IsTrue(modifiesProcTable.map.at(pti0) ==
                 VAR_TABLE_INDEXES{vti0, vti3});

  // We assert default values.
  Assert::IsTrue(modifiesTable.map[l3] == MODIFIES());

  // We test Pkb::transit.
  KeysTable<LINE_NO, VAR_TABLE_INDEXES> modifiesTableTransited =
      pkb.transit(modifiesTable, modifiesProcTable);
  Assert::IsTrue(modifiesTableTransited.map[l1] == VAR_TABLE_INDEXES{vti0});
  Assert::IsTrue(modifiesTableTransited.map[l2] == VAR_TABLE_INDEXES{vti3});
  Assert::IsTrue(modifiesTableTransited.map[l3] == VAR_TABLE_INDEXES());
  Assert::IsTrue(modifiesTableTransited.map[l4] == VAR_TABLE_INDEXES{vti3});

  // We test Pkb::pseudoinvertFlattenKeys.
  KeysTable<VAR_TABLE_INDEX, std::unordered_set<LINE_NO>>
      modifiesTableTransitedPseudoinvertedKeysFlattened =
          pkb.pseudoinvertFlattenKeys<LINE_NO, VAR_TABLE_INDEX>(
              modifiesTableTransited);
  Assert::IsTrue(modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti0] ==
                 std::unordered_set{l1});
  Assert::IsTrue(modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti1] ==
                 std::unordered_set<LINE_NO>());
  Assert::IsTrue(modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti2] ==
                 std::unordered_set<LINE_NO>());
  Assert::IsTrue(modifiesTableTransitedPseudoinvertedKeysFlattened.map[vti3] ==
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
  Pkb pkb;

  LINE_NO l1 = "1";
  LINE_NO l2 = "2";
  LINE_NO l3 = "3";
  LINE_NO l4 = "4";
  LINE_NO l5 = "5";
  LINE_NO l6 = "6";
  LINE_NO l7 = "7";
  LINE_NO l8 = "8";
  LINE_NO l9 = "9";

  pkb.addFollow(l1, l2);
  pkb.addFollow(l2, l6);
  pkb.addFollow(l3, l4);
  pkb.addFollow(l6, l7);
  pkb.addFollow(l8, l9);

  FOLLOW_TABLE followTable = pkb.getFollowTable();

  Assert::IsTrue(followTable.map[l1] == l2);
  Assert::IsTrue(followTable.map[l2] == l6);
  Assert::IsTrue(followTable.map[l3] == l4);
  Assert::IsTrue(followTable.map[l4] == FOLLOW());
  Assert::IsTrue(followTable.map[l5] == FOLLOW());
  Assert::IsTrue(followTable.map[l6] == l7);
  Assert::IsTrue(followTable.map[l7] == FOLLOW());
  Assert::IsTrue(followTable.map[l8] == l9);
  Assert::IsTrue(followTable.map[l9] == FOLLOW());

  KeysTable<FOLLOW, LINE_NO> followTableInverted =
      pkb.invert<LINE_NO, FOLLOW>(followTable);

  Assert::IsTrue(followTableInverted.map[l1] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l2] == l1);
  Assert::IsTrue(followTableInverted.map[l3] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l4] == l3);
  Assert::IsTrue(followTableInverted.map[l5] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l6] == l2);
  Assert::IsTrue(followTableInverted.map[l7] == l6);
  Assert::IsTrue(followTableInverted.map[l8] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l9] == l8);

  // Pkb::closeOnce does not compute the transitive closure.
  KeysTable<LINE_NO, FOLLOWS> followTableOnceClosed =
      pkb.closeOnce<FOLLOW>(followTable);

  Assert::IsTrue(followTableOnceClosed.map[l1] == FOLLOWS{l2, l6});
  Assert::IsTrue(followTableOnceClosed.map[l2] == FOLLOWS{l6, l7});
  Assert::IsTrue(followTableOnceClosed.map[l3] == FOLLOWS{l4});
  Assert::IsTrue(followTableOnceClosed.map[l4] == FOLLOWS());
  Assert::IsTrue(followTableOnceClosed.map[l5] == FOLLOWS());
  Assert::IsTrue(followTableOnceClosed.map[l6] == FOLLOWS{l7});
  Assert::IsTrue(followTableOnceClosed.map[l7] == FOLLOWS());
  Assert::IsTrue(followTableOnceClosed.map[l8] == FOLLOWS{l9});
  Assert::IsTrue(followTableOnceClosed.map[l9] == FOLLOWS());

  // Pkb::close does compute the transitive closure.
  KeysTable<LINE_NO, FOLLOWS> followTableClosed =
      pkb.close<FOLLOW>(followTable);

  Assert::IsTrue(followTableClosed.map[l1] == FOLLOWS{l2, l6, l7});
  Assert::IsTrue(followTableClosed.map[l2] == FOLLOWS{l6, l7});
  Assert::IsTrue(followTableClosed.map[l3] == FOLLOWS{l4});
  Assert::IsTrue(followTableClosed.map[l4] == FOLLOWS());
  Assert::IsTrue(followTableClosed.map[l5] == FOLLOWS());
  Assert::IsTrue(followTableClosed.map[l6] == FOLLOWS{l7});
  Assert::IsTrue(followTableClosed.map[l7] == FOLLOWS());
  Assert::IsTrue(followTableClosed.map[l8] == FOLLOWS{l9});
  Assert::IsTrue(followTableClosed.map[l9] == FOLLOWS());

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
  Pkb pkb;

  LINE_NO l1 = "1";
  LINE_NO l2 = "2";
  LINE_NO l3 = "3";
  LINE_NO l4 = "4";
  LINE_NO l5 = "5";
  LINE_NO l6 = "6";
  LINE_NO l7 = "7";

  pkb.addParent(l2, l1);
  pkb.addParent(l3, l2);
  pkb.addParent(l4, l3);
  pkb.addParent(l5, l2);

  PARENT_TABLE parentTable = pkb.getParentTable();

  Assert::IsTrue(parentTable.map[l1] == PARENT());
  Assert::IsTrue(parentTable.map[l2] == l1);
  Assert::IsTrue(parentTable.map[l3] == l2);
  Assert::IsTrue(parentTable.map[l4] == l3);
  Assert::IsTrue(parentTable.map[l5] == l2);
  Assert::IsTrue(parentTable.map[l6] == PARENT());
  Assert::IsTrue(parentTable.map[l7] == PARENT());

  KeysTable<PARENT, std::unordered_set<LINE_NO>> parentTablePseudoinverted =
      pkb.pseudoinvert<LINE_NO, PARENT>(parentTable);

  Assert::IsTrue(parentTablePseudoinverted.map[l1] == CHILDREN{l2});
  Assert::IsTrue(parentTablePseudoinverted.map[l2] == CHILDREN{l3, l5});
  Assert::IsTrue(parentTablePseudoinverted.map[l3] == CHILDREN{l4});
  Assert::IsTrue(parentTablePseudoinverted.map[l4] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinverted.map[l5] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinverted.map[l6] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinverted.map[l7] == CHILDREN());

  // Pkb::closeOnce does compute the transitive closure.
  KeysTable<LINE_NO, PARENTS> parentTableOnceClosed =
      pkb.closeOnce<PARENT>(parentTable);

  Assert::IsTrue(parentTableOnceClosed.map[l1] == PARENTS());
  Assert::IsTrue(parentTableOnceClosed.map[l2] == PARENTS{l1});
  Assert::IsTrue(parentTableOnceClosed.map[l3] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableOnceClosed.map[l4] == PARENTS{l3, l2, l1});
  Assert::IsTrue(parentTableOnceClosed.map[l5] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableOnceClosed.map[l6] == PARENTS());
  Assert::IsTrue(parentTableOnceClosed.map[l7] == PARENTS());

  // Test Pkb::closeFlatten.
  KeysTable<PARENT, CHILDREN> parentTablePseudoinvertedCloseFlattened =
      pkb.closeFlatten<PARENT>(parentTablePseudoinverted);

  Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l1] ==
                 CHILDREN{l2, l3, l5, l4});
  Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l2] ==
                 CHILDREN{l3, l5, l4});
  Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l3] ==
                 CHILDREN{l4});
  Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l4] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l5] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l6] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinvertedCloseFlattened.map[l7] == CHILDREN());

  // With reversed keys...
  PARENT_TABLE parentTableKeysReversed = pkb.getParentTable();
  std::reverse(parentTableKeysReversed.keys.begin(),
               parentTableKeysReversed.keys.end());

  // ... Pkb::closeOnce does not compute the transitive closure.
  KeysTable<LINE_NO, PARENTS> parentTableKeysReversedOnceClosed =
      pkb.closeOnce<PARENT>(parentTableKeysReversed);

  Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l1] == PARENTS());
  Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l2] == PARENTS{l1});
  Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l3] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l4] == PARENTS{l3, l2});
  Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l5] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l6] == PARENTS());
  Assert::IsTrue(parentTableKeysReversedOnceClosed.map[l7] == PARENTS());

  // ... Pkb::close does compute the transitive closure.
  KeysTable<LINE_NO, PARENTS> parentTableKeysReversedClosed =
      pkb.close<PARENT>(parentTableKeysReversed);

  Assert::IsTrue(parentTableKeysReversedClosed.map[l1] == PARENTS());
  Assert::IsTrue(parentTableKeysReversedClosed.map[l2] == PARENTS{l1});
  Assert::IsTrue(parentTableKeysReversedClosed.map[l3] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableKeysReversedClosed.map[l4] == PARENTS{l3, l2, l1});
  Assert::IsTrue(parentTableKeysReversedClosed.map[l5] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableKeysReversedClosed.map[l6] == PARENTS());
  Assert::IsTrue(parentTableKeysReversedClosed.map[l7] == PARENTS());

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
  Pkb pkb;

  PROC p0 = "main";
  PROC p1 = "aux";
  PROC_TABLE_INDEX pti0 = pkb.addProc(p0);
  PROC_TABLE_INDEX pti1 = pkb.addProc(p1);
  LINE_NO l1 = "1";
  LINE_NO l2 = "2";
  LINE_NO l3 = "3";
  LINE_NO l4 = "4";

  pkb.addStatementProc(l1, p0);
  pkb.addStatementProc(l2, p0);
  pkb.addStatementProc(l3, p1);
  pkb.addStatementProc(l4, p1);

  STATEMENT_PROC_TABLE statementProcTable = pkb.getStatementProcTable();

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
  Pkb pkb;

  LINE_NO l1 = "1";
  LINE_NO l2 = "2";
  LINE_NO l3 = "3";
  LINE_NO l4 = "4";
  LINE_NO l5 = "5";
  LINE_NO l6 = "6";

  pkb.addStatementType(l1, StatementType::WHILE);
  pkb.addStatementType(l2, StatementType::IF);
  pkb.addStatementType(l3, StatementType::ASSIGN);
  pkb.addStatementType(l4, StatementType::CALL);
  pkb.addStatementType(l5, StatementType::PRINT);
  pkb.addStatementType(l6, StatementType::READ);

  STATEMENT_TYPE_TABLE statementTypeTable = pkb.getStatementTypeTable();

  Assert::IsTrue(statementTypeTable.map[l1] == StatementType::WHILE);
  Assert::IsTrue(statementTypeTable.map[l2] == StatementType::IF);
  Assert::IsTrue(statementTypeTable.map[l3] == StatementType::ASSIGN);
  Assert::IsTrue(statementTypeTable.map[l4] == StatementType::CALL);
  Assert::IsTrue(statementTypeTable.map[l5] == StatementType::PRINT);
  Assert::IsTrue(statementTypeTable.map[l6] == StatementType::READ);

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
  Pkb pkb;
  LINE_NO l1 = "1";
  LINE_NO l2 = "2";
  LINE_NO l3 = "3";
  LINE_NO l4 = "4";
  VAR v0 = "a";
  VAR v1 = "x";
  VAR v2 = "y";
  TNode plus = TNode(Op::Plus);
  plus.left = new TNode(v1);
  plus.right = new TNode(v2);

  pkb.addAssignAst(l1, plus);

  ASSIGN_AST_TABLE assignAstTable = pkb.getAssignAstTable();

  Assert::IsTrue(assignAstTable.map[l1] == plus);
  Assert::IsTrue(assignAstTable.map[l2] == AST());
  Assert::IsTrue(assignAstTable.map[l3] == AST());
  Assert::IsTrue(assignAstTable.map[l4] == AST());

} // namespace UnitTesting

/** @brief Populate Pkb::constantTable. */
TEST_METHOD(TestConstantTable) {
  Pkb pkb;
  CONSTANT c0 = "-2";
  CONSTANT c1 = "-1";
  CONSTANT c2 = "0";
  CONSTANT c3 = "1";

  pkb.addConstant(c0);
  pkb.addConstant(c1);
  pkb.addConstant(c2);
  pkb.addConstant(c3);

  CONSTANT_TABLE constantTable = pkb.getConstantTable();

  Assert::IsTrue(constantTable == CONSTANT_TABLE{c0, c1, c2, c3});

} // namespace UnitTesting
}
;
}
