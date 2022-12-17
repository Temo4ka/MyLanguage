#include "headers/logger.h"

static void printNodeParams(StandTreeNode *node, size_t nodeNum, FILE *stream);

const char* treeGraphVizDump(StandTree *tree, const char *fileName, size_t cmd) {
    if (fileName == nullptr) return nullptr;
    if (  tree   == nullptr) return nullptr;

    FILE *stream = fopen(fileName, "w");
    if (stream == nullptr) return nullptr;

    fprintf(stream, "digraph Tree {\n"
                    "   rankdir = TB;\n");

    fprintf(stream, "   subgraph varriables {\n"
                    "       node[shape = component, style = filled, fillcolor = orange];\n"

                    "         Tree   [label = \"Head = %08X\"];\n"
                    "        Status  [label = \"Status = %d\"];\n"
                    "   }\n",
            tree -> tree, tree -> status);

    fprintf(stream, "   subgraph Nodes {\n");

    size_t cnt = 1;
    treePrintNodeGrVz(tree -> tree, &cnt, stream);

    fprintf(stream, "   }\n");
    fprintf(stream, "   Tree->node1;\n}\n");

    fclose(stream);

    static char picName[MAX_CMD_SIZE] = "";
    sprintf(picName, "logs/logPic%zu.png", cmd);

    char CmdBuffer[MAX_CMD_SIZE] = {0};
    sprintf(CmdBuffer, "dot -Tpng %s -o %s", fileName, picName);

    if (system(CmdBuffer)) return nullptr;

    return picName;
}


void treePrintNodeGrVz(StandTreeNode *tree, size_t *cur, FILE *stream) {
    // fprintf(stderr, "%d", tree -> printID);
    if (tree == nullptr || cur == nullptr || stream == nullptr || tree -> printID) return;

    tree -> printID = *cur;
    
    printNodeParams(tree, tree -> printID, stream);

    if (tree -> lft != nullptr) {
        (*cur)++;
        fprintf(stream, "       node%zu -> node%zu[xlabel = \"lft\", color = \"darkblue\"];\n",
                tree -> printID, ((tree -> lft) -> printID)? (tree -> lft) -> printID: *cur);
        treePrintNodeGrVz(tree -> lft, cur, stream);
    }
    if (tree -> rgt != nullptr) {
        (*cur)++;
        fprintf(stream, "       node%zu -> node%zu[xlabel = \"rgt\", color = \"darkgreen\"];\n", 
                tree -> printID, ((tree -> rgt) -> printID)? (tree -> rgt) -> printID: *cur);
        treePrintNodeGrVz(tree -> rgt, cur, stream);
    }

}

static void printNodeParams(StandTreeNode *node, size_t nodeNum, FILE* stream) {
    if (node == nullptr) return;

    switch(node -> type) {
        case Fictional: 
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = aqua,"
                    " label = \"{ Node: %zu | Type : Fictional |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> lft, node -> rgt);
            break;

        case Numeral:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = firebrick3,"
                    " label = \"{ Node: %zu | Type : Numeral | Num: %d |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> data.num, node -> lft, node -> rgt);
            break;

        case Operator:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = gold3,"
                    " label = \"{ Node: %zu | Type : Operator | OpCode: %d |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> data.op, node -> lft, node -> rgt);
            break;
        
        case Varriable:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = green3,"
                    " label = \"{ Node: %zu | Type : Varriable | Index: %zu |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> data.index, node -> lft, node -> rgt);
            break;
        
        case If:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = orangered,"
                    " label = \"{ Node: %zu | Type : If |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> lft, node -> rgt);
            break;

        case If_else:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = orangered2,"
                    " label = \"{ Node: %zu | Type : if-else |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> lft, node -> rgt);
            break;
        
        case While:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = yellow2,"
                    " label = \"{ Node: %zu | Type : While |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> lft, node -> rgt);
            break;

        case Declaration:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = violetred,"
                    " label = \"{ Node: %zu | Type : Declaration | Index: %zu |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> data.index, node -> lft, node -> rgt);
            break;
        
        case Definition:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = violetred1,"
                    " label = \"{ Node: %zu | Type : Definition | Index: %zu |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> data.index, node -> lft, node -> rgt);
            break;
        
        case Call:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = seagreen3,"
                    " label = \"{ Node: %zu | Type : Call | Index: %zu |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> data.index, node -> lft, node -> rgt);
            break;
        
        case Return:
            fprintf(stream, "node%zu[shape = Mrecord, style = filled, fillcolor = lightgreen,"
                    " label = \"{ Node: %zu | Type : Return |{ LeftSon: %08X | RigthSon: %08X} }\" ];\n",
                    nodeNum, nodeNum, node -> lft, node -> rgt);
            break;
    }
    return;
}