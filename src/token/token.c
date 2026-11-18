#include "token.h"

Token createToken(const char *text, Type type) {
    Token new_token;
    strncpy(new_token.text, text, MAX_TEXT - 1);

    new_token.text[MAX_TEXT - 1] = '\0';
    new_token.type = type;

    return new_token;
}

void printToken(Token *token) {
    if (!token) return;

    printf("Token: \"%s\", Type: ", token->text);
    switch (token->type) {
        case WORD: printf("Word\n"); break;
        case PUNCTUATION: printf("Punctuation\n"); break;
        case WHITESPACE: printf("Whitespace\n"); break;
    }
}