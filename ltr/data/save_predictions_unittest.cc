// Copyright 2012 Yandex School Practice

#include <stdlib.h>
#include <string>
#include <fstream>

#include "gtest/gtest.h"

#include "ltr/data/object.h"
#include "ltr/data/object_list.h"
#include "ltr/data/data_set.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/random_number_generator.h"

using std::cin;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::remove;
using std::string;

using ltr::DataSet;
using ltr::Object;
using ltr::utility::randomizer;

TEST(SavePredictionsTest, TestSavingOfPredictions) {
  int objects_count = 7;
  int features_count = 3;
  DataSet<Object> data;

  for (int object_index = 0; object_index < objects_count; ++object_index) {
    Object object;
    for (int feature_index = 0; feature_index < features_count; ++feature_index) {
      double feature_value = randomizer.rand() % 1337;
      feature_value += double(randomizer.rand()) / 1000.0;
      object << feature_value;
    }
    double label = randomizer.rand() % 1337;
    label += double(randomizer.rand()) / 1337;
    object.set_predicted_label(label);
    data.add(object);
  }

  string filename = "prediction.txt";
  EXPECT_NO_THROW(ltr::io_utility::savePredictions(data, filename));

  ifstream file(filename.c_str());
  for (int object_index = 0; object_index < objects_count; ++object_index) {
    double label;
    EXPECT_NO_THROW(file >> label);
    EXPECT_TRUE(ltr::utility::DoubleEqual(label, data[object_index].predicted_label()));
  }
  file.close();
  remove(filename.c_str());
}
