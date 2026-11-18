#include "node.h"
#include "dropOutStack.h"
#include <stdlib.h>

PtNode nodeCreate(DropOutStackElem element) {
    PtNode newNode = (PtNode)malloc(sizeof(NodeImpl));
    if (!newNode) return NULL;

    newNode->element = element;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}