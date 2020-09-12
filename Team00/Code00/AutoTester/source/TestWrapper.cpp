#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;
AbstractWrapper *WrapperFactory::createWrapper() {
  if (wrapper == 0)
    wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  pkb = PKB();
}

// read SIMPLE program from the given filename
std::string TestWrapper::read(std::string filename) {
  std::ifstream program(filename);
  if (!(program.is_open())) {
    std::cout << "Unable to open SIMPLE program file." << std::endl;
  }
  std::string input((std::istreambuf_iterator<char>(program)),
                    (std::istreambuf_iterator<char>()));
  return input;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
  std::cout << read(filename);
  Parser parser(read(filename), &pkb);
  parser.parse();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  // call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.

  results = PQL::evaluate(PQL::parse(PQL::lex(query)), pkb);
}
