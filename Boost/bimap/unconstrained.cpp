#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include <boost/bimap/unconstrained_set_of.hpp>
#include <iostream>
#include <string>

using namespace std;

int main() {
  typedef boost::bimap<string, boost::bimaps::unconstrained_set_of<int>> bimap;
  bimap animals;

  animals.insert({"cat", 4});
  animals.insert({"shark", 0});
  animals.insert({"spider", 8});

  auto it = animals.left.find("cat");
  animals.left.modify_key(it, boost::bimaps::_key = "dog");

  cout << it->first << endl;
}
