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

  for (int index = 0; index < dims[0]; ++index) {
    stringstream out;
    out << "tick1_" << index;
    table.setTickLabel(0, index, out.str());
  }
  for (int index = 0; index < dims[1]; ++index) {
    stringstream out;
    out << "tick2_" << index;
    table.setTickLabel(1, index, out.str());
  }
  for (int index = 0; index < dims[2]; ++index) {
    stringstream out;
    out << "tick3_" << index;
    table.setTickLabel(2, index, out.str());
  }

  for (MultiTable<int, 3>::Iterator cell = table.begin();
       cell != table.end();
       ++cell) {
    *cell = 100;
    EXPECT_EQ(100, *cell);
  }
  vector<int> multi_index;
  multi_index.push_back(0);
  multi_index.push_back(0);
  multi_index.push_back(0);
  table[multi_index] = 10;
  EXPECT_EQ(10, table[multi_index]);

  stringstream check_table_stream;

  check_table_stream <<
    "\nsecondDim: tick2_0\n" <<
    "Table (row: thirdDim, column: firstDim)\n" <<
    "         tick1_0  tick1_1  tick1_2  tick1_3  tick1_4  tick1_5 \n" <<
    "tick3_0  10       100      100      100      100      100     \n" <<
    "tick3_1  100      100      100      100      100      100     \n" <<
    "tick3_2  100      100      100      100      100      100     \n" <<
    "tick3_3  100      100      100      100      100      100     \n" <<
    "\n" <<
    "secondDim: tick2_1\n" <<
    "Table (row: thirdDim, column: firstDim)\n" <<
    "         tick1_0  tick1_1  tick1_2  tick1_3  tick1_4  tick1_5 \n" <<
    "tick3_0  100      100      100      100      100      100     \n" <<
    "tick3_1  100      100      100      100      100      100     \n" <<
    "tick3_2  100      100      100      100      100      100     \n" <<
    "tick3_3  100      100      100      100      100      100     \n";

  EXPECT_EQ(check_table_stream.str(), table.toString());

  for (MultiTable<int, 3>::Iterator cell = table.begin();
       cell != table.end();
       ++cell) {
    if (cell == table.begin()) {
      EXPECT_EQ(10, *cell);
    } else {
      EXPECT_EQ(100, *cell);
    }
  }
}

TEST(MultiTableTest, MultiTableAxisOrderTest) {
  vector<int> dim(2);
  dim[0] = 2;
  dim[1] = 3;
  MultiTable<int, 2> table(dim);
  table.setAxisLabel(0, "X");
  table.setAxisLabel(1, "Y");
  for (int index = 0; index < dim[0]; ++index) {
    stringstream stream;
    stream << "X_" << index + 1;
    table.setTickLabel(0, index, stream.str());
  }
  for (int index = 0; index < dim[1]; ++index) {
    stringstream stream;
    stream << "Y_" << index + 1;
    table.setTickLabel(1, index, stream.str());
  }
  for (MultiTable<int, 2>::Iterator cell = table.begin();
       cell != table.end();
       ++cell) {
    *cell = 42;
  }

  string check_table;
  check_table = "\n"
    "Table (row: Y, column: X)\n"
    "         X_1      X_2     \n"
    "Y_1      42       42      \n"
    "Y_2      42       42      \n"
    "Y_3      42       42      \n";

  vector<int> order(2);
  order[0] = 1;
  order[1] = 0;

  table.setAxisOrder(order);
  EXPECT_EQ(check_table, table.toString());

  check_table.clear();
  check_table = "\n"
    "Table (row: X, column: Y)\n"
    "         Y_1      Y_2      Y_3     \n"
    "X_1      42       42       42      \n"
    "X_2      42       42       42      \n";

  table.unsetAxisOrder();
  EXPECT_EQ(check_table, table.toString());
}
