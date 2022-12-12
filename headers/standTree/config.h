enum NodeType {
     Fictional  =  0,
      Numeral   =  1,
     Varriable  =  2,
         If     =  3,
        Else    =  4,
     Condition  =  5, 
       While    =  6,
      Operator  =  7,
    Declaration =  8,
        Call    =  9,
       Return   = 10,
};

enum OperandType {
    None   =  0,
    Add    =  1,
    Sub    =  2,
    Mul    =  3,
    Div    =  4,
    Pow    =  5,
    Op_in  =  6,
    Op_out =  7,
    Eq     =  8,
    Bg     =  9,
    Ls     = 10,
    BgEq   = 11,
    LsEq   = 12,
    NotEq  = 13,
    Not    = 14,
    Or     = 15,
    And    = 16,
    Ass    = 17,
    Cos    = 18,
    Sin    = 19,
    Log    = 20,
};

typedef int Elem_t;