#ifndef I2P2_VECTOR_H
#define I2P2_VECTOR_H
#include "I2P2_container.h"
#include "I2P2_iterator.h"

namespace I2P2 
{
class Vector : public randomaccess_container 
{
 private:
    pointer p_begin;
    pointer p_last;
    pointer p_end;

  /* The following are standard methods from the STL */
 public:
    ~Vector();
    Vector();
    Vector(const Vector &rhs);
    Vector &operator=(const Vector &rhs);
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    size_type capacity() const;
    size_type size() const;
    void clear();
    bool empty() const;
    void erase(const_iterator pos);
    void erase(const_iterator begin, const_iterator end);
    void insert(const_iterator pos, size_type count, const_reference val);
    void insert(const_iterator pos, const_iterator begin, const_iterator end);
    void pop_back();
    void pop_front();
    void push_back(const_reference val);
    void push_front(const_reference val);
    void reserve(size_type new_capacity);
    void shrink_to_fit();
  };
}  // namespace I2P2

#endif
