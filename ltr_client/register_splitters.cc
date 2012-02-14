// Copyright 2012 Yandex

#include "ltr_client/splitter_factory.h"
#include "ltr/crossvalidation/k_fold_simple_splitter.h"
#include "ltr/crossvalidation/tk_fold_simple_splitter.h"
#include "ltr/crossvalidation/leave_one_out_splitter.h"

using ltr::Object;
using ltr::ObjectPair;
using ltr::ObjectList;

using ltr::cv::KFoldSimpleSplitter;
using ltr::cv::TKFoldSimpleSplitter;
using ltr::cv::LeaveOneOutSplitter;

template<>
void SplitterFactory::registerAll() {
  REGISTER_POINTWISE_SPLITTER("K_FOLD", KFoldSimpleSplitter<Object>);
  REGISTER_PAIRWISE_SPLITTER("K_FOLD", KFoldSimpleSplitter<ObjectPair>);
  REGISTER_LISTWISE_SPLITTER("K_FOLD", KFoldSimpleSplitter<ObjectList>);

  REGISTER_POINTWISE_SPLITTER("TK_FOLD", TKFoldSimpleSplitter<Object>);
  REGISTER_PAIRWISE_SPLITTER("TK_FOLD", TKFoldSimpleSplitter<ObjectPair>);
  REGISTER_LISTWISE_SPLITTER("TK_FOLD", TKFoldSimpleSplitter<ObjectList>);

  REGISTER_POINTWISE_SPLITTER("LEAVE_ONE_OUT", LeaveOneOutSplitter<Object>);
  REGISTER_PAIRWISE_SPLITTER("LEAVE_ONE_OUT", LeaveOneOutSplitter<ObjectPair>);
  REGISTER_LISTWISE_SPLITTER("LEAVE_ONE_OUT", LeaveOneOutSplitter<ObjectList>);
}
