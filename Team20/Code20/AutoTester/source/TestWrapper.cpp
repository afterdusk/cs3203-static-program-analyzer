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
  pkb = Pkb();
}

// read SIMPLE program from the given filename
std::string TestWrapper::read(std::string filename) {
  std::ifstream program(filename);
  if (!(program.is_open())) {
    std::cout << "Unable to open SIMPLE program file." << std::endl;
    exit(1);
  }
  std::string input((std::istreambuf_iterator<char>(program)),
                    (std::istreambuf_iterator<char>()));
  return input;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  try {
    Parser parser(read(filename), pkb.getTables());
    parser.parse();
  } catch (ParseException &p) {
    std::cout << p.what() << std::endl;
    exit(1);
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  try {
    Pql::evaluate(Pql::rewrite(Pql::parse(Pql::lex(query))),
                  pkb.getQueryInterface(), results);
  } catch (const PqlSemanticErrorWithBooleanResultException &error) {
    results.clear();
    results.push_back(FALSE_RESULT);
  } catch (...) {
    results.clear();
  }

  pkb.getQueryInterface()->clearCache();
}
