#include "clases.h"

//==PESO==
peso::peso()
{
    vector[0]=0;
    vector[1]=0;
};

peso::peso(float p0, float p1)
{
    vector[0]=p0;
    vector[1]=p1;
};


//==SOLUCION==

solucion::solucion()
{
	vector = NULL;
	dimensiones = 0;
}
solucion::solucion(solucion *a)
{
	vector = new float[a[0].dimensiones];
	for (int i = 0; i < a[0].dimensiones; i++)
	{
		vector[i] = a[0].vector[i];
	}
	dimensiones=a[0].dimensiones;
}

solucion::solucion(float vectorp[], int p)
{
	vector = new float[p];
	for (int i = 0; i < p; i++)
	{
		vector[i] = vectorp[i];
	}
	dimensiones=p;
}

solucion::solucion(int dimensiones)
{
	vector = new float[dimensiones];
	for (int i = 0; i < dimensiones; i++)
	{
		if(i==0)
		{
			vector[i] = floatAleatorio(1,0);
		}
		else
		{
			vector[i] = floatAleatorio(2,-2);
		}
	}
	this->dimensiones = dimensiones;
}

solucion solucion:: operator - (solucion obj)
{
	float res[dimensiones] = {0};
	if(dimensiones == obj.dimensiones)
	{
		for (int i = 0; i < obj.dimensiones; i++)
		{
			res[i] = vector[i] - obj.vector[i];
		}
	}
	return solucion(res,dimensiones);
}

solucion solucion:: operator + (solucion obj)
{
	float res[dimensiones] = {0};
	if(dimensiones == obj.dimensiones)
	{
		for (int i = 0; i < obj.dimensiones; i++)
		{
			res[i] = vector[i] + obj.vector[i];
		}
	}
	return solucion(res,dimensiones);
}

solucion solucion::operator +(float obj)
{
	vector[0] = vector[0] + obj;
	vector[1] = vector[1] + obj;
}

solucion solucion::operator * (float obj){
	float res[dimensiones] = {0};
	for (int i = 0; i < dimensiones; i++)
	{
		res[i] = vector[i] * obj;
	}
	return solucion(res,dimensiones);
}

int solucion::domina (solucion s){
	solucion propia = solucion(vector, dimensiones);
	if(dimensiones > 2 && s.dimensiones==dimensiones)
	{
		solucion objPropio = busquedaAobjetivo(propia);
		solucion objAjeno = busquedaAobjetivo(s);
		if(objPropio.vector[0]<objAjeno.vector[0] && objPropio.vector[1]<objAjeno.vector[1])
		{
			return 1;
		}else return 0;
	}
	else if(dimensiones == 2 && s.dimensiones==dimensiones)
	{
		if(vector[0]<s.vector[0] && vector[1]<s.vector[1])
		{
			return 1;
		}else return 0;
	}
	else
	{
		return -1;
	}

}

//==SUBPROBLEMA==

subproblema::subproblema(int n, peso p, peso pesos[], float porcentaje_vecinos, int dimensiones)
{
    id=p; 																		//Asignación del peso que le identifica
    int num_de_vecinos = (int)(n*(porcentaje_vecinos/100));						//Número de vecinos que puede tener
    float distancias[n];														//Array donde ir guardando todas las distancias calculadas
    for (int i = 0; i < n; i++)													//Calculamos todas las distancias entre los vectores peso
    {
        distancias[i] = sqrt(pow(pesos[i].vector[0]-p.vector[0], 2) + pow(pesos[i].vector[1]-p.vector[1], 2));
    }

    float val_pos[num_de_vecinos][2] = {{0,0}};									//Aquí almacenamos la posición y el valor de los vecinos
    for (int i = 0; i < n; i++) 												//Recorre todas las distancias
    {
    	if(i>=num_de_vecinos){
    		float max[2] = {0, 0}; 												//Posición del máximo valor en nuestra lista de vecinos
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

    x = solucion(dimensiones);										//Crea una solución aleatoria

}

subproblema::subproblema()		//Constructor vacío de subproblema
{
    peso temp;
    solucion sol;
    x=sol;
    id = temp;
    grupo = NULL;

}

subproblema::subproblema(peso id, int *grupo, int num_vecinos, solucion x)
{
	this->id = id;
	this->x = x;
	this->grupo = new int[num_vecinos];
	for(int i = 0; i < num_vecinos; i++)
	{
		this->grupo[i] = grupo[i];
	}


}

solucion busquedaAobjetivo(solucion x)
{

	//Sumatorio de todas las y^2 impares
	double yImpar = 0.0;
	double sum1, sum2, yj;
	float x1 = x.vector[0];
	for (int j =2 ; j < x.dimensiones; j+=2)
	{
		yImpar += pow(x.vector[j] - 0.8 * x1 * cos(6 * M_PI * x1 + (((j+1)*M_PI)/(float)x.dimensiones)), 2);

	}

	//Sumatorio de todas las y^2 pares
	double yPar = 0.0;
	for (int j = 1; j < x.dimensiones; j+=2)
	{
		yPar += pow(x.vector[j] - 0.8 * x1 * sin(6 * M_PI * x1 + (((j+1)*M_PI)/(float)x.dimensiones)), 2);
	}

	float coordX = x1 + yImpar;				//Función f1
	float coordY = pow(1 - x1, 2) + yPar;	//Función f2

	float coord[2] = {coordX, coordY};
	return solucion(coord, 2);
}

solucion * busquedaAobjetivo(solucion * busq, int longitud)
{
	if(busq[0].dimensiones==2) return busq;	//Comprueba si los vectores ya son de dos dimensiones
	solucion * res = new solucion[longitud];
	for(int i = 0; i < longitud; i++)
	{
		res[i] = busquedaAobjetivo(busq[i]);
	}
	return res;
}

float floatAleatorio(int max, int min)	//Generador aleatorio de un número decimal entre dos límites
{
	float random = ((float) rand()) / (float) RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

float distanciaEuclidea(solucion s1, solucion s2)
{
	return  sqrt(pow(s2.vector[0]-s1.vector[0], 2) + pow(s2.vector[1]-s1.vector[1], 2));
}

float distanciaEuclidea(solucion s)
{
	return  sqrt(pow(s.vector[0], 2) + pow(s.vector[1], 2));
}

