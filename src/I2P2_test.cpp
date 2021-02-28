#include <list>
#include <vector>
#include <chrono>
#include <memory>
#include <random>
#include <cassert>
#include <utility>
#include <iostream>
#include <algorithm>
#include <type_traits>

#include "../header/I2P2_def.h"
#include "../header/I2P2_container.h"
#include "../header/I2P2_iterator.h"
#include "../header/I2P2_List.h"
#include "../header/I2P2_Vector.h"

namespace I2P2_test {
const size_t insert_cnt(20);
const unsigned long op_test_cnt(1 << 20);
const auto arithmetic_range(255);

thread_local std::mt19937_64 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());
std::remove_cv<decltype(arithmetic_range)>::type gen() {
  std::uniform_int_distribution<std::remove_cv<decltype(arithmetic_range)>::type> uniform(0, arithmetic_range);
  return uniform(mt);
}

struct Empty {
  template <class... Args>
  void operator()(Args &&... args) {}
};

template <class T>
const T &as_const(T &val) { return val; }

template <class T, class... Args>
void check_iterator_(Args &&... args) {
  T c(std::forward<Args>(args)...);
  I2P2::iterator(c.begin());
  I2P2::iterator(c.end());
  I2P2::const_iterator(c.begin());
  I2P2::const_iterator(c.end());
  I2P2::const_iterator(as_const(c).begin());
  I2P2::const_iterator(as_const(c).end());
}

struct no_tag {};
struct yes_tag {};

bool check(bool correct, const char *message = "fail") {
  if (correct) ;
  else std::cerr << message << '\n';
  return correct;
}

#ifdef TEST_LIST
I2P2::List create_list(size_t count) {
  I2P2::List lst;
  for (size_t i(0); i != count; ++i) lst.push_back(gen());
  return lst;
}
#endif
#ifdef TEST_VECTOR
I2P2::Vector create_vector(size_t count) {
  I2P2::Vector vec;
  for (size_t i(0); i != count; ++i) vec.push_back(gen());
  return vec;
}
#endif

template <class I, class P>
struct iterator_valid_check {
  template <class Iter>
  void store(Iter begin, Iter end) {
    while (begin != end) {
      iterator_.push_back(begin);
      pointer_.push_back(&*begin);
      ++begin;
    }
  }
  template <class Iter>
  bool valid(std::size_t begin, std::size_t end, Iter &src) {
    bool result(true);
    for (; begin != end; ++begin) {
      result &= check(iterator_[begin] == src, "old iterator is not equal to new iterator");
      result &= check(pointer_[begin] == &*src, "old pointer is not equal to new pointer");
      ++src;
    }
    return result;
  }

 private:
  std::vector<I> iterator_;
  std::vector<P> pointer_;
};

struct empty_and_size {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const { return check(c.empty() == s.empty()) && check(c.size() == s.size()); }
};

struct equal {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    return check(std::equal(c.begin(), c.end(), s.begin()), "content of container is not equal");
  }
};

template <class RandomIter>
struct next {
  template <class Iter>
  Iter operator()(Iter iter, std::ptrdiff_t pos = 1) const {
    return next_(iter, pos, typename std::is_same<RandomIter, yes_tag>::type());
  }
 private:
  template <class Iter>
  Iter next_(Iter iter, std::ptrdiff_t pos, std::true_type) const { return iter += pos; }
  template <class Iter>
  Iter next_(Iter iter, std::ptrdiff_t pos, std::false_type) const {
    for (std::ptrdiff_t i(0); i != pos; ++i) ++iter;
    return iter;
  }
};

template <class RandomIter>
struct prev {
  template <class Iter>
  Iter operator()(Iter iter, std::ptrdiff_t pos = 1) const {
    return prev_(iter, pos, typename std::is_same<RandomIter, yes_tag>::type());
  }

 private:
  template <class Iter>
  Iter prev_(Iter iter, std::ptrdiff_t pos, std::true_type) const { return iter -= pos; }
  template <class Iter>
  Iter prev_(Iter iter, std::ptrdiff_t pos, std::false_type) const {
    for (std::ptrdiff_t i(0); i != pos; ++i) --iter;
    return iter;
  }
};

struct back_and_front {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    if (c.size()) return check(c.back() == s.back(), "back is not equal") & check(c.front() == s.front(), "front is not equal");
    return empty_and_size()(c, s);
  }
};

template <class WithCapacity>
struct capacity {
  template <class T>
  std::size_t operator()(T &c) const { return capacity_(c, typename std::is_same<WithCapacity, yes_tag>::type()); }

 private:
  template <class T>
  std::size_t capacity_(T &c, std::true_type) const { return c.capacity(); }
  template <class T>
  std::size_t capacity_(T &c, std::false_type) const { return c.size(); }
};

struct copy_assignment_operator {
  template <class T, class Std>
  bool operator()(T &clhs, T &crhs, Std &slhs, Std &srhs) const {
    clhs = crhs;
    slhs = srhs;
    return equal()(clhs, slhs);
  }
};

struct count {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    const auto key(gen());
    return check(c.count(key) == s.count(key), "count of key is not equal");
  }
};

struct clear {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    c.clear();
    s.clear();
    return equal()(c, s);
  }
};

template <class RandomIter>
struct distance {
  template <class Iter>
  std::ptrdiff_t operator()(Iter lhs, Iter rhs) const {
    return distance_(lhs, rhs, typename std::is_same<RandomIter, yes_tag>::type());
  }

 private:
  template <class Iter>
  std::ptrdiff_t distance_(Iter lhs, Iter rhs, std::true_type) const { return rhs - lhs; }
  template <class Iter>
  std::ptrdiff_t distance_(Iter lhs, Iter rhs, std::false_type) const {
    std::ptrdiff_t dis(0);
    while (lhs != rhs) {
      ++dis;
      ++lhs;
    }
    return dis;
  }
};

template <class CheckAfter, class RandomIter>
struct erase {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    if (c.size()) {
      std::uniform_int_distribution<size_t> pos_gen(0, c.size() - 1);
      const auto pos(pos_gen(mt));
      const auto begin_pos(next<RandomIter>()(c.begin(), pos));
      iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
      iter_check.store(c.begin(), begin_pos);
      if (std::is_same<CheckAfter, yes_tag>::value) iter_check.store(next<RandomIter>()(begin_pos), c.end());
      c.erase(begin_pos);
      s.erase(std::next(s.begin(), pos));
      auto iter_begin(c.begin());
      result &= iter_check.valid(0, pos, iter_begin);
      if (std::is_same<CheckAfter, yes_tag>::value) result &= iter_check.valid(pos, c.size(), iter_begin);
      return result & equal()(c, s);
    }
    return empty_and_size()(c, s);
  }
};

template <class CheckAfter, class RandomIter>
struct erase_range {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    std::uniform_int_distribution<size_t> pos_gen(0, c.size());
    auto begin(pos_gen(mt));
    auto end(pos_gen(mt));
    if (begin > end) std::swap(begin, end);
    const auto begin_begin(next<RandomIter>()(c.begin(), begin));
    const auto begin_end(next<RandomIter>()(c.begin(), end));
    iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
    if (c.size()) {
      iter_check.store(c.begin(), begin_begin);
      if (std::is_same<CheckAfter, yes_tag>::value) iter_check.store(begin_end, c.end());
    }
    c.erase(begin_begin, begin_end);
    s.erase(std::next(s.begin(), begin), std::next(s.begin(), end));
    auto iter_begin(c.begin());
    result &= iter_check.valid(0, begin, iter_begin);
    if (std::is_same<CheckAfter, yes_tag>::value) result &= iter_check.valid(begin, c.size(), iter_begin);
    return result & equal()(c, s);
  }
};

template <class RandomIter>
struct find {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    const auto key(gen()); 
    return check(distance<RandomIter>()(c.begin(), c.find(key)) == std::distance(s.begin(), s.find(key)), "find is not equal");
  }
};

template <class Reallocation, class WithCapacity, class RandomIter>
struct insert {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    std::uniform_int_distribution<size_t> pos_gen(0, std::max<size_t>(1, c.size()) - 1);
    std::uniform_int_distribution<size_t> cnt_gen(0, insert_cnt);
    const auto pos(pos_gen(mt));
    const auto cnt(cnt_gen(mt));
    const auto val(gen());
    const auto old_size(c.size());
    const auto old_capacity(capacity<WithCapacity>()(c));
    iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
    iter_check.store(c.begin(), c.end());
    c.insert(next<RandomIter>()(c.begin(), pos), cnt, val);
    s.insert(std::next(s.begin(), pos), cnt, val);
    if (std::is_same<Reallocation, yes_tag>::value) {
      if ((old_size + cnt) <= old_capacity) {
        auto iter_begin(c.begin());
        result &= iter_check.valid(0, pos, iter_begin);
      }
    } else {
      auto iter_begin(c.begin());
      result &= iter_check.valid(0, pos, iter_begin);
      iter_begin = next<RandomIter>()(iter_begin, cnt);
      result &= iter_check.valid(pos, c.size() - cnt, iter_begin);
    }
    return result & equal()(c, s);
  }
};

template <class RandomIter>
struct insert_key_value {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    const auto key(gen()); 
    const auto val(key);
    iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
    iter_check.store(c.begin(), c.end());
    const auto not_exist(c.find(key) == c.end());
    c.insert(key, val);
    s.insert(key);
    const auto pos(distance<RandomIter>()(c.begin(), c.find(key)));
    auto iter_begin(c.begin());
    result &= iter_check.valid(0, pos, iter_begin);
    if (not_exist) iter_begin = next<RandomIter>()(iter_begin);
    result &= iter_check.valid(pos, c.size() - 1, iter_begin);
    return result & equal()(c, s);
  }
};

template <class WithPos, class Reallocation, class WithCapacity, class RandomIter>
struct insert_range {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    std::uniform_int_distribution<size_t> pos_gen(0, std::max<size_t>(1, c.size()) - 1);
    std::uniform_int_distribution<size_t> cnt_gen(0, insert_cnt);
    const auto pos(pos_gen(mt));
    const auto cnt(cnt_gen(mt));
    const auto old_size(c.size());
    const auto old_capacity(capacity<WithCapacity>()(c));
    iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
    iter_check.store(c.begin(), c.end());
    insert_(c, s, pos, cnt, typename std::is_same<WithPos, yes_tag>::type());
    if (std::is_same<Reallocation, yes_tag>::value && ((old_size + cnt) <= old_capacity)) {
      auto iter_begin(c.begin());
      result &= iter_check.valid(0, pos, iter_begin);
    }
    return result & equal()(c, s);
  }

 private:
  template <class T, class Std>
  void insert_(T &c, Std &s, std::size_t pos, std::size_t count, std::true_type) const {
#ifdef TEST_LIST
    auto lst(create_list(count));
#endif
#ifdef TEST_VECTOR
    auto vec(create_vector(count));
#endif
    switch (std::uniform_int_distribution<>(0, 1)(mt)) {
      case 0:
#ifdef TEST_LIST
        c.insert(next<RandomIter>()(c.begin(), pos), lst.begin(), lst.end());
        s.insert(std::next(s.begin(), pos), lst.begin(), lst.end());
#endif
        break;
      case 1:
#ifdef TEST_VECTOR
        c.insert(next<RandomIter>()(c.begin(), pos), vec.begin(), vec.end());
        s.insert(std::next(s.begin(), pos), vec.begin(), vec.end());
#endif
        break;
      default:
        assert(false);
    }
  }
  template <class T, class Std>
  void insert_(T &c, Std &s, std::size_t, std::size_t count, std::false_type) const {
#ifdef TEST_LIST
    auto lst(create_list(count));
#endif
#ifdef TEST_VECTOR
    auto vec(create_vector(count));
#endif
    switch (std::uniform_int_distribution<>(0, 1)(mt)) {
      case 0:
#ifdef TEST_LIST
        c.insert(lst.begin(), lst.end());
        s.insert(lst.begin(), lst.end());
#endif
        break;
      case 1:
#ifdef TEST_VECTOR
        c.insert(vec.begin(), vec.end());
        s.insert(vec.begin(), vec.end());
#endif
        break;
      default:
        assert(false);
    }
  }
};

template <class RandomIter>
struct lower_bound {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    const auto key(gen());
    return check(distance<RandomIter>()(c.begin(), c.lower_bound(key)) == std::distance(s.begin(), s.lower_bound(key)),
                 "lower_bound is not equal");
  }
};

struct pop_back {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    if (c.size()) {
      iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
      iter_check.store(c.begin(), prev<no_tag>()(c.end()));
      c.pop_back();
      s.pop_back();
      auto iter_begin(c.begin());
      result &= iter_check.valid(0, c.size(), iter_begin);
      return result & equal()(c, s);
    }
    return empty_and_size()(c, s);
  }
};

template <class CheckAfter, class UsrEraseBegin, class StdEraseBegin>
struct pop_front {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    if (c.size()) {
      iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
      if (std::is_same<CheckAfter, yes_tag>::value) iter_check.store(next<no_tag>()(c.begin()), c.end()); 
      pop_front_(c, typename std::is_same<UsrEraseBegin, yes_tag>::type());
      pop_front_(s, typename std::is_same<StdEraseBegin, yes_tag>::type());
      if (std::is_same<CheckAfter, yes_tag>::value) {
        auto iter_begin(c.begin());
        result &= iter_check.valid(0, c.size(), iter_begin);
      }
      return result & equal()(c, s);
    }
    return empty_and_size()(c, s);
  }

 private:
  template <class T>
  void pop_front_(T &c, std::true_type) const { c.erase(c.begin()); }
  template <class T>
  void pop_front_(T &c, std::false_type) const { c.pop_front(); }
};

template <class Reallocation, class WithCapacity>
struct push_back {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    const auto val(gen());
    const auto old_size(c.size());
    const auto old_capacity(capacity<WithCapacity>()(c));
    iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
    iter_check.store(c.begin(), c.end());
    c.push_back(val);
    s.push_back(val);
    if (std::is_same<Reallocation, no_tag>::value || (old_size < old_capacity)) {
      auto iter_begin(c.begin());
      result &= iter_check.valid(0, c.size() - 1, iter_begin);
    }
    return result & equal()(c, s);
  }
};

template <class CheckAfter, class UsrInsertBegin, class StdInsertBegin>
struct push_front {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    const auto val(gen());
    iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
    iter_check.store(c.begin(), c.end());
    push_front_(c, val, typename std::is_same<UsrInsertBegin, yes_tag>::type());
    push_front_(s, val, typename std::is_same<StdInsertBegin, yes_tag>::type());
    if (std::is_same<CheckAfter, yes_tag>::value) {
      auto iter_begin(next<no_tag>()(c.begin()));
      result &= iter_check.valid(0, c.size() - 1, iter_begin);
    }
    return result & equal()(c, s);
  }

 private:
  template <class T, class V>
  void push_front_(T &c, const V &val, std::true_type) const { c.insert(c.begin(), 1, val); }
  template <class T, class V>
  void push_front_(T &c, const V &val, std::false_type) const { c.push_front(val); }
};

struct random_access {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    if (c.size()) {
      std::uniform_int_distribution<std::size_t> pos_gen(0, c.size() - 1);
      const auto pos(pos_gen(mt));
      result &= check(c[pos] == s[pos], "your[x] is not equal to std[x]");
      auto data(&c[0]);
      for (std::size_t i(0); i != c.size(); ++i) ++data;
      result &= check((data - &c[0]) == c.size(), "memory is not consecutive");
    }
    return result & empty_and_size()(c, s);
  }
};

template <class WithAlgo>
struct random_algo {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    std::sort(c.begin(), c.end());
    c.erase(std::unique(c.begin(), c.end()), c.end());
    random_algo_(s, typename std::is_same<WithAlgo, yes_tag>::type());
    const auto result(equal()(c, s));
    std::shuffle(c.begin(), c.end(), mt);
    std::copy(c.begin(), c.end(), s.begin());
    return result & equal()(c, s);
  }

 private:
  template <class Std>
  void random_algo_(Std &s, std::true_type) const {
    s.sort();
    s.unique();
  }
  template <class Std>
  void random_algo_(Std &s, std::false_type) const {
    std::sort(s.begin(), s.end());
    s.erase(std::unique(s.begin(), s.end()), s.end());
  }
};

template <class WithCapacity>
struct reserve {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    bool result(true);
    std::uniform_int_distribution<size_t> new_capacity_gen
      (std::max<size_t>(1, capacity<WithCapacity>()(c)) - 1, capacity<WithCapacity>()(c) + 1);
    const auto new_capacity(new_capacity_gen(mt));
    const auto old_capacity(capacity<WithCapacity>()(c));
    iterator_valid_check<decltype(c.begin()), decltype(std::addressof(*c.begin()))> iter_check;
    iter_check.store(c.begin(), c.end());
    c.reserve(new_capacity);
    if (new_capacity <= old_capacity) {
      auto iter_begin(c.begin());
      result &= iter_check.valid(0, c.size(), iter_begin);
    }
    return result & equal()(c, s);
  }
};

struct shrink_to_fit {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    c.shrink_to_fit();
    return equal()(c, s);
  }
};

template <class RandomIter>
struct upper_bound {
  template <class T, class Std>
  bool operator()(T &c, Std &s) const {
    const auto key(gen());  
    return check(distance<RandomIter>()(c.begin(), c.lower_bound(key)) == std::distance(s.begin(), s.lower_bound(key)),
                 "lower_bound is not equal");
  }
};

struct iterator_iterate {
  template <class T>
  bool operator()(T &&c) const {
    bool result(true);
    result &= check(c.begin() == c.begin(), "begin is not equal to begin");
    result &= check(c.end() == c.end(), "end is not equal to end");
    if (c.size()) {
      result &= check((c.begin()++) == c.begin(), "begin++ is not equal to begin");
      result &= check((c.end()--) == c.end(), "end-- is not equal to end");
      std::uniform_int_distribution<std::size_t> uniform(0, c.size() - 1);
      const auto inc(uniform(mt));
      const auto dec(c.size() - inc);
      auto begin1(c.begin());
      auto begin2(c.begin());
      auto end1(c.end());
      auto end2(c.end());
      for (typename std::remove_cv<decltype(inc)>::type i(0); i != inc; ++i)
        if (check((begin1++) == begin2, "postfix increment incorrect")) ++begin2;
        else { result &= false; break; }
      for (typename std::remove_cv<decltype(dec)>::type i(0); i != dec; ++i)
        if (check((end1--) == end2, "postfix decrement incorrect")) --end2;
        else { result &= false; break; }
      result &= check(begin1 == end1, "postfix increment is not equal to postfix decrement");
      result &= check(begin1 == end2, "postfix increment is not equal to prefix decrement");
      result &= check(begin2 == end1, "prefix increment is not equal to postfix decrement");
      result &= check(begin2 == end2, "prefix increment is not equal to prefix decrement");
    } else result &= check(c.begin() == c.end(), "begin is not equal to end when container is empty");
    return result;
  }
};

struct iterator_random {
  template <class T>
  bool operator()(T &&c) const {
    bool result(true);
    result &= check((c.end() - c.begin()) == c.size(), "(end - begin) is not equal to size");
    result &= check(c.begin() <= c.end(), "begin is not smaller than or equal to end");
    result &= check(c.end() >= c.begin(), "end is not greater than or equal to begin");
    if (c.size()) {
      result &= check(c.begin() < c.end(), "begin is not smaller than end");
      result &= check(c.end() > c.begin(), "end is not greater than begin");
      std::uniform_int_distribution<std::size_t> uniform(0, c.size() - 1);
      const auto inc(uniform(mt));
      const std::ptrdiff_t dec(c.size() - inc);
      auto begin(c.begin());
      auto end(c.end());
      result &= check((c.begin() + inc) == (c.end() - dec), "(begin + x) is not equal to (end - (size - x))");
      end = c.end();
      end -= dec;
      result &= check((c.begin() + inc) == end, "(begin + x) is not equal to (end -= (size - x))");
      begin = c.begin();
      begin += inc;
      result &= check(begin == (c.end() - dec), "(begin += x) is not equal to (end - (size - x))");
      begin = c.begin();
      begin += inc;
      end = c.end();
      end -= dec;
      result &= check(begin == end, "(begin += x) is not equal to (end -= (size - x))");
      result &= check(c.begin()[inc] == c.end()[-dec], "begin[x] is not equal to end[-(size - x)]");
    }
    return result;
  }
};

void check_hierarchy() {
#ifdef TEST_LIST
  using container_base_is_List_base = std::enable_if<std::is_base_of<I2P2::container_base, I2P2::List>::value>::type;
#endif
#ifdef TEST_VECTOR
  using container_base_is_Vector_base = std::enable_if<std::is_base_of<I2P2::container_base, I2P2::Vector>::value>::type;
#endif
}

void check_iterator() {
#ifdef TEST_LIST
  check_iterator_<I2P2::List>();
#endif
#ifdef TEST_VECTOR
  check_iterator_<I2P2::Vector>();
#endif
}

#ifdef TEST_LIST
void check_list() {
  using test_type = I2P2::List;
  using std_type = std::list<I2P2::value_type>;
  using CheckAfter = yes_tag;
  using RandomIter = yes_tag;
  using Reallocation = no_tag;
  using StdEraseBegin = no_tag;
  using UsrEraseBegin = no_tag;
  using WithAlgo = yes_tag;
  using WithCapacity = yes_tag;
  using WithPos = yes_tag;
  using WithReserve = yes_tag;
  using WithShrink = yes_tag;
  std::vector<std::unique_ptr<test_type>> usr;
  usr.push_back(std::unique_ptr<test_type>(new test_type()));
  usr.push_back(std::unique_ptr<test_type>(new test_type()));
  std::vector<std_type> stdc(2);
  std::uniform_int_distribution<> mode(0, 36);
  std::uniform_int_distribution<> which(0, 1);
  for (unsigned long i(0); i != op_test_cnt; ++i) {
    const auto select(which(mt));
    switch (mode(mt)) {
      case 0:  
        usr[select].reset(new test_type());
        stdc[select].clear();
        break;
      case 1:  
      case 2:
        usr[select].reset(new test_type(*usr[1 - select]));
        stdc[select] = stdc[1 - select];
        break;
      case 3:
      case 4:
        copy_assignment_operator()(*usr[select], *usr[1 - select], stdc[select], stdc[1 - select]);
        break;
      case 5:
        back_and_front()(*usr[select], stdc[select]);
        back_and_front().operator()<const test_type &>(*usr[select], stdc[select]);
        break;
      case 6:
        clear()(*usr[select], stdc[select]);
        empty_and_size()(*usr[select], stdc[select]);
        break;
      case 7:
      case 8:
      case 9:
        erase<CheckAfter, RandomIter>()(*usr[select], stdc[select]);
        break;
      case 10:
      case 11:
      case 12:
        erase_range<CheckAfter, RandomIter>()(*usr[select], stdc[select]);
        break;
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
        insert<Reallocation, WithCapacity, RandomIter>()(*usr[select], stdc[select]);
        break;
      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
        insert_range<WithPos, Reallocation, WithCapacity, RandomIter>()(*usr[select], stdc[select]);
        break;
      case 23:
        pop_back()(*usr[select], stdc[select]);
        break;
      case 24:
      case 25:
        pop_front<CheckAfter, UsrEraseBegin, StdEraseBegin>()(*usr[select], stdc[select]);
        break;
      case 26:
        push_back<Reallocation, WithCapacity>()(*usr[select], stdc[select]);
        break;
      case 27:
      case 28:
        push_front<CheckAfter, UsrEraseBegin, StdEraseBegin>()(*usr[select], stdc[select]);
        break;
      case 29:
      case 30:
      case 31:
      case 32:
        random_algo<WithAlgo>()(*usr[select], stdc[select]);
        break;
      case 33:
      case 34:
        std::conditional<std::is_same<WithReserve, yes_tag>::value, reserve<WithCapacity>, Empty>::type()
          (*usr[select], stdc[select]);
        break;
      case 35:
      case 36:
        std::conditional<std::is_same<WithShrink, yes_tag>::value, shrink_to_fit, Empty>::type()
          (*usr[select], stdc[select]);
        break;
      default:
        assert(false);
    }
    iterator_iterate()(*usr[select]);
    iterator_iterate().operator()<const test_type &>(*usr[select]);
    std::conditional<std::is_same<RandomIter, yes_tag>::value, iterator_random, Empty>::type()
      (*usr[select]);
    std::conditional<std::is_same<RandomIter, yes_tag>::value, iterator_random, Empty>::type()
      .operator()<const test_type &>(*usr[select]);
  }
}
#endif

#ifdef TEST_VECTOR
void check_vector() {
  using test_type = I2P2::Vector;
  using std_type = std::vector<I2P2::value_type>;
  using CheckAfter = no_tag;
  using RandomIter = yes_tag;
  using Reallocation = yes_tag;
  using StdEraseBegin = yes_tag;
  using UsrEraseBegin = no_tag;
  using WithAlgo = no_tag;
  using WithCapacity = yes_tag;
  using WithPos = yes_tag;
  using WithReserve = yes_tag;
  using WithShrink = yes_tag;
  std::vector<std::unique_ptr<test_type>> usr;
  usr.push_back(std::unique_ptr<test_type>(new test_type()));
  usr.push_back(std::unique_ptr<test_type>(new test_type()));
  std::vector<std_type> stdc(2);
  std::uniform_int_distribution<> mode(0, 37);
  std::uniform_int_distribution<> which(0, 1);
  for (unsigned long i(0); i != op_test_cnt; ++i) {
    const auto select(which(mt));
    switch (mode(mt)) {
      case 0:  
        usr[select].reset(new test_type());
        stdc[select].clear();
        break;
      case 1:  
      case 2:
        usr[select].reset(new test_type(*usr[1 - select]));
        stdc[select] = stdc[1 - select];
        break;
      case 3:
      case 4:
        copy_assignment_operator()(*usr[select], *usr[1 - select], stdc[select], stdc[1 - select]);
        break;
      case 5:
        back_and_front()(*usr[select], stdc[select]);
        back_and_front().operator()<const test_type &>(*usr[select], stdc[select]);
        break;
      case 6:
        clear()(*usr[select], stdc[select]);
        empty_and_size()(*usr[select], stdc[select]);
        break;
      case 7:
      case 8:
      case 9:
        erase<CheckAfter, RandomIter>()(*usr[select], stdc[select]);
        break;
      case 10:
      case 11:
      case 12:
        erase_range<CheckAfter, RandomIter>()(*usr[select], stdc[select]);
        break;
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
        insert<Reallocation, WithCapacity, RandomIter>()(*usr[select], stdc[select]);
        break;
      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
        insert_range<WithPos, Reallocation, WithCapacity, RandomIter>()( *usr[select], stdc[select]);
        break;
      case 23:
        pop_back()(*usr[select], stdc[select]);
        break;
      case 24:
      case 25:
        pop_front<CheckAfter, UsrEraseBegin, StdEraseBegin>()(*usr[select], stdc[select]);
        break;
      case 26:
        push_back<Reallocation, WithCapacity>()(*usr[select], stdc[select]);
        break;
      case 27:
      case 28:
        push_front<CheckAfter, UsrEraseBegin, StdEraseBegin>()(*usr[select], stdc[select]);
        break;
      case 29:
        random_access()(*usr[select], stdc[select]);
        random_access().operator()<const test_type &>(*usr[select], stdc[select]);
        break;
      case 30:
      case 31:
      case 32:
      case 33:
        random_algo<WithAlgo>()(*usr[select], stdc[select]);
        break;
      case 34:
      case 35:
        std::conditional<std::is_same<WithReserve, yes_tag>::value, reserve<WithCapacity>, Empty>::type()
          (*usr[select], stdc[select]);
        break;
      case 36:
      case 37:
        std::conditional<std::is_same<WithShrink, yes_tag>::value, shrink_to_fit, Empty>::type()
          (*usr[select], stdc[select]);
        break;
      default:
        assert(false);
    }
    iterator_iterate()(*usr[select]);
    iterator_iterate().operator()<const test_type &>(*usr[select]);
    std::conditional<std::is_same<RandomIter, yes_tag>::value, iterator_random, Empty>::type()
      (*usr[select]);
    std::conditional<std::is_same<RandomIter, yes_tag>::value, iterator_random, Empty>::type()
      .operator()<const test_type &>(*usr[select]);
  }
}
#endif
}  // namespace I2P2_test

/* This file should not be modified */

