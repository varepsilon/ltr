// Copyright 2011 Yandex

#ifndef LTR_CLIENT_LTR_CLIENT_H_
#define LTR_CLIENT_LTR_CLIENT_H_

#include <string>

#include "ltr/interfaces/parameterized.h"
#include "ltr_client/configurator.h"

using ltr::Parameterized;

class LtrClientPrivate;
class LtrClient {
 public:
  LtrClient();
  ~LtrClient();
  void initFrom(const std::string &file_name);
  void launch();
 private:
//  LtrClientPrivate * const d;

  template <class TElement>
  void executeTrain(Parameterized* parameterized,
                const TrainLaunchInfo &train_info);

  ConfigParser configurator_;
  TXmlTokenSpecList getLoadQueue() const;
};

#endif  // LTR_CLIENT_LTR_CLIENT_H_
