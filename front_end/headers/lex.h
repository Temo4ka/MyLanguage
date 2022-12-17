// enum OperandType {
//         Add   = 1,
//         Sub   = 2,
//         Mul   = 3,
//         Div   = 4,
//         Pow   = 5,
//     FuncStart = 6,
//         Sin   = 6,
//         Cos   = 7,
//         Log   = 8,
// };

enum  NodeType { 
      None    = 0,
    Varriable = 1,
     Operand  = 2,
     Numeral  = 3,
     KeyWord  = 4,
};

typedef int ELem_t;

struct Lexema {
    NodeType type = None;

    union Data
    {
        OperandType     op   ;
            char     *varName;
           Elem_t       num  ;
    } data;
}

struct LexArray {
    Lexema *array = (Lexema *) calloc(1, sizeof(Lexema));
    size_t  size  =                  0                  ;
}