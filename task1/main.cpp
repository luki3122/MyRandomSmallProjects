#include "task1.hh"
#include <cstdlib>
#include <iostream>

void print(StringList &list) {
  std::cout << "List:{\n";
  for (int i = 0; i < StringLists::GetSize(list); i++) {
    std::cout << "  " << StringLists::StringListAtIndex(list, i) << '\n';
  }
  std::cout << "}\n\n";
}

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
  print(list);
  StringLists::Remove(list, a);
  print(list);
  StringLists::Add(list, b);
  StringLists::Add(list, b);
  print(list);
  StringLists::RemoveDuplicates(list);
  print(list);
}