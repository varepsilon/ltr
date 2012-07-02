// Copyright 2011 Yandex

#ifndef LTR_CLIENT_LTR_CLIENT_H_
#define LTR_CLIENT_LTR_CLIENT_H_

#include <string>

class LtrClientPrivate;
class LtrClient {
 public:
  LtrClient();
  ~LtrClient();
  void initFrom(const std::string &file_name);
  void launch();
 private:
  LtrClientPrivate * const d;
};

#endif  // LTR_CLIENT_LTR_CLIENT_H_
