#include "headers/standTree/front_end.h"
#include "headers/DSL.h"
#include <stdlib.h>

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
