#ifndef I2P2_CONTAINER_H
#define I2P2_CONTAINER_H
#include "I2P2_def.h"
#include "I2P2_iterator.h"

namespace I2P2 {
class iterator;
class const_iterator;

struct container_base {
  virtual bool empty() const = 0;
  virtual size_type size() const = 0;
};

struct dynamic_size_container : container_base {
  virtual iterator begin() = 0;
  virtual const_iterator begin() const = 0;
  virtual iterator end() = 0;
  virtual const_iterator end() const = 0;
  virtual void clear() = 0;
  virtual void erase(const_iterator pos) = 0;
  virtual void erase(const_iterator begin, const_iterator end) = 0;
  // The following need only be defined for vector
  virtual void reserve(size_type new_capacity) {} 
  virtual void shrink_to_fit() {}
  virtual size_type capacity() const { return size(); }
};

struct ordered_container : dynamic_size_container {
  virtual reference back() = 0;
  virtual const_reference back() const = 0;
  virtual reference front() = 0;
  virtual const_reference front() const = 0;
  virtual void insert(const_iterator pos, size_type count, const_reference val) = 0;
  virtual void insert(const_iterator pos, const_iterator begin, const_iterator end) = 0;
  virtual void pop_back() = 0;
  virtual void pop_front() = 0;
  virtual void push_back(const_reference val) = 0;
  virtual void push_front(const_reference val) = 0;
};

struct randomaccess_container : ordered_container {
  virtual reference operator[](size_type pos) = 0;
  virtual const_reference operator[](size_type pos) const = 0;
};
}  // namespace I2P2

#endif

/* This file should not be modified 
 * You Vector and List will be derived from these containers */

