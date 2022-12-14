#include <stdlib.h>
#include <stdio.h>
#include "standTree/front_end.h"
#include "config.h"

struct Buffer {
    const char *fileName = nullptr;

    FILE *stream = nullptr;
    
    char *buffer = nullptr;
    size_t size  =   0    ;

};

enum CalcErrors {
    calcOk               =       0,
    calcNullCaught       = 1 <<  0,
    calcGetG_Error       = 1 <<  1,
    calcGetE_Error       = 1 <<  1,
    calcGetT_Error       = 1 <<  1,
    calcGetD_Error       = 1 <<  2,
    calcGetP_Error       = 1 <<  3,
    calcGetU_Error       = 1 <<  4,
    calcGetN_Error       = 1 <<  5,
    calcGetV_Error       = 1 <<  6,
    calcGetF_Error       = 1 <<  7,
    calcGetIf_Error      = 1 <<  1,
    calcGetCosError      = 1 <<  9,
    calcGetSinError      = 1 << 10,
    calcGetLogError      = 1 << 11,
    calcGetNegError      = 1 << 12,
    calcGetRevError      = 1 << 13,
    calcBufferError      = 1 <<  8,
    calcDeclare_Error    = 1 << 11,
    calcGetWhile_Error   = 1 << 12,
    calcDevisionByZero   = 1 << 13,
    calcEndOfProgramErr  = 1 << 14,
    calcUnexpectedSymbol = 1 << 15,
};

struct NameList {
    char   **names  = (char **) calloc(1, sizeof(char*));
    int    *values  =  (int *)  calloc(1,  sizeof(int) ); //might be unneccessary(((
    size_t   size   =              0                    ;
    size_t capacity =              1                    ;
};

// int bufferCtor(Buffer *buf, const char *fileName);

// int bufferDtor(Buffer *buf);

Type_t getG(char **buffer, NameList *varList, size_t *err);

Type_t getWhile(char **buffer, NameList *varList, size_t *err);

Type_t getIf(char **buffer, NameList *varList, size_t *err);

Type_t declare(char **buffer, NameList *varList, size_t *err);

Type_t getB(char **buffer, NameList *varList, size_t *err);

Type_t getE(char **buffer, NameList *varList, size_t *err);

Type_t getT(char **buffer, NameList *varList, size_t *err);

Type_t getP(char **buffer, NameList *varList, size_t *err);

Type_t getN(char **buffer, size_t *err);

Type_t getV(char **buffer, NameList *varList, size_t *err);

Type_t getD(char **buffer, NameList *varList, size_t *err);

Type_t getU(char **buffer, NameList *varList, size_t *err);