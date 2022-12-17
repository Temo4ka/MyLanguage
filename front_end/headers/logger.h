#include "standTree/config.h"
#include "calc.h"

const char* treeGraphVizDump(StandTree *tree, const char *fileName, size_t cmd = 0);

void treePrintNodeGrVz(StandTreeNode *tree, size_t *cur, FILE *stream);