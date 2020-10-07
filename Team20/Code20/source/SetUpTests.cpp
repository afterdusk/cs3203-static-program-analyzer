#include "SetUpTests.h"

SetUpTests::SetUpTests(Pkb &pkb, TestNumber testNumber) {
  pkbTables = pkb.getTables();

  if (testNumber == TestNumber::A) {
    /* Example
    procedure aux {
    1	read x;
    2	read y;
    3	while (y != 0) {
    4		y = y / x;
    5		read y; }
    6	print x;}
    procedure main {
    7	call aux;
    8	read x;
    9   read r;
    10  m = x + r;
    11  call complicate;}
    procedure complicate {
    12	read q;
    13	read t;
    14	while (q < t) {
    15		if (t % 2 == 0) {
    16			t = t - 1;
    17			while (t > q + 1) {
    18				t = t - 1;}
    19			if (q % 2 == 0) {
    20				q = q - 1;}
                else {
    21              print q;}}
            else {
    22          print t;}
    23		while (q % 2 == 1) {
    24			q = q - 1;}}
    25	call extra;}
    procedure extra {
    26	print k;}
    procedure extratwo {
    27  call extra;
    28  call aux;}
    procedure extrathree {
    29  call main;
    30  call extratwo;}
    procedure extrafour {
    31  call extrathree;
    32  call main;
    33  call extrathree;}
    */

    w.type = Pkb::StatementType::While;
    r.type = Pkb::StatementType::Read;
    a.type = Pkb::StatementType::Assign;
    c.type = Pkb::StatementType::Call;
    i.type = Pkb::StatementType::If;
    p.type = Pkb::StatementType::Print;
    stmt.type = Pkb::StatementType::None;

    stry.name = "y";
    strx.name = "x";
    strr.name = "r";
    strq.name = "q";
    strt.name = "t";
    strk.name = "k";
    strm.name = "m";

    strpmain.name = "main";
    strpextra.name = "extra";
    strpcomplicate.name = "complicate";
    strpaux.name = "aux";
    strpextratwo.name = "extratwo";
    strpextrathree.name = "extrathree";
    strpextrafour.name = "extrafour";

    l1 = 1;
    l2 = 2;
    l3 = 3;
    l4 = 4;
    l5 = 5;
    l6 = 6;
    l7 = 7;
    l8 = 8;
    l9 = 9;
    l10 = 10;
    l11 = 11;
    l12 = 12;
    l13 = 13;
    l14 = 14;
    l15 = 15;
    l16 = 16;
    l17 = 17;
    l18 = 18;
    l19 = 19;
    l20 = 20;
    l21 = 21;
    l22 = 22;
    l23 = 23;
    l24 = 24;
    l25 = 25;
    l26 = 26;
    l27 = 27;
    l28 = 28;
    l29 = 29;
    l30 = 30;
    l31 = 31;
    l32 = 32;
    l33 = 33;
    l999 = 999;

    c1.number = l1;
    c2.number = l2;
    c3.number = l3;
    c4.number = l4;
    c5.number = l5;
    c6.number = l6;
    c7.number = l7;
    c8.number = l8;
    c9.number = l9;
    c10.number = l10;
    c11.number = l11;
    c12.number = l12;
    c13.number = l13;
    c14.number = l14;
    c15.number = l15;
    c16.number = l16;
    c17.number = l17;
    c18.number = l18;
    c19.number = l19;
    c20.number = l20;
    c21.number = l21;
    c22.number = l22;
    c23.number = l23;
    c24.number = l24;
    c25.number = l25;
    c26.number = l26;
    c999.number = l999;

    pkbTables->addFollow(l1, l2);
    pkbTables->addFollow(l2, l3);
    pkbTables->addFollow(l3, l6);
    pkbTables->addFollow(l4, l5);
    pkbTables->addFollow(l7, l8);
    pkbTables->addFollow(l8, l9);
    pkbTables->addFollow(l9, l10);
    pkbTables->addFollow(l10, l11);
    pkbTables->addFollow(l12, l13);
    pkbTables->addFollow(l13, l14);
    pkbTables->addFollow(l14, l25);
    pkbTables->addFollow(l15, l23);
    pkbTables->addFollow(l16, l17);
    pkbTables->addFollow(l17, l19);

    pkbTables->addParent(l4, l3);
    pkbTables->addParent(l5, l3);
    pkbTables->addParent(l15, l14);
    pkbTables->addParent(l23, l14);
    pkbTables->addParent(l16, l15);
    pkbTables->addParent(l17, l15);
    pkbTables->addParent(l19, l15);
    pkbTables->addParent(l22, l15);
    pkbTables->addParent(l18, l17);
    pkbTables->addParent(l20, l19);
    pkbTables->addParent(l21, l19);
    pkbTables->addParent(l24, l23);

    pkbTables->addStatementType(l1, Pkb::StatementType::Read);
    pkbTables->addStatementType(l2, Pkb::StatementType::Read);
    pkbTables->addStatementType(l3, Pkb::StatementType::While);
    pkbTables->addStatementType(l4, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l5, Pkb::StatementType::Read);
    pkbTables->addStatementType(l6, Pkb::StatementType::Print);
    pkbTables->addStatementType(l7, Pkb::StatementType::Call);
    pkbTables->addStatementType(l8, Pkb::StatementType::Read);
    pkbTables->addStatementType(l9, Pkb::StatementType::Read);
    pkbTables->addStatementType(l10, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l11, Pkb::StatementType::Call);
    pkbTables->addStatementType(l12, Pkb::StatementType::Read);
    pkbTables->addStatementType(l13, Pkb::StatementType::Read);
    pkbTables->addStatementType(l14, Pkb::StatementType::While);
    pkbTables->addStatementType(l15, Pkb::StatementType::If);
    pkbTables->addStatementType(l16, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l17, Pkb::StatementType::While);
    pkbTables->addStatementType(l18, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l19, Pkb::StatementType::If);
    pkbTables->addStatementType(l20, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l21, Pkb::StatementType::Print);
    pkbTables->addStatementType(l22, Pkb::StatementType::Print);
    pkbTables->addStatementType(l23, Pkb::StatementType::While);
    pkbTables->addStatementType(l24, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l25, Pkb::StatementType::Call);
    pkbTables->addStatementType(l26, Pkb::StatementType::Print);
    pkbTables->addStatementType(l27, Pkb::StatementType::Call);
    pkbTables->addStatementType(l28, Pkb::StatementType::Call);
    pkbTables->addStatementType(l29, Pkb::StatementType::Call);
    pkbTables->addStatementType(l30, Pkb::StatementType::Call);
    pkbTables->addStatementType(l31, Pkb::StatementType::Call);
    pkbTables->addStatementType(l32, Pkb::StatementType::Call);
    pkbTables->addStatementType(l33, Pkb::StatementType::Call);

    Pkb::VAR vx = "x";
    Pkb::VAR vy = "y";
    Pkb::VAR vr = "r";
    Pkb::VAR vm = "m";
    Pkb::VAR vq = "q";
    Pkb::VAR vt = "t";
    Pkb::VAR vk = "k";

    pkbTables->addVar(vx);
    pkbTables->addVar(vy);
    pkbTables->addVar(vr);
    pkbTables->addVar(vm);
    pkbTables->addVar(vq);
    pkbTables->addVar(vt);
    pkbTables->addVar(vk);

    Pkb::PROC paux = "aux";
    Pkb::PROC pmain = "main";
    Pkb::PROC pcomplicate = "complicate";
    Pkb::PROC pextra = "extra";
    Pkb::PROC pextratwo = "extratwo";
    Pkb::PROC pextrathree = "extrathree";
    Pkb::PROC pextrafour = "extrafour";

    pkbTables->addProc(paux);
    pkbTables->addProc(pmain);
    pkbTables->addProc(pcomplicate);
    pkbTables->addProc(pextra);
    pkbTables->addProc(pextratwo);
    pkbTables->addProc(pextrathree);
    pkbTables->addProc(pextrafour);

    pkbTables->addCall(pmain, paux);
    pkbTables->addCall(pmain, pcomplicate);
    pkbTables->addCall(pcomplicate, pextra);
    pkbTables->addCall(pextratwo, pextra);
    pkbTables->addCall(pextratwo, paux);
    pkbTables->addCall(pextrathree, pmain);
    pkbTables->addCall(pextrathree, pextratwo);
    pkbTables->addCall(pextrafour, pextrathree);
    pkbTables->addCall(pextrafour, pmain);
    pkbTables->addCall(pextrafour, pextrathree);

    pkbTables->addUses(l3, Pkb::VARS{vx, vy});
    pkbTables->addUses(l4, Pkb::VARS{vx, vy});
    pkbTables->addUses(l6, Pkb::VARS{vx});
    pkbTables->addUses(l7, paux);
    pkbTables->addUses(l10, Pkb::VARS{vx, vr});
    pkbTables->addUses(l11, pcomplicate);
    pkbTables->addUses(l14, Pkb::VARS{vq, vt});
    pkbTables->addUses(l15, Pkb::VARS{vq, vt});
    pkbTables->addUses(l16, Pkb::VARS{vt});
    pkbTables->addUses(l17, Pkb::VARS{vq, vt});
    pkbTables->addUses(l18, Pkb::VARS{vt});
    pkbTables->addUses(l19, Pkb::VARS{vq});
    pkbTables->addUses(l20, Pkb::VARS{vq});
    pkbTables->addUses(l21, Pkb::VARS{vq});
    pkbTables->addUses(l22, Pkb::VARS{vt});
    pkbTables->addUses(l23, Pkb::VARS{vq});
    pkbTables->addUses(l24, Pkb::VARS{vq});
    pkbTables->addUses(l25, pextra);
    pkbTables->addUses(l26, Pkb::VARS{vk});
    pkbTables->addUses(l27, pextra);
    pkbTables->addUses(l28, paux);
    pkbTables->addUses(l29, pmain);
    pkbTables->addUses(l30, pextratwo);
    pkbTables->addUses(l31, pextrathree);
    pkbTables->addUses(l32, pmain);
    pkbTables->addUses(l33, pextrathree);

    pkbTables->addUsesProc(paux, Pkb::VARS{vy, vx});
    pkbTables->addUsesProc(pmain, Pkb::VARS{vx, vr, vy, vq, vt, vk});
    pkbTables->addUsesProc(pcomplicate, Pkb::VARS{vq, vt, vk});
    pkbTables->addUsesProc(pextra, Pkb::VARS{vk});
    pkbTables->addUsesProc(pextratwo, Pkb::VARS{vk, vy, vx});
    pkbTables->addUsesProc(pextrathree, Pkb::VARS{vk, vy, vx, vr, vq, vt, vk});
    pkbTables->addUsesProc(pextrafour, Pkb::VARS{vk, vy, vx, vr, vq, vt, vk});

    pkbTables->addModifies(l1, Pkb::VARS{vx});
    pkbTables->addModifies(l2, Pkb::VARS{vy});
    pkbTables->addModifies(l3, Pkb::VARS{vy});
    pkbTables->addModifies(l4, Pkb::VARS{vy});
    pkbTables->addModifies(l5, Pkb::VARS{vy});
    pkbTables->addModifies(l7, paux);
    pkbTables->addModifies(l8, Pkb::VARS{vx});
    pkbTables->addModifies(l9, Pkb::VARS{vr});
    pkbTables->addModifies(l10, Pkb::VARS{vm});
    pkbTables->addModifies(l11, pcomplicate);
    pkbTables->addModifies(l12, Pkb::VARS{vq});
    pkbTables->addModifies(l13, Pkb::VARS{vt});
    pkbTables->addModifies(l14, Pkb::VARS{vq, vt});
    pkbTables->addModifies(l15, Pkb::VARS{vq, vt});
    pkbTables->addModifies(l16, Pkb::VARS{vt});
    pkbTables->addModifies(l17, Pkb::VARS{vt});
    pkbTables->addModifies(l18, Pkb::VARS{vt});
    pkbTables->addModifies(l19, Pkb::VARS{vq});
    pkbTables->addModifies(l20, Pkb::VARS{vq});
    pkbTables->addModifies(l23, Pkb::VARS{vq});
    pkbTables->addModifies(l24, Pkb::VARS{vq});
    pkbTables->addModifies(l25, pextra);
    pkbTables->addModifies(l27, pextra);
    pkbTables->addModifies(l28, paux);
    pkbTables->addModifies(l29, pmain);
    pkbTables->addModifies(l30, pextratwo);
    pkbTables->addModifies(l31, pextrathree);
    pkbTables->addModifies(l32, pmain);
    pkbTables->addModifies(l33, pextrathree);

    pkbTables->addModifiesProc(paux, Pkb::VARS{vy, vx});
    pkbTables->addModifiesProc(pmain, Pkb::VARS{vx, vr, vm, vy, vq, vt});
    pkbTables->addModifiesProc(pcomplicate, Pkb::VARS{vq, vt});
    pkbTables->addModifiesProc(pextratwo, Pkb::VARS{vy, vx});
    pkbTables->addModifiesProc(pextrathree, Pkb::VARS{vy, vx, vr, vm, vq, vt});
    pkbTables->addModifiesProc(pextrafour, Pkb::VARS{vy, vx, vr, vm, vq, vt});

    pkbTables->addConditionVars(l3, Pkb::VARS{vy});
    pkbTables->addConditionVars(l14, Pkb::VARS{vq, vt});
    pkbTables->addConditionVars(l15, Pkb::VARS{vt});
    pkbTables->addConditionVars(l17, Pkb::VARS{vt, vq});
    pkbTables->addConditionVars(l19, Pkb::VARS{vq});
    pkbTables->addConditionVars(l23, Pkb::VARS{vq});

    TNode T3 = TNode(TNode::Op::Divide);
    T3.left = new TNode("y");
    T3.right = new TNode("x");
    l4ast = T3;

    TNode T6 = TNode(TNode::Op::Plus);
    T6.left = new TNode("x");
    T6.right = new TNode("r");
    l10ast = T6;

    TNode T9 = TNode(TNode::Op::Minus);
    T9.left = new TNode("t");
    T9.right = new TNode("1");
    l16ast = T9;

    TNode T12 = TNode(TNode::Op::Minus);
    T12.left = new TNode("t");
    T12.right = new TNode("1");
    l18ast = T12;

    TNode T15 = TNode(TNode::Op::Minus);
    T15.left = new TNode("q");
    T15.right = new TNode("1");
    l20ast = T15;

    TNode T18 = TNode(TNode::Op::Minus);
    T18.left = new TNode("q");
    T18.right = new TNode("1");
    l24ast = T18;

    pkbTables->addAssignAst(l4, l4ast);
    pkbTables->addAssignAst(l10, l10ast);
    pkbTables->addAssignAst(l16, l16ast);
    pkbTables->addAssignAst(l18, l18ast);
    pkbTables->addAssignAst(l20, l20ast);
    pkbTables->addAssignAst(l24, l24ast);

    pkbTables->addConstant("1");
    pkbTables->addConstant("5");
    pkbTables->addConstant("11111111111111111111111111111111111111");
    pkbTables->addConstant("5");
    pkbTables->addConstant("0");

  } else if (testNumber == TestNumber::B) {
    /* To be tested: SIMPLE Program:
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
    12        if (x == y) {
    13          print z;
              } else {
    14          while (x == y) {
    15            print z; }}
            } else {
    16        if (x == y) {
    17          while (x == y) {
    18            while (x == y) {
    19              print z; }}
              } else {
    20          read z; }}
    21      read z; }
        }
        procedure aux2 {
    22    if (x == y) {
    23      print z;
          } else {
    24      print z; }
        }
    */

    l1 = 1;
    l2 = 2;
    l3 = 3;
    l4 = 4;
    l5 = 5;
    l6 = 6;
    l7 = 7;
    l8 = 8;
    l9 = 9;
    l10 = 10;
    l11 = 11;
    l12 = 12;
    l13 = 13;
    l14 = 14;
    l15 = 15;
    l16 = 16;
    l17 = 17;
    l18 = 18;
    l19 = 19;
    l20 = 20;
    l21 = 21;
    l22 = 22;
    l23 = 23;
    l24 = 24;

    p0 = "main";
    p1 = "aux";
    p2 = "aux2";
    pkbTables->addProc(p0);
    pkbTables->addProc(p1);
    pkbTables->addProc(p2);

    pkbTables->addStatementProc(l1, p0);
    pkbTables->addStatementProc(l2, p0);
    pkbTables->addStatementProc(l3, p0);
    pkbTables->addStatementProc(l4, p0);
    pkbTables->addStatementProc(l5, p0);
    pkbTables->addStatementProc(l6, p0);
    pkbTables->addStatementProc(l7, p0);
    pkbTables->addStatementProc(l8, p0);
    pkbTables->addStatementProc(l9, p0);
    pkbTables->addStatementProc(l10, p1);
    pkbTables->addStatementProc(l11, p1);
    pkbTables->addStatementProc(l12, p1);
    pkbTables->addStatementProc(l13, p1);
    pkbTables->addStatementProc(l14, p1);
    pkbTables->addStatementProc(l15, p1);
    pkbTables->addStatementProc(l16, p1);
    pkbTables->addStatementProc(l17, p1);
    pkbTables->addStatementProc(l18, p1);
    pkbTables->addStatementProc(l19, p1);
    pkbTables->addStatementProc(l20, p1);
    pkbTables->addStatementProc(l21, p1);
    pkbTables->addStatementProc(l22, p2);
    pkbTables->addStatementProc(l23, p2);
    pkbTables->addStatementProc(l24, p2);

    pkbTables->addFollow(l1, l2);
    pkbTables->addFollow(l2, l8);
    pkbTables->addFollow(l3, l4);
    pkbTables->addFollow(l4, l6);
    pkbTables->addFollow(l8, l9);
    pkbTables->addFollow(l11, l21);

    pkbTables->addParent(l3, l2);
    pkbTables->addParent(l4, l2);
    pkbTables->addParent(l5, l4);
    pkbTables->addParent(l6, l2);
    pkbTables->addParent(l7, l2);
    pkbTables->addParent(l11, l10);
    pkbTables->addParent(l12, l11);
    pkbTables->addParent(l13, l12);
    pkbTables->addParent(l14, l12);
    pkbTables->addParent(l15, l14);
    pkbTables->addParent(l16, l11);
    pkbTables->addParent(l17, l16);
    pkbTables->addParent(l18, l17);
    pkbTables->addParent(l19, l18);
    pkbTables->addParent(l20, l16);
    pkbTables->addParent(l21, l10);
    pkbTables->addParent(l23, l22);
    pkbTables->addParent(l24, l22);

    pkbTables->addStatementType(l1, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l2, Pkb::StatementType::If);
    pkbTables->addStatementType(l3, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l4, Pkb::StatementType::While);
    pkbTables->addStatementType(l5, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l6, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l7, Pkb::StatementType::Call);
    pkbTables->addStatementType(l8, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l9, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l10, Pkb::StatementType::While);
    pkbTables->addStatementType(l11, Pkb::StatementType::If);
    pkbTables->addStatementType(l12, Pkb::StatementType::If);
    pkbTables->addStatementType(l13, Pkb::StatementType::Print);
    pkbTables->addStatementType(l14, Pkb::StatementType::While);
    pkbTables->addStatementType(l15, Pkb::StatementType::Print);
    pkbTables->addStatementType(l16, Pkb::StatementType::If);
    pkbTables->addStatementType(l17, Pkb::StatementType::While);
    pkbTables->addStatementType(l18, Pkb::StatementType::While);
    pkbTables->addStatementType(l19, Pkb::StatementType::Print);
    pkbTables->addStatementType(l20, Pkb::StatementType::Read);
    pkbTables->addStatementType(l21, Pkb::StatementType::Read);
    pkbTables->addStatementType(l22, Pkb::StatementType::If);
    pkbTables->addStatementType(l23, Pkb::StatementType::Print);
    pkbTables->addStatementType(l24, Pkb::StatementType::Print);
  }

  pkbTables->deriveTables();
}
