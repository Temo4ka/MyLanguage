#include "headers/calc.h"
#include "headers/DSL.h"
#include "headers/standTree/front_end.h"

int main() {
    StandTree program = {};
    int err = treeCtor(&program);
    CHECK(err);

    err = newNode(&(program.tree), Fictional);
    CHECK(err);

    
    return EXIT_SUCCESS;
}