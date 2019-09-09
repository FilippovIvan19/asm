DEF_CMD_0(push, 1)
DEF_CMD_0(pop, 3)

DEF_CMD_1(in, 2)
DEF_CMD_1(out, 4)
DEF_CMD_1(end, 5)
DEF_CMD_1(plus, 11)
DEF_CMD_1(minus, 12)
DEF_CMD_1(mul, 13)
DEF_CMD_1(div, 14)
DEF_CMD_1(sqrt, 15)
/*DEF_CMD_1(sqrt, 15,
    {
    st_t a = StackPop(&cpu->st);
    StackPush(&cpu->st, sqrt(a));
    })

DEF_CMD_1(meow, 150,
    {
    st_t n = StackPop(&cpu->st);
    for (int i = 0; i < n; i++) printf ("MEOW\n");
    })*/

DEF_CMD_1(sin, 16)
DEF_CMD_1(cos, 17)
DEF_CMD_1(tg, 18)
DEF_CMD_1(ctg, 19)
DEF_CMD_1(ret, 28)

DEF_CMD_JMP(jmp, 21)
DEF_CMD_JMP(ja, 22)
DEF_CMD_JMP(jae, 23)
DEF_CMD_JMP(jb, 24)
DEF_CMD_JMP(jbe, 25)
DEF_CMD_JMP(je, 26)
DEF_CMD_JMP(call, 27)
