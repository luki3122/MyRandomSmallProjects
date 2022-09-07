#pragma once

#include <exception>
#include <string>
class ParserException : public std::exception {
private:
  std::string message;

public:
  ParserException(const std::string &message);
  virtual const char *what() const noexcept override;
};