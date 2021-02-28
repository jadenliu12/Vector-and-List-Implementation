#ifndef I2P2_LIST_H
#define I2P2_LIST_H
#include "I2P2_container.h"
#include "I2P2_iterator.h"

namespace I2P2 
{
  struct Node;

  class List : public ordered_container 
  {
    protected:
  // you may want to declare necessary members for your List container here
      Node* head;
      Node* tail;
      size_type _size;

  /* The following are standard methods from the STL
   * If you are not sure what they do
   * look them up and implement your own version */
    public:
      ~List();
      List();
      List(const List &rhs);
      List &operator=(const List &rhs);
      iterator begin();
      const_iterator begin() const;
      iterator end();
      const_iterator end() const;
      reference front();
      const_reference front() const;
      reference back();
      const_reference back() const;
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
  };
}  // namespace I2P2

#endif

