#include <boost/unordered_set.hpp>
#include <cstddef>
#include <string>

using namespace std;

struct animal {
  string name;
  int legs;
};

bool operator==(const animal &lhs, const animal &rhs) {
  return lhs.name == rhs.name && lhs.legs == rhs.legs;
}

size_t hash_value(const animal &a) {
  size_t seed = 0;
  boost::hash_combine(seed, a.name);
  boost::hash_combine(seed, a.legs);
  return seed;
}

int main() {
  typedef boost::unordered_set<animal> unordered_set;
  unordered_set animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});
}
