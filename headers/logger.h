#include "headers/config.h"
#include "headers/standTree/config.h"
#include "headers/calc.h"

const char* treeGraphVizDump(StandTree *tree, const char *fileName);

void treePrintNodeGrVz(StandTreeNode *tree, size_t *cur, FILE *stream);