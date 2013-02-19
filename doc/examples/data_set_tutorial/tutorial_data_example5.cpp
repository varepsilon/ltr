#include <iostream>
#include "ltr/data/object_pair.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;
using ltr::ObjectPair;

int main() {
  ObjectPair pair;
  Object obj1, obj2;
  obj1 << 1 << 2 << 3 << 4;
  obj2 << 4 << 6 << 7 << 8;
  pair.first = obj1;
  pair.second = obj2;

  cout << "pair = " << pair << endl;

  return 0;
}

