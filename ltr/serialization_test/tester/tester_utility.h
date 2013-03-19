// Copyright 2012 Yandex

#ifndef LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
#define LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_

#include <ostream>
#include <vector>
#include <string>

#include "ltr/data/data_set.h"
#include "ltr/data/object.h"

using std::vector;
using std::string;
using std::ostream;
using ltr::DataSet;
using ltr::Object;

namespace serialization_test {
  /**
   * \brief Epsilon for comparison doubles.
   */
  const double COMPARE_EPS = 1e-10;

  /**
   * \brief Compares two vector<double>.
   */
  bool Equal(const vector<double>& left, const vector<double>& right);
  /**
   * \brief Compares two vector<vector<double> >.
   */
  bool Equal(const vector<vector<double> >& left,
             const vector<vector<double> >& right);

  /**
   * \brief Class for publishing data in case of failed comparison of two
   * objects
   */
  template <class TData>
  class Report {
  public:
    /**
     * \brief Constructs class from two objects.
     */
    Report(const TData& in_first, const TData& in_second)
      : first(in_first), second(in_second) {}
    /**
     * \brief Writes objects to stream.
     * \param output output stream.
     */
    void write(ostream* output) const {}
  private:
    TData first;
    TData second;
  };

  /**
   * \brief Partial template specialization for publishing data in case of
   * failed comparison of two vector<double>
   */
  template <>
  class Report<vector<double> > {
   public:
    Report(const vector<double>& in_first, const vector<double>& in_second)
      : first(in_first), second(in_second) {}

    void write(ostream* output) const {
      for (int i = 0; i < (int)first.size(); ++i) {
        (*output) << first[i] << "\t\t" << second[i] << "\n";
      }
    }
   private:
    vector<double> first;
    vector<double> second;
  };

  /**
   * \brief Partial template specialization for publishing data in case of
   * failed comparison of two vector<vector<double> >
   */
  template <>
  class Report<vector<vector<double> > > {
   public:
    Report(
      const vector<vector<double> >& in_first,
      const vector<vector<double> >& in_second)
      : first(in_first), second(in_second) {}

    void write(ostream* output) const {
      for (int i = 0; i < (int)first.size(); ++i) {
        for (int j = 0; j < (int)first[i].size(); ++j) {
          (*output) << first[i][j] << "\t\t" << second[i][j] << "\n";
        }
      }
    }
   private:
    vector<vector<double> > first;
    vector<vector<double> > second;
  };

  /**
   * \brief operator<< for output error report to stream.
   */
  template <class T>
  ostream& operator<<(ostream& output, const Report<T>& report) {
    report.write(&output);
    return output;
  }
};

#endif  // LTR_SERIALIZATION_TEST_TESTER_TESTER_UTILITY_H_
