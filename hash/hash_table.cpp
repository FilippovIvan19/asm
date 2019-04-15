#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.cpp"

#undef p_Poison
#define p_Poison (List*)(-1)



struct hash_table
{
    List** lists;
    int *number;
    int size;
};




int hash_tableCtor(hash_table* h_tbl, int size)
{
    assert(h_tbl != NULL);
    h_tbl->size = size;

    h_tbl->lists = (List**)calloc(size, sizeof(List*));
    for (int i = 0; i < size; ++i)
    {
        h_tbl->lists[i] = (List*)calloc(1, sizeof(List));
        ListCtor(h_tbl->lists[i], 4);
    }

    h_tbl->number = (int*)calloc(size, sizeof(int));

    return 0;
}

int hash_tableDtor(hash_table* h_tbl)
{
    for (int i = 0; i < h_tbl->size; i++)
    {
        if (h_tbl->lists[i] != NULL)
            ListDtor(h_tbl->lists[i]);
        free(h_tbl->lists[i]);
    }

    free(h_tbl->lists);
    free(h_tbl->number);

    return 0;
}


int hash_tableDump(const hash_table* h_tbl, FILE* out_f)
{
    assert(h_tbl   != NULL);

    fprintf(out_f, "lists:\n");
    for(int i = 0; i < h_tbl->size; i++)
        ListDump(h_tbl->lists[i], out_f);
    fprintf(out_f, "\n");

    fprintf(out_f, "number:\n");
    for(int i = 0; i < h_tbl->size; i++)
        fprintf(out_f, "%d ", h_tbl->number[i]);
    fprintf(out_f, "\n");

    fprintf(out_f, "size: %d\n", h_tbl->size);

    return 0;
}