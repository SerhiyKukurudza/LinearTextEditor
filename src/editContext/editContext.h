/**
 * @file editContext.h
 * @brief Declares the editor context structure and its management functions.
 * 
 * This header defines the EditorContext structure used to manage the state
 * of the text editor, including the currently open document and modification flags.
 */

#pragma once

#include <stdbool.h>
#include "document.h"
#include "dropOutStack.h"

/**
 * @brief Represents the editor's current session context.
 *
 * This structure stores a pointer to the currently loaded document and a flag
 * indicating whether the document has been modified since the last save.
 */
typedef struct {
    Document *document;
    PtDropOutStack actions; 
    bool isModified;
} EditorContext;

/**
 * @brief Initializes a new EditorContext.
 * 
 * Creates an EditorContext with no loaded document and the modification flag set to false.
 * 
 * @return An initialized EditorContext instance.
 */
EditorContext editContextCreate();

/**
 * @brief Resets the EditorContext by freeing the current document and clearing modification state.
 * 
 * If a document is loaded in the context, it will be destroyed. The modification flag
 * will be reset to indicate no unsaved changes.
 *
 * @param ctx Pointer to the EditorContext structure to reset.
 */
void editContextReset(EditorContext *ctx);