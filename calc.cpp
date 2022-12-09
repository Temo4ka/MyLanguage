#include "headers/DSL.h"
#include "headers/calc.h"
#include "headers/standTree/front_end.h"
#include <math.h>
#include <ctype.h>
#include <string.h>

//----------------------------------------
//!
//! Присваивание:
//! F = V [=] G
//! Число:
//! G = B, '\n'
//! B = E {[<, <=, >, >=, ==] E}*
//! E = T {[+, -] T}*
//! T = D {[*, /] D}*
//! D = U {[^] U}*
//! U = cos(E) | sin(E) | ln(E) | -E | P
//! P = (E) | N | V
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

static  bool  isBinOp(char c);

Type_t getF(char **buffer, VarList *varList, size_t *err) {
    catchNullptr(buffer, nullptr, *err |= calcNullCaught;);

    char *name = getV(buffer, err);
    if (*err) ERR_EXE(err);

    size_t index = getInd(varList, name, err);
    if (index == -1 || *err) return nullptr;

    if (CUR_SYM != '=') ERR_EXE(calcGetF_Error);

    Type_t val = getE(buffer, err);

    StandType_t node = nullptr;
    *err |= createDeclarationNode(&node, index, val);

    if (CUR_SYM != '\n') ERR_EXE(calcEndOfProgramErr);
    
    return node;
}

Type_t getG(char **buffer, Vocabulary *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node = getE(buffer, varList, err);

    fprintf(stderr, "%c\n", CUR_SYM);
    if (CUR_SYM != '\n' && CUR_SYM != '\0') ERR_EXE(calcEndOfProgramErr);
    
    return node;
}

Type_t getB(char **buffer, size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t val = getE(buffer, err);

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

Type_t getE(char **buffer, Vocabulary *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node1 = getT(buffer, varList, err);

    while (CUR_SYM == '+' || CUR_SYM == '-') {
        Type_t node  = nullptr;

        if (CUR_SYM == '+')
            *err |= newOpNode(&node, Operand, Add);
        else
            *err |= newOpNode(&node, Operand, Sub);

        NEXT_SYM;

        Type_t node2 = getT(buffer, varList, err);

        if (node1 == nullptr) ERR_EXE(calcGetE_Error);
        if (node2 == nullptr) ERR_EXE(calcGetE_Error);

        node -> lft = node1;
        node -> rgt = node2;   

        node1 = node;         
    }

    return node1;
}

Type_t getT(char **buffer, Vocabulary *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node1 = getD(buffer, varList, err);

     while (CUR_SYM == '*' || CUR_SYM == '/') {
        Type_t node  = nullptr;

        if (CUR_SYM == '*')
            *err |= newOpNode(&node, Operand, Mul);
        else
            *err |= newOpNode(&node, Operand, Div);

        NEXT_SYM;

        Type_t node2 = getT(buffer, varList, err);

        if (node1 == nullptr) ERR_EXE(calcGetE_Error);
        if (node2 == nullptr) ERR_EXE(calcGetE_Error);

        node -> lft = node1;
        node -> rgt = node2;

        node1 = node;            
    }

    return node1;
}

Type_t getD(char **buffer, Vocabulary *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node1 = getU(buffer, varList, err);
    catchNullptr2(buffer, nullptr, *err |= calcGetD_Error;);

    while (CUR_SYM == '^') {
        NEXT_SYM;

        Type_t node  =     nullptr      ;
        *err |= newOpNode(&node, Operand, Pow);

        Type_t node2 = getU(buffer, varList, err);
        catchNullptr2(buffer, nullptr, *err |= calcGetD_Error;);

        node -> lft = node1;
        node -> rgt = node2;

        node1 = node;
    }

    return node1;
}

Type_t getU(char **buffer, Vocabulary *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    if (!strncmp("cos(", CUR_STR, 4)) {
        Type_t node = getCos(buffer, varList, err);
        if (node == nullptr || *err) ERR_EXE(calcGetU_Error);

        return node;
    }

    if (!strncmp("sin(", CUR_STR, 4)) {
        Type_t node = getSin(buffer, varList, err);
        if (node == nullptr || *err) ERR_EXE(calcGetU_Error);
        
        return node;
    }

    if (!strncmp("ln(", CUR_STR, 3)) {
        // fprintf(stderr, "Here\n");
        Type_t node = getLog(buffer, varList, err);
        if (node == nullptr || *err) ERR_EXE(calcGetU_Error);
        
        return node;
    }

    if (CUR_SYM == '-') {
        Type_t node = getNeg(buffer, varList, err);
        if (node == nullptr || *err) ERR_EXE(calcGetU_Error);

        return node;
    }

    Type_t node = getP(buffer, varList, err);

    if (node == nullptr || *err) ERR_EXE(calcGetU_Error);

    return node;
}

Type_t getP(char **buffer, Vocabulary *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    if (CUR_SYM == '(') {
        NEXT_SYM;
        Type_t node = getE(buffer, varList, err);
        if (CUR_SYM != ')') ERR_EXE(calcGetP_Error);
        NEXT_SYM;
        
        return node;
    }

    Type_t node = getN(buffer, err);
    if (node == nullptr)
        node = getV(buffer, varList, err);
    catchNullptr2(node, nullptr, *err |= calcGetP_Error;);

    return node;
}

Type_t getN(char **buffer, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Elem_t val = 0;

    if (CUR_SYM < '0' || CUR_SYM > '9') return nullptr;

    while (CUR_SYM >= '0' && CUR_SYM <= '9') {
        val = val * 10 + CUR_SYM - '0';
        NEXT_SYM;
    }

    Type_t node = nullptr;
    *err |= newNumNode(&node, Numeral, val);

    return node;
}

Type_t getV(char **buffer, Vocabulary *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    char *newVar = (char *) calloc(MAX_VAR_SIZE, sizeof(char));
    char *curVar = newVar;

    // fprintf(stderr, "Here\n");
    if (!isalpha(CUR_SYM)) {
        *err |= calcGetV_Error;
        return nullptr;
    }

    while (isalpha(CUR_SYM)) {
        *curVar = CUR_SYM;
        NEXT_SYM;
        curVar++;
    }
    *curVar = '\0';

    varList -> var[varList -> size++] = newVar;

    Type_t node = nullptr;
    *err |= newVarNode(&node, Varriable, newVar);
    
    return node;
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

static bool isBinOp(char *str) {

}