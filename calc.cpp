#include "headers/DSL.h"
#include "headers/calc.h"
#include "headers/standTree/front_end.h"
#include <math.h>
#include <ctype.h>
#include <string.h>

//----------------------------------------
//!
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

static Type_t getCos(char **buffer, VarList *varList, size_t *err);

static Type_t getSin(char **buffer, VarList *varList, size_t *err);

static Type_t getLog(char **buffer, VarList *varList, size_t *err);

static Type_t getNeg(char **buffer, VarList *varList, size_t *err);

static Type_t getRev(char **buffer, VarList *varList, size_t *err);

static Type_t getVarVal(size_t *err);

static Type_t getVarInd(size_t *err);

static size_t getInd(VarList *varList, char *name, size_t *err);

static size_t resize(VarList *list, size_t newSize);

static OperandType isBinOp(char **buffer, size_t *err);

Type_t getF(char **buffer, VarList *varList, size_t *err) {
    catchNullptr(buffer, nullptr, *err |= calcNullCaught;);

    char *name = getV(buffer, err);
    if (*err) ERR_EXE(err);

    size_t index = getInd(varList, name, err);
    if (index == -1 || *err) return nullptr;

    if (CUR_SYM != '=') ERR_EXE(calcGetF_Error);

    Type_t val = getB(buffer, err);

    Type_t node = nullptr;
    *err |= createDeclarationNode(&node, index, val);

    if (CUR_SYM != '\n') ERR_EXE(calcEndOfProgramErr);
    
    return node;
}

Type_t getG(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node = getB(buffer, varList, err);

    fprintf(stderr, "%c\n", CUR_SYM);
    if (CUR_SYM != '\n' && CUR_SYM != '\0') ERR_EXE(*err | calcEndOfProgramErr);
    
    return node;
}

Type_t getB(char **buffer,  size_t *err) {
    catchNullptr(buffer, POISON, *err |= calcNullCaught;);

    Type_t      node1 = getE(buffer, err);

    OperandType  op   = isBinOp(buffer, err);

    while (op) {
        Type_t  node  = nullptr;

        *err |= newOpNode(&node, op);

        NEXT_SYM;

        Type_t node2 = getT(buffer, varList, err);

        if (node1 == nullptr) ERR_EXE(calcGetE_Error);
        if (node2 == nullptr) ERR_EXE(calcGetE_Error);

        node -> lft = node1;
        node -> rgt = node2;   

        node1 = node;

        op = isBinOp(buffer, err);
    }

    return node1;
}

Type_t getE(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node1 = getT(buffer, varList, err);

    while (CUR_SYM == '+' || CUR_SYM == '-') {
        Type_t node  = nullptr;

        if (CUR_SYM == '+')
            *err |= newOpNode(&node, Add);
        else
            *err |= newOpNode(&node, Sub);

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

Type_t getT(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node1 = getD(buffer, varList, err);

     while (CUR_SYM == '*' || CUR_SYM == '/') {
        Type_t node  = nullptr;

        if (CUR_SYM == '*')
            *err |= newOpNode(&node, Mul);
        else
            *err |= newOpNode(&node, Div);

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

Type_t getD(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    Type_t node1 = getU(buffer, varList, err);
    catchNullptr2(buffer, nullptr, *err |= calcGetD_Error;);

    while (CUR_SYM == '^') {
        NEXT_SYM;

        Type_t node  =     nullptr      ;
        *err |= newOpNode(&node, Pow);

        Type_t node2 = getU(buffer, varList, err);
        catchNullptr2(buffer, nullptr, *err |= calcGetD_Error;);

        node -> lft = node1;
        node -> rgt = node2;

        node1 = node;
    }

    return node1;
}

Type_t getU(char **buffer, VarList *varList, size_t *err) {
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

    if (CUR_SYM == '!') {
        Type_t node = getRev(buffer, varList, err);
        if (node == nullptr || *err) ERR_EXE(calcGetU_Error);

        return node;
    }

    Type_t node = getP(buffer, varList, err);

    if (node == nullptr || *err) ERR_EXE(calcGetU_Error);

    return node;
}

Type_t getP(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    if (CUR_SYM == '(') {
        NEXT_SYM;
        Type_t node = getB(buffer, varList, err);
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

Type_t getV(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    char *newVar = (char *) calloc(MAX_VAR_SIZE, sizeof(char));
    char *curVar =                     newVar                 ;

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

    if (varList -> capacity == varList -> size)
        *err |= resize(varList, varList -> capacity * 2);
    if (*err) ERR_EXE(*err);
    
    varList -> names[varList -> size++] = newVar;

    Type_t node = nullptr;
    *err |= newIndexNode(&node, Varriable, varList -> size - 1);
    
    return node;
}

static Type_t getCos(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t arg  = getB(buffer, varList, err);
    Type_t node =      nullptr     ;

    *err |= newOpNode(&node, Cos);
    if (arg == nullptr || CUR_SYM != ')') ERR_EXE(calcGetCosError);

    NEXT_SYM;

    node -> lft = arg;

    return node;
}

static Type_t getSin(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t arg  = getB(buffer, varList, err);
    Type_t node =      nullptr     ;

    *err |= newOpNode(&node, Sin);
    if (arg == nullptr || CUR_SYM != ')') ERR_EXE(calcGetSinError);

    NEXT_SYM;

    node -> lft = arg;

    return node;
}

static Type_t getLog(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    CUR_STR += 3;

    Type_t arg  = getB(buffer, varList, err);
    Type_t node =           nullptr         ;

    *err |= newOpNode(&node, Log);
    if (arg == nullptr || CUR_SYM != ')') ERR_EXE(calcGetLogError);

    NEXT_SYM;

    node -> lft = arg;

    return node;
}

static Type_t getNeg(char **buffer, VarList *varList, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    CUR_STR += 4;

    Type_t arg  = getB(buffer, varList, err);
    Type_t node =      nullptr     ;
    Type_t neg  =      nullptr     ;

    *err |= newOpNode (&node, Operator, Mul);
    *err |= newNumNode(&node, Numeral,  -1);

    if (arg == nullptr) ERR_EXE(calcGetLogError);

    NEXT_SYM;

    node -> lft = neg;
    node -> rgt = arg;

    return node;
}

static Type_t getRev(char **buffer, size_t *err) {
    catchNullptr2(buffer, nullptr, *err |= calcNullCaught;);

    NEXT_SYM;

    Type_t arg1  = getB(buffer, varList, err);
    Type_t arg2 =           nullptr         ;
    Type_t node =           nullptr         ;

    *err |= newOpNode (&node, NotEq);
    *err |= newNumNode(&node, NULL );

    if (*err || arg1 == nullptr || arg2 == nullptr || CUR_SYM != ')') ERR_EXE(calcGetRevError);

    NEXT_SYM;

    node -> lft = arg1;
    node -> rgt = arg2;

    return node;
}

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

static OperandType isBinOp(char **buffer, size_t *err) {
    catchNullptr(buffer, None, *err |= calcNullCaught;);

    if (CUR_SYM == '<') {
        NEXT_SYM;
        if (CUR_SYM == '=') {
            NEXT_SYM;
            return LsEq;
        } else
            return Ls;
    }

    if (CUR_SYM == '>') {
    NEXT_SYM;
        if (CUR_SYM == '=') {
            NEXT_SYM;
            return BgEq;
        } else
            return Bg;
    }

    if (CUR_SYM == '=') {
        NEXT_SYM;
        if (CUR_SYM == '=') {
            NEXT_SYM;
            return Eq;
        } else {
            *err |= calcUnexpectedSymbol;
            return None;
        }
    }

    if (CUR_SYM == '!') {
        NEXT_SYM;
        if (CUR_SYM == '=') {
            NEXT_SYM;
            return NotEq;
        } else {
            *err |= calcUnexpectedSymbol;
            return None;
        }
    }

    return None;
}