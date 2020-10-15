#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestTnode){
  public :
      /** @brief Check invariants of fully constructed TNodes. */
      TEST_METHOD(TestMethod1){// "y * z"
                               TNode t1 = TNode("y");
TNode t2 = TNode("z");
TNode t3 = TNode(TNode::Op::Times);
t3.left = std::make_unique<TNode>(t1);
t3.right = std::make_unique<TNode>(t2);

// "x + y * z"
TNode t4 = TNode("x");
TNode t5 = TNode(TNode::Op::Plus);
t5.left = std::make_unique<TNode>(t4);
t5.right = std::make_unique<TNode>(t3);

// t6 is t3.
TNode t6 = TNode(TNode::Op::Times);
t6.left = std::make_unique<TNode>(TNode("y"));
t6.right = std::make_unique<TNode>(TNode("z"));

// All leaves have nonempty value.
Assert::IsFalse(t1.value.empty());
Assert::IsFalse(t2.value.empty());
Assert::IsFalse(t4.value.empty());

// All leaves have TNode::Op::Unknown op.
Assert::IsTrue(t1.op == TNode::Op::Unknown);
Assert::IsTrue(t2.op == TNode::Op::Unknown);
Assert::IsTrue(t4.op == TNode::Op::Unknown);

// [DEPRECATED] All leaves have uninitialized left and right children.

// All nodes have empty value.
Assert::IsTrue(t3.value.empty());
Assert::IsTrue(t5.value.empty());

// All nodes do not have TNode::Op::Unknown op.  Note: assume all TNode::op are
// created with the enum constructor TNode::Op::...
Assert::IsTrue(t3.op != TNode::Op::Unknown);
Assert::IsTrue(t5.op != TNode::Op::Unknown);

// [DEPRECATED] All nodes have initialized left and right children.

/* Test equality relation */
Assert::IsTrue(t5 == t5);
Assert::IsTrue(t4 == t4);
Assert::IsTrue(t3 == t3);
Assert::IsTrue(t2 == t2);
Assert::IsTrue(t1 == t1);

Assert::IsFalse(t5 == t4);
Assert::IsFalse(t5 == t3);
Assert::IsFalse(t5 == t2);
Assert::IsFalse(t5 == t1);

Assert::IsFalse(t4 == t5);
Assert::IsFalse(t4 == t3);
Assert::IsFalse(t4 == t2);
Assert::IsFalse(t4 == t1);

Assert::IsFalse(t3 == t5);
Assert::IsFalse(t3 == t4);
Assert::IsFalse(t3 == t2);
Assert::IsFalse(t3 == t1);

Assert::IsFalse(t2 == t5);
Assert::IsFalse(t2 == t4);
Assert::IsFalse(t2 == t3);
Assert::IsFalse(t2 == t1);

Assert::IsFalse(t1 == t5);
Assert::IsFalse(t1 == t4);
Assert::IsFalse(t1 == t3);
Assert::IsFalse(t1 == t2);

/* Test subtree relation */

// True by construction.
Assert::IsTrue(t5 >= t4);
Assert::IsTrue(t5 >= t3);
Assert::IsTrue(t3 >= t1);
Assert::IsTrue(t3 >= t2);

// True by reflexivity.
Assert::IsTrue(t4 >= t4);

// True by transitivity.
Assert::IsTrue(t5 >= t1);
Assert::IsTrue(t5 >= t2);

// False by antisymmetry.
Assert::IsFalse(t4 >= t5);
Assert::IsFalse(t3 >= t5);

// False by construction.
Assert::IsFalse(t4 >= t3);
Assert::IsFalse(t4 >= t2);
Assert::IsFalse(t4 >= t1);

// Tests replacing t3 with t6.
Assert::IsTrue(t6.value.empty());
Assert::IsTrue(t6.op != TNode::Op::Unknown);
Assert::IsTrue(t5 >= t6);
Assert::IsTrue(t6 >= t1);
Assert::IsTrue(t6 >= t2);
Assert::IsFalse(t6 >= t5);
Assert::IsFalse(t4 >= t6);
Assert::IsTrue(t6 == t6);
Assert::IsFalse(t5 == t6);
Assert::IsFalse(t4 == t6);
Assert::IsFalse(t6 == t5);
Assert::IsFalse(t6 == t4);
Assert::IsFalse(t6 == t2);
Assert::IsFalse(t6 == t1);
Assert::IsFalse(t2 == t6);
Assert::IsFalse(t1 == t6);

} // namespace UnitTesting
}
;

} // namespace UnitTesting
