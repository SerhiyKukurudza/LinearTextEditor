#include "document.h"
#include "list.h"
#include "dateTime.h"
#include "paragraph.h"

Document *documentCreate(const char *name) {
    Document *d = (Document *)malloc(sizeof(Document));

    if (!d) {
        fprintf(stderr, "Error allocating memory for document.\n");
        return NULL;
    }

    strncpy(d->name, name, MAX_NAME_LENGTH);
    d->loadDate = dateTimeCreate();
    d->modifiedDate = (DateTime){0};
    d->paragraphs = listCreate();

    if (!d->paragraphs) {
        free(d);
        fprintf(stderr, "Error creating paragraph list.\n");
        return NULL;
    }

    return d;
}

void documentPrint(Document *doc) {
    if (doc == NULL) {
        printf("\n[INFO] No document is currently loaded.\n");
    } else {
        printf("\n[Document Info]\n");
        printf("  Name: %s\n", doc->name);
    
        printf("  Load Date: ");
        dateTimePrint(doc->loadDate);
    
        printf("  Modified Date: ");
        dateTimePrint(doc->modifiedDate);
        printf("\n");
    }
}

void documentDestroy(Document *doc) {
    if (!doc) return;

    if (doc->paragraphs) {
        int size = 0;
        if (listSize(doc->paragraphs, &size) == LIST_OK) {
            for (int i = 0; i < size; i++) {
                Paragraph p;
                if (listGet(doc->paragraphs, i, &p) == LIST_OK) {
                    paragraphDestroy(&p);
                }
            }
        }
        listDestroy(&doc->paragraphs);
    }

    free(doc);
}

int documentAddParagraph(Document *doc, int index, Paragraph paragraph) {
    Paragraph clone = paragraphClone(paragraph);
    if (clone.numberTokens == 0 && paragraph.numberTokens != 0) {

        return LIST_FULL;
    }
    int ret = listAdd(doc->paragraphs, index, clone);
    return ret;
}

int documentParagraphsSize(Document *doc, int *ptSize) {
    return listSize(doc->paragraphs, ptSize);
}

int documentGetParagraph(Document *doc, int index, Paragraph *ptParagraph) {
    return listGet(doc->paragraphs, index, ptParagraph);
}

int documentRemoveParagraph(Document *doc, int index, Paragraph *ptParagraph) {
    return listRemove(doc->paragraphs, index, ptParagraph);
}

int documentSetParagraph(Document *doc, int index, Paragraph newParagraph, Paragraph *ptReplacedParagraph) {
    Paragraph old;
    int res = listGet(doc->paragraphs, index, &old);
    if (res != LIST_OK) return res;

    *ptReplacedParagraph = paragraphClone(old);
    Paragraph cloned = paragraphClone(newParagraph);

    int result = listSet(doc->paragraphs, index, cloned, &old);
    paragraphDestroy(&newParagraph);
    paragraphDestroy(&old);

    return result;
}