#include "headers/DSL.h"
#include "headers/calc.h"
#include "headers/standTree/front_end.h"
#include <math.h>
#include <ctype.h>
#include <string.h>

//----------------------------------------
//!
//! Условие 
//! Присваивание:
//! F = V [=] G
//! Число:
//! G = E, '\n'
//! E = T {[+ -] T}*
//! T = D {[* /] D}*
//! D = U {[^] U}*
//! U = cos(E) | sin(E) | ln(E) | -E | P
//! P = (E) | N | val(V)
//! N = ['0' - '9']+
//! V = ['a' - 'z' | 'A' - 'Z']+
//-----------------------------------------

static Type_t getCos(char **buffer, size_t *err);

static Type_t getSin(char **buffer, size_t *err);

static Type_t getLog(char **buffer, size_t *err);

static Type_t getNeg(char **buffer, size_t *err);

static Type_t getVarVal(size_t *err);

static Type_t getVarInd(size_t *err);

static size_t getInd(VarList *varList, char *name, size_t *err);

StandTreeNode *getF(char **buffer, VarList *varList, size_t *err) {
    catchNullptr(buffer, nullptr, *err |= calcNullCaught;);

    char *name = getV(buffer, err);
    if (*err) ERR_EXE(err);

    size_t index = getInd(varList, name, err);
    if (index == -1 || *err) return nullptr;

    if (CUR_SYM != '=') ERR_EXE(calcGetF_Error);

    Type_t val = getE(buffer, err);

    StandTreeNode *node = nullptr;
    *err |= createDeclarationNode(&node, index, val);

    if (CUR_SYM != '\n') ERR_EXE(calcEndOfProgramErr);
    
    return node;
}

Type_t getG(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getE(buffer, err);

    if (CUR_SYM != '\n') ERR_EXE(calcEndOfProgramErr);
    
    return val;
}

Type_t getE(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getT(buffer, err);

    while (CUR_SYM == '+' || CUR_SYM == '-') {
        char curOp = CUR_SYM;
        NEXT_SYM;
        Type_t val2 = getT(buffer, err);
        if (curOp == '+')
            val += val2;
        else
            val -= val2;
    }

    return val;
}

Type_t getT(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getD(buffer, err);

    while (CUR_SYM == '*' || CUR_SYM == '/') {
        char curOp = CUR_SYM;
        NEXT_SYM;

        Type_t val2 = getD(buffer, err);
        if (val2 == POISON) ERR_EXE(calcGetT_Error);

        if (curOp == '*')
            val *= val2;
        else {
            if (val2 == 0) ERR_EXE(calcDevisionByZero);
            val /= val2;
        }
    }

    return val;
}

Type_t getD(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getU(buffer, err);

    while (CUR_SYM == '^') {
        NEXT_SYM;

        Type_t val2 = getU(buffer, err);
        if (val2 == POISON) ERR_EXE(calcGetD_Error);

        val = pow(val, val2);
    }

    return val;
}

Type_t getU(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    if (!strncmp("cos(", CUR_STR, 4)) {
        Type_t val = getCos(buffer, err);
        if (val == POISON || *err) ERR_EXE(calcGetU_Error);

        return val;
    }

    if (!strncmp("sin(", CUR_STR, 4)) {
        Type_t val = getSin(buffer, err);
        if (val == POISON || *err) ERR_EXE(calcGetU_Error);
        
        return val;
    }

    if (!strncmp("ln(", CUR_STR, 3)) {
        Type_t val = getLog(buffer, err);
        if (val == POISON || *err) ERR_EXE(calcGetU_Error);
        
        return val;
    }

    if (CUR_SYM == '-') {
        Type_t val = getNeg(buffer, err);
        if (val == POISON || *err) ERR_EXE(calcGetU_Error);

        return val;
    }

    Type_t val = getP(buffer, err);

    if (val == POISON || *err) ERR_EXE(calcGetU_Error);

    return val;
}

Type_t getP(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    if (CUR_SYM == '(') {
        NEXT_SYM;
        Type_t val = getE(buffer, err);
        if (CUR_SYM != ')') ERR_EXE(calcGetP_Error);
        NEXT_SYM;
        
        return val;
    }
    Type_t val = getN(buffer, err);
    if (val == POISON) {
        val = getVarVal(err);
        if (val == POISON || *err) ERR_EXE(*err | calcGetP_Error);
    }

    return val;
}

Type_t getN(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = 0;

    if (CUR_SYM < '0' && CUR_SYM > '9') return POISON;

    while (CUR_SYM >= '0' && CUR_SYM <= '9') {
        val = val * 10 + CUR_SYM - '0';
        NEXT_SYM;
    }

    return val;
}

char* getV(char **buffer, size_t *err) {
    catchNullptr(buffer, nullptr, *err |= calcNullCaught;);

    char *newVar = (char *) calloc(MAX_VAR_SIZE, sizeof(char));
    char *curVar = newVar;

    if (!isalpha(CUR_SYM)) {
        *err |= calcGetV_Error;
        return nullptr;
    }

    while (isalpha(CUR_SYM)) {
        *curVar = CUR_SYM;
        NEXT_SYM;
        curVar++;
    }
    
    return newVar;
}

static Type_t getCos(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t val = getE(buffer, err);

    if (val == POISON || CUR_SYM != ')') ERR_EXE(calcGetCosError);

    NEXT_SYM;

    return cos(val);
}

static Type_t getSin(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t val = getE(buffer, err);

    if (val == POISON || CUR_SYM != ')') ERR_EXE(calcGetSinError);

    NEXT_SYM;

    return sin(val);
}

static Type_t getLog(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t val = getE(buffer, err);

    if (val == POISON || CUR_SYM != ')') ERR_EXE(calcGetLogError);

    NEXT_SYM;

    return log(val);
}

static Type_t getNeg(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    CUR_STR++;

    Type_t val = getE(buffer, err);

    if (val == POISON) ERR_EXE(calcGetNegError);

    return -val;
}

static size_t resize(VarList *list, size_t newSize);

size_t newVar(VarList *list, char *name, int val) {
    catchNullptr(list, calcNullCaught, NULL;);
    size_t err = 0;

    if (list -> size == list -> capacity)
        err = resize(list, list -> capacity * 2);

    if (err) return err;

    list ->  names[list -> size++] = name;
    list -> values[list -> size]   =  val;

    return calcOk;
}

int *getVal(VarList *varList, char *name, size_t *err) {
    catchNullptr(list, nullptr, *err |= calcNullCaught;);
    catchNullptr(name, nullptr, *err |= calcNullCaught;);
    
    for (size_t cur = 0; cur < list -> size; cur++) {
        if (!stricmp(name, list -> names[cur]));
            return list -> names + cur;
    }

    return nullptr;
}

static size_t resize(VarList *list, size_t newSize) {
    catchNullptr(list, calcNullCaught, NULL;);
    
    list -> names  = (char **) realloc(list -> names , newSize);
    catchNullptr(list -> names , calcNullCaught, NULL;);

    list -> values =  (int *)  realloc(list -> values, newSize);
    catchNullptr(list -> values, calcNullCaught, NULL;);

    for (size_t cur = list -> size; cur < newSize; cur++) {
        list -> values[cur] =    0   ;
        list ->  names[cur] = nullptr;
    }

    list -> capacity = newSize;

    return calcOk;
}

static Type_t getVarVal(size_t *err) {
    catchNullptr(name, POISON, *err |= calcNullCaught;);

    char *name = getV(buffer, err);
    if (*err) ERR_EXE(*err);

    int *value = getVal(varList, name, err);

    if (value == nullptr) *err |= calcGetP_Error;
    if (*err) ERR_EXE(*err);

    return *value;
}

size_t getInd(VarList *varList, char *name, size_t *err) {
    catchNullptr(list, nullptr, *err |= calcNullCaught;);
    catchNullptr(name, nullptr, *err |= calcNullCaught;);
    
    for (size_t cur = 0; cur < list -> size; cur++) {
        if (!stricmp(name, list -> names[cur]));
            return cur;
    }

    return -1;
}