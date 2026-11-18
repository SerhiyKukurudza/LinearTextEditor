#include <stdio.h>
#include "action.h"
#include "paragraph.h"

PtAction actionCreate(int index, Paragraph paragraph, Edit editOption) {
    PtAction newAction = (PtAction)malloc(sizeof(Action));
    if (!newAction) return NULL;

    newAction->index = index;
    newAction->paragraph = paragraph;
    newAction->editOption = editOption;
    newAction->next = NULL;

    return newAction;
}

void actionAppend(PtAction action, PtAction newAction) {
    if (action == NULL || newAction == NULL)
        return;

    PtAction current = action;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newAction;
}

void actionDestroy(PtAction action) {
    while (action != NULL) {
        PtAction next = action->next;
        paragraphDestroy(&action->paragraph); 
        free(action);
        action = next;
    }
}

void actionPrint(PtAction action) {
    if (!action) {
        printf("No action to display.\n");
        return;
    }

    int count = 1;
    while (action != NULL) {
        printf("---- Action %d ----\n", count++);
        printf("Index: %d\n", action->index);
        
        printf("Edit Type: ");
        switch (action->editOption) {
            case INSERT:
                printf("INSERT\n");
                break;
            case REMOVE:
                printf("REMOVE\n");
                break;
            case REPLACE:
                printf("REPLACE\n");
                break;
            default:
                printf("UNKNOWN\n");
        }

        printf("Paragraph:\n");
        paragraphPrint(&action->paragraph);
        printf("\n");

        action = action->next;
    }
}
