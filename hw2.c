#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct failfish {
	int num;
	struct failfish *next;
    struct failfish *prev;
} failfish;

typedef struct fish_list {
	failfish *head;
    failfish *tail;
} fish_list;

typedef struct pond {
	int num;
	char *name;
	int ni;
	int ei;
    int thi;
    fish_list *fl;
    fish_list *fq;
} pond;



failfish *create_failfish(int sequence_number)
{
    failfish *f;

    f = malloc(sizeof(failfish));
    f->num = sequence_number;
    f->next = NULL;
    f->prev = NULL;

    return f;
}

void dispose_failfish(failfish *f)
{
    free(f);
}

fish_list *new_failfish_list(void)
{
    fish_list *fl;

    fl = malloc(sizeof(fish_list));
    fl->head = NULL;
    fl->tail = NULL;

    return fl;
}

void dispose_failfish_list(fish_list *fl)
{
    free(fl);
}

void failfish_list_add(fish_list *fl, failfish *new_failfish)
{
    if(fl->head == NULL)
    {
        /* If we're adding to an empty list, just set the head equal to the tail equal
           to the new monster. */

        fl->head = new_failfish;
        fl->tail = new_failfish;
        new_failfish->prev = new_failfish;
        new_failfish->next = new_failfish;
    } else {
        /* If the list isn't empty, there's a head, so... */

        new_failfish->prev = fl->tail;
        new_failfish->next = fl->head;

        fl->head->prev = new_failfish;
        fl->tail->next = new_failfish;

        fl->head = new_failfish;
    }
}

static int get_num_ponds(FILE *ifp)
{
    char s[128];
    int num;

    // Get the first line of the file.
    fgets(s, 127, ifp);

    //pull out number from the file
    sscanf(s, "%d", &num);

    return num;
}

static void fill_pond(pond *p, int numPond, char *name, int ni, int ei, int thi)
{
    p->num = numPond;
    p->name = strdup(name);
    p->ni = ni;
    p->ei = ei;
    p->thi = thi;
}

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

pond *pond_array_constructor(FILE *ifp, int numPonds)
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

pond *pond_array_sorter(pond *ponds, int numPonds)
{
    int i, j;

    pond *sortedPonds;

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

void print_initial_pond_status(pond *ponds, int numPonds)
{
    int i, j;

    printf("Initial Pond Status\n");
    for (i = 0; i < numPonds; i++)
    {
        failfish *f = (ponds + i)->fl->head;
        printf("%d %s %d", (ponds + i)->num, (ponds + i)->name, f->num);

        for (j = 0; j < ((ponds + i)->ni) - 1; j++)
        {
            printf(" %d", f->next->num);
            f = f->next;
        }
        printf("\n");
    } 
}

void clear_links_or_dispose(failfish *f_to_delete, int dispose)
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

void fish_list_delete(fish_list *fl, failfish *f_to_delete, int dispose)
{
    //failfish *f = fl->head;
    //failfish *n;

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

pond *first_course(pond *ponds, int numPonds)
{
    int i, j, k;

    fish_list *temp;

    for (i = 0; i < numPonds; i++)
    {
        printf("Pond %d: %s\n", (ponds + i)->num, (ponds + i)->name);
        temp = (ponds + i)->fl;
        for (j = 0; j < ((ponds + i)->ei) - 1; j++)
        {
            temp->head = temp->head->next;
            temp->tail = temp->head->prev;
        }

        for (j = 0; j < ((ponds + i)->ni) - ((ponds + i)->thi); j++)
        {
            printf("Failfish %d eaten\n", temp->head->num);
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

pond *pond_fishlist_sorter(pond *ponds, int numPonds)
{
    int i, j, k;

    pond *sortedPonds;
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

pond *pond_fishlist_tail_fixer(pond *ponds, int numPonds)
{
    int i, j, k;

    pond *sortedPonds;
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

void print_end_pond_status(pond *ponds, int numPonds)
{
    int i, j;

    //failfish *f = (ponds + i)->fl->head;
    
    printf("End of Course Pond Status\n");
    for(i = 0; i < numPonds; i++)
    {
        failfish *f = (ponds + i)->fl->head;
        printf("%d %s %d", (ponds + i)->num, (ponds + i)->name, f->num);

        for(j = 0; j < ((ponds + i)->thi) - 1; j++)
        {
            printf(" %d", f->next->num);
            f = f->next;
        }
        printf("\n");
    }    
}

pond *initialize_failfish_queue(pond *ponds, int numPonds)
{
    int i, j, k;

    for (i = 0; i < numPonds; i++)
    {
        (ponds + i)->fq->head = (ponds + i)->fl->head;
        (ponds + i)->fq->tail = (ponds + i)->fl->tail;
    }

    return ponds;
}

int main()
{
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");

    int numPonds = get_num_ponds(ifp);
    pond *ponds;

    ponds = pond_array_constructor(ifp, numPonds);

    ponds = pond_array_sorter(ponds, numPonds);

    print_initial_pond_status(ponds, numPonds);

    ponds = first_course(ponds, numPonds);

    ponds = pond_fishlist_sorter(ponds, numPonds);

    print_end_pond_status(ponds, numPonds);

    return 0;
}