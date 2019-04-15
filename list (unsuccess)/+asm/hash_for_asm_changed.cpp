#include <sys/stat.h>
#include <ctype.h>
#include "list_of_list.cpp"

//int find_num = 0;

int hash1(const char *str)
{
    return 0;
}

int hash2(const char *str)
{
    return str[0];
}

int hash3(const char *str)
{
    int sum = 0;
    int len = strlen(str);

    for (int i = 0; i < len; ++i)
    {
        sum += str[i];
    }
    return sum;
}

int hash4(const char *str)
{
    return strlen(str);
}

int hash5(const char *str)
{
    return hash3(str) / hash4(str);
}

int hash6(const char *str)
{
    int sum = 0;
    int len = strlen(str);

    for (int i = 0; i < len; ++i)
    {
        sum = ( (sum << 31)|(sum >> 1) ) ^ str[i];
    }
    return sum;
}

int hash7(const char *str)
{
    int sum = 0;
    int len = strlen(str);

    for (int i = 0; i < len; ++i)
    {
        sum *= 33;

        sum += str[i];
    }

        

    //printf("%s %d\n", str, sum);
    return sum;
}

int hash8(const char *str)
{
    int sum = 0;
    int len = strlen(str);

    for (int i = 0; i < len; ++i)
    {
        sum = sum * 31 + str[i];
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

int calc_hash(char *word, int hash_func_num)
{
    switch (hash_func_num)
    {
        case 1:
            return hash1(word);
            break;
        case 2:
            return hash2(word);
            break;
        case 3:
            return hash3(word);
            break;
        case 4:
            return hash4(word);
            break;
        case 5:
            return hash5(word);
            break;
        case 6:
            return hash6(word);
            break;
        case 7:
            return hash7(word);
            break;
        case 8:
            return hash8(word);
            break;
        default:
            assert(0);
    }
}

int add_str_to_list_of_list(List_of_list* hash_table, char *word, int hash_func_num)
{
    int hash = calc_hash(word, hash_func_num);


    int table_size = hash_table->count;    
    for (int pos = 1; pos <= table_size; ++pos)
        if (hash_table->number[pos] == hash)
        {
            List* cur_list = hash_table->info[pos];
            int ls_size = cur_list->count;

            for (int ls_pos = 1; ls_pos <= ls_size; ++ls_pos)
                if ( !strcmp(cur_list->info[ls_pos], word) )
                    return 0;

            ListAdd(cur_list, word);
            return 0;
        }
    

    List *ls = (List*)calloc(1, sizeof(List));
    ListCtor(ls, 4);
    ListAdd(ls, word);
    List_of_listAdd(hash_table, ls, hash);
    return 0;
}

void fill_hash_table(char *text, List_of_list *hash_table, int hash_func_num)
{
    char *str = text;
    char word[30] = "";

    int scan_count = 0;
    while (sscanf(str, "%s%n", word, &scan_count))
    {
        if (!scan_count)
            break;
        add_str_to_list_of_list(hash_table, word, hash_func_num);
        str += scan_count;

        strcpy(word, "");
        scan_count = 0;
    }
}

void find_word(List_of_list *hash_table, const char *word, int hash_func_num)
{
    int hash = calc_hash((char*)word, hash_func_num);
        

    /*int table_size = hash_table->count;    
    for (int pos = 1; pos <= table_size; ++pos)
        if (hash_table->number[pos] == hash)
        {
            List* cur_list = hash_table->info[pos];
            int ls_size = cur_list->count;

            for (int ls_pos = 1; ls_pos <= ls_size; ++ls_pos)
                if ( !strcmp(cur_list->info[ls_pos], word) )
                    return;

            break;
        }*/
    

    

    int pos = -1;
    __asm__(".intel_syntax noprefix\n\t"
                
                "cld\n\t"
                
                "mov ebx, ecx\n\t"
                
                "repne scasd\n\t"

                "sub ebx, ecx\n\t"
                
                ".att_syntax prefix\n\t"
                :"=b"(pos)
                :"D"(&(hash_table->number[1])), "c"(hash_table->count), "a"(hash)
                :
                );

    if (hash_table->number[pos] == hash)
    {
        List* cur_list = hash_table->info[pos];
        int ls_size = cur_list->count;

        for (int ls_pos = 1; ls_pos <= ls_size; ++ls_pos)
            if ( !strcmp(cur_list->info[ls_pos], word) )
            {
                //++find_num;
                //printf("%d ", find_num);
                return;
            }
    }
    

            
        

    printf("ERROR WORD %s WASN'T FOUND\n", word);
    
}

void find_all_words(List_of_list *hash_table, int hash_func_num)
{
    int table_size = hash_table->count;    
    for (int pos = 1; pos <= table_size; ++pos)
        {
            List* cur_list = hash_table->info[pos];
            int ls_size = cur_list->count;

            for (int ls_pos = 1; ls_pos <= ls_size; ++ls_pos)
                find_word(hash_table, cur_list->info[ls_pos], hash_func_num);
        }
}

void print_hash_table(List_of_list *hash_table, FILE *out_f)
{
    fprintf(out_f, "hash,count\n");
    int pos = hash_table->head;
    while (pos != 0)
    {
        int hash = hash_table->number[pos];
        int count = (hash_table->info[pos])->count;
        fprintf(out_f, "%d,%d\n", hash, count);

        pos = hash_table->next[pos];
    }
}

int main()
{
    char *text = read_dict("dictionary1.txt");
    clean_text(text);

    List_of_list *hash_table = (List_of_list*)calloc(1, sizeof(List_of_list)); 
    List_of_listCtor(hash_table, 4);
    fill_hash_table(text, hash_table, 7);

    FILE* out_f = fopen("out7.csv", "w");
    print_hash_table(hash_table, out_f);
    fclose(out_f);

    for (int i = 0; i < 1; ++i)
        find_all_words(hash_table, 7);
    


    List_of_listDtor(hash_table);
    free(hash_table);
    free(text);
    return 0;
}