#include <stdio.h>
#include "paragraph.h"

typedef Paragraph ListElem;

void listElemPrint(ListElem elem) {
    paragraphPrint(&elem);
}