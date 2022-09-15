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
void AnyType::destroy() {
  type_name = "";
  delete storage;
  storage = new BaseType;
}
void AnyType::swap(AnyType &anytype) {
  std::swap(storage, anytype.storage);
  std::swap(type_name, anytype.type_name);
}

std::string AnyType::getTypeName() const { return type_name; }

template <typename T> void print(AnyType &a) {
  std::cout << "type: " << a.getTypeName() << '\n';
  std::cout << "val: " << a.getValue<T>() << '\n';
}
