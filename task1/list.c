#include "list.h"
#include <string.h>

// node struct functions
inline String NodeGetString(StringListNode node) { return *((String *)node); }

inline void NodeSetString(StringListNode node, String str) {
  if (NodeGetString(node)) {
    free(NodeGetString(node));
  }
  int size = strlen(str);
  String new_str = (String)malloc(size);
  strcpy(new_str, str);
  *((String *)node) = new_str;
}

inline StringListNode NodeGetNext(StringListNode node) {
  return *((void **)node + 1);
}

inline void NodeSetNext(StringListNode node, StringList next_node) {
  *((void **)node + 1) = next_node;
}

StringListNode NodeInit(String str, StringListNode next_node) {
  StringListNode node = malloc(16);
  int size = strlen(str);
  String new_str = (String)malloc(size);
  strcpy(new_str, str);
  NodeSetString(node, new_str);
  NodeSetNext(node, next_node);
  return node;
}

inline void NodeDestroy(StringListNode node) {
  free(NodeGetString(node));
  free(node);
}

// stringList node function
inline void StringListSetSize(StringList list, int size) {
  *((int *)list) = size;
}
inline int GetSize(StringList list) { return *((int *)list); }

inline StringListNode StringListGetNodes(StringList list) {
  return *((void **)list + 1);
}

inline void StringListSetNodes(StringList list, StringListNode node) {
  *((void **)list + 1) = node;
}

// helper functions
StringListNode StringListNodeAtIndex(StringList list, int index) {
  if (index < 0)
    return NULL;
  if (index >= GetSize(list))
    return NULL;
  StringList node = StringListGetNodes(list);
  for (int i = 0; i < index; i++, node = NodeGetNext(node))
    ;
  return node;
}

void StringListRemoveNode(StringListNode list, StringListNode prev_node,
                          StringListNode node_to_remove) {

  StringListNode next_node = NodeGetNext(node_to_remove);

  if (prev_node == NULL)
    StringListSetNodes(list, next_node);
  else
    NodeSetNext(prev_node, next_node);

  NodeDestroy(node_to_remove);
  StringListSetSize(list, GetSize(list) - 1);
}

// makes shallow copy.
// if you sort resulting array then the list will also be sorted.
// do not change string pointers or realloc them.
// do not free any strings.
// changing size of the array, removing and adding elements does not change the
// list.
// changing string chars will change them in the list.
StringListNode *StringListGetArrayRepresentation(StringList list, int *size) {
  StringListNode *array = malloc(GetSize(list) * 8);
  StringListNode node = StringListGetNodes(list);
  for (int i = 0, max = GetSize(list); i < max; i++, node = NodeGetNext(node)) {
    array[i] = node;
  }
  *size = GetSize(list);
  return array;
}

void swapStrings(StringListNode n1, StringListNode n2) {
  String s = NodeGetString(n1);
  NodeSetString(n1, NodeGetString(n2));
  NodeSetString(n2, s);
}
int compareNodes(StringListNode n1, StringListNode n2) {
  return strcmp(NodeGetString(n1), NodeGetString(n2));
}

int partition(StringListNode array[], int low, int high) {

  // select the rightmost element as pivot
  StringListNode pivot = array[high];

  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (compareNodes(array[j], pivot) <= 0) {

      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;

      // swap string of element at i with string of element at j
      swapStrings(array[i], array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swapStrings(array[i + 1], array[high]);

  // return the partition point
  return (i + 1);
}

void quickSort(StringListNode array[], int low, int high) {
  if (low < high) {

    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);

    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);

    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}

// list functions
void Init(StringList *list) {
  *list = malloc(16);
  StringListSetSize(*list, 0);
  StringListSetNodes(*list, NULL);
}

void Destroy(StringList *list) {
  StringListNode node = StringListGetNodes(*list);
  for (int i = GetSize(*list); i > 0; i--) {
    StringListNode current = node;
    node = NodeGetNext(node);
    NodeDestroy(current);
  }
  free(list);
  list = NULL;
}

void AddAtIndex(StringList list, String str, size_t index) {

  if (GetSize(list) < index) {
    return;
  }

  StringListNode new_node = NodeInit(str, NULL);
  StringListNode next_node;

  if (index == 0) {
    next_node = StringListGetNodes(list);
    StringListSetNodes(list, new_node);

  } else {
    next_node = StringListNodeAtIndex(list, index);
  }

  NodeSetNext(new_node, next_node);

  StringListSetSize(list, GetSize(list) + 1);
}
void PushBack(StringList list, String str) {
  AddAtIndex(list, str, GetSize(list) + 1);
}

void RemoveAllString(StringList list, String str) {
  StringListNode node = StringListGetNodes(list);
  StringListNode prev_node = NULL;
  for (; node != NULL; prev_node = node, node = NodeGetNext(node)) {

    if (!strcmp(NodeGetString(node), str)) {
      StringListRemoveNode(list, prev_node, node);
    }
  }
}

void RemoveAtIndex(StringList list, int index) {
  if (index < GetSize(list))
    return;

  StringListNode node = StringListGetNodes(list);
  StringListNode prev_node = NULL;
  for (int i = 0; i < index; i++, prev_node = node, node = NodeGetNext(node))
    ;

  StringListRemoveNode(list, prev_node, node);
}
int IndexOf(StringList list, String str) {
  StringListNode node = StringListGetNodes(list);
  for (int i = 0; i < GetSize(list); i++, node = NodeGetNext(node)) {
    if (!strcmp(NodeGetString(node), str)) {
      return i;
    }
  }
  return -1;
}

String StringAtIndex(StringList list, int index) {
  return NodeGetString(StringListNodeAtIndex(list, index));
}

void RemoveDuplicates(StringList list) {
  StringListNode node = StringListGetNodes(list);
  while (node != NULL) {
    String str = NodeGetString(node);
    StringListNode temp = NodeGetNext(node);
    StringListNode prev = NULL;

    while (temp != NULL) {
      if (!strcmp(NodeGetString(temp), str))
        StringListRemoveNode(list, prev, temp);
      prev = temp;
      temp = NodeGetNext(temp);
    }
    node = NodeGetNext(node);
  }
}

void Sort(StringList list) {
  int size;
  StringListNode *array;
  array = StringListGetArrayRepresentation(list, &size);
  quickSort(array, 0, size - 1);
}