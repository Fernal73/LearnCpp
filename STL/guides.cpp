#include <iterator>
#include <type_traits>
using namespace std;

template <typename T>
struct MyVec {
  template <typename Iter>
  MyVec(Iter, Iter) {
  }
};

template <typename Iter>
MyVec(Iter, Iter) -> MyVec<typename iterator_traits<Iter>::value_type>;

template <typename A, typename B>
struct MyAdvancedPair {
  template <typename T, typename U>
  MyAdvancedPair(T &&, U &&) {
  }
};

template <typename X, typename Y>
MyAdvancedPair(X, Y) -> MyAdvancedPair<X, Y>;

int main() {
  int *ptr = nullptr;
  MyVec v(ptr, ptr);
  static_assert(is_same_v<decltype(v), MyVec<int>>);
  MyAdvancedPair adv(1729, "taxicab");
  static_assert(
      is_same_v<decltype(adv), MyAdvancedPair<int, const char *>>);
}
