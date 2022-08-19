#include "StringUtils.h"
#include <stdio.h>

int charIsValid(char c) {
  if (c >= 32 && c <= 126)
    return 1;
  if (c == 0 || c == '\n' || c == '\r')
    return 1;
  return 0;
}

int charIsWhitespace(char c) { return c == 10 | c == 13 | c == 32; }
int charIsWhitespaceOrNull(char c) { return charIsWhitespace(c) || c == 0; }

void formatMessage(char *buffer, size_t buffer_size) {
  char *read = buffer;
  char *write = buffer;
  size_t iter = 0;
  while (*read != 0 && iter++ < buffer_size - 1) {
    if (charIsValid(*read))
      *write++ = *read++;
    else
      read++;
  }
  *write = 0;
}

int compareWords(const char *first, const char *second) {
  size_t iter = 0;
  while (*first != 0 && *second != 0) {
    if (charIsWhitespace(*first) && charIsWhitespace(*second))
      return 1;
    if (*first++ != *second++)
      return 0;
  }
  if (charIsWhitespaceOrNull(*first) && charIsWhitespaceOrNull(*second))
    return 1;
  return 0;
}

int stringContains(const char *string, const char *value) {
  while (*string != 0) {
    const char *v_ptr = value;
    const char *s_ptr = string;
    while (*v_ptr != 0) {
      if (*s_ptr != *v_ptr)
        break;
      s_ptr++;
      v_ptr++;
    }
    if (*v_ptr == 0)
      return 1;
    string++;
  }
  return 0;
}

const char *getNextWord(const char *string) {
  while (!charIsWhitespace(*string) && *string != 0) {
    string++;
  }
  if (charIsWhitespace(*string))
    return string + 1;
  return string;
}
