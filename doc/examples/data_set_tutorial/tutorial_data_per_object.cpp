#include <iostream>

#include "ltr/data/object_list.h"
#include "ltr/data/per_object_accessor.h"

using std::cout;
using std::vector;
using std::endl;
using ltr::Object;
using ltr::ObjectList;
using ltr::PerObjectAccessor;

template <class TElement>
void PrintActualLabels(const TElement &element) {
  PerObjectAccessor<const TElement> per_object_accessor(&element);
  for (int object_index = 0;
	   object_index < per_object_accessor.object_count(); ++object_index) {
	cout << "actual[" << object_index << "] ="
		 << per_object_accessor.object(object_index).actual_label() << endl;
  }
}

int main() {
  ObjectList list;
  Object obj1, obj2, obj3, obj4;
  obj1.set_actual_label(10);
  obj2.set_actual_label(20);
  obj3.set_actual_label(30);
  obj4.set_actual_label(40);
  list << obj1 << obj2 << obj3 << obj4;

  PrintActualLabels(list);
  return 0;
}