#include "headers/standTree/front_end.h"
#include "headers/DSL.h"

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

    node ->  status  =  Active ;

    return TreeIsOk;
}

int treeIndexNodeCtor(StandTreeNode *node, enum NodeType type, char *nodeData) {
    catchNullptr(node, TreeIsNull, NULL;);

    node ->   lft    =  nullptr;
    node ->   rgt    =  nullptr;

    node ->   type   =   type  ;
    node -> data.var = nodeData;

    node ->  status  =  Active ;

    return TreeIsOk;
}

int treeNumNodeCtor(StandTreeNode *node, Elem_t nodeData) {
    catchNullptr(node, TreeIsNull, NULL;);

    node ->    lft    =  nullptr;
    node ->    rgt    =  nullptr;

    node ->   type    =  Numeral;
    node ->  data.num = nodeData;

    node ->  status   =  Active ;

    return TreeIsOk;
}

int treeOpNodeCtor(StandTreeNode *node, enum OperandType nodeData) {
    catchNullptr(node, TreeIsNull, NULL;);

    node ->   lft    =  nullptr;
    node ->   rgt    =  nullptr;

    node ->   type   = Operator;
    node ->  data.op = nodeData;

    node ->  status  =  Active ;

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

size_t createDeclarationNode(StandTreeNode **node, size_t index, int val) {
    StandTreeNode *lft = nullptr;
    lft = newIndexNode(&lft, Varriable, index);
    if (lft == nullptr) return nullptr;

    StandTreeNode *rgt = nullptr;
    rgt = newNumNode(&lft, val);
    if (rgt == nullptr) return nullptr;

    int err = newNode(node, Declaration);
    if (err) return err;

    node -> lft = lft;
    node -> rgt = rgt;

    return node;
}
