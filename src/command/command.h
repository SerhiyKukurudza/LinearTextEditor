/**
 * @file command.h
 * @brief Contains the command function definitions and structures for the text editor.
 */

#pragma once

/**
 * @brief Typedef for the command functions.
 *
 * Each command function takes a context pointer, argument count (argc), and array of argument strings (argv),
 * and returns an integer indicating success (0) or failure (non-zero).
 */
typedef int (*CommandFunc)(void *context, int argc, char **argv);

/**
 * @brief Structure representing a command.
 */
typedef struct {
  const char *name;
  CommandFunc func;
  const char *description;
} Command;

/**
 * @brief Enum representing different types of edit actions.
 */
typedef enum {
  INSERT,
  REMOVE,
  REPLACE
} Edit;

/**
 * @brief Extern declaration for the list of commands available in the editor.
 */
extern Command commandList[];

/**
 * @brief Reads and executes a command from input.
 *
 * This function processes the user's input, parses the command and arguments, and calls the corresponding function.
 * Returns 0 if the command was "exit", or 1 to continue running.
 *
 * @param input The input string containing the command.
 * @return 0 to exit the loop, 1 to continue.
 */
int readCommand(char *input, void *context);

/**
 * @brief Help command: displays command list or details of a specific command.
 */
int help(void *context, int argc, char **argv);

/**
 * @brief Loads a text file and displays information about its content.
 */
int load(void *context, int argc, char **argv);

/**
 * @brief Saves the current document.
 */
int save(void *context, int argc, char **argv);

/**
 * @brief Closes the current file.
 */
int close(void *context, int argc, char **argv);

/**
 * @brief Displays the document content paragraph by paragraph.
 */
int show(void *context, int argc, char **argv);

/**
 * @brief Allows inserting, removing, or replacing paragraphs.
 */
int edit(void *context, int argc, char **argv);

/**
 * @brief Replaces all occurrences of a word with another.
 */
int replaceall(void *context, int argc, char **argv);

/**
 * @brief Undoes the last change made to the document.
 */
int undo(void *context, int argc, char **argv);

/**
 * @brief Prints the document left-aligned with a defined width.
 */
int printl(void *context, int argc, char **argv);

/**
 * @brief Prints the document center-aligned with a defined width.
 */
int printc(void *context, int argc, char **argv);

/**
 * @brief Displays word and character statistics of the document.
 */
int stats(void *context, int argc, char **argv);

/**
 * @brief Displays a punctuation character histogram of the document.
 */
int marks(void *context, int argc, char **argv);

/**
 * @brief Creates a matrix of word co-occurrences by paragraph.
 */
int matrix(void *context, int argc, char **argv);

/**
 * @brief Displays the N most frequent words in the document.
 */
int topn(void *context, int argc, char **argv);

/**
 * @brief Clears the terminal.
 */
int clear(void *context, int argc, char **argv);

/**
 * @brief Quits the program.
 */
int quit(void *context, int argc, char **argv);