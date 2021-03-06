#include <boost/intrusive/list.hpp>
#include <iostream>
#include <string>
#include <utility>

using namespace std;
using namespace boost::intrusive;

typedef link_mode<auto_unlink> mode;

struct animal : public list_base_hook<mode> {
  string name;
  int legs;
  animal(string n, int l) : name{move(n)}, legs{l} {
  }
};

int main() {
  animal a1{"cat", 4};
  animal a2{"shark", 0};
  animal *a3 = new animal{"spider", 8};

  typedef constant_time_size<false> constant_time_size;
  typedef list<animal, constant_time_size> animal_list;
  animal_list animals;

  animals.push_back(a1);
  animals.push_back(a2);
  animals.push_back(*a3);

  delete a3;

  for (const animal &a: animals)
    cout << a.name << " ";
  cout << endl;
}
