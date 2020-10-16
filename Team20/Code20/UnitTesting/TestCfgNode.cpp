#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
/** @brief Check children and parents of fully constructed CfgNodes. */
TEST_CLASS(TestCfgNode){

  public :
      /** @brief If statement with a diamond shape.
    1  if () {
    2
    3
       } else {
    4
    5
       }
    6
    7
    */
      TEST_METHOD(TestIf){
          Pkb::CFG_NODE ifStatement = std::make_shared<CfgNode>(1, 1);
Pkb::CFG_NODE thenBlock = std::make_shared<CfgNode>(2, 3);
Pkb::CFG_NODE elseBlock = std::make_shared<CfgNode>(4, 5);
Pkb::CFG_NODE followsIfStatement = std::make_shared<CfgNode>(6, 7);

CfgNode::addEdge(ifStatement, thenBlock);
CfgNode::addEdge(ifStatement, elseBlock);
CfgNode::addEdge(thenBlock, followsIfStatement);
CfgNode::addEdge(elseBlock, followsIfStatement);

Assert::IsTrue(ifStatement->children ==
               std::unordered_set<std::shared_ptr<CfgNode>>{thenBlock,
                                                            elseBlock});
Assert::IsTrue(thenBlock->children ==
               std::unordered_set<std::shared_ptr<CfgNode>>{
                   followsIfStatement});
Assert::IsTrue(elseBlock->children ==
               std::unordered_set<std::shared_ptr<CfgNode>>{
                   followsIfStatement});
Assert::IsTrue(followsIfStatement->children ==
               std::unordered_set<std::shared_ptr<CfgNode>>{});

Assert::IsTrue(ifStatement->parents ==
               std::unordered_set<std::shared_ptr<CfgNode>>{});
Assert::IsTrue(thenBlock->parents ==
               std::unordered_set<std::shared_ptr<CfgNode>>{ifStatement});
Assert::IsTrue(elseBlock->parents ==
               std::unordered_set<std::shared_ptr<CfgNode>>{ifStatement});
Assert::IsTrue(followsIfStatement->parents ==
               std::unordered_set<std::shared_ptr<CfgNode>>{thenBlock,
                                                            elseBlock});

} // namespace UnitTesting

/** @brief While statement with a loop shape.
1  while () {
2
3
   }
4
5
*/
TEST_METHOD(TestWhile) {
  Pkb::CFG_NODE whileStatement = std::make_shared<CfgNode>(1, 1);
  Pkb::CFG_NODE whileBlock = std::make_shared<CfgNode>(2, 3);
  Pkb::CFG_NODE followsWhileStatement = std::make_shared<CfgNode>(4, 5);

  CfgNode::addEdge(whileStatement, whileBlock);
  CfgNode::addEdge(whileBlock, whileStatement);
  CfgNode::addEdge(whileStatement, followsWhileStatement);

  Assert::IsTrue(whileStatement->children ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{
                     whileBlock, followsWhileStatement});
  Assert::IsTrue(whileBlock->children ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{whileStatement});
  Assert::IsTrue(followsWhileStatement->children ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{});

  Assert::IsTrue(whileStatement->parents ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{whileBlock});
  Assert::IsTrue(whileBlock->parents ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{whileStatement});
  Assert::IsTrue(followsWhileStatement->parents ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{whileStatement});
}

/* Nodes with special properties. */

/** @brief Node with more than two parents.
Here, there are no dummy nodes.
1  if () {
2    if () {
3
   } else {
4
   }
 } else {
5    if () {
6
   } else {
7
   }
 }
8
9
*/
TEST_METHOD(TestMoreThanTwoParents) {
  Pkb::CFG_NODE ifStatement1 = std::make_shared<CfgNode>(1, 1);
  Pkb::CFG_NODE ifStatement2 = std::make_shared<CfgNode>(2, 2);
  Pkb::CFG_NODE thenBlock2 = std::make_shared<CfgNode>(3, 3);
  Pkb::CFG_NODE elseBlock2 = std::make_shared<CfgNode>(4, 4);
  Pkb::CFG_NODE ifStatement3 = std::make_shared<CfgNode>(5, 5);
  Pkb::CFG_NODE thenBlock3 = std::make_shared<CfgNode>(6, 6);
  Pkb::CFG_NODE elseBlock3 = std::make_shared<CfgNode>(7, 7);
  Pkb::CFG_NODE followsIfStatement1 = std::make_shared<CfgNode>(8, 9);

  CfgNode::addEdge(ifStatement1, ifStatement2);
  CfgNode::addEdge(ifStatement1, ifStatement3);

  CfgNode::addEdge(ifStatement2, thenBlock2);
  CfgNode::addEdge(ifStatement2, elseBlock2);
  CfgNode::addEdge(thenBlock2, followsIfStatement1);
  CfgNode::addEdge(elseBlock2, followsIfStatement1);

  CfgNode::addEdge(ifStatement3, thenBlock3);
  CfgNode::addEdge(ifStatement3, elseBlock3);
  CfgNode::addEdge(thenBlock3, followsIfStatement1);
  CfgNode::addEdge(elseBlock3, followsIfStatement1);

  Assert::IsTrue(
      thenBlock2->children ==
      std::unordered_set<std::shared_ptr<CfgNode>>{followsIfStatement1});
  Assert::IsTrue(
      elseBlock2->children ==
      std::unordered_set<std::shared_ptr<CfgNode>>{followsIfStatement1});
  Assert::IsTrue(
      thenBlock3->children ==
      std::unordered_set<std::shared_ptr<CfgNode>>{followsIfStatement1});
  Assert::IsTrue(
      elseBlock3->children ==
      std::unordered_set<std::shared_ptr<CfgNode>>{followsIfStatement1});
  Assert::IsTrue(followsIfStatement1->children ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{});

  // Node with more than two parents.
  Assert::IsTrue(followsIfStatement1->parents ==
                 std::unordered_set<std::shared_ptr<CfgNode>>{
                     thenBlock2, elseBlock2, thenBlock3, elseBlock3});
}
}
; // namespace UnitTesting
} // namespace UnitTesting
