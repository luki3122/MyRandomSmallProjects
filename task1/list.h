#pragma once

#include <stddef.h>
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

void Init(StringList *list);
void Destroy(StringList *list);

void AddAtIndex(StringList list, String str, size_t index);
void PushBack(StringList list, String str);
void RemoveAllString(StringList list, String str);
void RemoveAtIndex(StringList list, int index);
int GetSize(StringList list);

String StringAtIndex(StringList list, int index);
int IndexOf(StringList list, String str);

void RemoveDuplicates(StringList list);

void ReplaceInStrings(StringList list, String before, String after);

void Sort(StringList list);
