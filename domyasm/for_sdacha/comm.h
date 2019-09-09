enum commands
{
    #define DEF_CMD_JMP(com, num) com##_c = num,
    #define DEF_CMD_1(com, num) com##_c = num,
    #define DEF_CMD_0(com, num) com##_c = num,

    #include "define_file.h"

    #undef DEF_CMD_JMP
    #undef DEF_CMD_0
    #undef DEF_CMD_1
};
