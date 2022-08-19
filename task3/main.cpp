#include "Parser.hh"
#include <iostream>
#include <thread>

int main(int argc, const char **argv) {
  Parser p(argc, argv);
  if (argc < 3) {

    std::cout << p.parse();
  } else {
    p.parseToFile();
  }
}