#include "../src/list.h"
#include <cstddef>
#include <cstring>
#include <gtest/gtest.h>

using namespace testing;

TEST(TestInitFunction, InitializesWithNULL) {

  StringList List;
  Init(&List);
  ASSERT_TRUE(*(size_t *)List == 0);
  ASSERT_TRUE(*((void **)List + 1) == NULL);
  Destroy(&List);
}
TEST(TestPushBackFunction, AddsStringToList) {
  char str1[3] = "v1";

  StringList list;
  Init(&list);
  PushBack(list, str1);
  ASSERT_TRUE(!strcmp(StringAtIndex(list, 0), str1));
}
TEST(TestPushBackFunction, AddsStringByCopy) {
  char str1[3] = "v1";

  StringList list;
  Init(&list);
  PushBack(list, str1);
  ASSERT_FALSE(StringAtIndex(list, 0) == str1);
}

TEST(TestPushBackFunction, AddsMultipleStringsToList) {
  char str1[3] = "v1";
  char str2[3] = "v2";
  char str3[3] = "v3";

  StringList list;
  Init(&list);
  PushBack(list, str1);
  PushBack(list, str2);
  PushBack(list, str3);

  ASSERT_TRUE(!strcmp(StringAtIndex(list, 0), str1));
  ASSERT_TRUE(!strcmp(StringAtIndex(list, 1), str2));
  ASSERT_TRUE(!strcmp(StringAtIndex(list, 2), str3));
}

TEST(TestAddAtIndexFunction, AddsStringToList) {
  StringList List;
  Init(&List);
  char str[6] = "value";
  AddAtIndex(List, str, 0);
  ASSERT_TRUE(!strcmp(StringAtIndex(List, 0), str));
}
TEST(TestAddAtIndexFunction, AddsStringByCopy) {
  char str1[3] = "v1";

  StringList list;
  Init(&list);
  AddAtIndex(list, str1, 0);
  ASSERT_FALSE(StringAtIndex(list, 0) == str1);
}

TEST(TestAddFunction, AddsMultipleStrings) {
  char str1[3] = "v1";
  char str2[3] = "v2";
  char str3[3] = "v3";

  StringList list;
  Init(&list);

  AddAtIndex(list, str1, 0);
  AddAtIndex(list, str2, 1);
  AddAtIndex(list, str3, 2);

  ASSERT_TRUE(!strcmp(StringAtIndex(list, 0), str1));
  ASSERT_TRUE(!strcmp(StringAtIndex(list, 1), str2));
  ASSERT_TRUE(!strcmp(StringAtIndex(list, 2), str3));
}

TEST(TestAddFunction, AddsStringBetweenOtherStrings) {
  char str1[3] = "v1";
  char str2[3] = "v2";
  char str3[3] = "v3";

  StringList list;
  Init(&list);

  AddAtIndex(list, str1, 0);
  AddAtIndex(list, str3, 1);
  AddAtIndex(list, str2, 1);

  ASSERT_TRUE(!strcmp(StringAtIndex(list, 0), str1));
  ASSERT_TRUE(!strcmp(StringAtIndex(list, 1), str2));
  ASSERT_TRUE(!strcmp(StringAtIndex(list, 2), str3));
}

TEST(TestRemoveAllString, DoesNothingIfNoStringIsPresent) {
  char str1[3] = "v1";

  StringList list;
  Init(&list);

  StringList list2;
  Init(&list2);

  RemoveAllString(list, str1);

  ASSERT_EQ(GetSize(list), GetSize(list));
  ASSERT_EQ(*((String *)list + 1), *((String *)list + 1));
}

TEST(TestRemoveAllString, DoesNothingIfNoMatchingStringIsPresent) {
  char str1[3] = "v1";

  StringList list;
  Init(&list);

  StringList list2;
  Init(&list2);

  PushBack(list, str1);
  PushBack(list2, str1);

  RemoveAllString(list, str1);

  ASSERT_EQ(GetSize(list), GetSize(list));
  ASSERT_EQ(*((String *)list + 1), *((String *)list + 1));
}
