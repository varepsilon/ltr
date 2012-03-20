// Copyright 2012 Yandex

#include "ltr/data/data_set.h"
#include "ltr/data/utility/io_utility.h"
#include "serialization_test/config.h"

using ltr::Object;
using ltr::DataSet;
using ltr::io_utility::loadDataSet;


int main(int argc, char* argv[]) {
  DataSet<Object> data = loadDataSet<Object>(data_path.string(), "SVMLITE");
	return 0;
}
