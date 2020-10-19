#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fish {
	int num;
	struct fish *next;
    struct fish *prev;
} fish;

typedef struct fish_list {
	fish *head;
    fish *tail;
} fish_list;

typedef struct pond {
	int num;
	char *name;
	int ni;
	int ei;
    int thi;
    fish_list *fl;
} pond;

typedef struct pond_list {
	pond *head;
    pond *tail;
} pond_list;


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
    fill_monster(p, numPond, name, ni, ei, thi);
}

int main()
{
    FILE *ifp;
    FILE *ofp;

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");
}