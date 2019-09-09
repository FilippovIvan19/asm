#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <iostream>
//#include "comm.h"

//const int MaxStrLen = 100;
const int byte_code_max_size = 1000;
const int PAGESIZE = 4096;
const int buf_size = PAGESIZE * 4;




int rx_num_to_bin_rx_num(int number)
{
    switch (number)
        {
            case 1:
                return 0;
            case 3:
                return 1;
            case 4:
                return 2;
            case 2:
                return 3;
            case 5:
                return 10;
            case 6:
                return 11;
            case 7:
                return 12;
            case 8:
                return 13;

            default: assert(0);
        }

    return -1;
}

int bin_pop(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen])
{
    int pop_type = byte_code_arr[*byte_code_pos + 1];
    int val = byte_code_arr[*byte_code_pos + 2];
    (*byte_code_pos) += 3;

    switch (pop_type)
    {
        case 2:
            str_out[0] = 0x58 + rx_num_to_bin_rx_num(val);
            return 1;

        default: assert(0);
    }


    return -1;
}

int bin_push(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen])
{
    int push_type = byte_code_arr[*byte_code_pos + 1];
    int val = byte_code_arr[*byte_code_pos + 2];
    (*byte_code_pos) += 3;

    switch (push_type)
    {
        case 1://число
            str_out[0] = 0x68;
            for (int i = 1; i <= 4; ++i)
            {
                str_out[i] = val % 256;
                val /= 256;
            }
            return 5;

        case 2://регистр
            str_out[0] = 0x50 + rx_num_to_bin_rx_num(val);
            return 1;
        /*
        case 3://адрес
            strcat(str_out, "[");
            strcat(str_out, val_s);
            strcat(str_out, "]");
            break;

        case 4://адрес в регистре
            strcat(str_out, "[");
            rx_num_to_bin_rx_num(val);
            strcat(str_out, rx_str);
            strcat(str_out, "]");
            break;
        */
        default: assert(0);
    }

    return -1;
}

int bin_plus(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen])
{
    memcpy(str_out, "\x41\x5e"          // pop r14
                    "\x41\x5f"          // pop r15
                    "\x4d\x01\xf7"      // add r15, r14
                    "\x41\x57", 9);     // push r15

    (*byte_code_pos) += 1;
    return 9;
}

int bin_minus(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen])
{
    memcpy(str_out, "\x41\x5e"          // pop r14
                    "\x41\x5f"          // pop r15
                    "\x4d\x29\xf7"      // sub r15, r14
                    "\x41\x57", 9);     // push r15

    (*byte_code_pos) += 1;
    return 9;
}

int bin_div(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen])
{
    memcpy(str_out, "\x41\x5e"              // pop r14
                    "\x41\x5f"              // pop r15
                    "\x50"                  // push rax
                    "\x52"                  // push rdx
                    "\x4c\x89\xf8"          // mov rax, r15
                    "\xba\x00\x00\x00\x00"  // mov rdx, 0
                    "\x49\xf7\xf6"          // div r14
                    "\x49\x89\xc7"          // mov r15, rax
                    "\x5a"                  // pop rdx
                    "\x58"                  // pop rax
                    "\x41\x57", 24);        // push r15

    (*byte_code_pos) += 1;
    return 24;
}

int bin_mul(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen])
{
    memcpy(str_out, "\x41\x5e"              // pop r14
                    "\x41\x5f"              // pop r15
                    "\x50"                  // push rax
                    "\x52"                  // push rdx
                    "\x4c\x89\xf8"          // mov rax, r15
                    "\x49\xf7\xe6"          // mul r14
                    "\x49\x89\xc7"          // mov r15, rax
                    "\x5a"                  // pop rdx
                    "\x58"                  // pop rax
                    "\x41\x57", 24);        // push r15

    (*byte_code_pos) += 1;
    return 19;
}

int bin_in(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen],
    unsigned char *bin_buf, int bin_buf_pos, unsigned char *func_buf)
{
    str_out[0] = 0xe8;      // call

    unsigned long int call_offset = (unsigned long int)func_buf
        - (unsigned long int)(bin_buf + bin_buf_pos) - 5;


    *(uint32_t*) (str_out + 1) = call_offset;

    memcpy(str_out + 5, "\x41\x57", 2);     // push r15

    (*byte_code_pos) += 1;
    return 7;
}

int bin_out(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen],
    unsigned char *bin_buf, int bin_buf_pos, unsigned char *func_buf)
{
    memcpy(str_out, "\x41\x5f", 2);     // pop r15

    unsigned long int call_offset = (unsigned long int)func_buf + 6
        - (unsigned long int)(bin_buf + bin_buf_pos + 2) - 5;

    str_out[2] = 0xe8;      // call

    *(uint32_t*) (str_out + 3) = call_offset;

    (*byte_code_pos) += 1;
    return 7;
}

int bin_end(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen],
    unsigned char *bin_buf, int bin_buf_pos, unsigned char *func_buf)
{
    str_out[0] = 0xe8;      // call

    unsigned long int call_offset = (unsigned long int)func_buf + 12
        - (unsigned long int)(bin_buf + bin_buf_pos) - 5;

    *(uint32_t*) (str_out + 1) = call_offset;

    (*byte_code_pos) += 1;
    return 5;
}



int bin_jmp(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen],
    int bin_buf_pos, unsigned long int *jump_adresses)
{
    int byte_adress = byte_code_arr[*byte_code_pos + 1];
    str_out[0] = 0xe9;      // jmp

    unsigned long int adr_offset = jump_adresses[byte_adress] - (unsigned long int)bin_buf_pos - 5;

    *(uint32_t*) (str_out + 1) = adr_offset;

    (*byte_code_pos) += 2;
    return 5;
}

int bin_call(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen],
    int bin_buf_pos, unsigned long int *jump_adresses)
{
    int byte_adress = byte_code_arr[*byte_code_pos + 1];
    str_out[0] = 0xe8;      // call

    unsigned long int adr_offset = jump_adresses[byte_adress] - (unsigned long int)bin_buf_pos - 5;

    *(uint32_t*) (str_out + 1) = adr_offset;
    
    (*byte_code_pos) += 2;
    return 5;
}


int bin_cond_jump(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen],
    int bin_buf_pos, unsigned long int *jump_adresses, int command)
{
    int byte_adress = byte_code_arr[*byte_code_pos + 1];
    memcpy(str_out, "\x41\x5e"          // pop r14
                    "\x41\x5f"          // pop r15
                    "\x4d\x39\xf7", 7); // cmp r15, r14

    str_out[7] = 0x0f;

    switch (command)
    {
        case ja_c:
            str_out[8] = 0x87;
            break;
        case jae_c:
            str_out[8] = 0x83;
            break;
        case jb_c:
            str_out[8] = 0x82;
            break;
        case jbe_c:
            str_out[8] = 0x86;
            break;
        case je_c:
            str_out[8] = 0x84;
            break;
        default:
            assert(0);
    }



    unsigned long int adr_offset = jump_adresses[byte_adress] - (unsigned long int)bin_buf_pos - 13;

    *(uint32_t*) (str_out + 9) = adr_offset;


    (*byte_code_pos) += 2;
    return 13;    
}

int bin_ret(int* byte_code_arr, int* byte_code_pos, unsigned char str_out[MaxStrLen])
{
    str_out[0] = 0xc3;

    (*byte_code_pos) += 1;
    return 1;
}

void change_adresses(unsigned char *func_buf, int code_len, unsigned char *data_start)
{
    int new_code_len = code_len;
    unsigned char* adress = nullptr;
    unsigned long int asm_data_start = (unsigned char)*data_start + (unsigned char)*(data_start + 1) * 0x100;
    //printf("asm_data_start: %lx\n", asm_data_start);

    while (adress = (unsigned char*)memmem(func_buf, new_code_len, "\x60\x00\x00\x00\x00\x00", 6))
    {
        new_code_len -= (adress - func_buf) + 1;
        func_buf = adress + 1;

        adress -= 2;
        unsigned long int asm_adr = *adress + *(adress + 1) * 0x100;
        unsigned long int new_asm_adr = (unsigned long int)(asm_adr - asm_data_start + data_start);

        // printf("asm_adr: %lx\n", asm_adr);
        // printf("new_asm_adr: %lx\n", new_asm_adr);

        for (int i = 0; i < 8; ++i)
        {
            adress[i] = (unsigned char)(new_asm_adr % 256);
            new_asm_adr /= 256;
        }
    }

}

unsigned char *create_func_buf(unsigned char *func_buf_0)
{
    //printf("func_buf\n\n");

    unsigned char *func_buf = (unsigned char*)( ( (unsigned long int)func_buf_0 + PAGESIZE-1) & ~(PAGESIZE-1) );
    mprotect(func_buf, buf_size * 2 - PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);

    FILE *func_file = fopen("in_out_end", "rb");

    struct stat file_stat;
    stat("in_out_end", &file_stat);
    int file_size = file_stat.st_size;

    unsigned char *read_buf = new unsigned char [file_size];
    fread(read_buf, sizeof(unsigned char), file_size, func_file);

    unsigned char *code_start = (unsigned char*)memmem(read_buf,   file_size, "\x90\x90\x90\x90", 4) + 4;
    unsigned char *data_start = (unsigned char*)memmem(code_start, file_size, "\x90\x90\x90\x90", 4) + 4;
    unsigned char *code_end   = (unsigned char*)memmem(data_start, file_size, "\x90\x90\x90\x90", 4); //first non code simbol
    size_t code_len = code_end - code_start;

    memcpy(func_buf, code_start, code_len);

    change_adresses(func_buf, data_start - code_start, data_start);

    // FILE *out_file = fopen("in_out_end_for_buf", "wb");
    // fwrite(func_buf, sizeof(unsigned char), code_len, out_file);
    // printf("dumped0\n");
    // fclose(out_file);
    // printf("111\n");
    // printf("data_start: %p\n", data_start);

    fclose(func_file);
    delete [] read_buf;
    return func_buf;
}

unsigned char *create_bin_buf(unsigned char *bin_buf_0)
{
    unsigned char *bin_buf = (unsigned char*)( ( (unsigned long int)bin_buf_0 + PAGESIZE-1) & ~(PAGESIZE-1) );
    mprotect(bin_buf, buf_size * 2 - PAGESIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    return bin_buf;
}

void fill_bin_buf_half(unsigned char* bin_buf, unsigned char* func_buf,
    int *byte_code_arr, int byte_code_len, unsigned long int *jump_adresses)
{
    unsigned char str_out[MaxStrLen] = "";
    int command = -1;
    int bin_buf_pos = 0;
    int str_out_len = -1;

    int byte_code_pos = 0;
    jump_adresses[0] = 0;
    while (byte_code_pos < byte_code_len)
    {
        command = byte_code_arr[byte_code_pos];
        //printf("command %d\n", command);
        switch (command)
        {
            case pop_c:
                str_out_len = bin_pop(byte_code_arr, &byte_code_pos, str_out);
                break;

            case push_c:
                str_out_len = bin_push(byte_code_arr, &byte_code_pos, str_out);
                break;

            case plus_c:
                str_out_len = bin_plus(byte_code_arr, &byte_code_pos, str_out);
                break;

            case minus_c:
                str_out_len = bin_minus(byte_code_arr, &byte_code_pos, str_out);
                break;

            case mul_c:
                str_out_len = bin_mul(byte_code_arr, &byte_code_pos, str_out);
                break;

            case div_c:
                str_out_len = bin_div(byte_code_arr, &byte_code_pos, str_out);
                break;

            case ret_c:
                str_out_len = bin_ret(byte_code_arr, &byte_code_pos, str_out);
                break;

            case ja_c:
            case jae_c:
            case jb_c:
            case jbe_c:
            case je_c:
                str_out_len = bin_cond_jump(byte_code_arr, &byte_code_pos, str_out, 
                    bin_buf_pos, jump_adresses, command);
                break;

            case jmp_c:
                str_out_len = bin_jmp(byte_code_arr, &byte_code_pos, str_out, 
                    bin_buf_pos, jump_adresses);
                break;

            case call_c:
                str_out_len = bin_call(byte_code_arr, &byte_code_pos, str_out, 
                    bin_buf_pos, jump_adresses);
                break;

            case in_c:
                str_out_len = bin_in(byte_code_arr, &byte_code_pos, str_out,
                    bin_buf, bin_buf_pos, func_buf);
                break;

            case out_c:
                str_out_len = bin_out(byte_code_arr, &byte_code_pos, str_out,
                    bin_buf, bin_buf_pos, func_buf);
                break;

            case end_c:
                str_out_len = bin_end(byte_code_arr, &byte_code_pos, str_out,
                    bin_buf, bin_buf_pos, func_buf);
                break;

            default: assert(0);
        }


        for (int i = 0; i < str_out_len; ++i)
        {
            bin_buf[bin_buf_pos] = str_out[i];
            ++bin_buf_pos;
        }

        jump_adresses[byte_code_pos] = bin_buf_pos;
    }
}


void fill_bin_buf(const char* in_name, unsigned char* bin_buf, unsigned char* func_buf)
{
    FILE* in_file = fopen(in_name, "r");
    int *byte_code_arr = new int [byte_code_max_size] {-1};

    int byte_code_pos = 0;
    while (fscanf(in_file, "%d", &byte_code_arr[byte_code_pos]) == 1)
        byte_code_pos++;
    int byte_code_len = byte_code_pos;

    unsigned long int *jump_adresses = new unsigned long int [byte_code_max_size];

    fill_bin_buf_half(bin_buf, func_buf, byte_code_arr, byte_code_len, jump_adresses);
    fill_bin_buf_half(bin_buf, func_buf, byte_code_arr, byte_code_len, jump_adresses);

    fclose(in_file);
    delete [] jump_adresses;
    delete [] byte_code_arr;
}

void run_bin_buf(unsigned char *func_buf)
{
    ((void(*)( ))(func_buf + buf_size))( );
}

/*
void run_bin_from_file(char *in_name)
{
    unsigned char *read_buf_0  = new unsigned char [buf_size * 2] {(unsigned char)0xcc};
    unsigned char *read_buf = create_bin_buf(read_buf_0);

    FILE *in_file = fopen(in_name, "rb");
    fread(read_buf, sizeof(unsigned char), buf_size * 2 - PAGESIZE, in_file);
    fclose(in_file);

    run_bin_buf(read_buf + buf_size);
    delete [] read_buf_0;
}
*/

void bin_compile_and_run(char *byte_file_name/*, char *out_name*/)
{
    unsigned char *func_buf_0 = new unsigned char [buf_size * 2] {(unsigned char)0xcc};

    unsigned char *func_buf = create_func_buf(func_buf_0);
    unsigned char *bin_buf  = func_buf + buf_size;
    //unsigned char *bin_buf  = create_bin_buf(bin_buf_0);

    //assert(bin_buf - func_buf != buf_size);

    fill_bin_buf(byte_file_name, bin_buf, func_buf);

    char out_name[100] = "bin_out_file_";
    FILE *out_file = fopen(out_name, "wb");
    fwrite(bin_buf, sizeof(unsigned char), 1024, out_file);
    fclose(out_file);

    run_bin_buf(func_buf);

    delete [] func_buf_0;
}
