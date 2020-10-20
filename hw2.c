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

pond *pond_array_constructor(FILE *ifp)
{
    int i, j;

    int numPonds = get_num_ponds(ifp);

    pond *ponds = calloc(numPonds, sizeof(pond));

    for(i = 0; i < numPonds; i++)
    {
        read_pond(ifp, ponds + i);
        for(j = 1; j < (((ponds + i)->ni) + 1); j++)
        {
            
        }
    }

    return ponds;
}

int main()
{
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");

    pond *ponds;

    ponds = pond_array_constructor(ifp);

    return 0;
}

/*
	for (int i = 0; i < n; i++)                     //Loop for ascending ordering
	{
		for (int j = 0; j < n; j++)             //Loop for comparing other values
		{
			if (a[j] > a[i])                //Comparing other array elements
			{
				int tmp = a[i];         //Using temporary variable for storing last value
				a[i] = a[j];            //replacing value
				a[j] = tmp;             //storing last value
			}  
		}
	}
*/	