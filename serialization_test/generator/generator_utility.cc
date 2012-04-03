// Copyright 2012 Yandex

#include <boost/lexical_cast.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "serialization_test/generator/generator_utility.h"
#include "serialization_test/generator/config.h"
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
      append("#include <string>\n").
      append("#include <iostream>\n\n");
    output.append("#include \"serialization_test/generator/config.h\"\n").
      append("#include \"serialization_test/tester/tester_utility.h\"\n\n").
      append("#include \"ltr/data/data_set.h\"\n").
      append("#include \"ltr/data/utility/io_utility.h\"\n\n");
    output.append("using std::vector;\n").
      append("using std::string;\n").
      append("using std::cout;\n").
      append("using ltr::Object;\n").
      append("using ltr::DataSet;\n").
      append("using ltr::io_utility::loadDataSet;\n\n").
      append("using namespace serialization_test;\n\n");

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

  string Generator::setMessageFunction(string function_name,
      string error_message) const {
    string output;
    output.append("string " + function_name +
      "() { return \"" + error_message + "\"; }\n\n");
    return output;
  }

  Generator::Generator():
    tab("  "),
    train_data(loadDataSet<Object>(TrainDataPath(), "SVMLITE")),
    test_data(loadDataSet<Object>(TestDataPath(), "SVMLITE")),
    tester_code(setIncludes()),
    scorers_to_test(0) {}

  string Generator::code() const {
    return tester_code;
  }

  void Generator::write(const char* filename) const {
    ofstream fout(filename);
    fout << code();
    fout.close();
  }

  void Generator::setMainCode() {
    tester_code.append("int main() {\n");
    tester_code.append(tab + "DataSet<Object> test_data = ").
      append("loadDataSet<Object>(TestDataPath(), \"SVMLITE\");\n");
    tester_code.append(tab + "vector<double> serializated_labels;\n").
      append(tab + "vector<double> test_labels;\n").
      append(tab + "bool ok = true;\n\n");

    for (int i = 0; i < scorers_to_test; ++i) {
      string function_number = boost::lexical_cast<string>(i);
      tester_code.append(tab + "serializated_labels = ").
        append("ApplySerializatedScorerToDataSet(test_data, &SavedScorer" +
        function_number + ");\n");
      tester_code.append(tab + "test_labels = SetupTestLabels"
        + function_number + "();\n");
      tester_code.append(tab + "ok = ok && CompareReport(").
        append("test_labels, serializated_labels, ").
        append("Message" + function_number + "());\n\n");
    }

    tester_code.append(tab + "return TotalReport(ok);\n}\n");
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
      string error_message) {
    Scorer::Ptr tested_scorer = learner->makeScorerPtr();
    MarkDataSet(test_data, *tested_scorer);

    string function_number = boost::lexical_cast<string>(scorers_to_test);

    tester_code.append(setBeginBlockComment(error_message));
    tester_code.append(setTestLabelsFunction("SetupTestLabels"
      + function_number));
    tester_code.append(setMessageFunction("Message"
      + function_number, error_message));
    tester_code.append(tested_scorer->generateCppCode(
      "SavedScorer" + function_number) + "\n");
    tester_code.append(setEndBlockComment(error_message));

    ++scorers_to_test;
  }
};
