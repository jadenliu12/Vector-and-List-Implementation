#include <iostream>
#include "header/I2P2_test.h"

int main() {
  
#ifdef TEST_LIST
  std::cout << "Checking list ..." << std::endl;
  I2P2_test::check_list();
#endif

#ifdef TEST_VECTOR
  std::cout << "Checking vector ..." << std::endl;
  I2P2_test::check_vector();
#endif

  std::cout << "Finished" << std::endl;

  return 0;
}

