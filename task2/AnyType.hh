#pragma once

#include <concepts>
#include <cstddef>
#include <exception>
#include <string>
#include <type_traits>
#include <typeinfo>

template <typename T>
concept storable = std::is_arithmetic<T>::value;

class BaseType {
public:
  virtual ~BaseType();
  template <typename T>
  requires storable<T>
  const T &get() const;

  template <typename T>
  requires storable<T>
  void set(const T &new_value);
};

template <typename T>
requires storable<T>
class Type : public BaseType {
private:
  T value;

public:
  Type(const T new_value);
  Type(const BaseType *type);
  const T &get() const { return value; };

  void set(const T new_value) { value = new_value; };
};

class AnyType {
private:
  BaseType *storage;
  std::string type_name;

public:
  template <typename T>
  requires storable<T> AnyType(const T value);
  AnyType() noexcept;
  AnyType(AnyType &&anytype);
  ~AnyType();

  template <typename T> T getValue() const;
  std::string getTypeName() const;

  template <typename T>
  requires storable<T> AnyType &operator=(const T &value);
};

template <typename T>
requires storable<T> Type<T>::Type(const T new_value) : value(new_value) {}

template <typename T>
requires storable<T> Type<T>::Type(const BaseType *type) {

  value = type->get<T>();
}

template <typename T>
requires storable<T>
const T &BaseType::get() const {
  return dynamic_cast<const Type<T> &>(*this).get();
}

template <typename T>
requires storable<T>
void BaseType::set(const T &new_value) {
  return dynamic_cast<Type<T> &>(*this).set(new_value);
}

template <typename T>
requires storable<T> AnyType::AnyType(const T value) {
  storage = new Type<T>(value);
  type_name = typeid(T).name();
}

template <typename T> T AnyType::getValue() const { return storage->get<T>(); }
template <typename T>
requires storable<T> AnyType &AnyType::operator=(const T &value) {
  if (typeid(T).name() == type_name) {
    storage->set(value);
  } else {
    delete storage;
    storage = new Type<T>(value);
  }
  type_name = typeid(T).name();

  return *this;
}
