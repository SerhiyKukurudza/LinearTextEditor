/**
 * @file document.h
 * @brief Provides the definition of the Document structure and functions for managing document metadata and content.
 */

#pragma once

#define DOCUMENT_OK				0
#define DOCUMENT_NULL			1
#define DOCUMENT_NO_MEMORY		2
#define DOCUMENT_EMPTY			3
#define DOCUMENT_FULL			4
#define DOCUMENT_INVALID_RANK	5
#define MAX_NAME_LENGTH 250

#include "list.h"
#include "dateTime.h"
#include "paragraph.h"

/**
 * @brief Structure representing a text document.
 *
 * This structure stores the document's name, load and modification timestamps,
 * and a list of Paragraph pointers that contain the actual text content.
 */
typedef struct {
    char name[MAX_NAME_LENGTH];   /**< Name of the document (e.g., filename) */
    DateTime loadDate;            /**< Date and time the document was loaded */
    DateTime modifiedDate;        /**< Date and time the document was last modified */
    PtList paragraphs;            /**< List of Paragraph* structures representing the document's content */
} Document;

/**
 * @brief Allocates and initializes a new Document.
 *
 * This function creates a new Document structure, sets its name,
 * assigns the provided list of paragraphs, and initializes the load date.
 *
 * @param name The name of the document (typically the filename).
 * @param paragraphs The list of paragraphs that make up the document content.
 * @return A pointer to the newly created Document, or NULL on failure.
 */
Document *documentCreate(const char *name);

/**
 * @brief Prints the document's metadata and structure.
 * 
 * Displays the name, timestamps, and paragraph details of the document.
 * This function is intended for debugging or visualization purposes.
 * 
 * @param doc A pointer to the Document to be printed.
 */
void documentPrint(Document *doc);

/**
 * @brief Deallocates all memory used by a Document.
 *
 * Frees the memory of the document, including all paragraphs in the list
 * and the list structure itself.
 *
 * @param doc Pointer to the Document to be destroyed.
 */
void documentDestroy(Document *doc);

/**
 * @brief Adds a paragraph to the document at a given index.
 */
int documentAddParagraph(Document *doc, int index, Paragraph paragraph);

/**
 * @brief Gets the number of paragraphs in the document.
 */
int documentParagraphsSize(Document *doc, int *ptSize);

/**
 * @brief Retrieves a paragraph from the document by index.
 */
int documentGetParagraph(Document *doc, int index, Paragraph *ptParagraph);

/**
 * @brief Removes a paragraph from the document by index.
 */
int documentRemoveParagraph(Document *doc, int index, Paragraph *ptParagraph);

/**
 * @brief Replaces a paragraph at the given index and returns the old one.
 */
int documentSetParagraph(Document *doc, int index, Paragraph newParagraph, Paragraph *ptReplacedParagraph);
