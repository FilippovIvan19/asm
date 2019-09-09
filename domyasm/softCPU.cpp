

#include "stack.cpp"
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <TXlib.h>

const int RAMsize = 100;
const int RX_count = 8;
const int Lab_count = 100;
const int MaxStrLen = 100;

#include "comm.h"
#include "compiler.cpp"
#include "uncompiler.cpp"


#undef ST_ASSERT
#undef assert
#undef $
#undef MEOW

#define MEOW ;


#ifdef MEOW
    #include <iostream>

    #define CPU_ASSERT(x)\
        if (!CPUOk(x))\
        {\
            CPUDump(x, "assertstat.txt", 0);\
            assert(0);\
        }


    #define assert( check )\
        if(!(check))\
        {\
            printf("OOPS: %s in %s, line %d, function %s\n", #check, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
            abort();\
        }

    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";
#else
    #define $(x) ;
    #define CPU_ASSERT(x) ;
#endif



struct softCPU
{
    Stack st;
    Stack calls;
    st_t* RAM;
    st_t* rx;
};



//---------------------------------------------------------------------
//! check if cpu is available to use
//!
//! @param in [cpu]          pointer to cpu
//!
//! @returns bool value is stack ok or not
//---------------------------------------------------------------------
bool CPUOk(const softCPU* cpu);

//---------------------------------------------------------------------
//! prints cpu information
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [out_file_name]        name of file for printing
//! @param in [pr]                   1/0 if needs or not to print in stdout too
//!
//! @returns true of false
//---------------------------------------------------------------------
int CPUDump(softCPU* cpu, const char *out_file_name, int pr);

//---------------------------------------------------------------------
//! create cpu
//!
//! @param in [cpu]           pointer to cpu
//!
//! @returns pointer to cpu
//---------------------------------------------------------------------
int CPUCtor(softCPU* cpu);

//---------------------------------------------------------------------
//! deletes cpu
//!
//! @param in [cpu]           pointer to cpu
//!
//! @returns 0
//---------------------------------------------------------------------
int CPUDtor(softCPU* cpu);

//---------------------------------------------------------------------
//! prints cpu information
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [out_file_name]        name of file for printing info
//!
//! @returns 0
//---------------------------------------------------------------------
int half_CPUDump(softCPU* cpu, const char *out_file_name);

//------------------------------------------
//! returns size of file in bytes
//!
//! @param in [in_f]  name of file
//!
//! @returns size of file in bytes
//------------------------------------------
int size_of_file(char* file_name);

//------------------------------------------
//! reads bytecode
//!
//! @param in [file_name]           name of file
//! @param in [codes]               array of bytes of code
//!
//! @returns count of codes
//------------------------------------------
int read_codes(char* file_name, st_t** codes);

//------------------------------------------
//! sums two top elements of stack and pushes their sum to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_plus(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! finds the difference between two top elements of stack and pushes result to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_minus(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! multiplies two top elements of stack and pushes result to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_mul(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! finds quotient of division two top elements of stack and pushes result to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_div(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! pushes square root of top element to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_sqrt(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! pushes sine of top element to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_sin(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! pushes cosine of top element to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_cos(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! pushes tangent of top element to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_tg(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! pushes cotangent of top element to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_ctg(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! prints top element of the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_out(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! reads value from atdin and pushes to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_in(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! returns current position after place from what was called
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_ret(softCPU* cpu, int* cur_pos);

//------------------------------------------
//! replaces current position
//!
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_jmp(int* cur_pos, st_t* codes);

//------------------------------------------
//! replaces current position and returnes it back then finds "ret"
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_call(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! pushes element to the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_push(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! pops element from the stack
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_pop(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! replaces current position if b > a (b is deeper in stack than a)
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_ja(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! replaces current position if b >= a (b is deeper in stack than a)
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_jae(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! replaces current position if b < a (b is deeper in stack than a)
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_jb(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! replaces current position if b <= a (b is deeper in stack than a)
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_jbe(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! replaces current position if b > a (b is deeper in stack than a)
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [cur_pos]              current position in array of bytes of code
//! @param in [codes]                array of bytes of code
//!
//! @returns 0
//------------------------------------------
int do_je(softCPU* cpu, int* cur_pos, st_t* codes);

//------------------------------------------
//! do commands
//!
//! @param in [cpu]                  pointer to cpu
//! @param in [codes]                array of bytes of code
//! @param in [codes_count]          count of codes
//!
//! @returns 0
//------------------------------------------
int do_commands(softCPU* cpu, st_t* codes, int codes_count);




int CPUCtor(softCPU* cpu)
{
    StackCtor(&cpu->st, 8);
    StackCtor(&cpu->calls, 8);
    cpu->RAM = (st_t*)calloc(RAMsize, sizeof(st_t));
    cpu->rx = (st_t*)calloc(RX_count + 1, sizeof(st_t));
    CPU_ASSERT(cpu);

    return 0;
}

int CPUDtor(softCPU* cpu)
{
    CPU_ASSERT(cpu);
    StackDtor(&cpu->st);
    StackDtor(&cpu->calls);

    free(cpu->RAM);
    free(cpu->rx);
    cpu->RAM = (st_t*)-1;
    cpu->rx = (st_t*)-1;

    return 0;
}

bool CPUOk(const softCPU* cpu)
{
    return (cpu != NULL)
        && (cpu->RAM != NULL) && (cpu->RAM != (st_t*)-1)
        && (cpu->rx != NULL) && (cpu->rx != (st_t*)-1)
        && (StackOk(&cpu->st)) && (StackOk(&cpu->calls));
}

int half_CPUDump(softCPU* cpu, const char *out_file_name)
{
    FILE *out_f = NULL;
    if (!strcmp(out_file_name, "_!stdout!_"))
        out_f = stdout;
    else
        out_f = fopen(out_file_name, "a");

    assert(out_f != NULL);

    fprintf(out_f, "RAM\n");
    for (int i = 0; i < RAMsize; i++)
        fprintf(out_f, "[%d] = %d\n", i, cpu->RAM[i]);

    fprintf(out_f, "rx\n");
    for (int i = 0; i < RX_count; i++)
        fprintf(out_f, "r%cx = %d\n", 'a' + i, cpu->rx[i + 1]);


    if (strcmp(out_file_name, "_!stdout!_"))
        fclose(out_f);

    return 0;
}

int CPUDump(softCPU* cpu, const char *out_file_name, int pr)
{

    StackDump(&cpu->st, out_file_name, pr);
    StackDump(&cpu->calls, out_file_name, pr);

    half_CPUDump(cpu, out_file_name);

    if (pr == PRINT_CON)
        half_CPUDump(cpu, "_!stdout!_");

    return 0;

}

int size_of_file(char* file_name)
{
    assert(file_name != nullptr);
    struct stat file_stat;
    stat(file_name, &file_stat);

    return file_stat.st_size;
}

int read_codes(char* file_name, st_t** codes)
{
    assert(file_name != NULL);

    FILE* in_file = fopen(file_name, "r");

    int file_len = size_of_file(file_name);
    *codes = (st_t*)calloc(file_len, sizeof(st_t*));

    int i = 0;
    for(; fscanf(in_file, "%d", &(*codes)[i]) == 1; i++);


    fclose(in_file);
    return i;
}

/*#define DEF_CMD_1( name, num, code )          \
    int do_##name(softCPU* cpu, int* cur_pos) \
    {                                         \
        code                                  \
                                              \
        (*cur_pos)++;                         \
                                              \
        return 0;                             \
    }

#include "define_file.cpp" */

int do_plus(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    StackPush(&cpu->st, b + a);
    (*cur_pos)++;

    return 0;
}

int do_minus(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    StackPush(&cpu->st, b - a);
    (*cur_pos)++;

    return 0;
}

int do_mul(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    StackPush(&cpu->st, b * a);
    (*cur_pos)++;

    return 0;
}

int do_div(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    StackPush(&cpu->st, b / a);
    (*cur_pos)++;

    return 0;
}

int do_sqrt(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    StackPush(&cpu->st, sqrt(a));
    (*cur_pos)++;

    return 0;
}

int do_sin(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    StackPush(&cpu->st, sin(a));
    (*cur_pos)++;

    return 0;
}

int do_cos(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    StackPush(&cpu->st, cos(a));
    (*cur_pos)++;

    return 0;
}

int do_tg(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    StackPush(&cpu->st, tan(a));
    (*cur_pos)++;

    return 0;
}

int do_ctg(softCPU* cpu, int* cur_pos)
{
    st_t a = StackPop(&cpu->st);
    StackPush(&cpu->st, 1 / tan(a));
    (*cur_pos)++;

    return 0;
}

int do_out(softCPU* cpu, int* cur_pos)
{
    st_t val;
    val = StackPop(&cpu->st);
    printf("%d\n", val);

    (*cur_pos)++;
    return 0;
}

int do_in(softCPU* cpu, int* cur_pos)
{
    printf("enter value\n");
    st_t val;
    scanf("%d", &val);

    StackPush(&cpu->st, val);

    (*cur_pos)++;
    return 0;
}

int do_end(softCPU* cpu, int* cur_pos)
{
    return 0;
}

int do_jmp(softCPU* cpu, int* cur_pos, st_t* codes)
{
    int val = (int)(codes[*cur_pos + 1] + 0.1);

    (*cur_pos) = val;
    return 0;
}

int do_call(softCPU* cpu, int* cur_pos, st_t* codes)
{
    int val = (int)(codes[*cur_pos + 1] + 0.1);
    StackPush(&cpu->calls, *cur_pos + 2);

    (*cur_pos) = val;
    return 0;
}

int do_push(softCPU* cpu, int* cur_pos, st_t* codes)
{
    (*cur_pos)++;
    st_t val;

    switch ( (int)(codes[(*cur_pos)++] + 0.1) )
    {
        case 1:
            val = codes[*cur_pos];
            break;
        case 2:
            val = cpu->rx[ (int)(codes[*cur_pos] + 0.1) ];
            break;
        case 3:
            val = cpu->RAM[ (int)(codes[*cur_pos] + 0.1) ];
            break;
        case 4:
            val = cpu->RAM[ (int)(cpu->rx[ (int)(codes[*cur_pos] + 0.1) ]) ];
            break;
        default: assert(0);
    }

    StackPush(&cpu->st, val);

    (*cur_pos)++;
    return 0;
}

int do_pop(softCPU* cpu, int* cur_pos, st_t* codes)
{
    (*cur_pos)++;
    st_t val = StackPop(&cpu->st);

    switch ( (int)(codes[(*cur_pos)++] + 0.1) )
    {
        case 2:
            cpu->rx[ (int)(codes[*cur_pos] + 0.1) ] = val;
            break;
        case 3:
            cpu->RAM[ (int)(codes[*cur_pos] + 0.1) ] = val;
            break;
        case 4:
            cpu->RAM[ (int)(cpu->rx[ (int)(codes[*cur_pos] + 0.1) ]) ] = val;
            break;
        default: assert(0);
    }

    (*cur_pos)++;
    return 0;
}

int do_ja(softCPU* cpu, int* cur_pos, st_t* codes)
{
    int val = (int)(codes[*cur_pos + 1] + 0.1);

    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);

    if (b > a)
        (*cur_pos) = val;
    else
        (*cur_pos) += 2;

    return 0;
}

int do_jae(softCPU* cpu, int* cur_pos, st_t* codes)
{
    int val = (int)(codes[*cur_pos + 1] + 0.1);

    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    if (b >= a)
        (*cur_pos) = val;
    else
        (*cur_pos) += 2;

    return 0;
}

int do_jb(softCPU* cpu, int* cur_pos, st_t* codes)
{
    int val = (int)(codes[*cur_pos + 1] + 0.1);

    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    if (b < a)
        (*cur_pos) = val;
    else
        (*cur_pos) += 2;

    return 0;
}

int do_jbe(softCPU* cpu, int* cur_pos, st_t* codes)
{
    int val = (int)(codes[*cur_pos + 1] + 0.1);

    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    if (b <= a)
        (*cur_pos) = val;
    else
        (*cur_pos) += 2;

    return 0;
}

int do_je(softCPU* cpu, int* cur_pos, st_t* codes)
{
    int val = (int)(codes[*cur_pos + 1] + 0.1);

    st_t a = StackPop(&cpu->st);
    st_t b = StackPop(&cpu->st);
    if (fabs(b - a) < EPS)
        (*cur_pos) = val;
    else
        (*cur_pos) += 2;

    return 0;
}

int do_ret(softCPU* cpu, int* cur_pos)
{
    st_t val = StackPop(&cpu->calls);

    (*cur_pos) = (int)(val + 0.1);
    return 0;
}


int do_commands(softCPU* cpu, st_t* codes, int codes_count)
{

    assert(codes != NULL);

    int cur_pos = 0;

    for(; cur_pos <= codes_count;)
    {
        if ((int)(codes[cur_pos] + 0.1) == end_c)
            break;

        //$((int)(codes[cur_pos] + 0.1))

        switch ( (int)(codes[cur_pos] + 0.1) )
        {


            #define DEF_CMD_0(name, x) \
                case name##_c:\
                    do_##name(cpu, &cur_pos, codes);\
                    break;

            #define DEF_CMD_1(name, x) \
                case name##_c:\
                    do_##name(cpu, &cur_pos);\
                    break;

            #define DEF_CMD_JMP(name, x) \
                case name##_c:\
                    do_##name(cpu, &cur_pos, codes);\
                    break;


            #include "define_file.cpp"


            #undef DEF_CMD_0
            #undef DEF_CMD_1
            #undef DEF_CMD_JMP


            default: assert(0);

        }
    }

    return 0;
}


int main()
{
    /*printf("enter file name\n");
    char f_name[MaxStrLen];
    scanf("%s", f_name);*/

    char f_name[MaxStrLen];
    strcpy(f_name, "linear.txt");


    char f_comp_name[MaxStrLen];
    strcpy(f_comp_name, "comp.txt");

    char f_uncomp_name[MaxStrLen];
    strcpy(f_uncomp_name, "uncomp.txt");

    compile(f_name, f_comp_name);
    $("compiled")
    uncompile(f_comp_name, f_uncomp_name);
    $("uncompiled")


    softCPU cpu;
    CPUCtor(&cpu);

    st_t* codes = NULL;
    int codes_count = read_codes(f_comp_name, &codes);

    do_commands(&cpu, codes, codes_count);

    free(codes);
    CPUDtor(&cpu);

    return 0;
}


#undef DEF_CMD
#undef DEF_CMD_1
#undef DEF_CMD_2
