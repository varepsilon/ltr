// Copyright 2012 Yandex

#include <gtest/gtest.h>

#include "ltr/utility/multitable.h"

using ltr::utility::MultiTable;


class MultiTableTest : public ::testing::Test {};

TEST(MultiTableTest, MultiTableTest) {
  vector<int> dims;
  dims.push_back(6);
  dims.push_back(2);
  dims.push_back(4);
  ltr::utility::MultiTable<int, 3> table(dims);

  table.setAxisLabel(0, "firstDim");
  table.setAxisLabel(1, "secondDim");
  table.setAxisLabel(2, "thirdDim");

  table.setTickLabel(1, 0, "first of second dim");

  for (int i = 0; i < 6; ++i) {
    stringstream out;
    out << "tick1_" << i;
    table.setTickLabel(0, i, out.str());
  }
  for (int j = 0; j < 2; ++j) {
    stringstream out;
    out << "tick2_" << j;
    table.setTickLabel(1, j, out.str());
  }
  for (int k = 0; k < 4; ++k) {
    stringstream out;
    out << "tick3_" << k;
    table.setTickLabel(2, k, out.str());
  }

  for (MultiTable<int, 3>::Iterator it = table.begin();
       it != table.end();
       ++it) {
    *it = 100;
    EXPECT_EQ(*it, 100);
  }
  vector<int> multi_index;
  multi_index.push_back(0);
  multi_index.push_back(0);
  multi_index.push_back(0);
  table[multi_index] = 10;
  EXPECT_EQ(table[multi_index], 10);

  stringstream check_table_stream;

  check_table_stream <<
    "\nsecondDim: tick2_0\n" <<
    "Table (row: thirdDim, column: firstDim)\n" <<
    "\ttick1_0\ttick1_1\ttick1_2\ttick1_3\ttick1_4\ttick1_5\t\n" <<
    "tick3_0\t10\t100\t100\t100\t100\t100\t\n" <<
    "tick3_1\t100\t100\t100\t100\t100\t100\t\n" <<
    "tick3_2\t100\t100\t100\t100\t100\t100\t\n" <<
    "tick3_3\t100\t100\t100\t100\t100\t100\t\n" <<
    "\n" <<
    "secondDim: tick2_1\n" <<
    "Table (row: thirdDim, column: firstDim)\n" <<
    "\ttick1_0\ttick1_1\ttick1_2\ttick1_3\ttick1_4\ttick1_5\t\n" <<
    "tick3_0\t100\t100\t100\t100\t100\t100\t\n" <<
    "tick3_1\t100\t100\t100\t100\t100\t100\t\n" <<
    "tick3_2\t100\t100\t100\t100\t100\t100\t\n" <<
    "tick3_3\t100\t100\t100\t100\t100\t100\t\n";

  EXPECT_EQ(check_table_stream.str(), table.toString());

  for (MultiTable<int, 3>::Iterator it = table.begin();
       it != table.end();
       ++it) {
    if (it == table.begin()) {
      EXPECT_EQ(*it, 10);
    } else {
      EXPECT_EQ(*it, 100);
    }
  }
}

