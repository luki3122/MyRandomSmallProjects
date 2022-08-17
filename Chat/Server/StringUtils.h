#pragma once

#include <sys/_types/_size_t.h>

int charIsValid(char c);
const char *getNextWord(const char *string);

int compareWords(const char *first, const char *second);

int stringContains(const char *string, const char *value);

void formatMessage(char *buffer, size_t buffer_size);

int charIsWhitespace(char c);
int charIsWhitespaceOrNull(char c);
