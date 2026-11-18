#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "input.h"
#include "document.h"
#include "paragraph.h"
#include "editContext.h"
#include "utilities.h"
#include "dropOutStack.h"

#define INPUT_SIZE 100           
#define HEADER_PADDING 10        

void printStars(int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("*");
    }
    printf("\n");
}

void template() {
    const char *header = "Welcome to the Simplified Text Editor!";
    const char *description = "Here, you can manipulate and analyze your text documents efficiently.";
    const char *tip = "Tip: Type 'help' to see the list of available commands.";

    int maxLen = strlen(description) > strlen(header) ? strlen(description) : strlen(header);
    if (strlen(tip) > maxLen) maxLen = strlen(tip);
    maxLen += HEADER_PADDING; 

    printStars(maxLen);
    printf("* %-*s *\n", maxLen - 4, header);
    printf("* %-*s *\n", maxLen - 4, description);
    printStars(maxLen);

    printf("\n%s\n\n", tip);
}

void printInsertCommand() {
    printf(">> Enter a command: ");
}

int main() {
    template();

    char input[INPUT_SIZE];
    PtDropOutStack actions = stackCreate(20);
    EditorContext editorContext = { NULL, actions, false };

    while (1) {
        printInsertCommand();
        readString(input, INPUT_SIZE);

        if (!readCommand(input, &editorContext)) {
            printInfo("\n", "Exiting the program. Goodbye!\n");
            break;
        }

        printf("\n"); 
    }

    return EXIT_SUCCESS;
}