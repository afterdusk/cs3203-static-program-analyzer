#include "CppUnitTest.h"
#include "Token.h"
#include "stdafx.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestParseExceptions){public : TEST_METHOD(TestNoProcedureException){
    PkbTables::PROC procedureName = "procedure1";
NoProcedureException exception = NoProcedureException(10, procedureName);
Assert::IsTrue(strcmp(exception.what(),
                      "Error: The procedure \"procedure1\" does not exist.") ==
               0);
} // namespace UnitTesting
}
;
}
