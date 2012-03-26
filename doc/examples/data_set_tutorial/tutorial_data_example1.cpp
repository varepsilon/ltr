#include <iostream>
#include "ltr/data/object.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;

int main() {
  Object obj;
  obj << 1.1 << 45.3 << 23 << 1e8;        // Object contains now 4 features

  cout << obj.featureCount() << endl;     // Method featureCount returns count of features in object

  obj << 2.4;                             // You can add new features at any time.
  cout << obj.featureCount()              // Returns 5
       << endl << endl;

  cout << obj.features()[0] << " "        // Two ways to get the feature value.
       << obj.features().at(1) << endl;   //

  vector<double> new_features;
  new_features.push_back(1);
  new_features.push_back(5.0);
  new_features.push_back(2);

  obj.features() = new_features;          // You can also set all objects features an once.
  return 0;
}