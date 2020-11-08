# Team 20 SPA Prototype

## Environment
Visual Studio 2019
Team20/Code20/StartupSPASolution.sln
Release Mode

## Usage (AutoTester)
1. Build the AutoTester in VS
2. Open a terminal
3. Navigate to Team20/Code20/Release/
4. Execute the appropriate command:
```
# Git Bash Command
./AutoTester.exe ../../Tests20/iteration3/system_test_1_source.txt ../../Tests20/iteration3/system_test_1_queries.txt ../../Tests20/out.xml

# cmd/Powertools Command
.\AutoTester ..\..\Tests20\iteration3\system_test_1_source.txt ..\..\Tests20\iteration3\system_test_1_queries.txt ..\..\Tests20\out.xml
```
5. Replace the source and query files accordingly to run other test cases. Note that the test cases are located in 
Team20/Tests20/, which is where the output file (out.xml) is placed by default.

