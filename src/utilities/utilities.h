/**
 * @file utilities.h
 * @brief Utility functions for string manipulation and input handling.
 *
 * Contains helper functions used throughout the text editor project,
 * such as converting strings to lowercase and reading valid indices from user input.
 */
#pragma once

#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[34m"   // For [INFO]
#define COLOR_GREEN "\033[0;32m"   // For [SUCCESS]
#define COLOR_YELLOW  "\x1b[33m"   // For [WARNING]
#define COLOR_RED     "\x1b[31m"   // For [ERROR]

#include <stdbool.h>
#include <stdarg.h>

/**
 * @brief Validates if the given string is a single valid word.
 *
 * A valid word is defined as a non-empty string containing only alphabetic characters (A-Z or a-z).
 * This function returns 1 (true) if the input is a valid word, or 0 (false) otherwise.
 *
 * @param word The input string to validate.
 * @return int Returns 1 if the string is a valid word; 0 otherwise.
 *
 * @note This function does not accept words with punctuation, digits, or spaces.
 *
 * @example
 * isValidSingleWord("hello");   // returns 1
 * isValidSingleWord("hello1");  // returns 0
 * isValidSingleWord("");        // returns 0
 */
int isValidSingleWord(const char *word);

/**
 * @brief Converts a given string to lowercase.
 * 
 * This function takes an input string and converts all its characters to lowercase.
 * The result is stored in a separate string passed as the second argument.
 * 
 * @param str The input string that needs to be converted to lowercase.
 * @param lowerStr The string where the lowercase version of the input will be stored.
 * 
 * @note The input string "str" is not modified. The lowercase result is stored in "lowerStr".
 * 
 * @example
 * char str[] = "Hello World!";
 * char lowerStr[50];
 * lowerString(str, lowerStr);
 * // lowerStr now contains "hello world!"
 */
void lowerString(char *str, char *lowerStr);

/**
 * @brief Reads a valid paragraph index from user input.
 *
 * Prompts the user to select a paragraph index for the given action,
 * ensuring the index is within the valid range [1, size].
 *
 * @param size Total number of paragraphs.
 * @param action Action being performed (e.g., "insert", "remove").
 * @return Zero-based valid index selected by the user.
 */
int readValidIndex(int size, const char *action);

/**
 * @brief Prints an info message in green.
 *
 * @param prefix Optional prefix (e.g., "\n") to be printed before the message.
 * @param format printf-style format string.
 * @param ... Arguments to format into the message.
 */
void printInfo(const char *prefix, const char *format, ...);

/**
 * @brief Prints a success message in green.
 *
 * @param prefix Optional prefix (e.g., "\n") to be printed before the message.
 * @param format printf-style format string.
 * @param ... Arguments to format into the message.
 */
void printSuccess(const char *prefix, const char *format, ...);

/**
 * @brief Prints a warning message in yellow.
 *
 * @param prefix Optional prefix (e.g., "\n") to be printed before the message.
 * @param format printf-style format string.
 * @param ... Arguments to format into the message.
 */
void printWarning(const char *prefix, const char *format, ...);

/**
 * @brief Prints an error message in red.
 *
 * @param prefix Optional prefix (e.g., "\n") to be printed before the message.
 * @param format printf-style format string.
 * @param ... Arguments to format into the message.
 */
void printError(const char *prefix, const char *format, ...);

/**
 * @brief Calculates the number of characters in a UTF-8 encoded string.
 *
 * This function counts the number of Unicode characters in a null-terminated
 * UTF-8 string, ignoring continuation bytes.
 *
 * @param s Pointer to a null-terminated UTF-8 string.
 * @return The number of Unicode characters in the string.
 */
int utf8_strlen(const char *s);