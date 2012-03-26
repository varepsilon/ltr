#include <iostream>
#include "ltr/data/object.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;

int main() {
  Object obj1, obj2;

  obj1 << 2 << 4 << 6 << 8;

  obj2 = obj1;                    // Simple copy

  cout << obj2.features()[1] << endl;
  obj1.features()[1] = -1;          // After changing value in obj1
  cout << obj2.features()[1]        // Value in obj2 changes too
       << endl << endl;

  obj2 = obj1.deepCopy();

  cout << obj2.features()[1] << endl;
  obj1.features()[1] = 5;           // Now nothing changes in obj2
  cout << obj2.features()[1];       // After changing obj1

  return 0;
}