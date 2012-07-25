#include <iostream>
#include "ltr/data/object_pair.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;
using ltr::ObjectPair;

int main() {
  ObjectPair pair1;
  Object obj1, obj2;
  obj1 << 1 << 34 << 5 << 3;
  obj2 << 4 << 65 << 2 << 5;
  pair1.first = obj1;
  pair1.second = obj2;

  cout << pair1.first.features()[0] << " "
       << pair1.first.features()[3] << endl;
  cout << pair1.first.features()[1] << " "
       << pair1.second.features()[2];
  return 0;
}