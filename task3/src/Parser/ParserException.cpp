#include "ParserException.hh"
ParserException::ParserException(const std::string &message)
    : message(message) {}

const char *ParserException::what() const noexcept { return message.c_str(); }
