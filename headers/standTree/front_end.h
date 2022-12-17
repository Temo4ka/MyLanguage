#pragma once

#include "config.h"
#include <stdlib.h>

enum TreeErrors {
    TreeIsOk                 =       0,
    TreeIsNull               = 1 <<  0,
    TreeFileInErr            = 1 <<  1,
    TreeFileOutErr           = 1 <<  2,
    TreeDataPoison           = 1 <<  3,
    TreeIsInActive           = 1 <<  4,
    TreeTypeIsNone           = 1 <<  5,
    TreeDevisionByZero       = 1 <<  6,
    TreeUnknownOperand       = 1 <<  7,
    TreeDoubleDestruction    = 1 <<  8,
    TreeDoubleConstruction   = 1 <<  9,
    TreeGraphVizExecutionErr = 1 << 10,
};

enum TreeStatus {
     Active  = 0,
    InActive = 1,
};

struct StandTreeInfo {
    size_t  line   =    0   ;

    char *  name   = nullptr;
    char *  file   = nullptr;
    char * pointer = nullptr;
    char *function = nullptr;
};

struct StandTreeNode {
    enum  NodeType  type;
    

    union NodeData {
        OperandType  op;
           int      num;
          size_t  index;
    } data;

    StandTreeNode *lft = nullptr;
    StandTreeNode *rgt = nullptr;

    int printID = 0;
};

struct NameList {
    char   **names  = (char **) calloc(1, sizeof(char*));
    size_t   size   =              0                    ;
    size_t capacity =              1                    ;
    // int    *values  =  (int *)  calloc(1,  sizeof(int) ); //might be unneccessary(((
};


struct StandTree {
    StandTreeNode   *tree   =    {}   ;
    StandTreeInfo    info   =    {}   ;
      TreeStatus    status  = InActive;
};

#define treeCtor(TREE) _treeCtor((TREE), #TREE, __FILE__, __PRETTY_FUNCTION__, __LINE__)

int newNode(StandTreeNode **node, enum NodeType type);

int newIndexNode(StandTreeNode **node, enum NodeType type, size_t nodeData);

int newNumNode(StandTreeNode **node, int nodeData);

int newOpNode (StandTreeNode **node, enum OperandType nodeData);

int treeNodeCtor(StandTreeNode *node, enum NodeType type);

int treeIndexNodeCtor(StandTreeNode *node, enum NodeType type, size_t nodeData);

int treeNumNodeCtor(StandTreeNode *node, int nodeData);

int treeOpNodeCtor (StandTreeNode *node, enum OperandType nodeData);

int _treeCtor(StandTree *root, const char * name, const char *file, const char *function, size_t line);

int printStandartTree(StandTree *tree, NameList* varList, NameList* funcList, const char *fileName);