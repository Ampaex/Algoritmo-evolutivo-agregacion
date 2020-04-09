/* Routines for storing population data into binary files */

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "global.h"



void read_pop_ascii (population *pop, int gen, int popsize, char *filename)
{
    int i,j,k;
    FILE *hdl_save;
int stot;
    char *line = NULL;
    size_t len=0;
int n;
double tmp;

    hdl_save=fopen(filename,"r");

    for (k=0; k<(gen-1); k++)
	{
        for (i=0; i<popsize; i++)
                getline(&line,&len,hdl_save);
	}
    for (i=0; i<popsize; i++)
    {
        getline(&line,&len,hdl_save);
        stot=0;
        for (j=0; j<nobj; j++)
        {
                sscanf(line+stot,"%le%n",&(pop->ind[i].obj[j]),&n);
                stot+=n;
        }

                sscanf(line+stot,"%le%n",&(pop->ind[i].constr_violation),&n);
    }
    fclose(hdl_save);
}


