
#include <stdio.h>
//#include <assert.h>
//#include <iostream.h>
#include <stdlib.h>
#include <string.h>

typedef  char* List_t;
#define p_Poison (List_t)(-1)

//#define MEOW ;




#ifdef MEOW
    #include <iostream>

    #define LISTASSERT(ls) \
        if (!ListOK(ls))\
        {\
            FILE* out_f = fopen("dump.txt", "w");\
            ListDump(ls, out_f);\
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
    #define LISTASSERT(ls) ;
    #define assert( check ) ;
#endif



const int Free_prev = -1;
const int i_Poison = -666;

struct List
{
    List_t* info;
    int *next;
    int *prev;
    int free;
    int head;
    int tail;
    int count;
    int capacity;
    //int number;
};



//---------------------------------------------------------------------
//! check if list is available to use
//!
//! @param in [ls]          pointer to list
//!
//! @returns bool value is list ok or not
//---------------------------------------------------------------------
bool ListOK(List* ls);

//---------------------------------------------------------------------
//! prints information about list
//!
//! @param in [ls]          pointer to list
//! @param in [f_name]      name of file to output
//!
//! @returns 0
//---------------------------------------------------------------------
int ListDump(const List* ls, FILE* out_f);

//---------------------------------------------------------------------
//! create list
//!
//! @param in [ls]          pointer to list
//!
//! @returns 0
//---------------------------------------------------------------------
int ListCtor(List* ls);

//---------------------------------------------------------------------
//! destroy list
//!
//! @param in [ls]          pointer to list
//!
//! @returns 0
//---------------------------------------------------------------------
int ListDtor(List* ls);

//---------------------------------------------------------------------
//! delete element from list
//!
//! @param in [ls]            pointer to list
//! @param in [position]      position of deleting element
//!
//! @returns 0
//---------------------------------------------------------------------
int ListDel(List* ls, int position);

//---------------------------------------------------------------------
//! add element to list
//!
//! @param in [ls]           pointer to list
//! @param in [val_p]        element that will be added
//!
//! @returns 0
//---------------------------------------------------------------------
int ListAdd(List* ls, List_t val_p);

//---------------------------------------------------------------------
//! increase list's size to new_size
//!
//! @param in [ls]            pointer to list
//! @param in [new_size]      new size of list
//!
//! @returns 0
//---------------------------------------------------------------------
int ListResize(List *ls, int new_size);


bool ListOK(List* ls)
{
    bool ok = 1;

    ok =   (ls->count >= 0) && (ls->count != i_Poison)
        && (ls->free  >  0) && (ls->free  != i_Poison)
        && (ls->head  >= 0) && (ls->head  != i_Poison)
        && (ls->tail  >= 0) && (ls->tail  != i_Poison);
    if (!ok) return ok;


    int i = ls->head;
    int j = 0;
    for (; i != ls->tail && j < ls->count;)
    {
        ok = (ls->next[i] > 0) && (ls->next[i] != i_Poison)
            && (ls->prev[i] > 0 || (ls->prev[i] == 0 && i == ls->head)) && (ls->prev[i] != i_Poison)
            && (ls->info[i] != p_Poison);

        if (!ok) return ok;



        i = ls->next[i];
        j++;
    }


    ok = ((i == ls->tail && j == ls->count - 1) || (0 == ls->tail && ls->count == 0));

    if (!ok) return ok;

    ok = (ls->next[i] == 0)
        && (ls->prev[i] > 0 || (ls->prev[i] == 0 && i == ls->head)) && (ls->prev[i] != i_Poison)
        && (ls->info[i] != p_Poison);
    if (!ok) return ok;



    i = ls->free;
    j = 0;
    for (; i != 0 && j < ls->capacity;)
    {
        ok = (ls->next[i] >= 0) && (ls->next[i] != i_Poison)
            && (ls->prev[i] == Free_prev);

        if (!ok) return ok;


        i = ls->next[i];
        j++;
    }



    return ok;
}

int ListCtor(List* ls, int size)
{
    assert(ls != NULL);

    ls->info = (List_t*)calloc(size, sizeof(List_t));
    ls->next = (int*)calloc(size, sizeof(int));
    ls->prev = (int*)calloc(size, sizeof(int));

    for (int i = 0; i < size; i++)
    {
        ls->info[i] = NULL;
        ls->next[i] = i + 1;
        ls->prev[i] = Free_prev;
    }

    ls->next[0] = 0;
    ls->prev[0] = 0;
    ls->next[size - 1] = 0;

    ls->capacity = size;
    ls->count = 0;
    ls->free = 1;
    ls->head = 0;
    ls->tail = 0;

    LISTASSERT(ls)
    return 0;
}

int ListDtor(List* ls)
{
    LISTASSERT(ls)

    for (int i = 0; i < ls->capacity; i++)
    {
        free(ls->info[i]);
        ls->info[i] = p_Poison;
        ls->next[i] = i_Poison;
        ls->prev[i] = i_Poison;
    }

    ls->capacity = i_Poison;
    ls->count = i_Poison;
    ls->free = i_Poison;
    ls->head = i_Poison;
    ls->tail = i_Poison;

    free(ls->info);
    free(ls->next);
    free(ls->prev);

    return 0;
}

int ListDel(List* ls, int position)
{
    LISTASSERT(ls)

    assert(position > 0);
    assert(position < ls->capacity);
    assert(ls->prev[position] != Free_prev);

    ls->next[ls->prev[position]] = ls->next[position];
    ls->prev[ls->next[position]] = ls->prev[position];

    free(ls->info[position]);
    ls->info[position] = NULL;
    ls->count--;

    if (ls->head == position)
        ls->head = ls->next[position];

    if (ls->tail == position)
        ls->tail = ls->prev[position];

    ls->prev[position] = Free_prev;
    ls->next[position] = ls->free;

    ls->free = position;


    LISTASSERT(ls)
    return 0;
}

int ListAdd(List* ls, List_t val_p)
{
    LISTASSERT(ls)
    int lsf = ls->free;

    if (ls->count == ls->capacity - 2)
        ListResize(ls, ls->capacity * 2);
    

    int len = strlen(val_p);
    ls->info[lsf] = (List_t)calloc(len, sizeof(char));
    
    for (int i = 0; i < len; i++)
    {
        ls->info[lsf][i] = val_p[i];
    }

    ls->prev[lsf] = ls->tail;
    if (ls->tail != 0)
        ls->next[ls->tail] = lsf;

    ls->tail = lsf;
    if (ls->count == 0)
        ls->head = ls->tail;

    ls->count++;

    ls->free = ls->next[lsf];

    ls->next[lsf] = 0;
    
    LISTASSERT(ls)
    return 0;
}

int ListDump(const List* ls, FILE* out_f)
{
    if (ls == NULL)
    {
        fprintf(out_f, "NULL list\n");

        return 0;
    }

    fprintf(out_f, "info:\n");
    for(int i = 0; i < ls->capacity; i++)
        if (ls->info[i] != NULL)
            fprintf(out_f, "%s ", (ls->info[i]) );
        else
            fprintf(out_f, "0 ");
    fprintf(out_f, "\n");

    fprintf(out_f, "next:\n");
    for(int i = 0; i < ls->capacity; i++)
        fprintf(out_f, "%d ", ls->next[i]);
    fprintf(out_f, "\n");

    fprintf(out_f, "prev:\n");
    for(int i = 0; i < ls->capacity; i++)
        fprintf(out_f, "%d ", ls->prev[i]);
    fprintf(out_f, "\n");


    fprintf(out_f, "capacity: %d\n", ls->capacity);
    fprintf(out_f, "count: %d\n", ls->count);
    fprintf(out_f, "free: %d\n", ls->free);
    fprintf(out_f, "head: %d\n", ls->head);
    fprintf(out_f, "tail: %d\n", ls->tail);


    return 0;
}

int ListResize(List *ls, int new_size)
{
    assert(new_size >= ls->capacity);

    ls->info = (List_t*)realloc(ls->info, new_size * sizeof(List_t));
    ls->next = (int*)realloc(ls->next, new_size * sizeof(int));
    ls->prev = (int*)realloc(ls->prev, new_size * sizeof(int));

    for (int i = ls->capacity; i < new_size; i++)
    {
        ls->info[i] = NULL;
        ls->next[i] = i + 1;
        ls->prev[i] = Free_prev;
    }

    ls->next[ls->free] = ls->capacity;
    ls->capacity = new_size;
    ls->next[new_size - 1] = 0;

    //ListDump(ls, "2.txt");

    return 0;
}


/*int main()
{
    List *ls = (List*)calloc(1, sizeof(List));
    ListCtor(ls, 4);

    FILE* in_f = fopen("in.txt", "r");

    int n = -1;
    fscanf(in_f, "%d", &n);

    char str[100];


    for(int i = 0; i < n; i++)
    {
        fscanf(in_f, "%s", str);
        ListAdd(ls, str);
        //ListDump(ls, "2.txt");
    }

    ListDump(ls, "1.txt");

    fclose(in_f);

    ListDtor(ls);
    free(ls);

    //assert(0);


    return 0;
}
*/