#ifndef I2P2_DEF_H
#define I2P2_DEF_H
#include <cstdint>

namespace I2P2 {
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type =
#ifdef INT64
    int64_t
#elif DOUBLE
    double
#else
    uint8_t
#endif
    ;

  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
}

#endif

/* Here is just some definitions for the types that your implementation should deal with
 * They should be pretty intuitive, and this file shouldn't be modified */

