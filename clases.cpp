#include "clases.h"

//==PESO==
peso::peso(){
    vector[0]=0;
    vector[1]=0;
};

peso::peso(float p0, float p1)
{
    vector[0]=p0;
    vector[1]=p1;
};


//==SOLUCION==

solucion::solucion(){
	vector = NULL;
}

solucion::solucion(float vectorp[]){
	vector = vectorp;
}


//==SUBPROBLEMA==

subproblema::subproblema(int n, peso p, peso pesos[], float porcentaje_vecinos)
{
    id=p; //Asignación del peso que le identifica
    int num_de_vecinos = (int)(n*(porcentaje_vecinos/100));//Número de vecinos que puede tener
    float distancias[n];
    for (int i = 0; i < n; i++)//Calculamos todas las distancias
    {
        distancias[i] = sqrt(pow(pesos[i].vector[0]-p.vector[0], 2) + pow(pesos[i].vector[1]-p.vector[1], 2));
    }

    float val_pos[num_de_vecinos][2] = {{0,0}};//Aquí almacenamos la posición y el valor de los vecinos
    for (int i = 0; i < n; i++) //Recorre todas las distancias
    {
    	if(i>=num_de_vecinos){
    		float max[2] = {0, 0}; //Posición del máximo valor en nuestra lista de vecinos
			int max_pos = 0;
			for(int j=0; j<num_de_vecinos; j++)
			{
				if(val_pos[j][0]>max[0])
				{
					max[0] = val_pos[j][0];
					max[1] = val_pos[j][1];
					max_pos = j;
				}
			}
			//Si el valor actual es menor que el máximo lo sustituye

			if(distancias[i]<val_pos[max_pos][0])
			{
				val_pos[max_pos][0]= distancias[i];
				val_pos[max_pos][1]= i;
			}
    	}else{
    		val_pos[i][0]= distancias[i];
			val_pos[i][1]= i;
    	}
	}

    //Guardamos sus vecinos en la variable grupo
    grupo = new int[num_de_vecinos];
    for (int i = 0; i < num_de_vecinos; i++)
    {
        grupo[i] = (int)val_pos[i][1];
    }


}

subproblema::subproblema()
{
    peso temp;
    solucion sol;
    id = temp;
    grupo = NULL;

}

