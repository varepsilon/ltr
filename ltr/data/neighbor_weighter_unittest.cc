// Copyright 2012 Yandex

#include "gtest/gtest.h"

#include "ltr/utility/neighbor_weighter.h"
#include "ltr/utility/numerical.h"

using ltr::utility::DoubleEqual;
using ltr::utility::NeighborWeighter;
using ltr::utility::InverseLinearDistance;
using ltr::utility::InverseSquareDistance;

TEST(NeighborWeighter, TestWeighter) {
  NeighborWeighter::Ptr neighbor_weighter1 =
    new ltr::utility::InverseLinearDistance;
  EXPECT_TRUE(DoubleEqual(0.5, neighbor_weighter1->getWeight(2.0, 10)));

  NeighborWeighter::Ptr neighbor_weighter2 =
    new ltr::utility::InverseSquareDistance;
  EXPECT_TRUE(DoubleEqual(0.25, neighbor_weighter2->getWeight(2.0, 3)));

  NeighborWeighter::Ptr neighbor_weighter3 =
    new ltr::utility::InverseOrder;
  EXPECT_TRUE(DoubleEqual(0.25, neighbor_weighter3->getWeight(12.1, 3)));
}
