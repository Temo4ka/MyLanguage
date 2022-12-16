#pragma once

#include <stdio.h>

#define CUR_STR cur_str(buffer)
#define CUR_SYM cur_sym(buffer)
#define NEXT_SYM ++(*buffer)
#define SKIP_SYM(N) skip_sym(N, buffer)

#define catchNullptr(POINTER, RETURN_VALUE,...) {                                                                  \
    if ((POINTER) == nullptr) {                                                                                     \
        fprintf(stderr, "%s pointer at %s at %s(%d) is NULL\n", #POINTER, __PRETTY_FUNCTION__, __FILE__, __LINE__);  \
        __VA_ARGS__                                                                                                   \
        return RETURN_VALUE;                                                                                           \
    }                                                                                                                   \
}
                                                                                                
#define ERR_EXE(ERROR_CODE) {                                                     \
    *err |= ERROR_CODE;                                                            \
    fprintf(stderr, "%s at %s(%d);\n", __PRETTY_FUNCTION__, __FILE__, __LINE__);    \
    return POISON;                                                                   \
}

#define CHECK(ERROR) {                                                                  \
    if (ERROR) {                                                                         \
        fprintf(stderr, "ERROR_CODE: %08d\n,"                                             \
                        "%s at %s(%d);\n",                                                 \
                ERROR, __PRETTY_FUNCTION__, __FILE__, __LINE__                              \
                );                                                                           \
        return EXIT_FAILURE;                                                                  \
    }                                                                                          \
}
