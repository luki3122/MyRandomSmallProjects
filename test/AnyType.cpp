#include "AnyType.hh"
#include <iostream>
#include <typeinfo>

BaseType::BaseType::~BaseType(){};

AnyType::AnyType() noexcept : storage(new BaseType), type_name(""){};

AnyType::AnyType(AnyType &&anytype) {
  storage = anytype.storage;
  anytype.storage = NULL;
  type_name = anytype.type_name;
}

AnyType::~AnyType() { delete storage; }

std::string AnyType::getTypeName() const { return type_name; }

template <typename T> void print(AnyType &a) {
  std::cout << "type: " << a.getTypeName() << '\n';
  std::cout << "val: " << a.getValue<T>() << '\n';
}

int main() {
  AnyType a = (unsigned int)5;
  print<unsigned int>(a);

  try {
    bool bo = a.getValue<bool>();

  } catch (std::bad_cast e) {
    std::cout << e.what() << '\n';
  }

  AnyType b;
  b = 1;
  print<int>(b);
  b = 'a';
  print<char>(b);
}