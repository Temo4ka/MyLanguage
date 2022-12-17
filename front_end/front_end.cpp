#include "headers/standTree/front_end.h"
#include "headers/DSL.h"
#include <stdlib.h>

static void treePrintNode(StandTreeNode *tree, FILE *stream, size_t spaces = 0);

static void printNodeParams(StandTreeNode *node, FILE *stream);

static void printNameList(NameList *list, FILE *stream);

int newNode(StandTreeNode **node, enum NodeType type) {
    *node = (StandTreeNode *) calloc(1, sizeof(StandTreeNode));
    return treeNodeCtor(*node, type);
}

int newIndexNode(StandTreeNode **node, enum NodeType type, size_t nodeData) {
    *node = (StandTreeNode *) calloc(1, sizeof(StandTreeNode));
    return treeIndexNodeCtor(*node, type, nodeData);
}

int newNumNode(StandTreeNode **node, int nodeData) {
    *node = (StandTreeNode *) calloc(1, sizeof(StandTreeNode));
    return treeNumNodeCtor(*node, nodeData);
}

int newOpNode (StandTreeNode **node, enum OperandType nodeData) {
    *node = (StandTreeNode *) calloc(1, sizeof(StandTreeNode));
    return treeOpNodeCtor(*node, nodeData);
}

int treeNodeCtor(StandTreeNode *node, enum NodeType type) {
    catchNullptr(node, TreeIsNull, NULL;);

    node ->   lft    =  nullptr;
    node ->   rgt    =  nullptr;

    node ->   type   =   type  ;

    return TreeIsOk;
}

int treeIndexNodeCtor(StandTreeNode *node, enum NodeType type, size_t nodeData) {
    catchNullptr(node, TreeIsNull, NULL;);

    node ->    lft     =  nullptr;
    node ->    rgt     =  nullptr;

    node ->    type    =   type  ;
    node -> data.index = nodeData;

    return TreeIsOk;
}

int treeNumNodeCtor(StandTreeNode *node, int nodeData) {
    catchNullptr(node, TreeIsNull, NULL;);

    node ->    lft    =  nullptr;
    node ->    rgt    =  nullptr;

    node ->   type    =  Numeral;
    node ->  data.num = nodeData;

    return TreeIsOk;
}

int treeOpNodeCtor(StandTreeNode *node, enum OperandType nodeData) {
    catchNullptr(node, TreeIsNull, NULL;);

    node ->   lft    =  nullptr;
    node ->   rgt    =  nullptr;

    node ->   type   = Operator;
    node ->  data.op = nodeData;

    return TreeIsOk;
}

int _treeCtor(StandTree *root, const char * name, const char *file, const char *function, size_t line) {
    catchNullptr(root, TreeIsNull, NULL;);
    if (root -> status == Active) return TreeDoubleConstruction;

    root -> info. pointer = (char *)   root  ;
    root -> info.  name   = (char *)   name  ;
    root -> info.  file   = (char *)   file  ;
    root -> info.function = (char *) function;
    root -> info.  line   =        line      ;
    root ->     tree      =       nullptr    ;

    root ->     status    =       Active     ;

    return TreeIsOk;
}


int printStandartTree(StandTree *tree, NameList* varList, NameList* funcList, const char *fileName) {
    catchNullptr(fileName, TreeIsNull, NULL;);
    catchNullptr(  tree  , TreeIsNull, NULL;);

    FILE *stream = fopen(fileName, "w");
    catchNullptr(stream, TreeFileOutErr, NULL;);

    printNameList(varList , stream);
    printNameList(funcList, stream);

    treePrintNode(tree -> tree, stream);

    fclose(stream);

    return TreeIsOk;
}


static void treePrintNode(StandTreeNode *tree, FILE *stream, size_t spaces) {
    // fprintf(stderr, "%d", tree -> printID);
    if (tree == nullptr || stream == nullptr || tree -> printID) {
        mPrint("{            }\n");
        return;
    }
    mPrint("{  ");

    tree -> printID = 1;
    
    printNodeParams(tree, stream);

    if (tree -> lft != nullptr)
        treePrintNode(tree -> lft, stream, spaces + 4);

    if (tree -> rgt != nullptr)
        treePrintNode(tree -> rgt, stream, spaces + 4);

    tree -> printID = 0;

    mPrint("}\n");
}

static void printNodeParams(StandTreeNode *node, FILE* stream) {
    if (node == nullptr) return;

    fprintf(stream, "%d ", node -> type);

    switch(node -> type) {

        case Numeral:
            fprintf(stream, "%d", node -> data.num);
            break;

        case Operator:
            fprintf(stream, "%d", node -> data.op);
            break;
        
        case Varriable:
            fprintf(stream, "%d", node -> data.index);
            break;

        case Declaration:
            fprintf(stream, "%d", node -> data.index);
            break;
        
        case Definition:
            fprintf(stream, "%d", node -> data.index);
            break;
        
        case Call:
            fprintf(stream, "%d", node -> data.index);
            break;
        
        default: break;
    }
    fprintf(stream, "\n");
    return;
}

static void printNameList(NameList *list, FILE *stream) {
    fprintf(stream, "%zu\n", list -> size);
    for (size_t cur = 0; cur < list -> size; cur++)
        fprintf(stream, "%s\n", list -> names[cur]);

    return;
}