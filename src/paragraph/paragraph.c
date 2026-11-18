#include "action.h"
#include "paragraph.h"

Paragraph paragraphCreate() {
    Paragraph p;
    p.numberTokens = 0;
    p.tokens = NULL;
    
    return p;
}

void paragraphPrint(const Paragraph *p) {
    if (!p || p->numberTokens == 0) {
        printf("Empty paragraph.\n");
        return;
    }

    for (int i = 0; i < p->numberTokens; i++) {
        printToken(&p->tokens[i]);  
    }
}

void printParagraphCentered(const Paragraph *p, int width) {
    if (!p || p->numberTokens == 0) {
        printf("Empty paragraph.\n");
        return;
    }

    char buffer[4096] = "";
    for (int j = 0; j < p->numberTokens; j++) {
        strcat(buffer, p->tokens[j].text);
    }

    char *start = buffer;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        return;
    }

    printf("\n");
    int len = strlen(start);
    int lineStart = 0;

    while (lineStart < len) {
        int lineEnd = lineStart + width;
        if (lineEnd >= len) {
            int lineLen = len - lineStart;
            int pad = (width - lineLen) / 2;
            if (pad < 0) pad = 0;
            printf("%*s%.*s\n", pad, "", lineLen, start + lineStart);
            break;
        }

        int breakPos = lineEnd;
        while (breakPos > lineStart && start[breakPos] != ' ') {
            breakPos--;
        }

        if (breakPos == lineStart) breakPos = lineEnd;

        int lineLen = breakPos - lineStart;
        int pad = (width - lineLen) / 2;
        if (pad < 0) pad = 0;
        printf("%*s%.*s\n", pad, "", lineLen, start + lineStart);

        while (start[breakPos] == ' ') breakPos++;
        lineStart = breakPos;
    }
}

void printParagraphLeftAligned(const Paragraph *p, int width) {
    if (!p || p->numberTokens == 0) {
        printf("Empty paragraph.\n");
        return;
    }

    char line[4096] = "";
    int lineLen = 0;

    for (int i = 0; i < p->numberTokens; i++) {
        Token *token = &p->tokens[i];

        if (token->type == WHITESPACE && lineLen == 0) {
            continue;
        }

        int tokenLen = strlen(token->text);

        if (lineLen + tokenLen > width && token->type == WHITESPACE) {
            printf("%s\n", line);
            line[0] = '\0';
            lineLen = 0;
            continue;
        }

        if (lineLen + tokenLen > width) {
            printf("%s\n", line);
            line[0] = '\0';
            lineLen = 0;

            if (token->type == WHITESPACE) continue;
        }

        strcat(line, token->text);
        lineLen += tokenLen;
    }

    if (lineLen > 0) {
        printf("%s\n", line);
    }
}

Paragraph processStringToParagraph(const char *input) {
    Paragraph p = paragraphCreate();

    int len = strlen(input);
    int i = 0;

    while (i < len) {
        Token t;

        if (isspace(input[i])) {
            char ws[2] = {input[i], '\0'};
            t = createToken(ws, WHITESPACE);
            i++;
        } else if (ispunct(input[i])) {
            char punct[2] = {input[i], '\0'};
            t = createToken(punct, PUNCTUATION);
            i++;
        } else {
            int start = i;
            while (i < len && !isspace(input[i]) && !ispunct(input[i])) i++;
            char word[256];
            int wordLen = i - start;
            if (wordLen >= sizeof(word)) wordLen = sizeof(word) - 1;
            strncpy(word, input + start, wordLen);
            word[wordLen] = '\0';
            t = createToken(word, WORD);
        }

        Token *newTokens = realloc(p.tokens, (p.numberTokens + 1) * sizeof(Token));
        if (!newTokens) {
            paragraphDestroy(&p);
            return paragraphCreate();
        }

        p.tokens = newTokens;
        p.tokens[p.numberTokens++] = t;
    }

    return p;
}

void addTokenToParagraph(Paragraph *p, Token token) {
    p->tokens = (Token *)realloc(p->tokens, (p->numberTokens + 1) * sizeof(Token ));
    if (!p->tokens) {
        fprintf(stderr, "Error reallocating memory for paragraph tokens.\n");
        return;
    }

    p->tokens[p->numberTokens] = token;  
    p->numberTokens++;
}

int countParagraphCharacters(const Paragraph *p, int includePunctuation, int includeWhitespace) {
    int count = 0;

    for (int i = 0; i < p->numberTokens; i++) {
        Type type = p->tokens[i].type;
        if ((type == WORD) || (includePunctuation && type == PUNCTUATION) || (includeWhitespace && type == WHITESPACE)) {
            count += strlen(p->tokens[i].text);
        }
    }

    return count;
}

void countParagraphPonctuation(const Paragraph *p, int arr[], char *str){
    for (int i = 0; i < p->numberTokens; i++) {
        for(int j = 0; j < 6; j++){
            if (p->tokens[i].type == PUNCTUATION && p->tokens[i].text[0] == str[j]) {
                arr[j]++;
            }
        }
    }
}

int countParagraphWords(const Paragraph *p) {
    int count = 0;

    for (int i = 0; i < p->numberTokens; i++) {
        if (p->tokens[i].type == WORD) {
            count++;
        }
    }

    return count;
}

void countSizeBiggestWord(const Paragraph *p, int *size, int *count){
    int sizeWord = 0;
    int countWord = 0;
    for (int i = 0; i < p->numberTokens; i++) {
        if (p->tokens[i].type == WORD) {
            if ( sizeWord < strlen(p->tokens[i].text)){
                sizeWord = strlen(p->tokens[i].text);
                countWord = 1;
            } else if(sizeWord == strlen(p->tokens[i].text)){
                countWord++;
            }
        }
    }

    *size = sizeWord;
    *count = countWord;
}

int countBiggestWords(const Paragraph *p, char **biggestWords, int offset, int size) {
    int count = 0;

    for (int j = 0; j < p->numberTokens; j++) {
        if (p->tokens[j].type == WORD && size == strlen(p->tokens[j].text)) {
            biggestWords[offset + count] = malloc(strlen(p->tokens[j].text) + 1);   
            strcpy(biggestWords[offset + count], p->tokens[j].text);
            count++;
        }
    }

    return count;
}

void paragraphWordsAppearance(PtMap map, Paragraph *p){
    int size, value; 
    mapSize(map, &size);
    for (int i = 0; i < p->numberTokens; i++) { 
        if (p->tokens[i].type == WORD && mapContains(map, p->tokens[i].text)) {
            mapGet(map, p->tokens[i].text, &value);
            mapPut(map, p->tokens[i].text, value + 1);
        } else if (p->tokens[i].type == WORD){
            mapPut(map, p->tokens[i].text, 1);
        }
    }
}

int replaceParagraphWord(Paragraph *p, int index, const char *oldWord, const char *newWord, PtAction *mainAction) {
    int count = 0;
    bool firstChangeInThisParagraph = true;

    for (int i = 0; i < p->numberTokens; i++) {
        Token *token = &p->tokens[i];

        if (token->type == WORD && strcmp(token->text, oldWord) == 0) {
            if (firstChangeInThisParagraph) {
                firstChangeInThisParagraph = false;
            }

            strncpy(token->text, newWord, MAX_TEXT - 1);
            token->text[MAX_TEXT - 1] = '\0';
            count++;
        }
    }

    if (count > 0) {
        Paragraph backup = paragraphClone(*p);

        PtAction newAction = actionCreate(index, backup, REPLACE);
        if (!newAction) return count;

        if (*mainAction == NULL) {
            *mainAction = newAction;
        } else {
            PtAction currentAction = *mainAction;
            while (currentAction->next != NULL)
                currentAction = currentAction->next;

            actionAppend(currentAction, newAction);
        }
    }

    return count;
}

void paragraphDestroy(Paragraph *p) {
    if (!p || !p->tokens) return;

    free(p->tokens); 
    p->tokens = NULL;
    p->numberTokens = 0;
}

Paragraph paragraphClone(Paragraph p) {
    Paragraph clone;
    clone.numberTokens = p.numberTokens;

    if (clone.numberTokens == 0) {
        clone.tokens = NULL;
        return clone;
    }

    clone.tokens = (Token *)malloc(sizeof(Token) * clone.numberTokens);
    if (!clone.tokens) {
        clone.numberTokens = 0;
        return clone;
    }

    for (int i = 0; i < clone.numberTokens; i++) {
        clone.tokens[i].type = p.tokens[i].type;
        strncpy(clone.tokens[i].text, p.tokens[i].text, MAX_TEXT - 1);
        clone.tokens[i].text[MAX_TEXT - 1] = '\0';
    }

    return clone;
}

int isParagraphEmpty(const Paragraph *p) {
    for (int j = 0; j < p->numberTokens; j++) {
        for (char *c = p->tokens[j].text; *c != '\0'; ++c) {
            if (!isspace((unsigned char)*c)) {
                return 0;
            }
        }
    }
    return 1;
}