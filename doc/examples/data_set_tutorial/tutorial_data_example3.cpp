#include <iostream>
#include <stdexcept>
#include "ltr/data/object.h"

using std::cout;
using std::vector;
using std::endl;
using std::logic_error;
using ltr::Object;

int main() {
  Object obj;

  obj.setMetaInfo("info1", "value1");
  cout << obj.getMetaInfo("info1") << endl;

  try {                               //
    obj.getMetaInfo("info2");         // Exception, because Object doesn't have meta info named "info2".
  } catch(logic_error err) {          //
    cout << err.what() << endl;       //
  }                                   //

  obj.setMetaInfo("info2", "value2");
  cout << obj.getMetaInfo("info2") << endl;
  return 0;
}