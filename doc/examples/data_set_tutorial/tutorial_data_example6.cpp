#include <iostream>
#include "ltr/data/object_list.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;
using ltr::ObjectList;

int main() {
  ObjectList list;
  Object obj1, obj2, obj3, obj4;
  obj1 << 1.4 << 5.6 << 12.5 << 56.88;
  obj2 << 2.4 << 7.6 << 42.5 << 12.88;
  list << obj1 << obj2;

  cout << "list.at(0) = " << list.at(0) << endl;
  cout << "list[1] = " << list[1] << endl;

  cout << "size of list: " << list.size() << endl;
  list.clear();
  cout << "list cleared" << endl;
  cout << "size of list: " << list.size() << endl;
  return 0;
}

