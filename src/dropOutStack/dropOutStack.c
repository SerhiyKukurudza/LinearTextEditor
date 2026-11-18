/**
 * @file listArrayList.c
 * 
 * @brief Provides an implementation of the ADT List with an array list
 * as the underlying data structure.
 */

#include "dropOutStack.h"
#include "node.h"

/**
 * @brief Represents a drop-out stack of fixed capacity.
 * 
 * When the stack is full and a new element is pushed, the oldest element is discarded.
 */
typedef struct dropOutStackImpl {
    PtNode header;
    PtNode trailer;
    int capacity;
    int size;
} DropOutStackImpl;

PtDropOutStack stackCreate(int capacity) {
    PtDropOutStack stack = (PtDropOutStack)malloc(sizeof(DropOutStackImpl));
    if (stack == NULL) return NULL;
    
    stack->header = (PtNode)malloc(sizeof(NodeImpl));
    if (stack->header == NULL) {
        free(stack);
        return NULL;
    }

    stack->trailer = (PtNode)malloc(sizeof(NodeImpl));
    if (stack->trailer == NULL) {
        free(stack->header);
        free(stack);
        return NULL;
    }

    stack->header->prev = NULL;
    stack->header->next = stack->trailer;

    stack->trailer->next = NULL;
    stack->trailer->prev = stack->header;

    stack->size = 0;
    stack->capacity = capacity;

    return stack;
}

int stackDestroy(PtDropOutStack *ptStack) {
    PtDropOutStack stack = (*ptStack);
    if (stack == NULL) return STACK_NULL;

    PtNode current = stack->header->next;
    while(current != stack->trailer) {
        current = current->next;
        free(current->prev);
    }

    free(stack->header);
    free(stack->trailer);
    free(stack);

    *ptStack = NULL;

    return STACK_OK;
}

int stackPush(PtDropOutStack stack, DropOutStackElem element) {
    if (stack == NULL) return STACK_NULL;
    
    if(stack->size == stack->capacity) {
        PtNode toRemove = stack->trailer->prev;
        PtNode newLast = toRemove->prev;
        free(toRemove);
        stack->trailer->prev = newLast;
        newLast->next = stack->trailer;
        stack->size--;
    }

    PtNode newTop = (PtNode)malloc(sizeof(NodeImpl));
    if(newTop == NULL) return STACK_NO_MEMORY;

    PtNode curTop = stack->header->next;

    newTop->element = element;
    newTop->prev = stack->header;
    newTop->next = curTop;

    stack->header->next = newTop;
    curTop->prev = newTop;

    stack->size++;

    return STACK_OK;
}

int stackPop(PtDropOutStack stack, DropOutStackElem *ptElement) {
    if (stack == NULL) return STACK_NULL;
    if (stack->size == 0) return STACK_EMPTY;

    PtNode curTop = stack->header->next;
    PtNode newTop = stack->header->next->next;

    *ptElement = curTop->element;

    stack->header->next = newTop;
    newTop->prev = stack->header;

    free(curTop);

    stack->size--;

    return STACK_OK;
}

int stackPeek(PtDropOutStack stack, DropOutStackElem *ptElement) {
    if (stack == NULL) return STACK_NULL;
    if (stack->size == 0) return STACK_EMPTY;

    PtNode curTop = stack->header->next;
    *ptElement = curTop->element;

    return STACK_OK;
}

bool stackIsEmpty(PtDropOutStack stack) {
    if (stack == NULL) return true;

    return (stack->size == 0);
}

int stackSize(PtDropOutStack stack, int *ptSize) {
    if (stack == NULL) return STACK_NULL;

    *ptSize = stack->size;

    return STACK_OK;
}

int stackClear(PtDropOutStack stack) {
    if (stack == NULL) return STACK_NULL;

    PtNode current = stack->header->next;
    while(current != stack->trailer) {
        PtNode next = current->next;

        if (current->element != NULL) {
            actionDestroy(current->element);
        }

        free(current);
        current = next;
    }

    stack->header->next = stack->trailer;
    stack->trailer->prev = stack->header;
    stack->size = 0;

    return STACK_OK;
}
