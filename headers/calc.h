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
    calcOk                    =       0,
    calcNullCaught            = 1 <<  0,
    calcGetG_Error            = 1 <<  1,
    calcGetE_Error            = 1 <<  2,
    calcGetT_Error            = 1 <<  3,
    calcGetD_Error            = 1 <<  4,
    calcGetP_Error            = 1 <<  5,
    calcGetU_Error            = 1 <<  6,
    calcGetN_Error            = 1 <<  7,
    calcGetV_Error            = 1 <<  8,
    calcGetF_Error            = 1 <<  9,
    calcGetIf_Error           = 1 << 19,
    calcGetCosError           = 1 << 11,
    calcGetSinError           = 1 << 12,
    calcGetLogError           = 1 << 13,
    calcGetNegError           = 1 << 14,
    calcGetRevError           = 1 << 15,
    calcBufferError           = 1 << 16,
    calcGetCall_Error         = 1 << 17,
    calcGetWhile_Error        = 1 << 17,
    calcDevisionByZero        = 1 << 18,
    calcGetReturn_Error       = 1 << 18,
    calcGetString_Error       = 1 << 19,
    calcUnexpectedSymbol      = 1 << 20,
    calcAssignation_Error     = 1 << 21,
    calcEndOfProgramError     = 1 << 22,
    calcUndefinedVarriable    = 1 << 23,
    calcGetDefenition_Error   = 1 << 24,
    calcGetDeclaration_Error  = 1 << 24,
    calcUnknownCommand_Error  = 1 << 25,
};

struct NameList {
    char   **names  = (char **) calloc(1, sizeof(char*));
    int    *values  =  (int *)  calloc(1,  sizeof(int) ); //might be unneccessary(((
    size_t   size   =              0                    ;
    size_t capacity =              1                    ;
};

// int bufferCtor(Buffer *buf, const char *fileName);

// int bufferDtor(Buffer *buf);

Type_t getG(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getWhile(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getIf(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getDeclaration(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getAssignation(char **buffer, NameList *varList, NameList *funcList, size_t *err, char *newVar = nullptr);

Type_t getB(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getE(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getT(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getP(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getN(char **buffer, size_t *err);

Type_t getV(char **buffer, NameList *varList, NameList *funcList, size_t *err, char *newVar = nullptr);

Type_t getD(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getU(char **buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getCall(char ** buffer, NameList *varList, NameList *funcList, size_t *err);

Type_t getReturn(char ** buffer, NameList *varList, NameList *funcList, size_t *err);

char *getString(char **buffer, size_t *err);

int assignBuffer(Buffer *buf, const char *fileName);

int bufferDtor(Buffer *buf);

char *cur_str(char **buffer);

char cur_sym(char **buffer);