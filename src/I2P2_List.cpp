#include "../header/I2P2_List.h"

namespace I2P2 
{
  List::~List()
  {
    clear();
    delete head;
    delete tail;
  }
  List::List()
  {
    head = new Node(0);
    tail = new Node(0);
    head->next = tail;
    tail->prev = head;
    head->prev = nullptr;
    tail->next = nullptr;
    _size = 0;
  }
  List::List(const List &rhs)
  {
    head = new Node(0);
    tail = new Node(0);
    head->next = tail;
    tail->prev = head;
    head->prev = nullptr;
    tail->next = nullptr;
    _size = 0;

    for(Node* now = rhs.head->next; now != rhs.tail; now = now->next)
      push_back(now->data);      
  }
  List& List::operator=(const List &rhs)
  {  
    if(this == &rhs)
      return *this;
    
    clear();

    for(Node* now = rhs.head->next; now != rhs.tail; now = now->next)
      push_back(now->data);      

    return *this;
  }
  iterator List::begin()
  {
    list_iterator head_ptr = list_iterator(head->next);
    iterator p_head(&head_ptr);
    return p_head;
  }
  const_iterator List::begin() const
  {
    list_iterator head_ptr = list_iterator(head->next);
    const_iterator p_head(&head_ptr);
    return p_head;
  }
  iterator List::end()
  {
    list_iterator tail_ptr = list_iterator(tail);
    iterator p_tail(&tail_ptr);
    return p_tail;
  }
  const_iterator List::end() const
  {
    list_iterator tail_ptr = list_iterator(tail);
    const_iterator p_tail(&tail_ptr);
    return p_tail;
  }
  reference List::front()
  {
    return head->next->data;
  }
  const_reference List::front() const
  {
    return head->next->data;
  }
  reference List::back()
  {
    return tail->prev->data;
  }
  const_reference List::back() const
  {
    return tail->prev->data;
  }
  size_type List::size() const
  {
    return _size;
  }
  void List::clear()
  {
    Node* curNode = head->next;
    while (curNode != tail) 
    {
      Node* temp = curNode;
      curNode = curNode->next;
      delete temp;
    }
    head->next = tail;
    tail->prev = head;
    _size = 0;
  }
  bool List::empty() const
  {
    return _size == 0;
  }
  void List::erase(const_iterator pos)
  {
    Node* curnode = pos.node_ref();

    curnode->prev->next = curnode->next;
    curnode->next->prev = curnode->prev;
      
    delete curnode;
    _size--;  
  }
  void List::erase(const_iterator begin, const_iterator end)
  {
    Node* start = begin.node_ref();
    Node* finish = end.node_ref();
    Node* tmp_head = start->prev;

    while(start != finish)
    {
      Node* curnode = start;
      start = start->next;
      delete curnode;
      _size--;
    }

    tmp_head->next = finish;
    finish->prev = tmp_head;
  }
  void List::insert(const_iterator pos, size_type count, const_reference val)
  {
    if(count == 0)
      return;

    Node* targetNode = pos.node_ref();
    Node* tmp_head = new Node(val);
    Node* tmp_tail = tmp_head;
    count--;
    _size++;

    while(count--)
    {
      tmp_tail->next = new Node(val);
      tmp_tail->next->prev = tmp_tail;
      tmp_tail = tmp_tail->next;
      _size++;
    }

    targetNode->prev->next = tmp_head;
    tmp_head->prev = targetNode->prev;
    tmp_tail->next = targetNode;
    targetNode->prev = tmp_tail;      
  }
  void List::insert(const_iterator pos, const_iterator begin, const_iterator end)
  {
    if(begin != end)
    {
      Node* targetNode = pos.node_ref();
        
      Node* tmp_head = new Node(*begin);
      Node* tmp_tail = tmp_head;
      begin++;
      _size++;

      while(begin != end)
      {
        tmp_tail->next = new Node(*begin);
        tmp_tail->next->prev = tmp_tail;
        tmp_tail = tmp_tail->next;
        begin++;
        _size++;
      }

      targetNode->prev->next = tmp_head;
      tmp_head->prev = targetNode->prev;
      tmp_tail->next = targetNode;
      targetNode->prev = tmp_tail;
    }
  }
  void List::pop_back()
  {
    Node* now = tail->prev;
    now->prev->next = tail;
    tail->prev = now->prev;
    delete now;
    _size--;
  }
  void List::pop_front()
  {
    Node* now = head->next;
    now->next->prev = head;
    head->next = now->next;
    delete now;
    _size--;
  }
  void List::push_back(const_reference val)
  {
    Node* newnode = new Node(val);

    tail->prev->next = newnode;
    newnode->prev = tail->prev;
    newnode->next = tail;
    tail->prev = newnode;

    _size++;    
  }
  void List::push_front(const_reference val)
  {
    Node* newnode = new Node(val);

    head->next->prev = newnode;
    newnode->next = head->next;
    newnode->prev = head;
    head->next = newnode;

    _size++;   
  }
}  // namespace I2P2
