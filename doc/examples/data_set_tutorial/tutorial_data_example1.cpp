#include <iostream>
#include "ltr/data/object.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;

int main() {
  Object obj;
  obj << 1.1 << 45.3 << 1e6;              // Object contains now 3 features
  cout << "obj = " << obj << endl;        // You can print an object

  cout << "obj.feature_count() = " 
	   << obj.feature_count() << endl;     // Method featureCount returns count of features in object

  obj << 2.4;                              // You can add new features at any time.
  cout << "feature was added" << endl;

  cout << "obj.feature_count() = " 
	   << obj.feature_count() << endl;     // Returns 4

  cout << "obj[0] = "  << obj[0] << ", "          // Two ways to get the feature value.
       << "obj.at(1) = "  << obj.at(1) << endl;   //

  Eigen::VectorXd new_features(3);
  new_features << 1, 5.0, 2;

  obj.set_eigen_features(new_features);  // You can also set all objects features at once.
  cout << "obj = " << obj;
  return 0;
}

