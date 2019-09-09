#include <sys/stat.h>
#include <ctype.h>
#include "hash_table.cpp"
//#include "time.h"

#define ASM_MODE_ENABLED ;


int hash_x33(char *str)
{
    int sum = 0;

    
    for (; *str; ++str)
    {
        sum *= 33;

        sum += *str;
    }

    return sum;
}

    

char *read_dict(const char *file_name)
{
    struct stat file_stat;
    stat(file_name, &file_stat);
    int file_size = file_stat.st_size;
    char *text = (char*)calloc(file_size, sizeof(char));

    FILE *in_f = fopen(file_name, "r");
    fread(text, sizeof(char), file_size, in_f);
    fclose(in_f);

    return text;
}

void clean_text(char *text)
{
    int len = strlen(text);
    
    for (int i = 0; i < len; ++i)
    {
        if (!isalpha(text[i]))
            text[i] = ' ';
    }
}


void add_str_to_hash_table(hash_table* h_tbl, char *word)
{
    int hash = hash_x33(word);


    List* cur_list = h_tbl->lists[abs(hash % h_tbl->size)];
    
    int ls_size = cur_list->count;

    for (int ls_pos = 1; ls_pos <= ls_size; ++ls_pos)
        if ( !strcmp(cur_list->info[ls_pos], word) )
            return;

    ListAdd(cur_list, word);
    return;
}

void fill_h_tbl(char *text, hash_table *h_tbl)
{
    char *str = text;
    char word[30] = "";

    int scan_count = 0;
    while (sscanf(str, "%s%n", word, &scan_count))
    {
        if (!scan_count)
            break;
        add_str_to_hash_table(h_tbl, word);
        str += scan_count;

        strcpy(word, "");
        scan_count = 0;
    }
}

char MYstrcmp(const char *str1, const char *str2)
{
    char diff;
    __asm__(".intel_syntax noprefix\n\t"
                
            "cld\n\t"
            "mov rax, 0\n\t"
            
            "cycle:\n\t"
            
            "cmp byte ptr [rsi], 0\n\t"
            "je end_1\n\t"
            "cmp byte ptr [rdi], 0\n\t"
            "je end_1\n\t"

            //"cmpsb\n\t"
            "mov al, byte ptr [rsi]\n\t"
            "cmp byte ptr [rdi], al\n\t"
            "inc rsi\n\t"
            "inc rdi\n\t"

            "je cycle\n\t"
            
            
            "end_1:\n\t"
            "mov al, byte ptr [rsi - 1]\n\t"
            "sub al, byte ptr [rdi - 1]\n\t"
            
            ".att_syntax prefix\n\t"
            :"=a"(diff)
            :"S"(str1), "D"(str2)
            :
            );

    return diff;
}

void find_word(hash_table *h_tbl, char *word)
{
    int hash = 0;


    #ifdef ASM_MODE_ENABLED
        

        __asm__(".intel_syntax noprefix\n\t"
                    
                "mov eax, 0\n\t"
                "mov edx, 0\n\t"
                "mov dl, byte ptr [rcx]\n\t"
                
                "next:\n\t"
                
                "mov ebx, eax\n\t"
                "shl ebx, 5\n\t"
                "add eax, ebx\n\t"
                "add eax, edx\n\t"

                "inc rcx\n\t"
                "mov dl, byte ptr [rcx]\n\t"
                "cmp dl, 0\n\t"
                "jne next\n\t"
                
                ".att_syntax prefix\n\t"
                :"=a"(hash)
                :"c"(word)
                :"ebx", "edx"
                );


    #else
        hash = hash_x33(word);
    #endif
        
    List* cur_list = h_tbl->lists[abs(hash % h_tbl->size)];
    int ls_size = cur_list->count;

    for (int ls_pos = 1; ls_pos <= ls_size; ++ls_pos)
        if ( !strcmp(cur_list->info[ls_pos], word) )
        {
            //printf("%s\n", word);
            return;
        }


    printf("ERROR WORD %s WASN'T FOUND\n", word);
}

void find_all_words(hash_table *h_tbl)
{
    int h_tbl_size = h_tbl->size; 
    for (int i = 0; i < h_tbl_size; ++i)
        {
            List* cur_list = h_tbl->lists[i];
            int ls_size = cur_list->count;

            for (int ls_pos = 1; ls_pos <= ls_size; ++ls_pos)
                find_word(h_tbl, cur_list->info[ls_pos]);
        }
}

void print_h_tbl(hash_table *h_tbl, FILE *out_f)
{
    fprintf(out_f, "hash,count\n");

    int h_tbl_size = h_tbl->size;
    for (int i = 0; i < h_tbl_size; ++i)
    {
    	fprintf(out_f, "%d,%d\n", i, h_tbl->lists[i]->count);
    }
}

const int h_tbl_size = 10000;

int main()
{
    //start_time();

    char *text = read_dict("dict.txt");
    clean_text(text);

    hash_table *h_tbl = (hash_table*)calloc(1, sizeof(hash_table)); 
    hash_tableCtor(h_tbl, h_tbl_size);
    fill_h_tbl(text, h_tbl);

    // FILE* out_f = fopen("out7.csv", "w");
    // print_h_tbl(h_tbl, out_f);
    // fclose(out_f);

    for (int i = 0; i < 5000; ++i)
        find_all_words(h_tbl);
    


    hash_tableDtor(h_tbl);
    free(h_tbl);
    free(text);

    //print_duration("finished");
    return 0;
}