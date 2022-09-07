#pragma once
#include <string>
class IParsedData {
public:
  virtual std::string print() const = 0;
  virtual ~IParsedData() = 0;
};
