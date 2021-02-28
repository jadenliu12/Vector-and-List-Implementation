#include "../header/I2P2_iterator.h"
#include <iostream>

namespace I2P2 
{
  //vector_iterator implementation
  vector_iterator::vector_iterator() 
  {
    vec = nullptr;
  } 
  iterator_impl_base& vector_iterator::operator++()
  {
    vec++;
    return *this;
  }
  iterator_impl_base& vector_iterator::operator--()
  {
    vec--;
    return *this;
  }
  iterator_impl_base& vector_iterator::operator+=(difference_type offset)
  {
    vec += offset;
    return *this;
  }
  iterator_impl_base& vector_iterator::operator-=(difference_type offset)
  {
    vec -= offset;
    return *this;
  }
  iterator_impl_base* vector_iterator::clone() const
  {
    return new vector_iterator(vec);
  }
  bool vector_iterator::operator==(const iterator_impl_base &rhs) const
  {
    return vec == rhs.ptr_ref();
  }
  bool vector_iterator::operator!=(const iterator_impl_base &rhs) const
  {
    return vec != rhs.ptr_ref();
  }
  bool vector_iterator::operator<(const iterator_impl_base &rhs) const
  {
    return vec < rhs.ptr_ref();
  }
  bool vector_iterator::operator>(const iterator_impl_base &rhs) const
  {
    return vec > rhs.ptr_ref();
  }
  bool vector_iterator::operator<=(const iterator_impl_base &rhs) const
  {
    return vec <= rhs.ptr_ref();
  }
  bool vector_iterator::operator>=(const iterator_impl_base &rhs) const
  {
    return vec >= rhs.ptr_ref();
  }
  difference_type vector_iterator::operator-(const iterator_impl_base &rhs) const
  {
    difference_type result = this->vec - rhs.ptr_ref();
    return result;
  }
  pointer vector_iterator::operator->() const
  {
    return vec;
  }
  reference vector_iterator::operator*() const
  {
    return *vec;
  }
  reference vector_iterator::operator[](difference_type offset) const
  {
    return vec[offset];
  }

  //list iterator implementation
  iterator_impl_base& list_iterator::operator++()
  {
    _node = _node->next;
    return *this;
  }
  iterator_impl_base& list_iterator::operator--()
  {
    _node = _node->prev;
    return *this;
  }
  iterator_impl_base& list_iterator::operator+=(difference_type offset)
  {
    while(offset--)
    {
      _node = _node->next;
    }
    return *this;
  }
  iterator_impl_base& list_iterator::operator-=(difference_type offset)
  {
    while(offset--)
    {
      _node = _node->prev;
    }
    return *this;
  }
  iterator_impl_base* list_iterator::clone() const
  {
    return new list_iterator(_node); 
  }
  bool list_iterator::operator==(const iterator_impl_base &rhs) const
  {
    return _node == rhs.node_ref();
  }
  bool list_iterator::operator!=(const iterator_impl_base &rhs) const
  {
    return _node != rhs.node_ref();
  }
  bool list_iterator::operator<(const iterator_impl_base &rhs) const
  {
    Node* now = this->_node;
    bool found = false;

    if(now == rhs.node_ref())
      return found;

    while(now != nullptr)
    {
      if(now == rhs.node_ref())
      {
        found = true;
        break;
      }      
      now = now->next;
    }

    return found;
  }
  bool list_iterator::operator>(const iterator_impl_base &rhs) const
  {
    Node* now = this->_node;
    bool found = false;

    if(now == rhs.node_ref())
      return found;

    while(now != nullptr)
    {
      if(now == rhs.node_ref())
      {
        found = true;
        break;
      }  
      now = now->prev;
    }

    return found;
  }
  bool list_iterator::operator<=(const iterator_impl_base &rhs) const
  {
    Node* now = this->_node;
    bool found = false;
    while(now != nullptr)
    {
      if(now == rhs.node_ref())
      {
        found = true;
        break;
      }      
      now = now->next;
    }

    return found;
  }
  bool list_iterator::operator>=(const iterator_impl_base &rhs) const
  {
    Node* now = this->_node;
    bool found = false;
    while(now != nullptr)
    {
      if(now == rhs.node_ref())
      {
        found = true;
        break;
      } 
      now = now->prev;
    }
  
    return found;
  }
  difference_type list_iterator::operator-(const iterator_impl_base &rhs) const
  {
    difference_type f_cnt = 0, b_cnt = 0;
    bool infront = true;
    Node* front = rhs.node_ref();
    Node* back = rhs.node_ref();
    Node* target = this->_node;

    while(back != nullptr)
    {
      if(back == target)
      {
        infront = false;
        break;
      }

      back = back->prev;
      b_cnt++;
    }

    if(infront == true)
    {
      while(front != nullptr)
      {
        if(front == target)
          break;
        
        front = front->next;
        f_cnt++;
      }  
      
      return f_cnt;    
    }

    return b_cnt;
  }
  pointer list_iterator::operator->() const
  {
    return &_node->data;
  }
  reference list_iterator::operator*() const
  {
    return _node->data;
  }
  reference list_iterator::operator[](difference_type offset) const
  {
    Node* now = this->_node;
    if(offset > 0)
    {
      while(offset--)
      {
        now = now->next;
      }
    }
    else
    {
      while(offset++)
      {
        now = now->prev;
      }
    }

    return now->data;
  }

  //const_iterator implementation
  const_iterator::~const_iterator() 
  {
    delete p_;
  }
  const_iterator::const_iterator() : p_(nullptr) {}
  const_iterator::const_iterator(const const_iterator &rhs)
  {
    p_ = rhs.p_->clone();
  }
  const_iterator::const_iterator(iterator_impl_base *p) 
  {
    p_ = p->clone();
  }
  const_iterator& const_iterator::operator=(const const_iterator &rhs)
  {
    this->p_ = rhs.p_->clone();
    return *this;
  }
  const_iterator& const_iterator::operator++()
  {
    p_->operator++();
    return *this;
  }
  const_iterator const_iterator::operator++(int)
  {
    const_iterator tmp(*this);
    p_->operator++();
    return tmp;
  }
  const_iterator& const_iterator::operator--()
  {
    p_->operator--();
    return *this;
  }
  const_iterator const_iterator::operator--(int)
  {
    const_iterator tmp(*this);
    p_->operator--();
    return tmp; 
  }
  const_iterator& const_iterator::operator+=(difference_type offset)
  {
    p_->operator+=(offset);
    return *this;
  }
  const_iterator const_iterator::operator+(difference_type offset) const
  {
    const_iterator it(*this);
    it.p_->operator+=(offset);
    return it;
  }
  const_iterator& const_iterator::operator-=(difference_type offset)
  {
    p_->operator-=(offset);
    return *this;
  }
  const_iterator const_iterator::operator-(difference_type offset) const
  {
    const_iterator it(*this);
    it.p_->operator-=(offset);
    return it;
  }
  difference_type const_iterator::operator-(const const_iterator &rhs) const
  {
    return p_->operator-(*(rhs.p_));
  }
  const_iterator::pointer const_iterator::operator->() const
  {
    return p_->operator->();
  }
  const_iterator::reference const_iterator::operator*() const
  {
    return p_->operator*();
  }
  const_iterator::reference const_iterator::operator[](difference_type offset) const
  {
    return p_->operator[](offset);
  }
  bool const_iterator::operator==(const const_iterator &rhs) const
  {
    return p_->operator==(*(rhs.p_));
  }
  bool const_iterator::operator!=(const const_iterator &rhs) const
  {
    return p_->operator!=(*(rhs.p_));
  }
  bool const_iterator::operator<(const const_iterator &rhs) const
  {
    return p_->operator<(*(rhs.p_));
  }
  bool const_iterator::operator>(const const_iterator &rhs) const
  {
    return p_->operator>(*(rhs.p_));
  }
  bool const_iterator::operator<=(const const_iterator &rhs) const
  {
    return p_->operator<=(*(rhs.p_));
  }
  bool const_iterator::operator>=(const const_iterator &rhs) const
  {
    return p_->operator>=(*(rhs.p_));
  }

  //iterator implementation
  iterator::iterator() : const_iterator() {}
  iterator::iterator(iterator_impl_base *p) : const_iterator(p) {}
  iterator::iterator(const iterator &rhs) : const_iterator(rhs) {}
  iterator& iterator::operator++()
  {
    p_->operator++();
    return *this;
  }
  iterator iterator::operator++(int)
  {
    iterator tmp(*this);
    p_->operator++();
    return tmp;
  }
  iterator& iterator::operator--()
  {
    p_->operator--();
    return *this;
  }
  iterator iterator::operator--(int)
  {
    iterator tmp(*this);
    p_->operator--();
    return tmp;
  }
  iterator& iterator::operator+=(difference_type offset)
  {
    p_->operator+=(offset);
    return *this;
  }
  iterator iterator::operator+(difference_type offset) const
  {
    iterator it(*this);
    it.p_->operator+=(offset);
    return it;
  }
  iterator& iterator::operator-=(difference_type offset)
  {
    p_->operator-=(offset);
    return *this;
  }
  iterator iterator::operator-(difference_type offset) const
  {
    iterator it(*this);
    it.p_->operator-=(offset);
    return it;
  }
  difference_type iterator::operator-(const iterator &rhs) const
  {
    return p_->operator-(*(rhs.p_));
  }
  iterator::pointer iterator::operator->() const
  {
    return p_->operator->();
  }
  iterator::reference iterator::operator*() const
  {
    return *(*p_);
  }
  iterator::reference iterator::operator[](difference_type offset) const
  { 
    return p_->operator[](offset);
  }
} // namespace I2P2