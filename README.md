# Vector-and-List-Implementation

## Basic Informations  
Description: An implementation of STL's vector and list data structures.  
Language: C++  

## Data Structures (Vector & List)
### Vector  
Definition: Vectors are sequence containers representing arrays that can change in size.  
Implemented functions:
<pre>
- Constructors  = constructor
- Destructors   = destructor
- begin         = return an iterator to the beginning of the array
- end           = return an iterator to the ending of the array
- front         = access first element
- back          = access last element
- operator[]    = access an element (depending on the parameter)
- capacity      = return size of allocated storage capacity
- size          = return current size of the array (number of elements present in the array)
- clear         = empties the array
- empty         = return a bool whether the array is empty or not
- erase         = erase elements
- insert        = insert elements
- pop_back      = delete the last element
- pop_front     = delete the first element
- push_back     = insert an element at the end of the array
- push_front    = insert an element at the beginning of the array
- reserve       = reverse the array
- shrink_to_fit = shrink the capacity of the array to be the same with its size
</pre>
Official documentation on vector class: [documentation](https://www.cplusplus.com/reference/vector/vector/)  

### List
Definition: Lists are sequence containers that allow constant time insert and erase operations anywhere within the sequence, and iteration in both directions.
Implemented functions:
<pre>
- Constructors  = constructor
- Destructors   = destructor
- begin         = return an iterator to the head node
- end           = return an iterator to the tail node
- front         = access first element
- back          = access last element
- size          = return current size of the list (how many nodes are currently in the list)
- clear         = empties the list
- empty         = return a bool whether the list is empty or not
- erase         = erase elements
- insert        = insert elements
- pop_back      = delete the last element
- pop_front     = delete the first element
- push_back     = insert an element at the end of the list
- push_front    = insert an element at the beginning of the list
</pre>
Official documentation on list class: [documentation](https://www.cplusplus.com/reference/list/list/)  
