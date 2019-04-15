#include <stdio.h>
//#include <assert.h>
//#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include "list.cpp"

typedef List* List_of_list_t;
#undef p_Poison
#define p_Poison (List_of_list_t)(-1)

//#define MEOW ;




#ifdef MEOW
    #include <iostream>

    #define LSLSASSERT(lsls) \
        if (!List_of_listOK(lsls))\
        {\
            FILE* out_f = fopen("dump.txt", "w");\
            List_of_listDump(lsls, out_f);\
            fclose(out_f);\
            assert(0);\
        }


    #define assert( check )\
        if(!(check))\
        {\
            printf("OOPS: %s in %s, line %d, function %s\n", #check, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
            abort();\
            /*exit(1)*/\
        }

    #define $(x) std::cout<<#x<<" = "<<(x)<<"\n";
#else
    #define $(x) ;
    #define LSLSASSERT(lsls) ;
    #define assert( check ) ;
#endif



struct List_of_list
{
    List_of_list_t* info;
    int *next;
    int *prev;
    int *number;
    int free;
    int head;
    int tail;
    int count;
    int capacity;
};



//---------------------------------------------------------------------
//! check if list_of_list is available to use
//!
//! @param in [lsls]          pointer to list_of_list
//!
//! @returns bool value is list ok or not
//---------------------------------------------------------------------
bool List_of_listOK(List_of_list* lsls);

//---------------------------------------------------------------------
//! prints information about list_of_list
//!
//! @param in [lsls]          pointer to list_of_list
//! @param in [f_name]        name of file to output
//!
//! @returns 0
//---------------------------------------------------------------------
int List_of_listDump(const List_of_list* lsls, FILE* out_f);

//---------------------------------------------------------------------
//! create list_of_list
//!
//! @param in [lsls]          pointer to list_of_list
//!
//! @returns 0
//---------------------------------------------------------------------
int List_of_listCtor(List_of_list* lsls);

//---------------------------------------------------------------------
//! destroy list_of_list
//!
//! @param in [lsls]          pointer to list_of_list
//!
//! @returns 0
//---------------------------------------------------------------------
int List_of_listDtor(List_of_list* lsls);

//---------------------------------------------------------------------
//! delete element from list_of_list
//!
//! @param in [lsls]            pointer to list_of_list
//! @param in [position]        position of deleting element
//!
//! @returns 0
//---------------------------------------------------------------------
int List_of_listDel(List_of_list* lsls, int position);

//---------------------------------------------------------------------
//! add element to list_of_list
//!
//! @param in [lsls]           pointer to list_of_list
//! @param in [val_p]          element that will be added
//!
//! @returns 0
//---------------------------------------------------------------------
int List_of_listAdd(List_of_list* lsls, List_of_list_t val_p);

//---------------------------------------------------------------------
//! increase list_of_list's size to new_size
//!
//! @param in [lsls]            pointer to list_of_list
//! @param in [new_size]        new size of list_of_list
//!
//! @returns 0
//---------------------------------------------------------------------
int List_of_listResize(List_of_list *lsls, int new_size);



bool List_of_listOK(List_of_list* lsls)
{
    bool ok = 1;

    ok =   (lsls->count >= 0) && (lsls->count != i_Poison)
        && (lsls->free  >  0) && (lsls->free  != i_Poison)
        && (lsls->head  >= 0) && (lsls->head  != i_Poison)
        && (lsls->tail  >= 0) && (lsls->tail  != i_Poison);
    if (!ok) return ok;


    int i = lsls->head;
    int j = 0;
    for (; i != lsls->tail && j < lsls->count;)
    {
        ok = (lsls->next[i] > 0) && (lsls->next[i] != i_Poison)
            && (lsls->prev[i] > 0 || (lsls->prev[i] == 0 && i == lsls->head)) && (lsls->prev[i] != i_Poison)
            && (lsls->info[i] != p_Poison);

        if (!ok) return ok;



        i = lsls->next[i];
        j++;
    }


    ok = ((i == lsls->tail && j == lsls->count - 1) || (0 == lsls->tail && lsls->count == 0));

    if (!ok) return ok;

    ok = (lsls->next[i] == 0)
        && (lsls->prev[i] > 0 || (lsls->prev[i] == 0 && i == lsls->head)) && (lsls->prev[i] != i_Poison)
        && (lsls->info[i] != p_Poison);
    if (!ok) return ok;

    i = lsls->free;
    j = 0;
    for (; i != 0 && j < lsls->capacity;)
    {
        ok = (lsls->next[i] >= 0) && (lsls->next[i] != i_Poison)
            && (lsls->prev[i] == Free_prev);

        if (!ok) return ok;


        i = lsls->next[i];
        j++;
    }

    return ok;
}

int List_of_listCtor(List_of_list* lsls, int size)
{
    assert(lsls != NULL);

    lsls->info = (List_of_list_t*)calloc(size, sizeof(List_of_list_t));
    lsls->next = (int*)calloc(size, sizeof(int));
    lsls->prev = (int*)calloc(size, sizeof(int));
    lsls->number = (int*)calloc(size, sizeof(int));

    for (int i = 0; i < size; i++)
    {
        lsls->info[i] = NULL;
        lsls->next[i] = i + 1;
        lsls->prev[i] = Free_prev;
    }

    lsls->next[0] = 0;
    lsls->prev[0] = 0;
    lsls->next[size - 1] = 0;

    lsls->capacity = size;
    lsls->count = 0;
    lsls->free = 1;
    lsls->head = 0;
    lsls->tail = 0;

    LSLSASSERT(lsls)
    return 0;
}

int List_of_listDtor(List_of_list* lsls)
{
    LSLSASSERT(lsls)

    for (int i = 0; i < lsls->capacity; i++)
    {
        if (lsls->info[i] != NULL)
            ListDtor(lsls->info[i]);
        free(lsls->info[i]);
        lsls->info[i] = p_Poison;
        lsls->next[i] = i_Poison;
        lsls->prev[i] = i_Poison;
        lsls->number[i] = i_Poison;
    }

    lsls->capacity = i_Poison;
    lsls->count = i_Poison;
    lsls->free = i_Poison;
    lsls->head = i_Poison;
    lsls->tail = i_Poison;

    free(lsls->info);
    free(lsls->next);
    free(lsls->prev);
    free(lsls->number);

    return 0;
}

int List_of_listDel(List_of_list* lsls, int position)
{
    LSLSASSERT(lsls)

    assert(position > 0);
    assert(position < lsls->capacity);
    assert(lsls->prev[position] != Free_prev);

    lsls->next[lsls->prev[position]] = lsls->next[position];
    lsls->prev[lsls->next[position]] = lsls->prev[position];

    ListDtor(lsls->info[position]);
    lsls->info[position] = NULL;
    lsls->count--;

    if (lsls->head == position)
        lsls->head = lsls->next[position];

    if (lsls->tail == position)
        lsls->tail = lsls->prev[position];

    lsls->prev[position] = Free_prev;
    lsls->next[position] = lsls->free;

    lsls->free = position;

    lsls->number[position] = i_Poison;

    LSLSASSERT(lsls)
    return 0;
}

int List_of_listAdd(List_of_list* lsls, List_of_list_t val_p, int num)
{
    LSLSASSERT(lsls)
    int lslsf = lsls->free;

    lsls->number[lslsf] = num;

    if (lsls->count == lsls->capacity - 2)
    {
        List_of_listResize(lsls, lsls->capacity * 2);
    }

    lsls->info[lslsf] = val_p;

    lsls->prev[lslsf] = lsls->tail;
    if (lsls->tail != 0)
        lsls->next[lsls->tail] = lslsf;

    lsls->tail = lslsf;
    if (lsls->count == 0)
        lsls->head = lsls->tail;

    lsls->count++;

    lsls->free = lsls->next[lslsf];

    lsls->next[lslsf] = 0;
    
    LSLSASSERT(lsls)
    //List_of_listDump(lsls, "3.txt");
    return 0;
}

int List_of_listDump(const List_of_list* lsls, FILE* out_f)
{
    assert(lsls   != NULL);

    fprintf(out_f, "info:\n");
    for(int i = 0; i < lsls->capacity; i++)
        ListDump(lsls->info[i], out_f);
    fprintf(out_f, "\n");

    fprintf(out_f, "next:\n");
    for(int i = 0; i < lsls->capacity; i++)
        fprintf(out_f, "%d ", lsls->next[i]);
    fprintf(out_f, "\n");

    fprintf(out_f, "prev:\n");
    for(int i = 0; i < lsls->capacity; i++)
        fprintf(out_f, "%d ", lsls->prev[i]);
    fprintf(out_f, "\n");

    fprintf(out_f, "number:\n");
    for(int i = 0; i < lsls->capacity; i++)
        fprintf(out_f, "%d ", lsls->number[i]);
    fprintf(out_f, "\n");


    fprintf(out_f, "capacity: %d\n", lsls->capacity);
    fprintf(out_f, "count: %d\n", lsls->count);
    fprintf(out_f, "free: %d\n", lsls->free);
    fprintf(out_f, "head: %d\n", lsls->head);
    fprintf(out_f, "tail: %d\n", lsls->tail);

    return 0;
}



int List_of_listResize(List_of_list *lsls, int new_size)
{
    assert(new_size >= lsls->capacity);

    lsls->info = (List_of_list_t*)realloc(lsls->info, new_size * sizeof(List_of_list_t));
    lsls->next = (int*)realloc(lsls->next, new_size * sizeof(int));
    lsls->prev = (int*)realloc(lsls->prev, new_size * sizeof(int));
    lsls->number = (int*)realloc(lsls->number, new_size * sizeof(int));


    for (int i = lsls->capacity; i < new_size; i++)
    {
        lsls->info[i] = NULL;
        lsls->next[i] = i + 1;
        lsls->prev[i] = Free_prev;
        lsls->number[i] = i_Poison;
    }

    lsls->next[lsls->free] = lsls->capacity;
    lsls->capacity = new_size;
    lsls->next[new_size - 1] = 0;
    return 0;
}

/*int main()
{

    FILE* out_f = fopen("1.txt", "w");

    List *ls1 = (List*)calloc(1, sizeof(List));
    ListCtor(ls1, 4);
    List *ls2 = (List*)calloc(1, sizeof(List));
    ListCtor(ls2, 4);
    List *ls3 = (List*)calloc(1, sizeof(List));
    ListCtor(ls3, 4);
    List *ls4 = (List*)calloc(1, sizeof(List));
    ListCtor(ls4, 4);
    List *ls5 = (List*)calloc(1, sizeof(List));
    ListCtor(ls5, 4);

    FILE* in_f = fopen("in.txt", "r");

    int n = -1;
    fscanf(in_f, "%d", &n);

    char str[100];


    for(int i = 0; i < n; i++)
    {
        fscanf(in_f, "%s", str);
        ListAdd(ls1, str);
        ListAdd(ls2, str);
        ListAdd(ls3, str);
        ListAdd(ls4, str);
        ListAdd(ls5, str);
        //ListDump(ls, "2.txt");
    }

    fclose(in_f);

    List_of_list *lsls = (List_of_list*)calloc(1, sizeof(List_of_list));
    $("1")

    List_of_listCtor(lsls, 4);
    List_of_listAdd(lsls, ls1);
    List_of_listAdd(lsls, ls2);
    List_of_listAdd(lsls, ls3);
    List_of_listAdd(lsls, ls4);
    List_of_listAdd(lsls, ls5);
    $("1")


    List_of_listDump(lsls, out_f);
    $("1")
    List_of_listDtor(lsls);

    $("1")
    //assert(0);


    fclose(out_f);
    return 0;
}*/
