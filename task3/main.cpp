#include "Parser.hh"
#include <iostream>
#include <thread>

int main(int argc, const char **argv) {
  Parser p(argc, argv);
  std::cout << p.parse();
}