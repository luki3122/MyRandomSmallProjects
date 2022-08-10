#include "task1.hh"
#include <cstdlib>
#include <iostream>
int main() {

  char str[9] = "charaara";
  char a[4] = "ara";
  char b[4] = "pog";

  // StringListNode node = StringLists::Nodes::NodeInit(a, NULL);
  StringList list;

  StringLists::Init(&list);
  StringLists::Add(list, str);
  StringLists::Add(list, a);
  StringLists::Add(list, b);

  for (int i = 0; i < StringLists::GetSize(list); i++) {
    std::cout << StringLists::StringListAtIndex(list, i) << '\n';
  }
}