#pragma once

#include "dropOutStack.h"

/**
 * @brief Represents a node in a doubly linked list.
 */
typedef struct nodeImpl {
    DropOutStackElem element;
    struct nodeImpl *prev;
    struct nodeImpl *next;
} NodeImpl;

typedef NodeImpl *PtNode;

/**
 * @brief Creates a new node with the given element.
 * 
 * @param data The DropOutStackElem to store in the node.
 * @return PtNode Pointer to the newly created node, or NULL on failure.
 */
PtNode nodeCreate(DropOutStackElem element);