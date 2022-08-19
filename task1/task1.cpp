#include "task1.hh"
#include <cstddef>
#include <cstdlib>
#include <cstring>

// helper functions

char *StringFindFirst(String target, String substring) {
  while (*target) {
    char *index = target;
    char *sub = substring;
    while (*sub && (*index == *sub)) {
      index++;
      sub++;
    }
    if (!*sub) {
      return target;
    }

    target++;
  }
  return NULL;
}

inline void StringListSetSize(StringList list, int size) {
  *((int *)list) = size;
}

inline void *StringListGetNodes(StringList list) {
  return *((void **)list + 1);
}

inline void StringListSetNodes(StringList list, StringListNode node) {
  *((void **)list + 1) = node;
}

StringListNode NodeAtIndex(StringList list, int index) {
  StringList node = StringListGetNodes(list);
  if (index >= StringLists::GetSize(list))
    return NULL;
  for (int i = 0; i < index; i++, node = StringLists::Nodes::NodeGetNext(node))
    ;
  return node;
}

/// @param start_index inclusive starting index
int NodeFindNextMatch(StringList list, String str, int start_index) {
  StringListNode node = NodeAtIndex(list, start_index);
  for (int i = start_index; i < StringLists::GetSize(list); i++) {
    if (!strcmp(StringLists::Nodes::NodeGetString(node), str)) {
      return i;
    }
  }
  return -1;
}

namespace StringLists::Nodes {
inline char *NodeGetString(StringListNode node) { return *((char **)node); }

inline void NodeSetString(StringListNode node, String str) {
  *((char **)node) = str;
}

inline void *NodeGetNext(StringListNode node) { return *((void **)node + 1); }

inline void NodeSetNext(StringListNode node, StringList next_node) {
  *((void **)node + 1) = next_node;
}

StringListNode NodeInit(String str, StringListNode next_node) {
  StringListNode node = malloc(16);
  int size = strlen(str);
  String new_str = (char *)malloc(size);
  strcpy(new_str, str);
  StringLists::Nodes::NodeSetString(node, new_str);
  StringLists::Nodes::NodeSetNext(node, next_node);
  return node;
}

inline void NodeDestroy(StringListNode node) {
  free(StringLists::Nodes::NodeGetString(node));
  free(node);
}
} // namespace StringLists::Nodes

namespace StringLists {

inline int GetSize(StringList list) { return *((int *)list); }

void Init(StringList *list) {
  *list = malloc(16);
  StringListSetSize(*list, 0);
  StringListSetNodes(*list, NULL);
}

void Destroy(StringList *list) {
  StringListNode node = StringListGetNodes(*list);
  for (int i = GetSize(*list); i > 0; i--) {
    StringListNode current = node;
    node = Nodes::NodeGetNext(node);
    current = Nodes::NodeGetNext(current);
    Nodes::NodeDestroy(current);
  }
  list = NULL;
}

void Add(StringList list, String str) {
  StringListNode new_node = Nodes::NodeInit(str, NULL);
  if (GetSize(list) == 0) {
    StringListSetNodes(list, new_node);
  } else {
    StringListNode last_node = NodeAtIndex(list, GetSize(list) - 1);
    Nodes::NodeSetNext(last_node, new_node);
  }
  StringListSetSize(list, GetSize(list) + 1);
}

void Remove(StringList list, String str) {
  StringListNode node = StringListGetNodes(list);
  for (int i = 0; i < GetSize(list); i++, node = Nodes::NodeGetNext(node)) {
    if (!strcmp(Nodes::NodeGetString(node), str)) {
      RemoveAtIndex(list, i);
    }
  }
}

void RemoveAtIndex(StringList list, int index) {
  if (index == -1)
    return;
  StringListNode node = NodeAtIndex(list, index);
  StringListNode next_node = StringLists::Nodes::NodeGetNext(node);
  StringLists::Nodes::NodeDestroy(node);
  if (index == 0) {
    StringListSetNodes(list, next_node);
    return;
  }
  StringListNode previous_node = NodeAtIndex(list, index - 1);
  StringLists::Nodes::NodeSetNext(previous_node, next_node);
  StringListSetSize(list, StringLists::GetSize(list) - 1);
}

int IndexOf(StringList list, String str) {
  StringListNode node = StringListGetNodes(list);
  for (int i = 0; i < GetSize(list); i++, node = Nodes::NodeGetNext(node)) {
    if (!strcmp(Nodes::NodeGetString(node), str)) {
      return i;
    }
  }
  return -1;
}

String StringListAtIndex(StringList list, int index) {
  return Nodes::NodeGetString(NodeAtIndex(list, index));
}

void RemoveDuplicates(StringList list) {
  StringListNode node = StringListGetNodes(list);
  for (int i = 0; i < GetSize(list); i++, node = Nodes::NodeGetNext(node)) {
    String str = Nodes::NodeGetString(node);
    int index = NodeFindNextMatch(list, str, i + 1);

    while (index != -1) {
      RemoveAtIndex(list, index);
      index = NodeFindNextMatch(list, str, index);
    }
  }
}

void Sort(StringList list) {}
} // namespace StringLists