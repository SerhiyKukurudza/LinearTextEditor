#pragma once

#define STACK_OK			0
#define STACK_NULL			1
#define STACK_EMPTY			2
#define STACK_NO_MEMORY		3

#include <stdbool.h>
#include "action.h"

/** Forward declaration of the data structure. */
struct dropOutStackImpl;

/** Definition of pointer to the data stucture. */
typedef struct dropOutStackImpl *PtDropOutStack;

/**
 * @brief Type alias for the stack element.
 *
 * Each DropOutStackElem corresponds to an Action pointer.
 */
typedef PtAction DropOutStackElem;

/**
 * @brief Creates a DropOutStack with the given capacity.
 * 
 * @param capacity Maximum number of elements the stack can hold.
 * @return A DropOutStack structure with initialized memory.
 */
PtDropOutStack stackCreate(int capacity);

/**
 * @brief Destroys the DropOutStack and frees all memory.
 * 
 * @param stack Pointer to the stack to destroy.
 */
int stackDestroy(PtDropOutStack *stack);

/**
 * @brief Pushes a new element onto the stack.
 * 
 * If the stack is full, the oldest element is discarded (drop-out behavior).
 * 
 * @param stack Pointer to the stack.
 * @param element The element to push.
 * @return 0 on success, -1 on allocation or logic error.
 */
int stackPush(PtDropOutStack stack, DropOutStackElem element);

/**
 * @brief Pops the most recent element from the stack.
 * 
 * @param stack Pointer to the stack.
 * @param element Pointer to store the popped element.
 * @return 0 on success, -1 if the stack is empty.
 */
int stackPop(PtDropOutStack stack, DropOutStackElem *element);

/**
 * @brief Peeks at the most recent element without removing it.
 * 
 * @param stack Pointer to the stack.
 * @param element Pointer to store the top element.
 * @return 0 on success, -1 if the stack is empty.
 */
int stackPeek(PtDropOutStack stack, DropOutStackElem *element);

/**
 * @brief Checks whether the stack is empty.
 * 
 * @param stack Pointer to the stack.
 * @return true if empty, false otherwise.
 */
bool stackIsEmpty(PtDropOutStack stack);

/**
 * @brief Returns the current number of elements in the stack.
 * 
 * @param stack Pointer to the stack.
 * @return The number of elements in the stack.
 */
int stackSize(PtDropOutStack stack, int *ptSize);

/**
* @brief Clears the contents of a stack.
*
* This operation will remove all current elements in the stack,
* returning to an empty state.
*
* @param stack [in] pointer to the stack
*
* @return STACK_OK if successful, or
* @return STACK_NULL if 'stack' is NULL
*/
int stackClear(PtDropOutStack stack);