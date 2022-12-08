#include "headers/DSL.h"
#include "headers/calc.h"
#include <sys/types.h>
#include <sys/stat.h>

static size_t getFileSize(const char *fileName);

int assignBuffer(Buffer *buf, const char *fileName) {
    catchNullptr(  buf   , calcNullCaught, NULL;);
    catchNullptr(fileName, calcNullCaught, NULL;);

    buf -> fileName =        fileName      ;
    buf ->  stream  =  fopen(fileName, "r");
    buf ->   size   = getFileSize(fileName);

    buf ->  buffer  = (char *) calloc(buf -> size + 1, sizeof(char));

    int gotSymbols  = fread(buf -> buffer, sizeof(char), buf -> size, buf -> stream);
    buf -> buffer[gotSymbols] = '\0';

    if (!feof(buf -> stream)) return calcBufferError;

    return calcOk;
}

int bufferDtor(Buffer *buf) {
    catchNullptr(     buf     , calcNullCaught, NULL;);
    catchNullptr(buf -> stream, calcNullCaught, NULL;);
    catchNullptr(buf -> buffer, calcNullCaught, NULL;);

    fclose(buf -> stream);
    free  (buf -> buffer);

    buf -> fileName = nullptr;

    buf ->  stream  = nullptr;
    buf ->   size   =   NULL ;
    buf ->  buffer  = nullptr;

    return calcOk;
}

static size_t getFileSize(const char *fileName) {
    catchNullptr(fileName, EXIT_FAILURE);

    struct stat buf = {};
    if (stat(fileName, &buf)) {
        fprintf(stderr, "An Error in reading file occured\n");
        return 0;
    }

    return buf.st_size;
}