#include <iostream>
#include "ltr/data/object.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;

int main() {
  Object obj;
  obj.set_actual_label(23.65);
  obj.set_predicted_label(21.12);

  cout << obj.actual_label() << endl;
  cout << obj.predicted_label() << endl;
  cout << obj;
  return 0;
}