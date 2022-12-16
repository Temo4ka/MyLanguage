#pragma once

#include <string.h>

typedef StandTreeNode* Type_t;

static const Type_t    POISON    = nullptr;
static const size_t MAX_VAR_SIZE =   100  ;
static const size_t MAX_CMD_SIZE =   100  ;

static const char *defaultIn  = "in.txt";
static const char *defaultOut = "out.txt";