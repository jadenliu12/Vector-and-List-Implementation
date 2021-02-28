#include "../header/I2P2_Vector.h"

namespace I2P2 {
    Vector::~Vector()
    {
      for (size_type i = 0;i < size();i++)
        p_begin[i].~value_type();
      
      if (p_begin != nullptr) 
        operator delete[](p_begin);         
    }
    Vector::Vector()
    {
      p_begin = nullptr;
      p_end = nullptr;
      p_last = nullptr;
    }
    Vector::Vector(const Vector &rhs)
    {
      p_begin = nullptr;
      p_end = nullptr;
      p_last = nullptr;

      reserve(rhs.size());
      for (size_type i = 0;i < rhs.size();i++)
        insert(this->begin()+i, 1, rhs[i]);
    }
    Vector& Vector::operator=(const Vector &rhs)
    {
      if (this == &rhs) 
        return *this;

      for(size_type i = 0; i < size(); i++)
        p_begin[i].~value_type();

      p_last = p_begin;

      reserve(rhs.size());
      for (size_type i = 0;i < rhs.size();i++)
        insert(this->begin()+i, 1, rhs[i]);

      return *this;
    }
    iterator Vector::begin()
    {
      vector_iterator start_ptr = vector_iterator(p_begin);
      iterator p_start(&start_ptr);
      return p_start;
    }
    const_iterator Vector::begin() const
    {
      vector_iterator start_ptr = vector_iterator(p_begin);
      const_iterator p_start(&start_ptr);
      return p_start;
    }
    iterator Vector::end()
    {
      vector_iterator last_ptr = vector_iterator(p_last);
      iterator p_last(&last_ptr);
      return p_last;
    }
    const_iterator Vector::end() const
    {
      vector_iterator last_ptr = vector_iterator(p_last);
      const_iterator p_last(&last_ptr);
      return p_last;
    }
    reference Vector::front()
    {
      return *p_begin;    
    }
    const_reference Vector::front() const
    {
      return *p_begin;
    }
    reference Vector::back()
    {
      return p_begin[size()-1];       
    }
    const_reference Vector::back() const
    {
      return p_begin[size()-1];
    }
    reference Vector::operator[](size_type pos)
    {
      return p_begin[pos];
    }
    const_reference Vector::operator[](size_type pos) const
    {
      return p_begin[pos];
    }
    size_type Vector::capacity() const
    {
      return p_end - p_begin;
    }
    size_type Vector::size() const
    {
      return p_last - p_begin;
    }
    void Vector::clear()
    {
      size_type old_s = size();
      for(size_type i = 0; i < old_s; i++)
        pop_back();
    }
    bool Vector::empty() const
    {
      return p_last == p_begin;
    }
    void Vector::erase(const_iterator pos)
    {
      if(pos != this->end())
      {
        size_type position = pos - this->begin();
                
        if(position >= 0 && position < size())
        {
          for(size_type i = position; i < size()-1; i++)
          {
            p_begin[i] = p_begin[i+1];
          }
          p_begin[size()-1].~value_type();
          p_last--;
        }
      }
    }
    void Vector::erase(const_iterator begin, const_iterator end)
    {
      if(begin != end)
      {
        size_type start = begin - this->begin();
        size_type fin = end - this->begin();
        size_type old_size = size();
        size_type count = fin - start;

        if(fin == old_size && start == 0)
        {
          clear();
        }
        else 
        {
          for(size_type i = start; i < old_size - count; i++)
            p_begin[i] = p_begin[i + count];
          for(size_type i = old_size-1; i >= old_size-count; i--)
            p_begin[i].~value_type();
          p_last -= count;
        }
      }
    }
    void Vector::insert(const_iterator pos, size_type count, const_reference val)
    {
      if(count <= 0)
        return;
        
      size_type position = pos - this->begin();
      size_type old_capacity = capacity();
      size_type old_size = size();

      if(position > old_size)
        return;  
        
      if (old_size + count > old_capacity) 
      {
        if (old_capacity + count > old_capacity * 3)
          reserve(old_capacity + count);
        else 
          reserve(old_capacity * 3);
      }
      p_last += count;
        
      for(size_type i = 0; i < count; i++)
        new (p_begin + old_size + i) value_type(0);
      for(size_type i = size()-1; i >= position + count; i--)
        p_begin[i] = p_begin[i-count];
      for(size_type i = 0; i < count; i++)
        p_begin[position+i] = val;
    }
    void Vector::insert(const_iterator pos, const_iterator begin, const_iterator end)
    {
      if(begin != end)
      {
        size_type position = pos - this->begin(); 
        size_type count = end - begin;
        size_type old_capacity = capacity();
        size_type old_size = size(); 

        size_type inc = 0;
        pointer tmp = new value_type[count+1];
        while(begin != end)
        {
          tmp[inc++] = *begin;
          begin++;
        }

        if (old_size + count > old_capacity) 
        {
          if (old_capacity + count > old_capacity * 3)
            reserve(old_capacity + count);
          else 
            reserve(old_capacity * 3);
        }
        p_last += count;
        
        for(size_type i = 0; i < count; i++)
          new (p_begin + old_size + i) value_type(0);
        for(size_type i = size()-1; i >= position + count; i--)
          p_begin[i] = p_begin[i-count];
        for(size_type i = 0; i < count; i++)
          p_begin[position+i] = tmp[i];

        delete [] tmp;
      }
    }
    void Vector::pop_back()
    {
      erase(this->begin() + size() - 1);
    }
    void Vector::pop_front()
    {
      erase(this->begin());
    }
    void Vector::push_back(const_reference val)
    {
      insert(this->end(), 1, val);
    }
    void Vector::push_front(const_reference val)
    {
      insert(this->begin(), 1, val);
    }
    void Vector::reserve(size_type new_capacity)
    {
      size_type old_capacity = capacity();
      size_type old_size = size();
      if (old_capacity < new_capacity) 
      {
        pointer new_begin(static_cast<pointer>(operator new[](new_capacity * sizeof(value_type))));
        for (size_type i = 0;i < old_size;i++) 
        {
          new (new_begin + i) value_type(p_begin[i]);
          p_begin[i].~value_type();
        }

        if (p_begin != nullptr) 
          operator delete[](p_begin);
        
        p_begin = new_begin;
        p_last = &p_begin[old_size];
        p_end = &p_begin[new_capacity];
      }
    }
    void Vector::shrink_to_fit()
    {
      size_type old_size = size();
      size_type old_capacity = capacity();

      if(old_capacity > old_size)
      {
        pointer new_begin(static_cast<pointer>(operator new[](old_size * sizeof(value_type))));
        for (size_type i = 0;i < old_size;i++) 
        {
          new (new_begin + i) value_type(p_begin[i]);
          p_begin[i].~value_type();
        }

        if (p_begin != nullptr) 
          operator delete[](p_begin);
        
        p_begin = new_begin;
        p_last = &p_begin[old_size];
        p_end = &p_begin[old_size];
      }
    }
}  // namespace I2P2


