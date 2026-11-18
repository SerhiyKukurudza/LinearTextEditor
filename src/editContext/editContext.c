#include <stdlib.h>
#include "editContext.h"

#define NULL_DOCUMENT NULL
#define NOT_MODIFIED false
#define MODIFIED true

EditorContext editContextCreate() {
    EditorContext ctx;
    ctx.document = NULL_DOCUMENT;
    ctx.isModified = NOT_MODIFIED;
    return ctx;
}

void editContextReset(EditorContext *ctx) {
    if (ctx->document != NULL_DOCUMENT) {
        documentDestroy(ctx->document);
        ctx->document = NULL_DOCUMENT;
    }
    
    stackClear(ctx->actions);

    ctx->isModified = NOT_MODIFIED;
}