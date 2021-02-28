#ifndef I2P2_TEST_H
#define I2P2_TEST_H

namespace I2P2_test {
  void check_hierarchy();
  void check_iterator();
#ifdef TEST_LIST
  void check_list();
#endif
#ifdef TEST_VECTOR
  void check_vector();
#endif
}

#endif

/* This file should not be modified */

