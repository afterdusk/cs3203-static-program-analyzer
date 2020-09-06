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

Assert::IsTrue(pkb.getVarTableIndex(v0) == 0);
Assert::IsTrue(pkb.getVarTableIndex(v1) == 1);
Assert::IsTrue(pkb.getVarTableIndex(v2) == 2);
Assert::IsTrue(pkb.getVarTableIndex(v3) == 3);

pkb.invertVarTable();

Assert::IsTrue(pkb.getVar(0) == v0);
Assert::IsTrue(pkb.getVar(1) == v1);
Assert::IsTrue(pkb.getVar(2) == v2);
Assert::IsTrue(pkb.getVar(3) == v3);
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

  Assert::IsTrue(pkb.getProc(0) == p0);
  Assert::IsTrue(pkb.getProc(1) == p1);
  Assert::IsTrue(pkb.getProc(2) == p2);
  Assert::IsTrue(pkb.getProc(3) == p3);
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

  // Line 3:
  VAR_TABLE_INDEXES vtis3 = std::get<VAR_TABLE_INDEXES>(pkb.getUses(l3));
  Assert::IsTrue(vtis3 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti1`
  // key of UsesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti1Vars = vtis3;
  pkb.addUsesProc(pti1, pti1Vars);
  Assert::IsTrue(pkb.getUsesProc(pti1) == VAR_TABLE_INDEXES{vti3});

  // Lastly, we populate UsesProcTable with the first procedure "main".
  // We populate the `pti0` key of UsesProcTable.

  // Line 1:
  VAR_TABLE_INDEXES vtis1 = std::get<VAR_TABLE_INDEXES>(pkb.getUses(l1));
  Assert::IsTrue(vtis1 == VAR_TABLE_INDEXES{vti1, vti2});

  // Line 2:
  // Now that the `pti1` key of UsesProcTable has been populated, we can soundly
  // call `pkb.getUsesProc`. Note: here we use both UsesTable and UsesProcTable.
  Assert::IsTrue(std::get<PROC_TABLE_INDEX>(pkb.getUses(l2)) == pti1);
  VAR_TABLE_INDEXES vtis2 = pkb.getUsesProc(pti1);
  Assert::IsTrue(vtis2 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti0`
  // key of UsesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti0Vars = vtis1;
  pti0Vars.merge(vtis2);
  pkb.addUsesProc(pti0, pti0Vars);
  Assert::IsTrue(pkb.getUsesProc(pti0) == VAR_TABLE_INDEXES{vti1, vti2, vti3});

  // To finish the unit test, we assert default values.
  USES defaultValue = USES();
  Assert::IsTrue(pkb.getUses(4) == defaultValue);

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

  // Line 4:
  VAR_TABLE_INDEXES vtis4 = std::get<VAR_TABLE_INDEXES>(pkb.getModifies(l4));
  Assert::IsTrue(vtis4 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti1`
  // key of ModifiesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti1Vars = vtis4;
  pkb.addModifiesProc(pti1, pti1Vars);
  Assert::IsTrue(pkb.getModifiesProc(pti1) == VAR_TABLE_INDEXES{vti3});

  // Lastly, we populate ModifiesProcTable with the first procedure "main".
  // We populate the `pti0` key of ModifiesProcTable.

  // Line 1:
  VAR_TABLE_INDEXES vtis1 = std::get<VAR_TABLE_INDEXES>(pkb.getModifies(l1));
  Assert::IsTrue(vtis1 == VAR_TABLE_INDEXES{vti0});

  // Line 2:
  // Now that the `pti1` key of ModifiesProcTable has been populated, we can
  // soundly call `pkb.getModifiesProc`. Note: here we use both ModifiesTable
  // and ModifiesProcTable.
  Assert::IsTrue(std::get<PROC_TABLE_INDEX>(pkb.getModifies(l2)) == pti1);
  VAR_TABLE_INDEXES vtis2 = pkb.getModifiesProc(pti1);
  Assert::IsTrue(vtis2 == VAR_TABLE_INDEXES{vti3});

  // There are no more lines in the procedure "aux", so we populate the `pti0`
  // key of ModifiesProcTable with all collected VAR_TABLE_INDEXES.
  VAR_TABLE_INDEXES pti0Vars = vtis1;
  pti0Vars.merge(vtis2);
  pkb.addModifiesProc(pti0, pti0Vars);
  Assert::IsTrue(pkb.getModifiesProc(pti0) == VAR_TABLE_INDEXES{vti0, vti3});

  // To finish the unit test, we assert default values.
  MODIFIES defaultValue = MODIFIES();
  Assert::IsTrue(pkb.getModifies(3) == defaultValue);

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

  FOLLOW defaultValue = FOLLOW();
  Assert::IsTrue(pkb.getFollow(l1) == l2);
  Assert::IsTrue(pkb.getFollow(l2) == l6);
  Assert::IsTrue(pkb.getFollow(l3) == l4);
  Assert::IsTrue(pkb.getFollow(l4) == defaultValue);
  Assert::IsTrue(pkb.getFollow(l5) == defaultValue);
  Assert::IsTrue(pkb.getFollow(l6) == defaultValue);
  Assert::IsTrue(pkb.getFollow(l7) == l8);
  Assert::IsTrue(pkb.getFollow(l8) == defaultValue);

  pkb.invertFollowTable();

  Assert::IsTrue(pkb.getFollowLineNo(l1) == defaultValue);
  Assert::IsTrue(pkb.getFollowLineNo(l2) == l1);
  Assert::IsTrue(pkb.getFollowLineNo(l3) == defaultValue);
  Assert::IsTrue(pkb.getFollowLineNo(l4) == l3);
  Assert::IsTrue(pkb.getFollowLineNo(l5) == defaultValue);
  Assert::IsTrue(pkb.getFollowLineNo(l6) == l2);
  Assert::IsTrue(pkb.getFollowLineNo(l7) == defaultValue);
  Assert::IsTrue(pkb.getFollowLineNo(l8) == l7);

  pkb.closeFollowTable();

  FOLLOWS followsDefaultValue = FOLLOWS();
  Assert::IsTrue(pkb.getFollowStar(l1) == FOLLOWS{l2, l6});
  Assert::IsTrue(pkb.getFollowStar(l2) == FOLLOWS{l6});
  Assert::IsTrue(pkb.getFollowStar(l3) == FOLLOWS{l4});
  Assert::IsTrue(pkb.getFollowStar(l4) == followsDefaultValue);
  Assert::IsTrue(pkb.getFollowStar(l5) == followsDefaultValue);
  Assert::IsTrue(pkb.getFollowStar(l6) == followsDefaultValue);
  Assert::IsTrue(pkb.getFollowStar(l7) == FOLLOWS{l8});
  Assert::IsTrue(pkb.getFollowStar(l8) == followsDefaultValue);

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

  PARENT defaultValue = PARENT();
  Assert::IsTrue(pkb.getParent(l1) == defaultValue);
  Assert::IsTrue(pkb.getParent(l2) == l1);
  Assert::IsTrue(pkb.getParent(l3) == l2);
  Assert::IsTrue(pkb.getParent(l4) == l3);
  Assert::IsTrue(pkb.getParent(l5) == l2);
  Assert::IsTrue(pkb.getParent(l6) == defaultValue);
  Assert::IsTrue(pkb.getParent(l7) == defaultValue);

  pkb.pseudoInvertParentTable();

  CHILDREN childrenDefaultValue = CHILDREN();
  Assert::IsTrue(pkb.getParentChildren(l1) == CHILDREN{l2});
  Assert::IsTrue(pkb.getParentChildren(l2) == CHILDREN{l3, l5});
  Assert::IsTrue(pkb.getParentChildren(l3) == CHILDREN{l4});
  Assert::IsTrue(pkb.getParentChildren(l4) == childrenDefaultValue);
  Assert::IsTrue(pkb.getParentChildren(l5) == childrenDefaultValue);
  Assert::IsTrue(pkb.getParentChildren(l6) == childrenDefaultValue);
  Assert::IsTrue(pkb.getParentChildren(l7) == childrenDefaultValue);

  pkb.closeParentTable();

  PARENTS parentsDefaultValue = PARENTS();
  Assert::IsTrue(pkb.getParentStar(l1) == parentsDefaultValue);
  Assert::IsTrue(pkb.getParentStar(l2) == PARENTS{l1});
  Assert::IsTrue(pkb.getParentStar(l3) == PARENTS{l2, l1});
  Assert::IsTrue(pkb.getParentStar(l4) == PARENTS{l3, l2, l1});
  Assert::IsTrue(pkb.getParentStar(l5) == PARENTS{l2, l1});
  Assert::IsTrue(pkb.getParentStar(l6) == parentsDefaultValue);
  Assert::IsTrue(pkb.getParentStar(l7) == parentsDefaultValue);

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

  Assert::IsTrue(pkb.getStatementProc(l1) == p0);
  Assert::IsTrue(pkb.getStatementProc(l2) == p0);
  Assert::IsTrue(pkb.getStatementProc(l3) == p1);
  Assert::IsTrue(pkb.getStatementProc(l4) == p1);

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

  Assert::IsTrue(pkb.getStatementType(l1) == StatementType::WHILE);
  Assert::IsTrue(pkb.getStatementType(l2) == StatementType::IF);
  Assert::IsTrue(pkb.getStatementType(l3) == StatementType::ASSIGN);
  Assert::IsTrue(pkb.getStatementType(l4) == StatementType::CALL);
  Assert::IsTrue(pkb.getStatementType(l5) == StatementType::PRINT);
  Assert::IsTrue(pkb.getStatementType(l6) == StatementType::READ);

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

  AST defaultValue = AST();
  Assert::IsTrue(pkb.getAssignAst(l1) == plus);
  Assert::IsTrue(pkb.getAssignAst(l2) == defaultValue);
  Assert::IsTrue(pkb.getAssignAst(l3) == defaultValue);
  Assert::IsTrue(pkb.getAssignAst(l4) == defaultValue);

} // namespace UnitTesting
}
;
}
