/*
    Giovanni Collica
    COP 3052C
    Mr. Matthew Gerber
    Programming Assignment 2
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"



//structures for program
typedef struct failfish
{
    int num;
    struct failfish *next;
    struct failfish *prev;
} failfish;

typedef struct fish_list
{
    failfish *head;
    failfish *tail;
} fish_list;

typedef struct pond
{
    int num;
    char *name;
    int ni;
    int ei;
    int thi;
    fish_list *fl;
    fish_list *fq;
} pond;



//function to allocate a single failfish and fill in elements
static failfish *create_failfish(int sequence_number)
{
    failfish *f;

    f = malloc(sizeof(failfish));
    f->num = sequence_number;
    f->next = NULL;
    f->prev = NULL;

    return f;
}

//function to free a single failfish in memory
static void dispose_failfish(failfish *f)
{
    free(f);
}

//function to allocate a new failfish list and fill in elements
static fish_list *new_failfish_list(void)
{
    fish_list *fl;

    fl = malloc(sizeof(fish_list));
    fl->head = NULL;
    fl->tail = NULL;

    return fl;
}


//function to free a failfish list in memory
static void dispose_failfish_list(fish_list *fl)
{
    free(fl);
}

//function to add a failfish to a failfish list
static void failfish_list_add(fish_list *fl, failfish *new_failfish)
{
    if (fl->head == NULL)
    {
        fl->head = new_failfish;
        fl->tail = new_failfish;
        new_failfish->prev = new_failfish;
        new_failfish->next = new_failfish;
    }
    else
    {
        new_failfish->prev = fl->tail;
        new_failfish->next = fl->head;

        fl->head->prev = new_failfish;
        fl->tail->next = new_failfish;

        fl->head = new_failfish;
    }
}

//function to read in the number of ponds from the input file
static int get_num_ponds(FILE *ifp)
{
    char s[128];
    int num;

    fgets(s, 127, ifp);

    sscanf(s, "%d", &num);

    return num;
}

//function to fill in elements of a pond
static void fill_pond(pond *p, int numPond, char *name, int ni, int ei, int thi)
{
    p->num = numPond;
    p->name = strdup(name);
    p->ni = ni;
    p->ei = ei;
    p->thi = thi;
}

//function to read in values of a pond from the input file
static void read_pond(FILE *ifp, pond *p)
{
    char numPond_string[128];
    int numPond;
    char name[128];
    char ni_string[128];
    int ni;
    char ei_string[128];
    int ei;
    char thi_string[128];
    int thi;

    fscanf(ifp, "%s", numPond_string);
    numPond = atoi(numPond_string);
    fscanf(ifp, "%s", name);
    fscanf(ifp, "%s", ni_string);
    ni = atoi(ni_string);
    fscanf(ifp, "%s", ei_string);
    ei = atoi(ei_string);
    fscanf(ifp, "%s", thi_string);
    thi = atoi(thi_string);
    fill_pond(p, numPond, name, ni, ei, thi);
}

//contructor to create and allocate each pond into a pond array
static pond *pond_array_constructor(FILE *ifp, int numPonds)
{
    int i, j;

    pond *ponds = calloc(numPonds, sizeof(pond));

    failfish *f;

    for (i = 0; i < numPonds; i++)
    {
        read_pond(ifp, ponds + i);
        (ponds + i)->fl = new_failfish_list();
        (ponds + i)->fq = new_failfish_list();
        for (j = (ponds + i)->ni; j > 0; j--)
        {
            f = create_failfish(j);
            failfish_list_add((ponds + i)->fl, f);
        }
    }

    return ponds;
}

//function to free a single pond from memory
static void dispose_pond(pond *p)
{
    free(p);
}

//function to sort the pond array from least to greatest by pond number
static pond *pond_array_sorter(pond *ponds, int numPonds)
{
    int i, j;

    for (i = 0; i < numPonds; i++)
    {
        for (j = 0; j < numPonds; j++)
        {
            if ((ponds + j)->num > (ponds + i)->num)
            {
                pond tmp = ponds[i];
                ponds[i] = ponds[j];
                ponds[j] = tmp;
            }
        }
    }

    return ponds;
}

//function to print the initial status of each pond
static void print_initial_pond_status(FILE *ofp, pond *ponds, int numPonds)
{
    int i, j;

    fprintf(ofp, "Initial Pond Status\n");
    for (i = 0; i < numPonds; i++)
    {
        failfish *f = (ponds + i)->fl->head;
        fprintf(ofp, "%d %s %d", (ponds + i)->num, (ponds + i)->name, f->num);

        for (j = 0; j < ((ponds + i)->ni) - 1; j++)
        {
            fprintf(ofp, " %d", f->next->num);
            f = f->next;
        }
        fprintf(ofp, "\n");
    }
}

//disposes failfish if wanted, otherwise it will set both of its links to null
static void clear_links_or_dispose(failfish *f_to_delete, int dispose)
{
    if (dispose != 0)
    {
        dispose_failfish(f_to_delete);
    }
    else
    {
        f_to_delete->next = NULL;
        f_to_delete->prev = NULL;
    }
}

//Deletes a failfish from a list of failfish.
static void fish_list_delete(fish_list *fl, failfish *f_to_delete, int dispose)
{
    if (f_to_delete->next == f_to_delete)
    {
        clear_links_or_dispose(f_to_delete, dispose);
        fl->head = NULL;
        fl->tail = NULL;
        return;
    }

    if (fl->head == f_to_delete)
    {
        fl->head = f_to_delete->next;
    }

    if (fl->tail == f_to_delete)
    {
        fl->tail = f_to_delete->prev;
    }

    f_to_delete->prev->next = f_to_delete->next;
    f_to_delete->next->prev = f_to_delete->prev;

    clear_links_or_dispose(f_to_delete, dispose);
}

//function for the first course
static pond *first_course(FILE *ofp, pond *ponds, int numPonds)
{
    int i, j, k;

    fish_list *temp;

    fprintf(ofp, "\nFirst Course\n");

    //itierates through each pond
    for (i = 0; i < numPonds; i++)
    {
        fprintf(ofp, "\nPond %d: %s\n", (ponds + i)->num, (ponds + i)->name);
        temp = (ponds + i)->fl;
        //sets new head and tail value for the elements
        //that need to be deleted from the list
        for (j = 0; j < ((ponds + i)->ei) - 1; j++)
        {
            temp->head = temp->head->next;
            temp->tail = temp->head->prev;
        }

        //outputs to file the failfish that has been eaten
        //a deletes from the failfish list
        for (j = 0; j < ((ponds + i)->ni) - ((ponds + i)->thi); j++)
        {
            fprintf(ofp, "Failfish %d eaten\n", temp->head->num);
            fish_list_delete((ponds + i)->fl, temp->head, (ponds + i)->thi);
            for (k = 0; k < ((ponds + i)->ei) - 1; k++)
            {
                temp->head = temp->head->next;
                temp->tail = temp->head->prev;
            }
        }
    }

    return ponds;
}

//sort failfish list from least to greatest
static pond *pond_fishlist_sorter(pond *ponds, int numPonds)
{
    int i, k;

    for (k = 0; k < numPonds; k++)
    {
        for (i = 0; i < (ponds + k)->thi; i++)
        {
            while ((ponds + k)->fl->head->num > (ponds + k)->fl->head->next->num)
            {
                (ponds + k)->fl->head = (ponds + k)->fl->head->next;
            }

            while ((ponds + k)->fl->head->num > (ponds + k)->fl->tail->num)
            {
                (ponds + k)->fl->head = (ponds + k)->fl->head->next;
            }
        }
    }

    return ponds;
}

//function to fix the tail for each fishlist after it has gone through
//the first_course function and pond_fishlist_sorter function
static pond *pond_fishlist_tail_fixer(pond *ponds, int numPonds)
{
    int i, k;

    for (k = 0; k < numPonds; k++)
    {
        for (i = (ponds + i)->thi; i > 0; i--)
        {
            while ((ponds + k)->fl->tail->num < (ponds + k)->fl->tail->next->num)
            {
                (ponds + k)->fl->tail = (ponds + k)->fl->tail->next;
            }

            while ((ponds + k)->fl->tail->num < (ponds + k)->fl->tail->num)
            {
                (ponds + k)->fl->tail = (ponds + k)->fl->tail->next;
            }
        }
    }

    return ponds;
}

//function to print the status of each pond after it has gone through the first course
static void print_end_pond_status(FILE *ofp, pond *ponds, int numPonds)
{
    int i, j;

    fprintf(ofp, "\nEnd of Course Pond Status\n");
    for (i = 0; i < numPonds; i++)
    {
        failfish *f = (ponds + i)->fl->head;
        fprintf(ofp, "%d %s %d", (ponds + i)->num, (ponds + i)->name, f->num);

        for (j = 0; j < ((ponds + i)->thi) - 1; j++)
        {
            fprintf(ofp, " %d", f->next->num);
            f = f->next;
        }
        fprintf(ofp, "\n");
    }
}

//function to initialize the failfish queue for each pond
static pond *initialize_failfish_queue(pond *ponds, int numPonds)
{
    int i;

    for (i = 0; i < numPonds; i++)
    {
        (ponds + i)->fq->head = (ponds + i)->fl->head;
        (ponds + i)->fq->tail = (ponds + i)->fl->tail;
    }

    return ponds;
}

//function to sort the ponds based on the requirments layed out in the assignment
static pond *pond_queue_sorter(pond *ponds, int numPonds)
{
    int i, j;

    for (i = 0; i < numPonds; i++)
    {
        for (j = 0; j < numPonds; j++)
        {
            //sorts the ponds based on their first number in their
            //failfish list from greatest to least
            if ((ponds + j)->fl->head->num < (ponds + i)->fl->head->num)
            {
                pond tmp = ponds[i];
                ponds[i] = ponds[j];
                ponds[j] = tmp;
            }
            //if ponds have same value for their first value in their failfish
            //list the pond with lowest number is set first
            if ((ponds + j)->fl->head->num == (ponds + i)->fl->head->num)
            {
                if ((ponds + j)->num > (ponds + i)->num)
                {
                    pond tmp = ponds[i];
                    ponds[i] = ponds[j];
                    ponds[j] = tmp;
                }
            }
        }
    }

    return ponds;
}

//function to go through the second course
static pond *second_course(FILE *ofp, pond *ponds, int numPonds)
{
    ponds = initialize_failfish_queue(ponds, numPonds);

    ponds = pond_queue_sorter(ponds, numPonds);

    int i, j;
    int finalPondNum;

    fish_list *temp;
    
    fprintf(ofp, "\nSecond Course\n");

    for (i = 0; i < numPonds; i++)
    {
        temp = (ponds + i)->fq;
        //goes through each pond before the final one and displays the failfish that was eaten
        if (i < numPonds)
        {
            for (j = 0; j < (ponds + i)->thi; j++)
            {
                fprintf(ofp, "\nEaten: Failfish %d from pond %d", temp->head->num, (ponds + i)->num);
                temp->head = temp->head->next;
            }
        }
        //the final pond has every failfish eaten except the last one
        //this failfish becomes the surviving failfish
        if (i == numPonds - 1)
        {
            for (j = 0; j < ((ponds + i)->thi) - 1; j++)
            {
                fprintf(ofp, "\nEaten: Failfish %d from pond %d", temp->head->num, (ponds + i)->num);
                temp->head = temp->head->next;
            }
        }
        finalPondNum = (ponds + i)->num;
    }

    fprintf(ofp, "\n\nFailfish %d from pond %d remains\n", temp->head->num, finalPondNum);

    return ponds;
}

//function to free all allocated memory for ponds, failfish list, and failfish
static void dispose_ponds_all(pond *ponds, int numPonds)
{
    int i, j, k;

    failfish *f;
    failfish *f2;
    failfish *n;
    fish_list *fl;
    fish_list *fq;

    for(i = 0; i < numPonds; i++)
    {
        f = (ponds + i)->fl->head;
        f2 = (ponds + i)->fq->head;
        fl = (ponds + i)->fl;
        fq = (ponds + i)->fq;

        dispose_pond(ponds + i);

        if (f == NULL)
        {
            dispose_failfish_list((ponds + i)->fl);
            return;
        }

        if (f2 == NULL)
        {
            dispose_failfish_list((ponds + i)->fq);
            return;
        }
        
        do
        {
            n = f->next;
            dispose_failfish(f);
            f = n;
        } while (f != fl->head);

        dispose_failfish_list((ponds + i)->fl);
        dispose_failfish_list((ponds + i)->fq);
    }
}

//main function
int main(void)
{
    atexit(report_mem_leak);

    FILE *ifp;
    FILE *ofp;

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");

    int numPonds = get_num_ponds(ifp);
    pond *ponds;

    ponds = pond_array_constructor(ifp, numPonds);

    ponds = pond_array_sorter(ponds, numPonds);

    print_initial_pond_status(ofp, ponds, numPonds);

    ponds = first_course(ofp, ponds, numPonds);

    ponds = pond_fishlist_sorter(ponds, numPonds);

    ponds = pond_fishlist_tail_fixer(ponds, numPonds);

    print_end_pond_status(ofp, ponds, numPonds);

    ponds = second_course(ofp, ponds, numPonds);

    dispose_ponds_all(ponds, numPonds);

    return 0;
}