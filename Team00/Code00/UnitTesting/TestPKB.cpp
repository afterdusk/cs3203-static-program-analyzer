#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPKB){public :
                        /** @brief Populate PKB::varTable.
                        Add variables "a", "b", ..., "d", so that "a" has index
                        0, "b" has index 1, ...
                        */
                        TEST_METHOD(TestVarTable){PKB pkb;
VAR v0 = "a";
VAR v1 = "b";
VAR v2 = "c";
VAR v3 = "d";

Assert::IsTrue(pkb.addVar(v0) == 0);
Assert::IsTrue(pkb.addVar(v1) == 1);
Assert::IsTrue(pkb.addVar(v2) == 2);
Assert::IsTrue(pkb.addVar(v3) == 3);

// Adding an existing variable returns the existing index.
Assert::IsTrue(pkb.addVar(v2) == 2);

VAR_TABLE varTable = pkb.getVarTable();

Assert::IsTrue(varTable.map[v0] == 0);
Assert::IsTrue(varTable.map[v1] == 1);
Assert::IsTrue(varTable.map[v2] == 2);
Assert::IsTrue(varTable.map[v3] == 3);

KeysTable<VAR_TABLE_INDEX, VAR> varTableInverted =
    pkb.invert<VAR, VAR_TABLE_INDEX>(varTable);

Assert::IsTrue(varTableInverted.map[0] == v0);
Assert::IsTrue(varTableInverted.map[1] == v1);
Assert::IsTrue(varTableInverted.map[2] == v2);
Assert::IsTrue(varTableInverted.map[3] == v3);

} // namespace UnitTesting

/** @brief Populate PKB::procTable.
Add procedures "a", "b", ..., "d", so that "a" has index
0, "b" has index 1, ...
*/
TEST_METHOD(TestProcTable) {
  PKB pkb;
  PROC p0 = "a";
  PROC p1 = "b";
  PROC p2 = "c";
  PROC p3 = "d";

  Assert::IsTrue(pkb.addProc(p0) == 0);
  Assert::IsTrue(pkb.addProc(p1) == 1);
  Assert::IsTrue(pkb.addProc(p2) == 2);
  Assert::IsTrue(pkb.addProc(p3) == 3);

  PROC_TABLE procTable = pkb.getProcTable();

  Assert::IsTrue(procTable.map[p0] == 0);
  Assert::IsTrue(procTable.map[p1] == 1);
  Assert::IsTrue(procTable.map[p2] == 2);
  Assert::IsTrue(procTable.map[p3] == 3);

  KeysTable<PROC_TABLE_INDEX, PROC> procTableInverted =
      pkb.invert<PROC, PROC_TABLE_INDEX>(procTable);

  Assert::IsTrue(procTableInverted.map[0] == p0);
  Assert::IsTrue(procTableInverted.map[1] == p1);
  Assert::IsTrue(procTableInverted.map[2] == p2);
  Assert::IsTrue(procTableInverted.map[3] == p3);

} // namespace UnitTesting

/** @brief Populate PKB::usesTable.
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
  PKB pkb;

  PROC p0 = "main";
  PROC p1 = "aux";
  PROC_TABLE_INDEX pti0 = pkb.addProc(p0);
  PROC_TABLE_INDEX pti1 = pkb.addProc(p1);
  LINE_NO l1 = 1;
  LINE_NO l2 = 2;
  LINE_NO l3 = 3;
  LINE_NO l4 = 4;
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

  // Line 3:
  VAR_TABLE_INDEXES vtis3 = std::get<VAR_TABLE_INDEXES>(usesTable.map[l3]);
  Assert::IsTrue(vtis3 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti1`
  // key of UsesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti1Vars = vtis3;
  pkb.addUsesProc(pti1, pti1Vars);

  USES_PROC_TABLE usesProcTable = pkb.getUsesProcTable();

  Assert::IsTrue(usesProcTable.map[pti1] == VAR_TABLE_INDEXES{vti3});

  // Lastly, we populate UsesProcTable with the first procedure "main".
  // We populate the `pti0` key of UsesProcTable.

  // Line 1:
  VAR_TABLE_INDEXES vtis1 = std::get<VAR_TABLE_INDEXES>(usesTable.map[l1]);
  Assert::IsTrue(vtis1 == VAR_TABLE_INDEXES{vti1, vti2});

  // Line 2:
  // Now that the `pti1` key of UsesProcTable has been populated, we can soundly
  // call `pkb.getUsesProc`. Note: here we use both UsesTable and UsesProcTable.
  Assert::IsTrue(std::get<PROC_TABLE_INDEX>(usesTable.map[l2]) == pti1);
  VAR_TABLE_INDEXES vtis2 = usesProcTable.map[pti1];
  Assert::IsTrue(vtis2 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti0`
  // key of UsesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti0Vars = vtis1;
  pti0Vars.merge(vtis2);
  pkb.addUsesProc(pti0, pti0Vars);

  usesProcTable = pkb.getUsesProcTable();

  Assert::IsTrue(usesProcTable.map[pti0] ==
                 VAR_TABLE_INDEXES{vti1, vti2, vti3});

  // To finish the unit test, we assert default values.
  Assert::IsTrue(usesTable.map[l4] == USES());
} // namespace UnitTesting

/** @brief Populate PKB::modifiesTable.
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
  PKB pkb;

  PROC p0 = "main";
  PROC p1 = "aux";
  PROC_TABLE_INDEX pti0 = pkb.addProc(p0);
  PROC_TABLE_INDEX pti1 = pkb.addProc(p1);
  LINE_NO l1 = 1;
  LINE_NO l2 = 2;
  LINE_NO l3 = 3;
  LINE_NO l4 = 4;
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

  // Line 4:
  VAR_TABLE_INDEXES vtis4 = std::get<VAR_TABLE_INDEXES>(modifiesTable.map[l4]);
  Assert::IsTrue(vtis4 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti1`
  // key of ModifiesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti1Vars = vtis4;
  pkb.addModifiesProc(pti1, pti1Vars);

  MODIFIES_PROC_TABLE modifiesProcTable = pkb.getModifiesProcTable();

  Assert::IsTrue(modifiesProcTable.map[pti1] == VAR_TABLE_INDEXES{vti3});

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
  VAR_TABLE_INDEXES vtis2 = modifiesProcTable.map[pti1];
  Assert::IsTrue(vtis2 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti0`
  // key of ModifiesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti0Vars = vtis1;
  pti0Vars.merge(vtis2);
  pkb.addModifiesProc(pti0, pti0Vars);

  modifiesProcTable = pkb.getModifiesProcTable();

  Assert::IsTrue(modifiesProcTable.map[pti0] == VAR_TABLE_INDEXES{vti0, vti3});

  // To finish the unit test, we assert default values.
  Assert::IsTrue(modifiesTable.map[l3] == MODIFIES());

} // namespace UnitTesting

/** @brief Populate PKB::followTable.
To be tested: SIMPLE Program:
    procedure main {
1     a = x;
2     if (x == y) {
3       x = y;
4       a = x;
      } else {
5       call aux; }
6     a = z;
    }
    procedure aux {
7     print z;
8     read z;
    }
*/
TEST_METHOD(TestFollowTable) {
  PKB pkb;

  LINE_NO l1 = 1;
  LINE_NO l2 = 2;
  LINE_NO l3 = 3;
  LINE_NO l4 = 4;
  LINE_NO l5 = 5;
  LINE_NO l6 = 6;
  LINE_NO l7 = 7;
  LINE_NO l8 = 8;

  pkb.addFollow(l1, l2);
  pkb.addFollow(l2, l6);
  pkb.addFollow(l3, l4);
  pkb.addFollow(l7, l8);

  FOLLOW_TABLE followTable = pkb.getFollowTable();

  Assert::IsTrue(followTable.map[l1] == l2);
  Assert::IsTrue(followTable.map[l2] == l6);
  Assert::IsTrue(followTable.map[l3] == l4);
  Assert::IsTrue(followTable.map[l4] == FOLLOW());
  Assert::IsTrue(followTable.map[l5] == FOLLOW());
  Assert::IsTrue(followTable.map[l6] == FOLLOW());
  Assert::IsTrue(followTable.map[l7] == l8);
  Assert::IsTrue(followTable.map[l8] == FOLLOW());

  KeysTable<FOLLOW, LINE_NO> followTableInverted =
      pkb.invert<LINE_NO, FOLLOW>(followTable);

  Assert::IsTrue(followTableInverted.map[l1] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l2] == l1);
  Assert::IsTrue(followTableInverted.map[l3] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l4] == l3);
  Assert::IsTrue(followTableInverted.map[l5] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l6] == l2);
  Assert::IsTrue(followTableInverted.map[l7] == LINE_NO());
  Assert::IsTrue(followTableInverted.map[l8] == l7);

  KeysTable<LINE_NO, FOLLOWS> followTableClosed =
      pkb.close<LINE_NO, FOLLOW>(followTable);

  Assert::IsTrue(followTableClosed.map[l1] == FOLLOWS{l2, l6});
  Assert::IsTrue(followTableClosed.map[l2] == FOLLOWS{l6});
  Assert::IsTrue(followTableClosed.map[l3] == FOLLOWS{l4});
  Assert::IsTrue(followTableClosed.map[l4] == FOLLOWS());
  Assert::IsTrue(followTableClosed.map[l5] == FOLLOWS());
  Assert::IsTrue(followTableClosed.map[l6] == FOLLOWS());
  Assert::IsTrue(followTableClosed.map[l7] == FOLLOWS{l8});
  Assert::IsTrue(followTableClosed.map[l8] == FOLLOWS());

} // namespace UnitTesting

/** @brief Populate PKB::parentTable.
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
  PKB pkb;

  LINE_NO l1 = 1;
  LINE_NO l2 = 2;
  LINE_NO l3 = 3;
  LINE_NO l4 = 4;
  LINE_NO l5 = 5;
  LINE_NO l6 = 6;
  LINE_NO l7 = 7;

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

  KeysTable<PARENT, std::vector<LINE_NO>> parentTablePseudoinverted =
      pkb.pseudoinvert<LINE_NO, PARENT>(parentTable);

  Assert::IsTrue(parentTablePseudoinverted.map[l1] == CHILDREN{l2});
  Assert::IsTrue(parentTablePseudoinverted.map[l2] == CHILDREN{l3, l5});
  Assert::IsTrue(parentTablePseudoinverted.map[l3] == CHILDREN{l4});
  Assert::IsTrue(parentTablePseudoinverted.map[l4] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinverted.map[l5] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinverted.map[l6] == CHILDREN());
  Assert::IsTrue(parentTablePseudoinverted.map[l7] == CHILDREN());

  KeysTable<LINE_NO, PARENTS> parentTableClosed =
      pkb.close<LINE_NO, PARENT>(parentTable);

  Assert::IsTrue(parentTableClosed.map[l1] == PARENTS());
  Assert::IsTrue(parentTableClosed.map[l2] == PARENTS{l1});
  Assert::IsTrue(parentTableClosed.map[l3] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableClosed.map[l4] == PARENTS{l3, l2, l1});
  Assert::IsTrue(parentTableClosed.map[l5] == PARENTS{l2, l1});
  Assert::IsTrue(parentTableClosed.map[l6] == PARENTS());
  Assert::IsTrue(parentTableClosed.map[l7] == PARENTS());

} // namespace UnitTesting

/** @brief Populate PKB::statementProcTable.
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
  PKB pkb;

  PROC p0 = "main";
  PROC p1 = "aux";
  PROC_TABLE_INDEX pti0 = pkb.addProc(p0);
  PROC_TABLE_INDEX pti1 = pkb.addProc(p1);
  LINE_NO l1 = 1;
  LINE_NO l2 = 2;
  LINE_NO l3 = 3;
  LINE_NO l4 = 4;

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

/** @brief Populate PKB::statementTypeTable.
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
  PKB pkb;

  LINE_NO l1 = 1;
  LINE_NO l2 = 2;
  LINE_NO l3 = 3;
  LINE_NO l4 = 4;
  LINE_NO l5 = 5;
  LINE_NO l6 = 6;

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

/** @brief Populate PKB::assignAstTable.
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
  PKB pkb;
  LINE_NO l1 = 1;
  LINE_NO l2 = 2;
  LINE_NO l3 = 3;
  LINE_NO l4 = 4;
  VAR v0 = "a";
  VAR v1 = "x";
  VAR v2 = "y";
  TNode plus = TNode(Op::Plus);
  plus.left = &TNode(v1);
  plus.right = &TNode(v2);

  pkb.addAssignAst(l1, plus);

  ASSIGN_AST_TABLE assignAstTable = pkb.getAssignAstTable();

  Assert::IsTrue(assignAstTable.map[l1] == plus);
  Assert::IsTrue(assignAstTable.map[l2] == AST());
  Assert::IsTrue(assignAstTable.map[l3] == AST());
  Assert::IsTrue(assignAstTable.map[l4] == AST());

} // namespace UnitTesting
}
;
}
