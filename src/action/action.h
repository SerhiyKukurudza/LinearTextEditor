/**
 * @file action.h
 * @brief Defines the Action structure for undo operations.
 */

#pragma once

#include "paragraph.h"
#include "command.h"

/**
 * Forward declaration of Action pointer.
 */
typedef struct Action* PtAction;

/**
 * @brief Structure representing a single edit action.
 *
 * This structure holds:
 * - the index of the modified paragraph
 * - the paragraph content before modification
 * - the edit type performed
 * - an optional pointer to the next action (for grouped multiple changes)
 */
typedef struct Action {
    int index;
    Paragraph paragraph;
    Edit editOption;
    PtAction next;
} Action;

/**
 * @brief Creates a new Action instance.
 *
 * @param index The paragraph index affected.
 * @param paragraph The paragraph content before modification.
 * @param editOption The type of edit operation.
 * @return Pointer to the created Action, or NULL if allocation failed.
 */
PtAction actionCreate(int index, Paragraph paragraph, Edit editOption);

/**
 * @brief Appends a new sub-action to the end of an Action chain.
 *
 * This function traverses the chain starting at 'action' and appends 'newAction'
 * to the end of the list.
 * 
 * @param action The root Action to which we want to append.
 * @param newAction The Action to append at the end.
 */
void actionAppend(PtAction action, PtAction newAction);

/**
 * @brief Frees memory for an Action and all linked sub-actions.
 *
 * @param action The Action structure to destroy.
 */
void actionDestroy(PtAction action);

/**
 * @brief Prints the details of an Action (for debugging purposes).
 *
 * @param action The Action to print.
 */
void actionPrint(PtAction action);