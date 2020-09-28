# Team 20 SPA Prototype

## Environment
Visual Studio 2019
Team20/Code20/StartupSPASolution.sln
Release Mode

## Usage (AutoTester)
### Option 1: Bash Script
1. Open a Git Bash terminal
2. Navigate to Team20/Tests20/
3. Execute ./run

Note that firefox should already be configured to open the output xml files, as the script
will open a firefox tab for each test output. Negative SIMPLE source codes are also not
covered by this script.

### Option 2: Manual Execution
1. Build the AutoTester in VS
2. Open a terminal
3. Navigate to Team20/Code20/Release/
4. Execute the appropriate command:
```
# Git Bash Command
./AutoTester.exe ../../Tests20/system_test_1_source.txt ../../Tests20/system_test_1_queries.txt ../../Tests20/out.xml

# cmd/Powertools Command
.\AutoTester ..\..\Tests20\system_test_1_source.txt ..\..\Tests20\system_test_1_queries.txt ..\..\Tests20\out.xml
```
5. Replace the source and query files accordingly to run other test cases. Note that the test cases are located in 
Team20/Tests20/, which is where the output file (out.xml) is placed by default.

## Test Cases (Team20/Tests20/)
|Source                    |Queries              |Remarks           |
|--------------------------|---------------------|------------------|
|system_test_1_source      |system_test_1_queries|                  |
|system_test_2_source      |system_test_2_queries|                  |
|system_test_3_source      |system_test_3_queries|                  |
|system_test_wrong_1_source|-                    |Negative Test Case|
|system_test_wrong_2_source|-                    |Negative Test Case|
|system_test_wrong_3_source|-                    |Negative Test Case|
|system_test_wrong_4_source|-                    |Negative Test Case|
