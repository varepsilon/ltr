#include <iostream>
#include "ltr/data/object.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;

int main() {
  Object obj;

  obj.setActualLabel(23.65);
  cout << obj.actualLabel() << endl;

  obj.setPredictedLabel(21.12);
  cout << obj.predictedLabel();
  return 0;
}