/************************************
*									*
* Calcula metricas de dos frentes	*
*									*
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "global.h"

/* Variables globales */
int nreal;
int nbin;
int nobj;
int ncon;
int popsizeA, popsizeB;
int ngenA, ngenB;
int igenA, igenB;
int ngen;
int nconA, nconB;
int nrealA, nrealB;
int nexecutions, allgen, calcref;
int ngenmax;



int *nbits;

double cantidad_dominancia(individual *ind_a, individual *ind_b, double *bounds_up, double *bounds_low);

int main()
{
	int i,j,k,m,l,p;
	double bounds;
	double *extent, *extent2;
	double *boundsA_up, *boundsB_up;
	double *boundsA_low, *boundsB_low;
	double *bounds_low, *bounds_up;
	double *minimum_distanceA, *minimum_distanceB;
	double tmp_minimum_distance, mean_distance;
	double *spacing;
	double *spacing2;
	double  *hypervol;
	double *hypervol2;
	double *num_cs,*num_ecs,*num_cs2,*num_ecs2;
	int flag_cs,flag_cs2;
	int popsizeA2, popsizeB2;
	int popsizeACS, popsizeBCS;
	population *frontA,*frontB;
	population *frontACS,*frontBCS;
	double *dataA, *dataB;
	double *referenceA, *referenceB, *reference;
        char filenameA[100], filenameB[100];
        double *referente;
	double refdum;
	
	int calculate_bounds_1front();
	double *data_maximum();
	double hv();
	FILE *gp1, *gp2, *gp3, *gp4;
	
	FILE *fcs;
	FILE *fcs2;
	FILE *fextent,*fspacing, *fspacing2, *fextent2;
	FILE *fhypervol;
	FILE *fhypervol2;
	FILE *fhv;

	fhv=fopen("hvref.out","w");

	

        gp1 = popen(GNUPLOT_COMMAND,"w");
        if (gp1==NULL)
        {
            printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file global.h\n");
            printf("\n Edit the string to suit your system configuration and rerun the program\n");
            exit(1);
        }
        gp2 = popen(GNUPLOT_COMMAND,"w");
        if (gp2==NULL)
        {
            printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file global.h\n");
            printf("\n Edit the string to suit your system configuration and rerun the program\n");
            exit(1);
        }
        gp3 = popen(GNUPLOT_COMMAND,"w");
        if (gp3==NULL)
        {
            printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file global.h\n");
            printf("\n Edit the string to suit your system configuration and rerun the program\n");
            exit(1);
        }
        gp4 = popen(GNUPLOT_COMMAND,"w");
        if (gp4==NULL)
        {
            printf("\n Could not open a pipe to gnuplot, check the definition of GNUPLOT_COMMAND in file global.h\n");
            printf("\n Edit the string to suit your system configuration and rerun the program\n");
            exit(1);
        }

	printf("Do you want to generate metrics of one execution or compare two executions? (1 for one execution) (2 for two executions)\n");
	scanf("%d",&nexecutions);
	if (nexecutions!=1 && nexecutions!=2)
		{
        	printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",nexecutions);
        	exit(1);
    		}
	if (nexecutions==2)
	{
	printf("Do you want to calculate metrics for a single generation or for all generations? (0 for single generation) (1 for all generations)\n");
	scanf("%d",&allgen);
	if (allgen!=0 && allgen!=1)
		{
        	printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",allgen);
        	exit(1);
    		}
    	printf("\n Enter the number of objectives : ");
    	scanf("%d",&nobj);
   	if (nobj<2)
    		{
        	printf("\n number of objectives entered is : %d",nobj);
        	printf("\n Wrong number of objectives entered, hence exiting \n");
        	exit (1);
    		}
	if (allgen==1)
		{
		printf("Enter name of first file with information of evolution (max 50 characters)\n");
		scanf("%s",&filenameA);
    		printf("\n Enter the population size in first file : ");
    		scanf("%d",&popsizeA);
    		if (popsizeA<1)
    			{
        		printf("\n population size read is : %d",popsizeA);
        		printf("\n Wrong population size entered, hence exiting \n");
        		exit (1);
			 }
    		printf("\n Enter the number of generations in first file: ");
    		scanf("%d",&ngenA);
    		if (ngenA<1)
    			{
        		printf("\n number of generations read is : %d",ngenA);
        		printf("\n Wrong nuber of generations entered, hence exiting \n");
        		exit (1);
			 }
		printf("Enter name of second file with information of evolution (max 50 characters)\n");
		scanf("%s",&filenameB);
    		printf("\n Enter the population size in second file : ");
    		scanf("%d",&popsizeB);
    		if (popsizeB<1)
    			{
        		printf("\n population size read is : %d",popsizeB);
        		printf("\n Wrong population size entered, hence exiting \n");
        		exit (1);
			 }
    		printf("\n Enter the number of generations in second file: ");
    		scanf("%d",&ngenB);
    		if (ngenB<1)
    			{
        		printf("\n number of generations read is : %d",ngenB);
        		printf("\n Wrong nuber of generations entered, hence exiting \n");
        		exit (1);
			 }
		}
	if (allgen==0)
		{
		printf("Enter name of first file with information of evolution (max 30 characters)\n");
		scanf("%s",&filenameA);
    		printf("\n Enter the population size in first file : ");
    		scanf("%d",&popsizeA);
    		if (popsizeA<1)
    			{
        		printf("\n population size read is : %d",popsizeA);
        		printf("\n Wrong population size entered, hence exiting \n");
        		exit (1);
			 }
    		printf("\n Enter the desired generation in first file: ");
    		scanf("%d",&igenA);
    		if (igenA<1)
    			{
        		printf("\n Generation number read is : %d",igenA);
        		printf("\n Wrong generation number entered, hence exiting \n");
        		exit (1);
			 }
		printf("Enter name of second file with information of evolution (max 30 characters)\n");
		scanf("%s",&filenameB);
    		printf("\n Enter the population size in second file : ");
    		scanf("%d",&popsizeB);
    		if (popsizeB<1)
    			{
        		printf("\n population size read is : %d",popsizeB);
        		printf("\n Wrong population size entered, hence exiting \n");
        		exit (1);
			 }
    		printf("\n Enter the desired generation in second file: ");
    		scanf("%d",&igenB);
    		if (igenB<1)
    			{
        		printf("\n Generation number read is : %d",igenB);
        		printf("\n Wrong generation number entered, hence exiting \n");
        		exit (1);
			 }
		}
	}
	if (nexecutions==1)
	{
	printf("Do you want to calculate metrics for a single generation or for all generations? (0 for single generation) (1 for all generations)\n");
	scanf("%d",&allgen);
	if (allgen!=0 && allgen!=1)
		{
        	printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",allgen);
        	exit(1);
    		}
    	printf("\n Enter the number of objectives : ");
    	scanf("%d",&nobj);
   	if (nobj<2)
    		{
        	printf("\n number of objectives entered is : %d",nobj);
        	printf("\n Wrong number of objectives entered, hence exiting \n");
        	exit (1);
    		}
	if (allgen==1)
		{
		printf("Enter file name with information of evolution (max 30 characters)\n");
		scanf("%s",&filenameA);
    		printf("\n Enter the population size : ");
    		scanf("%d",&popsizeA);
    		if (popsizeA<1)
    			{
        		printf("\n population size read is : %d",popsizeA);
        		printf("\n Wrong population size entered, hence exiting \n");
        		exit (1);
			 }
    		printf("\n Enter the number of generations : ");
    		scanf("%d",&ngenA);
    		if (ngenA<1)
    			{
        		printf("\n number of generations read is : %d",ngenA);
        		printf("\n Wrong nuber of generations entered, hence exiting \n");
        		exit (1);
			 }
		}
	if (allgen==0)
		{
		printf("Enter file name with information of evolution (max 30 characters)\n");
		scanf("%s",&filenameA);
    		printf("\n Enter the population size : ");
    		scanf("%d",&popsizeA);
    		if (popsizeA<1)
    			{
        		printf("\n population size read is : %d",popsizeA);
        		printf("\n Wrong population size entered, hence exiting \n");
        		exit (1);
			 }
    		printf("\n Enter the desired generation: ");
    		scanf("%d",&igenA);
    		if (igenA<1)
    			{
        		printf("\n Generation number read is : %d",igenA);
        		printf("\n Wrong generation number entered, hence exiting \n");
        		exit (1);
			 }
		}
	}

	printf("\\nnDo you want to calculate reference point for hypervolume automatically? (0 for NO) (1 for YES)\n");
	scanf("%d",&calcref);
	if (calcref!=0 && calcref!=1)
		{
        	printf("\n Entered the wrong choice, hence exiting, choice entered was %d\n",calcref);
        	exit(1);
    		}
	reference = (double *)calloc(nobj,sizeof(double));
	if (calcref==0)
		{
		for(i=0;i<nobj;i++)
			{
			printf("\nEnter the coordinate of the reference point corresponding to objective #%d: ",(i+1));
			scanf("%lg",&refdum);
			reference[i]=refdum;
			}
		}
		
/* for(j=0;j<nobj;j++)
	printf("ref[%d]=%lg\n",j,reference[j]);
*/
if(allgen==1)
{
	
	/* Reserva memoria para las poblaciones */
	frontA = (population *)malloc(sizeof(population));
	frontB = (population *)malloc(sizeof(population));
	frontACS = (population *)malloc(sizeof(population));
	frontBCS = (population *)malloc(sizeof(population));
	allocate_memory_pop(frontA, popsizeA);
    	allocate_memory_pop(frontB, popsizeB);
	allocate_memory_pop(frontACS, popsizeA);
    	allocate_memory_pop(frontBCS, popsizeB);
	boundsA_up = (double *)calloc(nobj,sizeof(double));
	boundsA_low = (double *)calloc(nobj,sizeof(double));
	boundsB_up = (double *)calloc(nobj,sizeof(double));
	boundsB_low = (double *)calloc(nobj,sizeof(double));
	bounds_up = (double *)calloc(nobj,sizeof(double));
	bounds_low = (double *)calloc(nobj,sizeof(double));
	referenceA = (double *)calloc(nobj,sizeof(double));
	referenceB = (double *)calloc(nobj,sizeof(double));
	
	minimum_distanceB = (double *)calloc(popsizeB,sizeof(double));
	minimum_distanceA = (double *)calloc(popsizeA,sizeof(double));
		
	
	extent = (double *)calloc((ngenA+1),sizeof(double));
	extent2 = (double *)calloc((ngenB+1),sizeof(double));
	spacing = (double *)calloc((ngenA+1),sizeof(double));
	spacing2 = (double *)calloc((ngenB+1),sizeof(double));
	hypervol = (double *)calloc((ngenA+1),sizeof(double));
	hypervol2 = (double *)calloc((ngenB+1),sizeof(double));
	
	
	num_cs = (double *)calloc((ngenA+1),sizeof(double));
	num_cs2 = (double *)calloc((ngenB+1),sizeof(double));

	/* Abre ficheros */
	fcs=fopen("cs.out","w");
	fcs2=fopen("cs2.out","w");
	fextent=fopen("extent.out","w");
	fextent2=fopen("extent2.out","w");
	fspacing=fopen("spacing.out","w");
	fspacing2=fopen("spacing2.out","w");
	fhypervol=fopen("hypervol.out","w");
	fhypervol2=fopen("hypervol2.out","w");
	
/* Comparison up to the smallest number of generations of the 2 fronts
  */
        ngen=ngenA;
        ngenmax=ngenA;
	if(nexecutions==2)
		{
		if(ngenB<ngen)
			ngen=ngenB;
		else
			ngenmax=ngenB;
		}
	
	
if(calcref!=0)
{
/* Initialization to - infinite */

 	for(j=0;j<nobj;j++)
		reference[j]=-1.0e100;

/* Calculating the reference point for hypervolume calculation */
	for(i=1; i<=ngenA; i++)
	{
        read_pop_ascii(frontA, i, popsizeA, filenameA);
	dataA = (double *)calloc((nobj*popsizeA),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeA2=0;m<popsizeA;m++)    /* sweep all the individuals in frontA */
		{
		if(frontA->ind[m].constr_violation!=0.0)
				continue;      /* not feasible. Go to next individual */
		popsizeA2++;
		for(j=0;j<nobj;j++,k++)
			dataA[k]=frontA->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
	referenceA=data_maximum(dataA,nobj,popsizeA2);
	if(popsizeA2)
 	for(j=0;j<nobj;j++)
		{
		if(referenceA[j]>reference[j])
			reference[j]=referenceA[j];
		}
    	free(dataA);
	}
	if(nexecutions==2)  {
	for(i=1; i<=ngenB; i++)
        {
         read_pop_ascii(frontB, i, popsizeB, filenameB);
	dataB = (double *)calloc((nobj*popsizeB),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeB2=0;m<popsizeB;m++)
		{
		if(frontB->ind[m].constr_violation!=0.0)
				continue;      /* Not feasible. Go to next individual */
		popsizeB2++;
		for(j=0;j<nobj;j++,k++)
			dataB[k]=frontB->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
	referenceB=data_maximum(dataB,nobj,popsizeB2);
	if(popsizeB2)
 	for(j=0;j<nobj;j++)
		{
		if(referenceB[j]>reference[j])
			reference[j]=referenceB[j];
		}
    	free(dataB);
		}

	}
}

printf("Reference point for hypervolume calculation\n");
 for(j=0;j<nobj;j++)
	printf("ref[%d]=%lg\n",(j+1),reference[j]);
 for(j=0;j<nobj;j++)
	fprintf(fhv,"%lg\t",reference[j]);
fprintf(fhv,"\n");

	for(i=1; i<=ngenmax; i++){
		printf("# Frente gen. %d\n", i);

		if(i<=ngenA) {
                read_pop_ascii(frontA, i, popsizeA,  filenameA);

		num_cs[i]=0;
		
		popsizeACS=0;
		for(k=0;k<nobj;k++)
			frontACS->ind[popsizeACS].obj[k]=frontA->ind[0].obj[k];
		frontACS->ind[popsizeACS++].constr_violation=frontA->ind[0].constr_violation;
		for(j=1; j<popsizeA; j++)
			{		
			for(l=0;l<popsizeACS;l++)
				{
				for(k=0;k<nobj;k++)
					{
					if(frontA->ind[j].obj[k]!=frontACS->ind[l].obj[k])
						break;
					}
				if(k==nobj)  /*equal individual found */
					break;
				}
			if(l==popsizeACS)	/* new individual */
				{
				for(k=0;k<nobj;k++)
					frontACS->ind[popsizeACS].obj[k]=frontA->ind[j].obj[k];
				frontACS->ind[popsizeACS++].constr_violation=frontA->ind[j].constr_violation;
				}
			}
                }
		if((nexecutions==2) && (i<=ngenB)) { 
		num_cs2[i]=0;
                read_pop_ascii(frontB, i, popsizeB,  filenameB);
		popsizeBCS=0;
		for(k=0;k<nobj;k++)
			frontBCS->ind[popsizeBCS].obj[k]=frontB->ind[0].obj[k];
		frontBCS->ind[popsizeBCS++].constr_violation=frontB->ind[0].constr_violation;
		for(j=1; j<popsizeB; j++)
			{		
			for(l=0;l<popsizeBCS;l++)
				{
				for(k=0;k<nobj;k++)
					{
					if(frontB->ind[j].obj[k]!=frontBCS->ind[l].obj[k])
						break;
					}
				if(k==nobj)  /*equal individual found */
					break;
				}
			if(l==popsizeBCS)	/* new individual */
				{
				for(k=0;k<nobj;k++)
					frontBCS->ind[popsizeBCS].obj[k]=frontB->ind[j].obj[k];
				frontBCS->ind[popsizeBCS++].constr_violation=frontB->ind[j].constr_violation;
				}
			}

		}
		if((nexecutions==2) && (i<=ngen)) { 
				
		for(k=0; k<popsizeACS; k++){		
			flag_cs=0;
			
			for(j=0; j<popsizeBCS; j++){	
		
				if(check_dominance(&(frontBCS->ind[j]), &(frontACS->ind[k]))==1){
					flag_cs++;
					break;
				}
			}
			if(flag_cs>0){
				num_cs[i]++;
			}
		}
		num_cs[i]=num_cs[i]/popsizeACS;
		for(k=0; k<popsizeBCS; k++){		
			flag_cs2=0;
			
			for(j=0; j<popsizeACS; j++){	
		
				if(check_dominance(&(frontACS->ind[j]), &(frontBCS->ind[k]))==1){
					flag_cs2++;
					break;
				}
			}
			if(flag_cs2>0){
				num_cs2[i]++;
			}
		}
		num_cs2[i]=num_cs2[i]/popsizeBCS;
		fprintf(fcs, "%d\t%.10f\n", i,num_cs[i]);
		fprintf(fcs2, "%d\t%.10f\n", i,num_cs2[i]);
		fflush(fcs);
		fflush(fcs2);
		}

		/* EXTENT  */
		if(i<=ngenA)
		{
if(calculate_bounds_1front(frontA,boundsA_up,boundsA_low,popsizeA)!=1)
		{
		extent[i]=1;
		for(j=0;j<nobj;j++)
			{
			extent[i]*=(boundsA_up[j]-boundsA_low[j]);
			}
		extent[i]=extent[i]/nobj;
		}
		/* Imprime cada metrica en un fichero */
		fprintf(fextent, "%d\t%.10f\t%.10f\n" ,i,extent[i],log10(extent[i]));
		}
		if((nexecutions==2) && (i<=ngenB)) { 
if(calculate_bounds_1front(frontB,boundsB_up,boundsB_low,popsizeB)!=1)
		{
		extent2[i]=1;
		for(j=0;j<nobj;j++)
			{
			extent2[i]*=(boundsB_up[j]-boundsB_low[j]);
			}
		extent2[i]=extent2[i]/nobj;
		}	
		fprintf(fextent2, "%d\t%.10f\t%.10f\n"	,i,extent2[i],log10(extent2[i]));
		}
		

	/* SPACING  */
		if(i<=ngenA)
		{
	popsizeA2=popsizeACS;
	for(m=0;m<popsizeACS;m++)
		{
		if(frontACS->ind[m].constr_violation!=0.0)
				{
				popsizeA2--;
				continue;
				}
		minimum_distanceA[m]=1e30;
		for(j=0;j<popsizeACS;j++)
			{
			if(j==m)	continue;
		if(frontACS->ind[j].constr_violation!=0.0)
				continue;
			tmp_minimum_distance=0.0;
			for(k=0;k<nobj;k++)
				{
				tmp_minimum_distance+=(fabs(frontACS->ind[m].obj[k]-frontACS->ind[j].obj[k])/(boundsA_up[k]-boundsA_low[k]));
				}		
			if(tmp_minimum_distance==0.0)
				{
				continue;
				}
			if(tmp_minimum_distance<minimum_distanceA[m])	minimum_distanceA[m]=tmp_minimum_distance;
			}	
		}
	mean_distance=0.0;
	for(m=0;m<popsizeACS;m++)
		{
		if(frontACS->ind[m].constr_violation!=0.0)
				continue;
		mean_distance+=minimum_distanceA[m];
		}	
	if(popsizeA2)
	mean_distance=mean_distance/popsizeA2;
	spacing[i]=0.0;
	for(m=0;m<popsizeACS;m++)
		{
		if(frontACS->ind[m].constr_violation!=0.0)
				continue;
                spacing[i]+=(minimum_distanceA[m]-mean_distance)*(minimum_distanceA[m]-mean_distance);
		}
	if(popsizeA2)
       		spacing[i]=sqrt(spacing[i]/popsizeA2);
	else
		spacing[i]=0.0;
	fprintf(fspacing, "%d\t%.10f\n",i,spacing[i]);
        fflush(fspacing);
		}

	if((nexecutions==2) && (i<=ngenB)) { 
	popsizeB2=popsizeBCS;
	for(m=0;m<popsizeBCS;m++)
		{
		if(frontBCS->ind[m].constr_violation!=0.0)
				{
				popsizeB2--;
				continue;
				}
		minimum_distanceB[m]=1e30;
		for(j=0;j<popsizeBCS;j++)
			{
			if(j==m)	continue;
		if(frontBCS->ind[j].constr_violation!=0.0)
				continue;
			tmp_minimum_distance=0.0;
			for(k=0;k<nobj;k++)
				{
				tmp_minimum_distance+=(fabs(frontBCS->ind[m].obj[k]-frontBCS->ind[j].obj[k])/(boundsB_up[k]-boundsB_low[k]));
				}		
			if(tmp_minimum_distance==0.0)
				{
				continue;
				}
			if(tmp_minimum_distance<minimum_distanceB[m])	minimum_distanceB[m]=tmp_minimum_distance;
			}	
		}
	mean_distance=0.0;
	for(m=0;m<popsizeBCS;m++)
		{
		if(frontBCS->ind[m].constr_violation!=0.0)
				continue;
		mean_distance+=minimum_distanceB[m];
		}	
	if(popsizeB2)
	mean_distance=mean_distance/popsizeB2;
	spacing2[i]=0.0;
	for(m=0;m<popsizeBCS;m++)
		{
		if(frontBCS->ind[m].constr_violation!=0.0)
				continue;
                spacing2[i]+=(minimum_distanceB[m]-mean_distance)*(minimum_distanceB[m]-mean_distance);
		}
	if(popsizeB2)
       		spacing2[i]=sqrt(spacing2[i]/popsizeB2);
	else
		spacing2[i]=0.0;
	fprintf(fspacing2, "%d\t%.10f\n",i,spacing2[i]);
        fflush(fspacing2);
	}
	

	/* HYPERVOLUME  */
	if(i<=ngenA)
	{
	dataA = (double *)calloc((nobj*popsizeA),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeA2=0;m<popsizeA;m++)    /* sweep all the individuals in frontA */
		{
		if(frontA->ind[m].constr_violation!=0.0)
				continue;      /* not feasible. Go to next individual */
		popsizeA2++;
		for(j=0;j<nobj;j++,k++)
			dataA[k]=frontA->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
/*
if(i==ngenmax)
{
printf("ref=[%e,%e]\n",reference[0],reference[1]);
for(p=0;p<k;p++)
{
printf("dataA[%d]=%e\n",p,dataA[p]);
}
}
*/
	if(popsizeA2)
		hypervol[i]=hv(dataA,nobj,popsizeA2,reference);
	else /* no feasible individual in population */
		hypervol[i]=0;
	fprintf(fhypervol, "%d\t%.10f\n"	,i,hypervol[i]);
        fflush(fhypervol);
    	free(dataA);
	}
	if((nexecutions==2) && (i<=ngenB)) { 
	dataB = (double *)calloc((nobj*popsizeB),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeB2=0;m<popsizeB;m++)
		{
		if(frontB->ind[m].constr_violation!=0.0)
				continue;      /* Not feasible. Go to next individual */
		popsizeB2++;
		for(j=0;j<nobj;j++,k++)
			dataB[k]=frontB->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
	if(popsizeB2)
		hypervol2[i]=hv(dataB,nobj,popsizeB2,reference);
	else
		hypervol2[i]=0;
	fprintf(fhypervol2, "%d\t%.10f\n"	,i,hypervol2[i]);
        fflush(fhypervol2);
    	free(dataB);
	}

		
	}
	
	/* Cierra ficheros*/
	fclose(fextent);
	fclose(fextent2);
	fclose(fcs2);
	fclose(fhypervol);
	fclose(fhypervol2);
	fclose(fspacing);
	fclose(fspacing2);
	fclose(fcs);

	
if(nexecutions==1)  
{
fprintf(gp1,"set title 'Hypervolume'\n plot 'hypervol.out' title 'front 1' w lines\n set xlabel 'ngen'\nset ylabel 'hypervolume'\n");
/* fprintf(gp2,"set title 'Extent'\n plot 'extent.out' using 1:2 title 'front 1' w lines\n set xlabel 'ngen'\nset ylabel 'extent'\n");
  */
fprintf(gp3,"set title 'Spacing'\n plot 'spacing.out' using 1:2 title 'front 1' w lines\n set xlabel 'ngen'\nset ylabel 'spacing'\n");
}
if(nexecutions==2)  
{
fprintf(gp1,"set title 'Hypervolume'\n plot 'hypervol.out' title 'front 1' w lines, 'hypervol2.out' title 'front 2' w lines\n set xlabel 'ngen'\nset ylabel 'hypervolume'\n");
/* fprintf(gp2,"set title 'Extent'\n plot 'extent.out' using 1:2 title 'front 1' w lines, 'extent2.out' using 1:2  title 'front 2' w lines\n set xlabel 'ngen'\nset ylabel 'extent'\n");
 */
fprintf(gp3,"set title 'Spacing'\n plot 'spacing.out' using 1:2 title 'front 1' w lines, 'spacing2.out' using 1:2  title 'front 2' w lines\n set xlabel 'ngen'\nset ylabel 'spacing'\n");
fprintf(gp4,"set title 'Coverage set'\n plot 'cs.out' using 1:2 title 'C(2,1)' w lines, 'cs2.out' using 1:2  title 'C(1,2)' w lines\n set xlabel 'ngen'\nset ylabel 'Coverage'\n");
}
	
	/* Libera memoria */
	deallocate_memory_pop(frontA, popsizeA);
    deallocate_memory_pop(frontB, popsizeB);
    free(frontA);
    free(frontB);
    pclose(gp1);
    pclose(gp2);
    pclose(gp3);
    pclose(gp4);
}

if(allgen==0)
{
	
	/* Reserva memoria para las poblaciones */
	frontA = (population *)malloc(sizeof(population));
	frontB = (population *)malloc(sizeof(population));
	frontACS = (population *)malloc(sizeof(population));
	frontBCS = (population *)malloc(sizeof(population));
	allocate_memory_pop(frontA, popsizeA);
    	allocate_memory_pop(frontB, popsizeB);
	allocate_memory_pop(frontACS, popsizeA);
    	allocate_memory_pop(frontBCS, popsizeB);
	boundsA_up = (double *)calloc(nobj,sizeof(double));
	boundsA_low = (double *)calloc(nobj,sizeof(double));
	boundsB_up = (double *)calloc(nobj,sizeof(double));
	boundsB_low = (double *)calloc(nobj,sizeof(double));
	bounds_up = (double *)calloc(nobj,sizeof(double));
	bounds_low = (double *)calloc(nobj,sizeof(double));
	referenceA = (double *)calloc(nobj,sizeof(double));
	referenceB = (double *)calloc(nobj,sizeof(double));
	minimum_distanceB = (double *)calloc(popsizeB,sizeof(double));
	minimum_distanceA = (double *)calloc(popsizeA,sizeof(double));
	
	num_cs = (double *)calloc(ngenA,sizeof(double));
	num_cs2 = (double *)calloc(ngenB,sizeof(double));
	extent = (double *)calloc(ngenA,sizeof(double));
	extent2 = (double *)calloc(ngenB,sizeof(double));
	spacing = (double *)calloc(ngenA,sizeof(double));
	spacing2 = (double *)calloc(ngenB,sizeof(double));
	hypervol = (double *)calloc(ngenA,sizeof(double));
	hypervol2 = (double *)calloc(ngenB,sizeof(double));
	
	

	/* Abre ficheros */
	fcs=fopen("cs.out","w");
	fcs2=fopen("cs2.out","w");
	fextent=fopen("extent.out","w");
	fextent2=fopen("extent2.out","w");
	fspacing=fopen("spacing.out","w");
	fspacing2=fopen("spacing2.out","w");
	fhypervol=fopen("hypervol.out","w");
	fhypervol2=fopen("hypervol2.out","w");
	
	
	

if(calcref!=0)
{
/* Initialization to - infinite */

 	for(j=0;j<nobj;j++)
		reference[j]=-1.0e100;

/* Calculating the reference point for hypervolume calculation */
        read_pop_ascii(frontA, igenA, popsizeA, filenameA);
	dataA = (double *)calloc((nobj*popsizeA),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeA2=0;m<popsizeA;m++)    /* sweep all the individuals in frontA */
		{
		if(frontA->ind[m].constr_violation!=0.0)
				continue;      /* not feasible. Go to next individual */
		popsizeA2++;
		for(j=0;j<nobj;j++,k++)
			dataA[k]=frontA->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
	referenceA=data_maximum(dataA,nobj,popsizeA2);
	if(popsizeA2)
 	for(j=0;j<nobj;j++)
		{
		if(referenceA[j]>reference[j])
			reference[j]=referenceA[j];
		}
    	free(dataA);
	if(nexecutions==2)  {
         read_pop_ascii(frontB, igenB, popsizeB, filenameB);
	dataB = (double *)calloc((nobj*popsizeB),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeB2=0;m<popsizeB;m++)
		{
		if(frontB->ind[m].constr_violation!=0.0)
				continue;      /* Not feasible. Go to next individual */
		popsizeB2++;
		for(j=0;j<nobj;j++,k++)
			dataB[k]=frontB->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
	referenceB=data_maximum(dataB,nobj,popsizeB2);
	if(popsizeB2)
 	for(j=0;j<nobj;j++)
		{
		if(referenceB[j]>reference[j])
			reference[j]=referenceB[j];
		}
    	free(dataB);
		}

}
printf("Reference point for hypervolume calculation\n");
 for(j=0;j<nobj;j++)
	printf("ref[%d]=%lg\n",(j+1),reference[j]);
 for(j=0;j<nobj;j++)
	fprintf(fhv,"%lg\t",reference[j]);
fprintf(fhv,"\n");

                read_pop_ascii(frontA, igenA, popsizeA,  filenameA);

		num_cs[0]=num_cs2[0]=0;
		
		popsizeACS=0;
		for(k=0;k<nobj;k++)
			frontACS->ind[popsizeACS].obj[k]=frontA->ind[0].obj[k];
		frontACS->ind[popsizeACS++].constr_violation=frontA->ind[0].constr_violation;
		for(j=1; j<popsizeA; j++)
			{		
			for(l=0;l<popsizeACS;l++)
				{
				for(k=0;k<nobj;k++)
					{
					if(frontA->ind[j].obj[k]!=frontACS->ind[l].obj[k])
						break;
					}
				if(k==nobj)  /*equal individual found */
					break;
				}
			if(l==popsizeACS)	/* new individual */
				{
				for(k=0;k<nobj;k++)
					frontACS->ind[popsizeACS].obj[k]=frontA->ind[j].obj[k];
				frontACS->ind[popsizeACS++].constr_violation=frontA->ind[j].constr_violation;
				}
			}
		if(nexecutions==2) { 
                read_pop_ascii(frontB, igenB, popsizeB,  filenameB);
		popsizeBCS=0;
		for(k=0;k<nobj;k++)
			frontBCS->ind[popsizeBCS].obj[k]=frontB->ind[0].obj[k];
		frontBCS->ind[popsizeBCS++].constr_violation=frontB->ind[0].constr_violation;
		for(j=1; j<popsizeB; j++)
			{		
			for(l=0;l<popsizeBCS;l++)
				{
				for(k=0;k<nobj;k++)
					{
					if(frontB->ind[j].obj[k]!=frontBCS->ind[l].obj[k])
						break;
					}
				if(k==nobj)  /*equal individual found */
					break;
				}
			if(l==popsizeBCS)	/* new individual */
				{
				for(k=0;k<nobj;k++)
					frontBCS->ind[popsizeBCS].obj[k]=frontB->ind[j].obj[k];
				frontBCS->ind[popsizeBCS++].constr_violation=frontB->ind[j].constr_violation;
				}
			}

				
                for(k=0; k<popsizeACS; k++){
                        flag_cs=0;

                        for(j=0; j<popsizeBCS; j++){

                                if(check_dominance(&(frontBCS->ind[j]), &(frontACS->ind[k]))==1){
                                        flag_cs++;
                                        break;
                                }
                        }
                        if(flag_cs>0){
                                num_cs[0]++;
                        }
                }
                num_cs[0]=num_cs[0]/popsizeACS;
                for(k=0; k<popsizeBCS; k++){
                        flag_cs2=0;

                        for(j=0; j<popsizeACS; j++){

                                if(check_dominance(&(frontACS->ind[j]), &(frontBCS->ind[k]))==1){
                                        flag_cs2++;
                                        break;
                                }
                        }
                        if(flag_cs2>0){
                                num_cs2[0]++;
                        }
                }
                num_cs2[0]=num_cs2[0]/popsizeBCS;
                fprintf(fcs, "%d\t%.10f\n", i,num_cs[0]);
                fprintf(fcs2, "%d\t%.10f\n", i,num_cs2[0]);

		}

		/* EXTENT  */
if(calculate_bounds_1front(frontA,boundsA_up,boundsA_low,popsizeA)!=1)
		{
		extent[0]=1;
		for(j=0;j<nobj;j++)
			{
			extent[0]*=(boundsA_up[j]-boundsA_low[j]);
			}
		extent[0]=extent[0]/nobj;
		}
		/* Imprime cada metrica en un fichero */
		fprintf(fextent, "%d\t%.10f\t%.10f\n" ,igenA,extent[0],log10(extent[0]));
		if(nexecutions==2) { 
if(calculate_bounds_1front(frontB,boundsB_up,boundsB_low,popsizeB)!=1)
		{
		extent2[0]=1;
		for(j=0;j<nobj;j++)
			{
			extent2[0]*=(boundsB_up[j]-boundsB_low[j]);
			}
		extent2[0]=extent2[0]/nobj;
		}	
		fprintf(fextent2, "%d\t%.10f\t%.10f\n"	,igenB,extent2[0],log10(extent2[0]));
		}
		

	/* SPACING  */
	popsizeA2=popsizeACS;
	for(m=0;m<popsizeACS;m++)
		{
		if(frontACS->ind[m].constr_violation!=0.0)
				{
				popsizeA2--;
				continue;
				}
		minimum_distanceA[m]=1e30;
		for(j=0;j<popsizeACS;j++)
			{
			if(j==m)	continue;
		if(frontACS->ind[j].constr_violation!=0.0)
				continue;
			tmp_minimum_distance=0.0;
			for(k=0;k<nobj;k++)
				{
				tmp_minimum_distance+=(fabs(frontACS->ind[m].obj[k]-frontACS->ind[j].obj[k])/(boundsA_up[k]-boundsA_low[k]));
				}
			if(tmp_minimum_distance==0.0)
				{
				continue;
				}
			if(tmp_minimum_distance<minimum_distanceA[m])	minimum_distanceA[m]=tmp_minimum_distance;
			}	
		}
	mean_distance=0.0;
	for(m=0;m<popsizeACS;m++)
		{
		if(frontACS->ind[m].constr_violation!=0.0)
				continue;
		mean_distance+=minimum_distanceA[m];
		}	
	if(popsizeA2)
	mean_distance=mean_distance/popsizeA2;
	spacing[0]=0.0;
	for(m=0;m<popsizeACS;m++)
		{
		if(frontACS->ind[m].constr_violation!=0.0)
				continue;
                spacing[0]+=(minimum_distanceA[m]-mean_distance)*(minimum_distanceA[m]-mean_distance);
		}
	if(popsizeA2)
       		spacing[0]=sqrt(spacing[0]/popsizeA2);
	else
		spacing[0]=0.0;
	fprintf(fspacing, "%d\t%.10f\n",igenA,spacing[0]);

	if(nexecutions==2) { 
	popsizeB2=popsizeBCS;
	for(m=0;m<popsizeBCS;m++)
		{
		if(frontBCS->ind[m].constr_violation!=0.0)
				{
				popsizeB2--;
				continue;
				}
		minimum_distanceB[m]=1e30;
		for(j=0;j<popsizeBCS;j++)
			{
			if(j==m)	continue;
		if(frontBCS->ind[j].constr_violation!=0.0)
				continue;
			tmp_minimum_distance=0.0;
			for(k=0;k<nobj;k++)
				{
				tmp_minimum_distance+=(fabs(frontBCS->ind[m].obj[k]-frontBCS->ind[j].obj[k])/(boundsB_up[k]-boundsB_low[k]));
				}		
			if(tmp_minimum_distance==0.0)
				{
				continue;
				}
			if(tmp_minimum_distance<minimum_distanceB[m])	minimum_distanceB[m]=tmp_minimum_distance;
			}	
		}
	mean_distance=0.0;
	for(m=0;m<popsizeBCS;m++)
		{
		if(frontBCS->ind[m].constr_violation!=0.0)
				continue;
		mean_distance+=minimum_distanceB[m];
		}	
	if(popsizeB2)
	mean_distance=mean_distance/popsizeB2;
	spacing2[0]=0.0;
	for(m=0;m<popsizeBCS;m++)
		{
		if(frontBCS->ind[m].constr_violation!=0.0)
				continue;
                spacing2[0]+=(minimum_distanceB[m]-mean_distance)*(minimum_distanceB[m]-mean_distance);
		}
	if(popsizeB2)
       		spacing2[0]=sqrt(spacing2[0]/popsizeB2);
	else
		spacing2[0]=0.0;
	fprintf(fspacing2, "%d\t%.10f\n",igenB,spacing2[0]);
	}
	

	/* HYPERVOLUME  */
	dataA = (double *)calloc((nobj*popsizeA),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeA2=0;m<popsizeA;m++)    /* sweep all the individuals in frontA */
		{
		if(frontA->ind[m].constr_violation!=0.0)
				continue;      /* not feasible. Go to next individual */
		popsizeA2++;
		for(j=0;j<nobj;j++,k++)
			dataA[k]=frontA->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
/*
printf("ref=[%e,%e]\n",reference[0],reference[1]);
for(p=0;p<k;p++)
{
printf("dataA[%d]=%e\n",p,dataA[p]);
}
*/
	if(popsizeA2)
		hypervol[0]=hv(dataA,nobj,popsizeA2,reference);
	else /* no feasible individual in population */
		hypervol[0]=0;
	fprintf(fhypervol, "%d\t%.10f\n"	,igenA,hypervol[0]);
	printf("\nFront 1 hypervolume: %.10f\n",hypervol[0]);
	printf("Front 1 spacing: %.10f\n",spacing[0]);
/*	printf("Front 1 extent: %.10f\n",extent[0]);
 *	*/
    	free(dataA);
	if(nexecutions==2) { 
	dataB = (double *)calloc((nobj*popsizeB),sizeof(double)); /* allocate memory for feasible front info */
	for(m=0,k=0,popsizeB2=0;m<popsizeB;m++)
		{
		if(frontB->ind[m].constr_violation!=0.0)
				continue;      /* Not feasible. Go to next individual */
		popsizeB2++;
		for(j=0;j<nobj;j++,k++)
			dataB[k]=frontB->ind[m].obj[j];    /* copy all objectives of feasible individual */
		}
	if(popsizeB2)
		hypervol2[0]=hv(dataB,nobj,popsizeB2,reference);
	else
		hypervol2[0]=0;
	fprintf(fhypervol2, "%d\t%.10f\n"	,igenB,hypervol2[0]);
	printf("Front 2 hypervolume: %.10f\n",hypervol2[0]);
	printf("Front 2 spacing: %.10f\n",spacing2[0]);
/*	printf("Front 2 extent: %.10f\n",extent2[0]);
 */
	printf("C(front2,front1)=%.10f\n",num_cs[0]);
	printf("C(front1,front2)=%.10f\n",num_cs2[0]);
    	free(dataB);
	}

		
	
	/* Cierra ficheros*/
	fclose(fcs);
	fclose(fextent);
	fclose(fextent2);
	fclose(fcs2);
	fclose(fhypervol);
	fclose(fhypervol2);
	fclose(fspacing);
	fclose(fspacing2);
	
	
	/* Libera memoria */
	deallocate_memory_pop(frontA, popsizeA);
    deallocate_memory_pop(frontB, popsizeB);
    free(frontA);
    free(frontB);
    pclose(gp1);
    pclose(gp2);
    pclose(gp3);
    pclose(gp4);
}
fclose(fhv);

	return 0;
}



int calculate_bounds_1front(population *front, double *bounds_up, double
*bounds_low, int popsize)
{
        int i,j;
        int k=0;

        for(i=0; i<nobj; i++)
                {
                while(front->ind[k].constr_violation!=0.0)
                        {
                        k++;
                        if(k==popsize)   return(1);
                        continue;
                        }
                bounds_up[i]=front->ind[k].obj[i];
                bounds_low[i]=bounds_up[i];
                for(j=k+1; j<popsize; j++)
                        {
                        if(front->ind[j].constr_violation!=0.0)
                                continue;
                        if(front->ind[j].obj[i]>bounds_up[i]) bounds_up[i]=front->ind[j].obj[i];
                        if(front->ind[j].obj[i]<bounds_low[i]) bounds_low[i]=front->ind[j].obj[i];
                        }
                }
return(0);
}

