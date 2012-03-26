#include <iostream>
#include "ltr/data/data_set.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;
using ltr::DataSet;

int main() {
  DataSet<Object> data;
  Object obj1, obj2, obj3, obj4;

  obj1 << 12.0 << 54.11;
  obj2 << 1.30 << 4.15;
  obj3 << 10.30 << 14.3;
  obj4 << 19.0 << 47.5;

  data << obj1 << obj2;
  data.add(obj3, 2.12);
  data.add(obj4, 1.21);

  cout << "data.size() = " << data.size() << endl;
  cout << "data[0] = " << data[0] << endl;
  cout << "data.at(1) = " << data.at(1) << endl;
  cout << "data.getWeight(2) = " << data.getWeight(2) << endl;

  data.setWeight(2, 0.01);
  cout << "data.getWeight(2) = " << data.getWeight(2) << endl;

  data.erase(1);
  cout << "called data.erase(1)" << endl;
  cout << "data[1] = " << data[1] << endl;

  data.clear();
  cout << "called data.clear()" << endl;
  cout << "data.size() = " << data.size() << endl;
  return 0;
}