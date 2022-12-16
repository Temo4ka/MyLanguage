#include "headers/calc.h"
#include "headers/DSL.h"
#include "headers/standTree/front_end.h"
#include "headers/logger.h"

int main(int argc, char *argv[]) {
    const char* inFileName  = defaultIn;
    const char* outFileName = defaultOut;
    
    switch (argc) {
        case 2:
            inFileName = argv[0];
            break;

        case 3:
            inFileName  = argv[0];
            outFileName = argv[1];
            break;

        default: break;
    }

    StandTree program = {};
    size_t err = treeCtor(&program);
    CHECK(err);

    Buffer code;
    err = assignBuffer(&code, inFileName);
    CHECK(err);

    NameList varList  = {};
    NameList funcList = {};

    program.tree = getG(&(code.buffer), &varList, &funcList, &err);
    CHECK(err);

    treeGraphVizDump(&program, "logs/treeDump.dot");
    
    return EXIT_SUCCESS;
}