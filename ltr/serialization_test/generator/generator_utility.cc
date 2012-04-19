// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "ltr/serialization_test/generator/generator_utility.h"
#include "ltr/serialization_test/generator/config.h"
#include "ltr/scorers/scorer.h"
#include "ltr/data/data_set.h"
#include "ltr/data/utility/io_utility.h"
#include "ltr/scorers/utility/scorer_utility.h"

using std::string;
using std::vector;
using std::cout;
using std::ofstream;
using std::copy;

using ltr::DataSet;
using ltr::Object;
using ltr::io_utility::loadDataSet;
using ltr::utility::MarkDataSet;
using ltr::Scorer;

namespace serialization_test {
  string Generator::setIncludes() const {
    string output;
    output.append("// Copyright 2012 Yandex\n\n").
      append("#include <vector>\n").
      append("#include \"gtest/gtest.h\"\n\n");
    output.append("#include \"ltr/serialization_test/generator/config.h\"\n").
      append("#include \"ltr/serialization_test/tester/tester_utility.h\"\n\n").
      append("#include \"ltr/data/data_set.h\"\n").
      append("#include \"ltr/data/utility/io_utility.h\"\n\n");
    output.append("using std::vector;\n").
      append("using ltr::Object;\n").
      append("using ltr::DataSet;\n").
      append("using ltr::io_utility::loadDataSet;\n\n").
      append("using namespace serialization_test;\n\n");

    return output;
  }

  string Generator::setFixture() const {
    string output;
    output.append("class SerializationTest : public ::testing::Test {\n").
      append(tab + "protected:\n").
      append(tab + "DataSet<Object> test_data;\n").
      append(tab + "vector<double> serializated_labels;\n").
      append(tab + "vector<double> test_labels;\n\n").
      append(tab + "virtual void SetUp() {\n").
      append(tab + tab + "test_data = loadDataSet<Object>" +
        "(TestDataPath(), \"SVMLITE\");\n").
      append(tab + "}\n").
      append("};\n\n");
      return output;
  }

  string Generator::setTestLabelsFunction(string function_name) const {
    vector<double> labels;
    for (int i = 0; i < test_data.size(); ++i) {
      labels.push_back(test_data[i].predictedLabel());
    }

    string output;
    output.append("vector<double> " + function_name + "() {\n");
    output.append(tab + "vector<double> test_labels;\n");
    for (int i = 0; i < labels.size(); ++i) {
      output.append(tab + "test_labels.push_back(" +
        boost::lexical_cast<string>(labels[i]) + ");\n");
    }
    output.append(tab + "return test_labels;\n}\n\n");

    return output;
  }

  string Generator::setTestCode(int index,
      string test_name) const {
    string output;
    output.append("TEST_F(SerializationTest, " + test_name +
      ") {\n");

    string function_number = boost::lexical_cast<string>(index);
    output.append(tab + "serializated_labels = ").
      append("ApplySerializatedScorerToDataSet(test_data, &SavedScorer" +
      function_number + ");\n");
    output.append(tab + "test_labels = SetupTestLabels"
      + function_number + "();\n");
    output.append(tab + "EXPECT_TRUE(Equal(").
      append("test_labels, serializated_labels));\n").
      append("}\n\n");
    return output;
  }

  Generator::Generator():
      tab("  "),
      train_data(loadDataSet<Object>(TrainDataPath(), "SVMLITE")),
      test_data(loadDataSet<Object>(TestDataPath(), "SVMLITE")),
      tester_code(setIncludes()),
      scorers_to_test(0) {
    tester_code.append(setFixture());
  }

  string Generator::code() const {
    return tester_code;
  }

  void Generator::write(const char* filename) const {
    ofstream fout(filename);
    fout << code();
    fout.close();
  }

  string Generator::setBeginBlockComment(string message) const {
    string slashes(100, '#');
    string inserted = message + "_block_begin";
    copy(inserted.begin(), inserted.end(), slashes.begin() + 35);
    return "//" + slashes + "\n";
  }

  string Generator::setEndBlockComment(string message) const {
    string slashes(100, '#');
    string inserted = message + "_block_end";
    copy(inserted.begin(), inserted.end(), slashes.begin() + 35);
    return "//" + slashes + "\n\n";
  }

  void Generator::setScorerTest(BaseLearner<Object>::Ptr learner,
      string test_name) {
    Scorer::Ptr tested_scorer = learner->makeScorerPtr();
    MarkDataSet(test_data, *tested_scorer);

    string function_number = boost::lexical_cast<string>(scorers_to_test);

    tester_code.append(setBeginBlockComment(test_name));
    tester_code.append(setTestLabelsFunction("SetupTestLabels"
      + function_number));
    tester_code.append(tested_scorer->generateCppCode(
      "SavedScorer" + function_number) + "\n");
    tester_code.append(setTestCode(scorers_to_test, test_name));
    tester_code.append(setEndBlockComment(test_name));

    ++scorers_to_test;
  }
};
