/**
 * @file paragraph.h
 * @brief Defines the Paragraph structure and operations for handling text paragraphs.
 * 
 * This header declares the Paragraph structure and associated utility functions used for
 * managing and processing paragraph content in the text editor project. It includes
 * creation, tokenization, formatting (left/center alignment), statistics (character count,
 * word count), and memory management functionalities.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "map.h"

typedef struct Action* PtAction;


/**
 * @brief Structure representing a paragraph in the document.
 * 
 * This structure holds the number of tokens in the paragraph and an array of pointers to tokens.
 */
typedef struct {
    int numberTokens;
    Token *tokens; 
} Paragraph;

/**
 * @brief Creates a new Paragraph.
 * 
 * This function allocates memory for a new Paragraph and initializes its properties.
 * 
 * @return A pointer to the newly created Paragraph.
 */
Paragraph paragraphCreate();

/**
 * @brief Adds a Token to the specified Paragraph.
 * 
 * This function adds a token to the given paragraph, increasing its token count and updating the token array.
 * 
 * @param p A pointer to the Paragraph where the token will be added.
 * @param token The Token to be added to the paragraph.
 */
void addTokenToParagraph(Paragraph *p, Token token);

/**
 * @brief Prints the content of the Paragraph.
 * 
 * This function prints each token in the paragraph, typically by printing each token's string representation.
 * 
 * @param p A pointer to the Paragraph to be printed.
 */
void paragraphPrint(const Paragraph *p);

/**
 * @brief Prints the entire paragraph centered in lines of a given width.
 *
 * This function prints all tokens of the paragraph as one combined string,
 * centered within the specified line width.
 *
 * @param p Pointer to the Paragraph to print.
 * @param width The width of the line for centering the paragraph text.
 */
void printParagraphCentered(const Paragraph *p, int width);

/**
 * @brief Prints the paragraph with left alignment, wrapping lines at a maximum width.
 *
 * This function formats and prints the paragraph so that each line has at most `maxWidth` characters.
 * Words are wrapped to the next line at whitespace boundaries. The whitespace used for wrapping is
 * not printed at the beginning of a new line.
 *
 * @param p Pointer to the Paragraph structure to be printed.
 * @param maxWidth Maximum number of characters allowed per line (should be between 10 and 50).
 */
void printParagraphLeftAligned(const Paragraph *p, int width);

/**
 * @brief Processes a string and converts it into a Paragraph structure.
 *
 * This function tokenizes the input string into words, punctuation, and whitespace,
 * creating a corresponding list of tokens and storing them in a newly allocated Paragraph.
 *
 * @param input The input string to tokenize.
 * @return A pointer to the newly created Paragraph structure.
 */
Paragraph processStringToParagraph(const char *input);

/**
 * @brief Counts the number of characters in a paragraph.
 *
 * This function calculates the total number of characters in all tokens of a paragraph.
 * You can optionally include punctuation and whitespace characters in the count.
 *
 * @param p Pointer to the Paragraph.
 * @param includePunctuation If non-zero, punctuation tokens are included in the count.
 * @param includeWhitespace If non-zero, whitespace tokens are included in the count.
 * @return The total number of characters as specified.
 */
int countParagraphCharacters(const Paragraph *p, int includePunctuation, int includeWhitespace);


void countParagraphPonctuation(const Paragraph *p, int arr[], char *str);

/**
 * @brief Counts the number of words in a paragraph.
 *
 * This function calculates the total number of word tokens present in the given paragraph.
 * A word is typically defined as a sequence of alphabetic characters and may exclude punctuation
 * or whitespace depending on how tokens are defined in the paragraph structure.
 *
 * @param p Pointer to the Paragraph to be analyzed.
 * @return The total number of word tokens in the paragraph.
 */
int countParagraphWords(const Paragraph *p);

/**
 * @brief Determines the length of the longest word in a paragraph and counts how many words have that length.
 * 
 * @param p Pointer to the paragraph to analyze.
 * @param size Output parameter to store the maximum word length found.
 * @param count Output parameter to store the number of words with the maximum length.
 */
void countSizeBiggestWord(const Paragraph *p, int *size, int *count);

/**
 * @brief Collects all words of a given length from a paragraph into an array.
 * 
 * @param p Pointer to the paragraph to analyze.
 * @param biggestWords Array to store the found words.
 * @param offset Position in the array to start storing words.
 * @param size Length of the words to match.
 * @return Number of words added to the array.
 */
int countBiggestWords(const Paragraph *p, char **biggestWords, int offset, int size);

/**
 * @brief Counts the appearances of each word in a paragraph.
 *
 * This function processes all WORD-type tokens in the paragraph and stores
 * their frequency in the given map. If a word already exists in the map,
 * its count is incremented; otherwise, it is added with an initial count of 1.
 *
 * @param map Pointer to the map where word frequencies will be stored.
 * @param p Pointer to the Paragraph to be analyzed.
 */
void paragraphWordsAppearance(PtMap map, Paragraph *p);

/**
 * @brief Replaces all occurrences of a word in a paragraph with another word.
 *
 * This function searches through all WORD-type tokens in the paragraph and replaces
 * any that match `oldWord` with `newWord`.
 *
 * @param p Pointer to the Paragraph where the replacement should occur.
 * @param oldWord The word to be replaced.
 * @param newWord The word to replace with.
 */
int replaceParagraphWord(Paragraph *p, int index, const char *oldWord, const char *newWord, PtAction *mainAction);

/**
 * @brief Frees the memory allocated for a Paragraph.
 * 
 * This function frees the memory used by the Paragraph and its associated tokens.
 * 
 * @param p A pointer to the Paragraph to be freed.
 */
void paragraphDestroy(Paragraph *p);

/**
 * @brief Creates a deep copy of a paragraph.
 *
 * This function allocates and returns a new Paragraph structure that is an exact
 * copy of the input paragraph, including duplicated tokens.
 *
 * @param src The source Paragraph to clone.
 * @return A new Paragraph with copied contents, or NULL on memory allocation failure.
 */
Paragraph paragraphClone(Paragraph p);

/**
 * @brief Checks if a Paragraph is empty.
 * 
 * This function checks if the given Paragraph contains no tokens or only whitespace tokens.
 * 
 * @param p Constant pointer to the Paragraph to be checked.
 * 
 * @return int Returns 1 if the Paragraph is empty, 0 otherwise.
 */
int isParagraphEmpty(const Paragraph *p);