# Linear Text Editor - ATAD 2024/25

This repository contains the project implementation for the **Algorithms and Abstract Data Types (ATAD)** course (2024/25). The goal of this project is the development of a **Linear Text Editor** in C, which allows for the manipulation, editing, and analysis of text documents through a command-line interface, making use of Abstract Data Types (ADTs).

## Features (Commands)

The editor interacts with the user via a command prompt and supports the following operations:

### Base Commands
* **`LOAD`**: Loads a text file into memory.
* **`SAVE`**: Saves the current document changes to the disk.
* **`CLOSE`**: Closes the current file (prompting to save changes if any).
* **`QUIT`**: Safely terminates the program, freeing all allocated memory.
* **`SHOW`**: Interactively displays the document paragraph by paragraph (`Next`, `Prev`, `Stop`).
* **`HELP`**: Displays the list of available commands.

### Editing Commands
* **`EDIT`**: Adds (`Insert`), removes (`Remove`), or changes (`Replace`) a specific paragraph.
* **`REPLACEALL`**: Replaces all occurrences of a specific word with another throughout the document.
* **`UNDO`**: Reverts the last change made to the document (supported by the *DropOutStack* ADT).

### Printing Commands
* **`PRINTL`**: Displays the document formatted and left-aligned with a user-defined column width.
* **`PRINTC`**: Displays the document formatted and center-aligned with a user-defined column width.

### Analysis Commands
* **`STATS`**: Displays document statistics (number of words, characters, average words per paragraph, longest word).
* **`MARKS`**: Displays a histogram (simple bar chart) of punctuation characters.
* **`MATRIX`**: Shows a co-occurrence matrix for a selected set of words.
* **`TOPN`**: Displays the N most frequent words in the document, using a Dictionary (Map).

## Data Structures and ADTs

The project strictly manages dynamic memory (verified via Valgrind) and organizes information hierarchically:
* **Document**: Represents the file in memory, containing metadata (load/modification dates) and a list of paragraphs.
* **Paragraph**: Represents a paragraph, containing a dynamic array of tokens.
* **Token**: Represents a single word, punctuation mark, or whitespace.

**Employed ADTs:**
* **List**: To store the document's paragraphs.
* **Map (Dictionary)**: To count word frequencies (for the `TOPN` command).
* **DropOutStack**: A stack with a maximum capacity limit, used to support the edit history (`UNDO` command), discarding the oldest changes once the limit is reached.

---

## Technologies and Tools

This project was designed to be developed and tested in a Linux/WSL environment using the following tools:
* **GCC** (C Compiler)
* **GDB** (Debugger)
* **Make** (Build automation)
* **Valgrind** (Memory leak detection)
* **Doxygen** (Documentation generation)
* **VS Code** (Recommended IDE)