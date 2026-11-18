#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include "utilities.h"
#include "input.h"

int isValidSingleWord(const char *word) {
    int len = strlen(word);
    if (len == 0) return 0;

    for (int i = 0; i < len; i++) {
        if (!isalpha(word[i])) {
            return 0;
        }
    }
    return 1;
}

void lowerString(char *str, char *lowerStr){
  int len = strlen(str);
  for(int i = 0; i < len; i++){
    lowerStr[i] = tolower(str[i]);
  }
  lowerStr[len] = '\0';
}

int readValidIndex(int size, const char *action){
  int index = -1;

  while(true) {
    printf("\nYour document has %d paragraphs. Choose a position [1-%d] to %s: ", size, size, action);
    readInteger(&index);
    if(index > 0 && index <= size) break;
        printError("\n", "Invalid paragraph index!\n");
  }

  //This needs to be - 1 because the index given don't count with the postion 0
  return index - 1;
}

void printInfo(const char *prefix, const char *format, ...) {
    if (prefix && strlen(prefix) > 0) {
        printf("%s", prefix);
    }

    printf(COLOR_BLUE "[INFO] " COLOR_RESET);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void printSuccess(const char *prefix, const char *format, ...) {
    if (prefix && strlen(prefix) > 0) {
        printf("%s", prefix);
    }

    printf(COLOR_GREEN "[SUCCESS] " COLOR_RESET);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void printWarning(const char *prefix, const char *format, ...) {
    if (prefix && strlen(prefix) > 0) {
        printf("%s", prefix);
    }

    printf(COLOR_YELLOW "[WARNING] " COLOR_RESET);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void printError(const char *prefix, const char *format, ...) {
    if (prefix && strlen(prefix) > 0) {
        printf("%s", prefix);
    }

    printf(COLOR_RED "[ERROR] " COLOR_RESET);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

int utf8_strlen(const char *s) {
    int len = 0;
    while (*s) {
        if ((*s & 0xC0) != 0x80) len++;  
        s++;
    }
    return len;
}
