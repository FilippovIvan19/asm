#include "list_of_list.cpp"
#include <sys/stat.h>
#include <ctype.h>

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
        sum = sum * 33 + str[i];
    }
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

int add_str_to_list_of_list(List_of_list* hash_table, char* str, int hash_func_num)
{
    int hash = 0;

    switch (hash_func_num)
    {
        case 1:
            hash = hash1(str);
            break;
        case 2:
            hash = hash2(str);
            break;
        case 3:
            hash = hash3(str);
            break;
        case 4:
            hash = hash4(str);
            break;
        case 5:
            hash = hash5(str);
            break;
        case 6:
            hash = hash6(str);
            break;
        case 7:
            hash = hash7(str);
            break;
        case 8:
            hash = hash8(str);
            break;
        default:
            assert(0);
    }

    int pos = hash_table->head;

    while (pos != 0)
    {
        List* cur_list = hash_table->info[pos];

        if (cur_list->number == hash)
        {
            int ls_pos = cur_list->head;

            while (ls_pos != 0)
            {
                if ( !strcmp(cur_list->info[ls_pos], str) )
                    break;
                ls_pos = cur_list->next[ls_pos];
            }

            if (ls_pos == 0)
                ListAdd(cur_list, str);

            return 0;
        }

        pos = hash_table->next[pos];
    }

    if (pos == 0)
    {

        List *ls = (List*)calloc(1, sizeof(List));
        ListCtor(ls, 4);
        ls->number = hash;
        ListAdd(ls, str);
        List_of_listAdd(hash_table, ls);
    }

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

int main()
{
    char *text = read_dict("dictionary.txt");
    clean_text(text);

    FILE* out_f = fopen("out.csv", "w");

    List_of_list *hash_table = NULL;


    for (int i = 1; i <= 6; ++i)
    {
        hash_table = (List_of_list*)calloc(1, sizeof(List_of_list));
        List_of_listCtor(hash_table, 4);


        fill_hash_table(text, hash_table, i);


        fprintf(out_f, "hash:,");
        int pos = hash_table->head;
        while (pos != 0)
        {
            int hash = (hash_table->info[pos])->number;
            fprintf(out_f, "%d,", hash);

            pos = hash_table->next[pos];
        }


        fprintf(out_f, "\ncount:,");
        pos = hash_table->head;
        while (pos != 0)
        {
            int count = (hash_table->info[pos])->count;
            fprintf(out_f, "%d,", count);

            pos = hash_table->next[pos];
        }

        fprintf(out_f, "\n\n\n");
        

        List_of_listDtor(hash_table);
        free(hash_table);
    }


    fclose(out_f);
    free(text);
	return 0;
}