#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Functions to use in program

/*
failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th);
void print_failfish_queue(failfish_queue *q);
*/

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
} pond;

typedef struct {
  failfish *head;
  failfish *tail;
} failfish_queue;

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

    for(i = 0; i < numPonds; i++)
    {
        read_pond(ifp, ponds + i);
        (ponds + i)->fl = new_failfish_list();
        for(j = (ponds + i)->ni; j > 0; j--)
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

	for (i = 0; i < numPonds; i++)                     //Loop for ascending ordering
	{
		for (j = 0; j < numPonds; j++)             //Loop for comparing other values
		{
			if ((ponds + j)->num < (ponds + i)->num)                //Comparing other array elements
			{
				pond *tmp = (ponds + i);         //Using temporary variable for storing last value
				(ponds + i) = (ponds + j);            //replacing value
				(ponds + j) = tmp;             //storing last value
			}  
		}
	}

    return ponds;
}

void print_initial_pond_status(pond *ponds, int numPonds)
{
    int i, j;

    //failfish *f = (ponds + i)->fl->head;
    
    printf("Initial Pond Status\n");
    for(i = 0; i < numPonds; i++)
    {
        failfish *f = (ponds + i)->fl->head;
        printf("%d %s %d", (ponds + i)->num, (ponds + i)->name, f->num);

        for(j = 0; j < ((ponds + i)->ni) - 1; j++)
        {
            printf(" %d", f->next->num);
            f = f->next;
        }
        printf("\n");
    }    
}

void clear_links_or_dispose(failfish *f_to_delete, int dispose)
{
  if(dispose != 0) {
    dispose_failfish(f_to_delete);
  } else {
    f_to_delete->next = NULL;
    f_to_delete->prev = NULL;
  }
}

/* Delete a monster from a list of monsters.  Always removes it from the list; if
   dispose, also disposes the monster structure from memory. */

void monster_list_delete(fish_list *fl, failfish *f_to_delete, int dispose)
{
    failfish *f = fl->head;
    failfish *n;

    /* First thing to check is if this will empty the list. */

    if(f_to_delete->next == f_to_delete) // We could also use if(ml->head == ml->tail)
    {
      clear_links_or_dispose(f_to_delete, dispose);
      fl->head = NULL;
      fl->tail = NULL;
      return;
    }

    /* We now know we're not deleting the only item in the list.  To make this less 
       messy, we're going to repair the head and/or tail pointers *before* the actual
       linkbreaking and deletion. */

    if(fl->head == f_to_delete)
    {
      fl->head = f_to_delete->next;
    }

    if(fl->tail == f_to_delete) 
    {
      fl->tail = f_to_delete->prev;
    }

    /* We now know we're deleting an item that is not the only item in the list, and
       that is neither the head nor the tail.  We don't have to worry about head or
       tail pointers, just about our next and prev pointers - which makes the link
       repair really easy. */

    f_to_delete->prev->next = f_to_delete->next;
    f_to_delete->next->prev = f_to_delete->prev;

    /* Nothing in the list points to us any more, so we can now safely delete. */

    clear_links_or_dispose(f_to_delete, dispose);
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

    print_initial_pond_status(ponds, numPonds);

    return 0;
}