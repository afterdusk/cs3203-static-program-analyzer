#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestAST){
  public :
      /** @brief Check invariants of fully constructed TNodes. */
      TEST_METHOD(TestMethod1){// "y * z"
                               TNode T1 = TNode("y");
TNode T2 = TNode("z");
TNode T3 = TNode(Op::Times);
T3.left = &T1;
T3.right = &T2;

// "x + y * z"
TNode T4 = TNode("x");
TNode T5 = TNode(Op::Plus);
T5.left = &T4;
T5.right = &T3;

// All leaves have nonempty value.
Assert::IsFalse(T1.value.empty());
Assert::IsFalse(T2.value.empty());
Assert::IsFalse(T4.value.empty());

// All leaves have Op::Unknown op.
Assert::IsTrue(T1.op == Op::Unknown);
Assert::IsTrue(T2.op == Op::Unknown);
Assert::IsTrue(T4.op == Op::Unknown);

// All leaves have uninitialized left and right children.
Assert::IsNull(T1.left);
Assert::IsNull(T1.right);
Assert::IsNull(T2.left);
Assert::IsNull(T2.right);
Assert::IsNull(T4.left);
Assert::IsNull(T4.right);

// All nodes have empty value.
Assert::IsTrue(T3.value.empty());
Assert::IsTrue(T5.value.empty());

// All nodes do not have Op::Unknown op.  Note: assume all TNode::op are created
// with the enum constructor Op::...
Assert::IsTrue(T3.op != Op::Unknown);
Assert::IsTrue(T5.op != Op::Unknown);

// All nodes have initialized left and right children.
Assert::IsNotNull(T3.left);
Assert::IsNotNull(T3.right);
Assert::IsNotNull(T5.left);
Assert::IsNotNull(T5.right);

/* Test subtree relation */

// True by construction.
Assert::IsTrue(T5 >= T4);
Assert::IsTrue(T5 >= T3);
Assert::IsTrue(T3 >= T1);
Assert::IsTrue(T3 >= T2);

// True by reflexivity.
Assert::IsTrue(T4 >= T4);

// True by transitivity.
Assert::IsTrue(T5 >= T1);
Assert::IsTrue(T5 >= T2);

// False by antisymmetry.
Assert::IsFalse(T4 >= T5);
Assert::IsFalse(T3 >= T5);

// False by construction.
Assert::IsFalse(T4 >= T3);
Assert::IsFalse(T4 >= T2);
Assert::IsFalse(T4 >= T1);

} // namespace UnitTesting
}
;

} // namespace UnitTesting
