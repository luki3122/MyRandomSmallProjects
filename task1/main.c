#include "list.h"
#include <stdio.h>

int main() {
  StringList list;
  Init(&list);
  char str[4] = "pog";
  PushBack(list, str);
  PushBack(list, str);
  printf("%s\n", StringAtIndex(list, 0));
  printf("%s", StringAtIndex(list, 1));
}