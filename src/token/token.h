/**
 * @file token.h
 * @brief Defines the Token structure and operations for handling lexical elements in text.
 *
 * Provides types and functions for creating, printing, and working with tokens such as words,
 * punctuation, and whitespace in the context of a text editor or tokenizer.
 */

#pragma once

#define MAX_TEXT 50

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
 * @enum Type
 * @brief Represents the possible types of tokens.
 * 
 * This enum is used to categorize tokens based on their content.
 */
typedef enum {
    WORD,
    PUNCTUATION,
    WHITESPACE
} Type;

/**
 * @struct Token
 * @brief Represents a single token in the text.
 * 
 * A token is a unit of meaningful content identified during text processing.
 * This struct stores the token's text and its type.
 * The previous linked list structure has been removed, and the `Token` is now a standalone entity.
 */
typedef struct Token {
    char text[MAX_TEXT];
    Type type;
} Token;

/**
 * @brief Creates a new token.
 * 
 * This function initializes a `Token` with the provided text and type, and returns it.
 * 
 * @param text The text content of the token.
 * @param type The type of the token (e.g., WORD, PUNCTUATION, WHITESPACE).
 * 
 * @return A newly created `Token`.
 * 
 * @note The text provided is copied directly into the `Token`, and there is no need to handle memory allocation for the `Token` itself.
 */
Token createToken(const char *text, Type type);

/**
 * @brief Prints a token's information.
 * 
 * This function prints the text and type of a token to the standard output.
 * 
 * @param token A pointer to the token to be printed.
 * 
 * @note If the token is NULL, this function does nothing.
 */
void printToken(Token *token);