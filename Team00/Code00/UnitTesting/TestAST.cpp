#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestAST){
  public :
      /** @brief Check invariants of fully constructed TNodes. */
      TEST_METHOD(TestMethod1){// "y * z"
                               TNode *T1 = new TNode("y");
TNode *T2 = new TNode("z");
TNode *T3 = new TNode(TNode::Op::Times);
T3->left = T1;
T3->right = T2;

// "x + y * z"
TNode *T4 = new TNode("x");
TNode *T5 = new TNode(TNode::Op::Plus);
T5->left = T4;
T5->right = T3;

// T6 is T3.
TNode *T6 = new TNode(TNode::Op::Times);
T6->left = new TNode("y");
T6->right = new TNode("z");

// All leaves have nonempty value.
Assert::IsFalse(T1->value.empty());
Assert::IsFalse(T2->value.empty());
Assert::IsFalse(T4->value.empty());

// All leaves have TNode::Op::Unknown op.
Assert::IsTrue(T1->op == TNode::Op::Unknown);
Assert::IsTrue(T2->op == TNode::Op::Unknown);
Assert::IsTrue(T4->op == TNode::Op::Unknown);

// All leaves have uninitialized left and right children.
Assert::IsNull(T1->left);
Assert::IsNull(T1->right);
Assert::IsNull(T2->left);
Assert::IsNull(T2->right);
Assert::IsNull(T4->left);
Assert::IsNull(T4->right);

// All nodes have empty value.
Assert::IsTrue(T3->value.empty());
Assert::IsTrue(T5->value.empty());

// All nodes do not have TNode::Op::Unknown op.  Note: assume all TNode::op are
// created with the enum constructor TNode::Op::...
Assert::IsTrue(T3->op != TNode::Op::Unknown);
Assert::IsTrue(T5->op != TNode::Op::Unknown);

// All nodes have initialized left and right children.
Assert::IsNotNull(T3->left);
Assert::IsNotNull(T3->right);
Assert::IsNotNull(T5->left);
Assert::IsNotNull(T5->right);

/* Test equality relation */
Assert::IsTrue(*T5 == *T5);
Assert::IsTrue(*T4 == *T4);
Assert::IsTrue(*T3 == *T3);
Assert::IsTrue(*T2 == *T2);
Assert::IsTrue(*T1 == *T1);

Assert::IsFalse(*T5 == *T4);
Assert::IsFalse(*T5 == *T3);
Assert::IsFalse(*T5 == *T2);
Assert::IsFalse(*T5 == *T1);

Assert::IsFalse(*T4 == *T5);
Assert::IsFalse(*T4 == *T3);
Assert::IsFalse(*T4 == *T2);
Assert::IsFalse(*T4 == *T1);

Assert::IsFalse(*T3 == *T5);
Assert::IsFalse(*T3 == *T4);
Assert::IsFalse(*T3 == *T2);
Assert::IsFalse(*T3 == *T1);

Assert::IsFalse(*T2 == *T5);
Assert::IsFalse(*T2 == *T4);
Assert::IsFalse(*T2 == *T3);
Assert::IsFalse(*T2 == *T1);

Assert::IsFalse(*T1 == *T5);
Assert::IsFalse(*T1 == *T4);
Assert::IsFalse(*T1 == *T3);
Assert::IsFalse(*T1 == *T2);

/* Test subtree relation */

// True by construction.
Assert::IsTrue(*T5 >= *T4);
Assert::IsTrue(*T5 >= *T3);
Assert::IsTrue(*T3 >= *T1);
Assert::IsTrue(*T3 >= *T2);

// True by reflexivity.
Assert::IsTrue(*T4 >= *T4);

// True by transitivity.
Assert::IsTrue(*T5 >= *T1);
Assert::IsTrue(*T5 >= *T2);

// False by antisymmetry.
Assert::IsFalse(*T4 >= *T5);
Assert::IsFalse(*T3 >= *T5);

// False by construction.
Assert::IsFalse(*T4 >= *T3);
Assert::IsFalse(*T4 >= *T2);
Assert::IsFalse(*T4 >= *T1);

// Tests replacing T3 with T6.
Assert::IsTrue(T6->value.empty());
Assert::IsTrue(T6->op != TNode::Op::Unknown);
Assert::IsNotNull(T6->left);
Assert::IsNotNull(T6->right);
Assert::IsTrue(*T5 >= *T6);
Assert::IsTrue(*T6 >= *T1);
Assert::IsTrue(*T6 >= *T2);
Assert::IsFalse(*T6 >= *T5);
Assert::IsFalse(*T4 >= *T6);
Assert::IsTrue(*T6 == *T6);
Assert::IsFalse(*T5 == *T6);
Assert::IsFalse(*T4 == *T6);
Assert::IsFalse(*T6 == *T5);
Assert::IsFalse(*T6 == *T4);
Assert::IsFalse(*T6 == *T2);
Assert::IsFalse(*T6 == *T1);
Assert::IsFalse(*T2 == *T6);
Assert::IsFalse(*T1 == *T6);

} // namespace UnitTesting
}
;

} // namespace UnitTesting
