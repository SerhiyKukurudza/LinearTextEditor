#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "map.h"
#include "command.h"
#include "input.h"
#include "utilities.h"
#include "document.h"
#include "paragraph.h"
#include "token.h"
#include "dateTime.h"
#include "editContext.h"
#include "action.h"

#define MAX_COMMANDS 16
#define MAX_ARGUMENTS 10    
#define MAX_INPUT_LENGTH 100 
#define COMMAND_DESCRIPTION_LENGTH 100 
#define MAX_WORD_LENGTH 50
#define CELL_WIDTH 20

Command commandList[] = {
    {"help", help, "help [command] - Lists all commands or shows detailed help for a specific command"},
    {"load", load, "load <filename> - Loads a text file and displays paragraph/token count"},
    {"save", save, "save - Saves the current file to disk"},
    {"close", close, "close - Closes the current file (asks to save if there are unsaved changes)"},
    {"show", show, "show - Displays the file content paragraph by paragraph"},
    {"edit", edit, "edit - Opens the editor to modify, insert or delete paragraphs"},
    {"replaceall", replaceall, "replaceall - Replaces all occurrences of a word"},
    {"undo", undo, "undo - Reverts the last edit operation"},
    {"printl", printl, "printl <width> - Prints text left-aligned to the given width"},
    {"printc", printc, "printc <width> - Prints text center-aligned to the given width"},
    {"stats", stats, "stats - Displays word, sentence and character statistics"},
    {"marks", marks, "marks - Shows a histogram of punctuation marks"},
    {"matrix", matrix, "matrix - Generates a matrix of word co-occurrences by paragraph"},
    {"topn", topn, "topn <N> - Displays the N most frequent words in the document"},
    {"clear", clear, "clear - Clears the terminal"},
    {"quit", quit, "quit - Exits the program, saving changes if necessary"},
};

void strToArrWords(char *str, char **words, int *argc) {
    char lowerStr[MAX_INPUT_LENGTH];
    lowerString(str, lowerStr);

    char *token = strtok(lowerStr, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGUMENTS) {
        words[i] = malloc(strlen(token) + 1); 
        strcpy(words[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    *argc = i;
}

int readCommand(char *input, void *context) {
  char *argv[MAX_ARGUMENTS];
  int argc = 0;

  strToArrWords(input, argv, &argc);

  if (argc == 0) return 1;

  for (int i = 0; i < MAX_COMMANDS; i++) {
    if (strcmp(argv[0], commandList[i].name) == 0) {
      commandList[i].func(context, argc, argv);
      for (int j = 0; j < argc; j++) free(argv[j]);
      return 1;
    }
  }

  printWarning("\n", "Unknown command. See HELP.\n");

  for (int i = 0; i < argc; i++) free(argv[i]);
  return 1;
}

int help(void *context, int argc, char **argv) {
  if (argc == 1) {
      printInfo("\n", "Available commands (type 'help <command>' for usage info):\n");

      for (int i = 0; i < MAX_COMMANDS; i++) {
          printf("  %-10s - %s\n", commandList[i].name, commandList[i].description);
      }
      return EXIT_SUCCESS;
  } else if (argc == 2) {
      for (int i = 0; i < MAX_COMMANDS; i++) {
          if (strcmp(argv[1], commandList[i].name) == 0) {
              printf("\nUsage: %s\n", commandList[i].description);
              return EXIT_SUCCESS;
          }
      }
      printWarning("\n", "Command '%s' not found. Type 'help' for a list of valid commands.\n", argv[1]);

      return EXIT_FAILURE;
  }

  printWarning("\n", "Invalid usage. Try: help or help <command>\n");
  return EXIT_FAILURE;
}

int load(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if (argc < 2) {
        printWarning("\n", "Please specify a file to load. Usage: load <filename>\n");
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
        printError("\n", "Failed to open file: %s\n", filename);
        return EXIT_FAILURE;
    }

    if (ctx->document != NULL) {
        editContextReset(ctx);
    }

    ctx->document = documentCreate(filename);
    if (!ctx->document) {
        fclose(file);
        printError("\n", "Failed to create document structure.\n");
        return EXIT_FAILURE;
    }

    char line[4096];
    int tokenCount = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        Paragraph paragraph;
        if (strlen(line) == 0 || strspn(line, " \t\r\n") == strlen(line)) {
            paragraph = paragraphCreate();
        } else {
            paragraph = processStringToParagraph(line);
            tokenCount += paragraph.numberTokens;
        }

        int size = 0;
        documentParagraphsSize(ctx->document, &size);
        documentAddParagraph(ctx->document, size, paragraph);    
        paragraphDestroy(&paragraph);
    }

    ctx->isModified = false;

    Document *doc = ctx->document;
    documentPrint(doc);

    int paragraphSize = 0;
    documentParagraphsSize(ctx->document, &paragraphSize);
    printf("  %d paragraphs and %d tokens loaded.\n", paragraphSize, tokenCount);

    fclose(file);
    return EXIT_SUCCESS;
}

int save(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if (ctx->document == NULL || ctx->document->paragraphs == NULL) {
        printWarning("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(ctx->document->name, "w");
    if (file == NULL) {
        printError("\n", "Failed opening file for writing.");
        return EXIT_FAILURE;
    }

    int paragraphCount = 0;
    documentParagraphsSize(ctx->document, &paragraphCount); 

    for (int i = 0; i < paragraphCount; i++) {
        Paragraph p;
        if (documentGetParagraph(ctx->document, i, &p) != DOCUMENT_OK) {
            printError("\n", "Failed to read paragraph %d\n", i + 1);
            continue;
        }

        for (int j = 0; j < p.numberTokens; j++) {
            fprintf(file, "%s", p.tokens[j].text);
        }

        fprintf(file, "\n");
    }

    fclose(file);
    ctx->document->modifiedDate = (DateTime){0};
    ctx->isModified = false;

    if (stackClear(ctx->actions) != STACK_OK) {
        printError("\n", "Failed to clear the edit history\n");
    }

    printSuccess("\n", "Changes saved successfully.\n");
    return EXIT_SUCCESS;
}

int close(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if (ctx->document == NULL) {
        printInfo("\n", "No document is currently open.\n");
        return EXIT_SUCCESS;
    }

    if (ctx->isModified) {
        printInfo("\n", "You have unsaved changes. Do you want to save before closing? (y/n): ");
        
        char response;
        readChar(&response);

        if (response == 'y' || response == 'Y') {
            save(context, 0, NULL);
        } else {
            printInfo("\n", "Changes were not saved.\n");
        }
    }

    printInfo("\n", "Closing document: %s\n", ctx->document->name);
    documentPrint(ctx->document);

    editContextReset(ctx);

    return EXIT_SUCCESS;
}

int show(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;
    Document *doc = ctx->document;

    documentPrint(doc);

    if(doc == NULL) {
        return EXIT_FAILURE;
    }

    int paragraphCount = 0;
    documentParagraphsSize(doc, &paragraphCount);
    printf("\n  Total Paragraphs: %d\n\n", paragraphCount);

    if (paragraphCount == 0) {
        printInfo("\n", "The document has no paragraphs.\n");
        return EXIT_SUCCESS;
    }

    int current = 0;
    char input[20];

    while (1) {
        Paragraph p;
        if (documentGetParagraph(doc, current, &p) == DOCUMENT_OK) {
            printf("\n[Paragraph %d]:\n", current + 1);
            paragraphPrint(&p);
        } else {
            printError("\n", "Failed to retrieve paragraph %d.\n", current + 1);
        }

        printf("\nOptions: [Next] [Prev] [Stop]\n> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\r\n")] = '\0';

        if (strcasecmp(input, "Next") == 0) {
            if (current < paragraphCount - 1) {
                current++;
            } else {
                printInfo("\n", "Already at the last paragraph.\n");
            }
        } else if (strcasecmp(input, "Prev") == 0) {
            if (current > 0) {
                current--;
            } else {
                printInfo("\n", "Already at the first paragraph.\n");
            }
        } else if (strcasecmp(input, "Stop") == 0) {
            printInfo("\n", "Exiting view mode.\n");
            break;
        } else {
            printWarning("\n", "Invalid option. Use: Next, Prev, Stop.\n");
        }
    }

    return EXIT_SUCCESS;
}

int edit(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if(ctx->document == NULL) {
        printError("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    if (argc < 2) {
        printWarning("\n", "Please specify the action that you want to execute: 'Insert' | 'Remove' | 'Replace'\n");
        return EXIT_FAILURE;
    }

    int size;
    documentParagraphsSize(ctx->document, &size);

    Edit editOption;

    if (strcmp(argv[1], "insert") == 0) {
        printf("\nWrite the paragraph that you want to write in the document! \n\n>> ");
        char s[1024];
        readString(s, 1024);
        Paragraph addedParagraph = processStringToParagraph(s);

        int indexInserted = readValidIndex(size, argv[1]);
        documentAddParagraph(ctx->document, indexInserted, addedParagraph);

        ctx->document->modifiedDate = dateTimeCreate();
        ctx->isModified = true;
        
        editOption = INSERT;
        PtAction action = actionCreate(indexInserted, addedParagraph, editOption);
        stackPush(ctx->actions, action);

        printSuccess("\n", "Paragraph added successfully.\n");
        return EXIT_SUCCESS;

    } else if (strcmp(argv[1], "remove") == 0){
        int indexRemoved = readValidIndex(size, argv[1]);

        Paragraph removedParagraph;
        if (documentGetParagraph(ctx->document, indexRemoved, &removedParagraph) != DOCUMENT_OK) {
            printError("\n", "Could not retrieve paragraph.\n");
            return EXIT_FAILURE;
        }

        printInfo("\n", "Pharagraph chosen: \n");
        paragraphPrint(&removedParagraph);

        printWarning("\n", "Are you sure you want to remove this paragraph? (y/n): ");
        char response;
        readChar(&response);

        if (response != 'y' && response != 'Y') {
            printInfo("\n", "Operation cancelled.\n");
            return EXIT_SUCCESS;
        }

        if (documentRemoveParagraph(ctx->document, indexRemoved, &removedParagraph) != DOCUMENT_OK) {
            printError("\n", "Failed to remove paragraph.\n");
            return EXIT_FAILURE;
        }

        ctx->document->modifiedDate = dateTimeCreate();
        ctx->isModified = true;
       
        editOption = REMOVE;
        Paragraph backup = paragraphClone(removedParagraph);
        PtAction action = actionCreate(indexRemoved, backup, REMOVE);
        stackPush(ctx->actions, action);
        paragraphDestroy(&removedParagraph);

        printSuccess("\n", "Paragraph removed successfully.\n");
        return EXIT_SUCCESS;

    } else if (strcmp(argv[1], "replace") == 0){
        int indexReplace = readValidIndex(size, argv[1]);

        printf("\nWrite the paragraph that you want to write in the document! \n\n>> ");
        char s[1024];
        readString(s, 1024);
        Paragraph newParagraph = processStringToParagraph(s);

        Paragraph replacedParagraph;
        if (documentGetParagraph(ctx->document, indexReplace, &replacedParagraph) != DOCUMENT_OK) {
            printError("\n", "Could not retrieve paragraph.\n");
            paragraphDestroy(&newParagraph);
            return EXIT_FAILURE;
        }

        printf("\nReplace pharagraph: \n");
        paragraphPrint(&replacedParagraph);

        printf("\nNew pharagraph: \n");
        paragraphPrint(&newParagraph);

        printWarning("\n", "Are you sure you want to replace this paragraph? (y/n): ");
        char confirmation;
        readChar(&confirmation);
        if (confirmation != 'y' && confirmation != 'Y') {
            printInfo("\n", "Operation cancelled.\n");
            return EXIT_SUCCESS;
        }

        if (documentSetParagraph(ctx->document, indexReplace, newParagraph, &replacedParagraph) != DOCUMENT_OK) {
            printError("\n", "Failed to replace paragraph.\n");
            return EXIT_FAILURE;
        }

        ctx->document->modifiedDate = dateTimeCreate();
        ctx->isModified = true;

        editOption = REPLACE;
        Paragraph backup = paragraphClone(replacedParagraph);
        PtAction action = actionCreate(indexReplace, backup, REPLACE);
        stackPush(ctx->actions, action);
        paragraphDestroy(&replacedParagraph);

        printSuccess("\n", "Paragraph replaced successfully.\n");
        return EXIT_SUCCESS;
    } else {
        printError("\n", "The edit option inserted is not valid!\n");
        return EXIT_FAILURE;
    }
}

int replaceall(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if(ctx->document == NULL) {
        printWarning("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    char oldWord[MAX_TEXT], newWord[MAX_TEXT];
    printf("\nPlease insert the word that you want to replace: ");
    readString(oldWord, MAX_TEXT);
    printf("\nPlease insert the new word: ");
    readString(newWord, MAX_TEXT);

    int size;
    documentParagraphsSize(ctx->document, &size);

    PtAction mainAction = NULL;
    Paragraph paragraph;
    int count = 0;
    for(int i = 0; i < size; i++) {
        documentGetParagraph(ctx->document, i, &paragraph);
        count += replaceParagraphWord(&paragraph, i, oldWord, newWord, &mainAction);
    }
    
    stackPush(ctx->actions, mainAction);
    ctx->document->modifiedDate = dateTimeCreate();
    ctx->isModified = true;

    if(count == 0){
        printInfo("\n", "No occurrences found for %s\n", oldWord);
    } else {
        printSuccess("\n", "The word -> %s was replace %d times with the word -> %s\n", oldWord, count, newWord);
    }

    return 0;
}

int undo(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if(ctx->document == NULL) {
        printError("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    PtAction action = NULL;
    if (stackPop(ctx->actions, &action) != STACK_OK || action == NULL) {
        printError("\n", "Failed to retrieve last action for undo.\n");
        return EXIT_FAILURE;
    }

    int index = action->index;

    switch(action->editOption) {
        case INSERT: {
            Paragraph removedParagraph;

            if (documentGetParagraph(ctx->document, index, &removedParagraph) != DOCUMENT_OK) {
                printError("\n", "Could not retrieve paragraph.\n");
                return EXIT_FAILURE;
            }

            printInfo("\n", "Pharagraph chosen: \n");
            paragraphPrint(&removedParagraph);

            printWarning("\n", "Are you sure you want to remove this paragraph? (y/n): ");
            char response;
            readChar(&response);

            if (response != 'y' && response != 'Y') {
                printInfo("\n", "Operation cancelled.\n");
                return EXIT_SUCCESS;
            }

            if (documentRemoveParagraph(ctx->document, index, &removedParagraph) != DOCUMENT_OK) {
                printError("\n", "Failed to remove paragraph.\n");
                return EXIT_FAILURE;
            }

            printSuccess("\n", "Paragraph removed successfully.\n");
            break;
        }

        case REPLACE: {
            PtAction current = action;
            while (current != NULL) {
                int idx = current->index;
                actionPrint(current);
                int docSize = 0;
                if (documentParagraphsSize(ctx->document, &docSize) != DOCUMENT_OK) {
                    printError("\n", "Failed to get document size.\n");
                    return EXIT_FAILURE;
                }

                if (idx < 0 || idx >= docSize) {
                    printError("\n", "Invalid paragraph index: %d (document has %d paragraphs).\n", idx, docSize);
                    return EXIT_FAILURE;
                }

                Paragraph old;
                if (documentGetParagraph(ctx->document, idx, &old) != DOCUMENT_OK) {
                    printError("\n", "Could not retrieve paragraph at index %d.\n", idx);
                    return EXIT_FAILURE;
                }

                if (current->paragraph.tokens == NULL || current->paragraph.numberTokens <= 0) {
                    printWarning("\n", "Stored paragraph for undo appears empty or invalid.\n");
                }

                if (documentSetParagraph(ctx->document, idx, current->paragraph, &old) != DOCUMENT_OK) {
                    printError("\n", "Failed to apply replacement at index %d.\n", idx);
                    return EXIT_FAILURE;
                }

                PtAction previous = current;
                current = previous->next;
            }

            printSuccess("\n", "Undo completed successfully.\n");
            break;
        }

        case REMOVE: {
            if (documentAddParagraph(ctx->document, index, action->paragraph) != DOCUMENT_OK) {
                printError("\n", "Failed to replace paragraph.\n");
                return EXIT_FAILURE;
            }

            printSuccess("\n", "Paragraph replaced successfully.\n");
            break;
        }
    }

    if(stackIsEmpty(ctx->actions)) {
        ctx->document->modifiedDate = (DateTime){0};
        ctx->isModified = false;
    } else {
        ctx->document->modifiedDate = dateTimeCreate();
        ctx->isModified = true;
    }

    actionDestroy(action);

    return EXIT_SUCCESS;
}

int printl(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if (ctx->document == NULL) {
        printError("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    int width = 0;
    while (1) {
        printf("\nEnter column width [10-50]: ");
        readInteger(&width);
        if (width >= 10 && width <= 50) {
            break;
        }

        printError("\n", "Invalid width. Please enter a number between 10 and 50.\n");
    }

    int paragraphCount;
    documentParagraphsSize(ctx->document, &paragraphCount);

    for (int i = 0; i < paragraphCount; i++) {
        Paragraph p;
        if (documentGetParagraph(ctx->document, i, &p) != DOCUMENT_OK) {
            continue;
        }

        if (isParagraphEmpty(&p)){
            continue;
        }

        printf("\n");
        printParagraphLeftAligned(&p, width);
    }

    return EXIT_SUCCESS;
}

int printc(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if(ctx->document == NULL) {
        printError("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    int width = 0;
    while (1)
    {
        printf("\nEnter column width [10-50]: ");
        readInteger(&width);
        if (width >= 10 && width <= 50) {
            break;
        }
        printError("\n", "Invalid width. Please enter a number between 10 and 50.\n");
    }

    int paragraphCount;
    documentParagraphsSize(ctx->document, &paragraphCount);

    for (int i = 0; i < paragraphCount; i++) {
        Paragraph p;
        if (documentGetParagraph(ctx->document, i, &p) != DOCUMENT_OK) { 
            continue;   
        }

        if (isParagraphEmpty(&p)){
            continue;
        }

        printf("\n");
        printParagraphCentered(&p, width);
    }

    return EXIT_SUCCESS;
}

int stats(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;
    
    if(ctx->document == NULL) {
        printWarning("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    int size;
    documentParagraphsSize(ctx->document, &size);

    Paragraph p;
    int sumAll = 0, sumLess = 0, total = 0;
    int sizeWords = 0, totalWords = 0;

    for(int i = 0; i < size; i++){
        documentGetParagraph(ctx->document, i, &p);
        sumLess += countParagraphCharacters(&p, 0, 0);
        sumAll += countParagraphCharacters(&p, 1, 1);
        total += countParagraphWords(&p);

        int len = 0, cnt = 0;
        countSizeBiggestWord(&p, &len, &cnt);
        if(sizeWords < len){
            sizeWords = len;
            totalWords = 0;
            totalWords += cnt;
        } else if(sizeWords == len) {
            totalWords += cnt;
        }
    }

    double average = (double)total / size;

    char **biggest = malloc(totalWords * sizeof(char *));
    int count = 0;
    for(int i = 0; i < size; i++){
        documentGetParagraph(ctx->document, i, &p);
        count += countBiggestWords(&p, biggest, count, sizeWords);
    }

    for (int j = 0; j < count - 1; j++) {
        for (int k = 0; k < count - j - 1; k++) {
            if (strcmp(biggest[k], biggest[k + 1]) > 0) {
                char *temp = biggest[k];
                biggest[k] = biggest[k + 1];
                biggest[k + 1] = temp;
            }
        }
    }

    printf("\nTotal number of words: %d.", total);
    printf("\nNumber of characters with ponctuation and whitespace: %d.", sumAll);
    printf("\nNumber of characters without ponctuation and whitespace: %d.", sumLess);
    printf("\nAverage words number per paragraph: %.2lf.", average);
    printf("\nLongest word(s): \n");
    for (int i = 0; i < totalWords; i++) printf("-> %s \n", biggest[i]);
    printf("\n");

    for (int i = 0; i < totalWords; i++) free(biggest[i]);
    free(biggest);

    return 0;
}

int marks(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;
    if(ctx->document == NULL) {
        printf("\n[ERROR] No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    int size;
    documentParagraphsSize(ctx->document, &size);

    Paragraph p;
    char puntuationList[] = {',', '.', ':', '?', '!', ';'};
    int puntuationNumber[] = {0, 0, 0, 0, 0, 0};
    for(int i = 0; i < size; i++){
        documentGetParagraph(ctx->document, i, &p);
        countParagraphPonctuation(&p, puntuationNumber, puntuationList);
    }

    for(int j = 0; j < 6; j++){
        printf("\n%c - %d - ", puntuationList[j], puntuationNumber[j]);
        for(int h = 0; h <= puntuationNumber[j]; h++){
            printf("#");
        }
    }

    printf("\n");

    return 0;
}

int matrix(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if(ctx->document == NULL) {
        printError("\n", "No document available, load a document first!\n");
        return EXIT_FAILURE;
    }
    
    int n;
    while(true) {
        printf("\nHow many words do you want to analyze (3-6): ");
        readInteger(&n);
    
        if(n >= 3 && n <= 6) break;
        printWarning("\n", "The number of words should be in the range.\n");
    }

    int matrix[n][n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = 0;

    char **words = malloc(n * sizeof(char *));
    int count = 0;

    while (count < n) {
        char input[100];
        printf("\nType word %d of %d: ", count + 1, n);
        readString(input, MAX_WORD_LENGTH);
        input[strcspn(input, "\r\n")] = '\0';

        char clean[MAX_WORD_LENGTH];
        lowerString(input, clean);

        if (!isValidSingleWord(clean)) {
            printWarning("\n", "Invalid word. Please use only letters.\n");
            continue;
        }

        words[count] = malloc(strlen(clean) + 1);
        strcpy(words[count], clean);
        count++;
    }

    int numberOfParagraphs;
    documentParagraphsSize(ctx->document, &numberOfParagraphs);

    for (int i = 0; i < numberOfParagraphs; i++) {
        Paragraph p;
        documentGetParagraph(ctx->document, i, &p);

        bool appeared[n];
        for (int a = 0; a < n; a++) appeared[a] = false;

        for (int j = 0; j < p.numberTokens; j++) {
            if (p.tokens[j].type != WORD) continue;

            for (int k = 0; k < n; k++) {
                if (strcmp(p.tokens[j].text, words[k]) == 0) {
                    appeared[k] = true;
                }
            }
        }

        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (appeared[r] && appeared[c]) {
                    matrix[r][c]++;
                }
            }
        }
    }

    printf("%*s", CELL_WIDTH, ""); // empty corner
    for (int i = 0; i < n; i++) {
        printf("| %-*s", CELL_WIDTH - 2, words[i]);
    }
    printf("|\n");

    // Separator line
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) printf("_");
    }
    printf("\n");

    // Rows
    for (int i = 0; i < n; i++) {
        printf("| %-*s", CELL_WIDTH - 2, words[i]); // row header
        for (int j = 0; j < n; j++) {
            printf("| %*d ", CELL_WIDTH - 3, matrix[i][j]);
        }
        printf("|\n");
    }

    return 0;
}

int topn(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;
    if(ctx->document == NULL) {
        printf("\n[ERROR] No document available, load a document first!\n");
        return EXIT_FAILURE;
    }

    int numberWords = 0;
    printf("Enter the number of most frequent words you want to consult: ");
    readInteger(&numberWords);

    int size;
    documentParagraphsSize(ctx->document, &size);

    PtMap map = mapCreate();
    Paragraph p;
    for(int i = 0; i < size; i++){
        documentGetParagraph(ctx->document, i, &p);
        paragraphWordsAppearance(map, &p);
    }

    int mSize;
    mapSize(map, &mSize);

    MapKey* keys = mapKeys(map);
    MapValue* values = mapValues(map);

    for(int j = 0; j < mSize; j++){
        for(int k = 0; k < mSize - j - 1; k++){
            if(values[k] < values[k+1]){
                int temp = values[k];
                values[k] = values[k+1];
                values[k+1] = temp;

                char* str = keys[k];
                keys[k] = keys[k+1];
                keys[k+1] = str;
            } else if(values[k] == values[k+1]){
                if(strcasecmp(keys[k], keys[k+1]) > 0){
                    int temp = values[k];
                    values[k] = values[k+1];
                    values[k+1] = temp;

                    char* str = keys[k];
                    keys[k] = keys[k+1];
                    keys[k+1] = str;
                }
            }
        }
    }

    mapClear(map);

    for(int l = 0; l < numberWords; l++){
        mapPut(map, keys[l], values[l]);
    }

    printf("\n\nParameters: \t\nN = %d", numberWords);

    int maxKeyLen = 0;
    for (int i = 0; i < numberWords && i < mSize; i++) {
        int len = utf8_strlen(keys[i]);
        if (len > maxKeyLen)
            maxKeyLen = len; 
    }

    printf("\n\n%*s - Frequency\n", maxKeyLen, "Word");

    for (int i = 0; i < numberWords && i < mSize; i++) {
        printf("%*s - %d\n", maxKeyLen, keys[i], values[i]);
    }

    free(keys);
    free(values);
    mapDestroy(&map);

    return 0;
}

int clear(void *context, int argc, char **argv) {
    system("clear");
    printf("Tip: Type 'help' to see the list of available commands.\n");
  
    return EXIT_SUCCESS;
}

int quit(void *context, int argc, char **argv) {
    EditorContext *ctx = (EditorContext *)context;

    if (ctx->document != NULL || ctx->actions != NULL) {
        close(context, argc, argv);
    }

    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }

    if (ctx->actions) {
        stackDestroy(&ctx->actions);
    }

    printInfo("\n", "Exiting the simplified text editor. Thank you for using our program!\n");
    exit(EXIT_SUCCESS);
}