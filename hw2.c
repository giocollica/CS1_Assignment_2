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

    pond *sortedPonds;

	for (i = 0; i < numPonds; i++)                     //Loop for ascending ordering
	{
		for (j = 0; j < numPonds; j++)             //Loop for comparing other values
		{
			if ((ponds + j)->num > (ponds + i)->num)                //Comparing other array elements
			{
				pond tmp = ponds[i];         //Using temporary variable for storing last value
				ponds[i] = ponds[j];            //replacing value
				ponds[j] = tmp;             //storing last value
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

void fish_list_delete(fish_list *fl, failfish *f_to_delete, int dispose)
{
    //failfish *f = fl->head;
    //failfish *n;

    if(f_to_delete->next == f_to_delete) 
    {
      clear_links_or_dispose(f_to_delete, dispose);
      fl->head = NULL;
      fl->tail = NULL;
      return;
    }

    if(fl->head == f_to_delete)
    {
      fl->head = f_to_delete->next;
    }

    if(fl->tail == f_to_delete) 
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

    //failfish *head;
    failfish *temp, *fish;

    for(i = 0; i < numPonds; i++)
    {
        printf("Pond %d: %s\n", (ponds + i)->num, (ponds + i)->name);
        temp = (ponds + i)->fl->head;
        for(j = 0; j < ((ponds + i)->ei) - 1; j++)
        {
            temp = temp->next;
        }

        fish = temp;

        for(j = 0; j < ((ponds + i)->ni) - ((ponds + i)->thi); j++)
        {
            
            printf("Failfish %d eaten\n", fish->num);
            //fish_list_delete((ponds + i)->fl, fish, 0);
            fish_list_delete((ponds + i)->fl, (ponds + i)->fl->head, (ponds + i)->fl->head->num);
            for(k = 0; k < ((ponds + i)->ei) - 1; k++)
            {
                fish = fish->next;
            }
        }
    }
}

/*
void first_course(pond *ponds, int numPonds)
{
    int i, j, k;

    failfish *head;
    failfish *temp;

    for(i = 0; i < numPonds; i++)
    {
        printf("Pond %d: %s\n", (ponds + i)->num, (ponds + i)->name);
        fish_list *fl = (ponds + i)->fl;
        head = fl->head;
        for(j = 0; j < ((ponds + i)->ni) - 1; j++)
        {
            temp = head->next;
            for(k = 0; k < ((ponds + i)->ni) - ((ponds + i)->thi); k++)
            {
                printf("Failfish %d eaten\n", temp->num);
                fish_list_delete(fl, temp, 0);
            }
        }                    
    }
}
*/
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

    first_course(ponds, numPonds);

    return 0;
}


/*
void deleteNode(struct Node* head, int key) 
{ 
    if (head == NULL) 
        return; 
  
    // Find the required node 
    struct Node *curr = head, *prev; 
    while (curr->data != key) { 
        if (curr->next == head) { 
            printf("\nGiven node is not found"
                   " in the list!!!"); 
            break; 
        } 
  
        prev = curr; 
        curr = curr->next; 
    } 
  
    // Check if node is only node 
    if (curr->next == head) { 
        head = NULL; 
        free(curr); 
        return; 
    } 
  
    // If more than one node, check if 
    // it is first node 
    if (curr == head) { 
        prev = head; 
        while (prev->next != head) 
            prev = prev->next; 
        head = curr->next; 
        prev->next = head; 
        free(curr); 
    } 
  
    // check if node is last node 
    else if (curr->next == head) { 
        prev->next = head; 
        free(curr); 
    } 
    else { 
        prev->next = curr->next; 
        free(curr); 
    } 
} 
*/