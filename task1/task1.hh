#pragma once

#include <stdlib.h>
#include <string.h>

typedef char *String;
typedef void *StringList;
typedef void *StringListNode;

/*
StringList:
  0-7  int size
  8-15 void* nodes
node:
  0-7   char*
  8-15  void* next

*/

namespace StringLists {

namespace Nodes {

inline char *NodeGetString(StringListNode node);

inline void NodeSetString(StringListNode node, String str);

inline void *NodeGetNext(StringListNode node);

inline void NodeSetNext(StringListNode node, StringList next_node);

StringListNode NodeInit(String str, StringListNode next_node);

inline void NodeDestroy(StringListNode node);

} // namespace Nodes

void Init(StringList *list);
void Destroy(StringList *list);

void Add(StringList list, String str);
void Remove(StringList list, String str);
void RemoveAtIndex(StringList list, int index);
int GetSize(StringList list);

String StringListAtIndex(StringList list, int index);
int IndexOf(StringList list, String str);

void RemoveDuplicates(StringList list);
void ReplaceInStrings(StringList list, String before, String after);
void Sort(StringList list);

} // namespace StringLists
