#include "config.h"

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
};


struct StandTree {
    StandTreeNode   *tree   =    {}   ;
    StandTreeInfo    info   =    {}   ;
      TreeStatus    status  = InActive;
};

#define treeCtor(TREE) _treeCtor((TREE), #TREE, __FILE__, __PRETTY_FUNCTION__, __LINE__)

int newNode(StandTreeNode **node, enum NodeType type);

int newIndexNode(StandTreeNode **node, enum NodeType type, size_t nodeData);

int newNumNode(StandTreeNode **node, double nodeData);

int newOpNode (StandTreeNode **node, char   nodeData);

int treeNodeCtor(StandTreeNode *node, enum NodeType type);

int treeIndexNodeCtor(StandTreeNode *node, enum NodeType type, size_t nodeData);

int treeNumNodeCtor(StandTreeNode *node, double nodeData);

int treeOpNodeCtor (StandTreeNode *node, char   nodeData);

int _treeCtor(Tree *root, const char * name, const char *file, const char *function, size_t line);