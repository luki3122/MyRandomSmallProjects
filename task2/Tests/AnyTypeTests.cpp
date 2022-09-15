#include "../src/AnyType.hh"
#include <gtest/gtest.h>
#include <typeinfo>

TEST(AnyTypeConstructor, StoresInt) {
  AnyType a((int)7);
  ASSERT_EQ(a.getValue<int>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(int).name());
}
TEST(AnyTypeConstructor, StoresChar) {
  AnyType a((char)7);
  ASSERT_EQ(a.getValue<char>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(char).name());
}
TEST(AnyTypeConstructor, StoresFloat) {
  AnyType a((float)7);
  ASSERT_EQ(a.getValue<float>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(float).name());
}
TEST(AnyTypeConstructor, StoresDouble) {
  AnyType a((double)7);
  ASSERT_EQ(a.getValue<double>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(double).name());
}
TEST(AnyTypeConstructor, StoresBool) {
  AnyType a((bool)1);
  ASSERT_EQ(a.getValue<bool>(), 1);
  ASSERT_EQ(a.getTypeName(), typeid(bool).name());
}

TEST(AnyTypeAssigns, StoresInt) {
  AnyType a = (int)7;
  ASSERT_EQ(a.getValue<int>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(int).name());
}
TEST(AnyTypeAssigns, StoresChar) {
  AnyType a = (char)7;
  ASSERT_EQ(a.getValue<char>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(char).name());
}
TEST(AnyTypeAssigns, StoresFloat) {
  AnyType a = (float)7;
  ASSERT_EQ(a.getValue<float>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(float).name());
}

TEST(AnyTypeAssigns, StoresDouble) {
  AnyType a = (double)7;
  ASSERT_EQ(a.getValue<double>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(double).name());
}

TEST(AnyTypeAssigns, StoresBool) {
  AnyType a = (bool)1;
  ASSERT_EQ(a.getValue<bool>(), 1);
  ASSERT_EQ(a.getTypeName(), typeid(bool).name());
}
TEST(AnyTypeDestroy, DestroysStoredType) {
  AnyType a = (int)7;
  a.destroy();
  ASSERT_EQ(a.getTypeName(), "");
  ASSERT_THROW(a.getValue<int>(), std::bad_cast);
}
TEST(AnyTypeSwap, SwapsTwoAnyTypes) {
  AnyType a = (char)'p';
  AnyType b = (int)7;
  a.swap(b);

  ASSERT_EQ(a.getValue<int>(), 7);
  ASSERT_EQ(a.getTypeName(), typeid(int).name());

  ASSERT_EQ(b.getValue<char>(), 'p');
  ASSERT_EQ(b.getTypeName(), typeid(char).name());
}
TEST(AnyTypeGetValue, ThrowsBadCastOnDifferentTypeAccess) {
  AnyType a = (int)7;
  ASSERT_THROW(a.getValue<bool>(), std::bad_cast);
}

// int main() {
//   AnyType a = (unsigned int)5;
//   print<unsigned int>(a);

//   try {
//     bool bo = a.getValue<bool>();

//   } catch (std::bad_cast e) {
//     std::cout << e.what() << '\n';
//   }

//   AnyType b;
//   b = 1;
//   print<int>(b);
//   b = 'a';
//   print<char>(b);
// }