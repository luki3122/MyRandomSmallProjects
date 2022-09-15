#include "src/Parser/Parser.hh"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, const char **argv) {
  Parser parser;
  fs::path to_parse("./");
  if (argc > 1) {
    to_parse = argv[1];
  }
  std::cout << parser.parseDirectoryAsync(to_parse).print();
}