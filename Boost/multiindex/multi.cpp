#include<boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace boost::multi_index;

struct animal {
  string name;
  int legs;
};

typedef multi_index_container<
    animal,
    indexed_by<hashed_non_unique<member<animal, string, &animal::name> >,
               hashed_non_unique<member<animal, int, &animal::legs> > > >
    animal_multi;

int main() {
  animal_multi animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  cout << animals.count("cat") << endl;

  const animal_multi::nth_index<1>::type &legs_index = animals.get<1>();
  cout << legs_index.count(8) << endl;
}
