#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include "clases.h"

using namespace std;


int n = 30; 					//Número de subproblemas
int g = 333; 					//Número de generaciones
int dim = 6; 					//Número de dimensiones

float f = 0.5; 					//Parámetro de mutación
float cr = 0.5; 				//Parámetro de cruce

float ls = 1; 					//Límite superior
float li = 0; 					//Límite inferior

float vecindad=20; 				//Porcentaje de vecinos de cada subproblema

float semilla = 1695;

subproblema * subp;				//Puntero al vector de subproblemas (tamaño n)

solucion z;						//Mejor solución ideal encontrada
subproblema * mejoresSubproblemas;	//Mejores subproblemas encontrados

ofstream archivoOut;			//Declaración de archivo de salida con las soluciones de todas las iteraciones
ofstream archivoBest;			//Declaración del archivo de salida con las mejores soluciones

void parametrosDeEntrada();
void inicializacion(int n, int dim, int max, int min);
void operacionED();
void ejecucion();
float tchebycheff(subproblema subpro);							//Devuelve la distancia de tchebycheff
void imprimeSolucion(solucion s);								//Escribe en el archivo de salida la solución(Si no es de objetivo lo convierte)
void imprimeNoDominadas(subproblema *subproblemas);				//Imprime el conjunto de soluciones no dominadas
void imprimeSoluciones(solucion * soluciones);					//Imprime un conjunto de soluciones
void imprimeSoluciones(subproblema * subproblemas);				//Imprime las soluciones de un conjunto de problemas
void compruebaMejoresSubproblemas();

//Código principal
int main()
{
	archivoOut.open("salida.out");
	archivoBest.open("salidaTOP.out");
	srand(semilla);					//Inicialización de los números aleatorios con semilla
	inicializacion(n, dim, ls, li);	//Inicialización del problema
	ejecucion();
	imprimeNoDominadas(mejoresSubproblemas);
	parametrosDeEntrada();			//Recogida de parámetros de entrada


    return 0;
}

//Recogida de parámetros de entrada
void parametrosDeEntrada()
{
    cout << "=====BIENVENIDO AL ALGORITMO EVOLUTIVO MULTIOBJETIVO BASADO EN AGREGACIÓN===== \n \n";
    cout << "Indica el NÚMERO DE SUBPROBLEMAS: ";
    cin >> n;
    cout << "Ha seleccionado " << n << " subproblemas.\n\n";

    cout << "Indica el NÚMERO DE GENERACIONES: ";
    cin >> g;
    cout << "Ha seleccionado " << g << " generaciones.\n\n";

    cout << "Indica el NÚMERO DE DIMENSIONES: ";
    cin >> dim;
    cout << "Ha seleccionado " << dim << " dimensiones.\n\n";

    cout << "=AJUSTES DE EVOLUCIÓN=\n" << endl;
;
    cout << "-Evolución diferencial-" << endl;
    cout << "Defina el parámetro de mutación F:";
    cin >> f;
    cout << "Ha seleccionado " << f << " como parámetro de mutación.\n\n";

    cout << "Defina el parámetro de cruce CR:";
    cin >> cr;
    cout << "Ha seleccionado " << cr << " como parámetro de cruce.\n\n";

    cout << "=AJUSTE DE ESPACIO DE BÚSQUEDA Y SUBPROBLEMAS=\n" << endl;
    cout << "Defina el tamaño de vecindad de cada subproblema(porcentaje):";
    cin >> vecindad;
    cout << "El " << vecindad << "% de la población total formará parte de un subgrupo.\n\n";

    cout << "Límite superior del subproblema:";
    cin >> ls;
    cout << "Ha seleccionado " << ls << " como límite superior.\n\n";

    cout << "Límite inferior del subproblema:";
    cin >> li;
    cout << "Ha seleccionado " << li << " como límite inferior.\n\n";

    cout << "Semilla de aleatoriedad:";
    cin >> semilla;
    cout << "Ha seleccionado " << semilla << " como semilla.\n\n";

}

void inicializacion(int n, int dim, int max, int min)
{
    peso pesos[n]; 							//Inicialización de los vectores peso
    mejoresSubproblemas = new subproblema[n];	//Reserva de memoria para el puntero a mejoresSoluciones
    float paso = (float)1/(float)(n-1);
    for(int i =0; i<n ;i++)
    {
        pesos[i] = peso(1-paso*i,0+paso*i);
    }

    //Creación de subproblemas y asignación de pesos
    //Durante la creación de cada subproblema se asignan sus vecinos
    subproblema subproblemas[n];
    for (int i = 0; i < n; i++)
    {
        subproblemas[i] = subproblema(n, pesos[i], pesos, vecindad, dim, max, min);
    }

    //Copiamos todas las soluciones en un puntero para no perderlas
    subp = new subproblema[n];
    for(int i = 0; i<n ; i++)
    {
    	subp[i] = subproblemas[i];
    }

    /*[DEBUG]IMPRIME LAS SOLUCIONES DE TODOS LOS SUBPROBLEMAS
    for (int i = 0; i < n; ++i)
    {
    	cout << "Subproblema" << i << ":";
		for (int j = 0; j < dim; ++j)
		{
			cout << subproblemas[i].x.vector[j] << ",";
		}
		cout<<endl;
	}*/

    //Búsqueda de la mejor solución de la inicialización (z)
    solucion temp;
    float f[2] = {200.0,200.0};
    z = solucion(f,2);
    for(int i = 0; i < n; i++)
    {
    	temp = busquedaAobjetivo(subproblemas[i].x);
    	if(temp.vector[0]<z.vector[0])
    	{
    		float tempa[2] = {temp.vector[0], z.vector[1]};
    		z = solucion(tempa,2);
    	}
    	if(temp.vector[1]<z.vector[1])
		{
			float tempa[2] = {z.vector[0], temp.vector[1]};
			z = solucion(tempa,2);
		}
    }

   //[DEBUG] IMPRIME EL VECTOR Z
    cout << "Solución Z primera iteración:" << z.vector[0] << "," << z.vector[1];

}

void ejecucion(){

	imprimeSoluciones(subp);
	compruebaMejoresSubproblemas();

	for (int i = 0; i < g; i++)
	{
		cout<< "Gen: " << i <<endl;
		operacionED();
		cout<<"z: "<<z.vector[0]<<","<<z.vector[1];
		imprimeSoluciones(subp);
		compruebaMejoresSubproblemas();
	}


}

void operacionED()									//Mutación y cruce de evolución diferencial
{
	int num_vecinos = (int)((vecindad/100)*n);
	if(num_vecinos < 3)
	{
		cout << "No existen suficientes vecinos para ejecutar la evolución diferencial(3), aumente el número de subproblemas o el porcentaje de vecindad";
		return;
	}

	for (int i = 0; i < n; ++i)
	{
		int pos[3] = {0};							//3 posiciones aleatorias de entre los vecinos
		for (int j = 0; j < 3; ++j)					//Bucle para crear las posiciones aleatorias sin repetición
		{
			if(j==0)
			{
				pos[0] = (int)rand()%num_vecinos;
			}
			if(j==1)
			{
				pos[1] = (int)rand()%num_vecinos;
				while(pos[1]==pos[0]) pos[1] = (int)rand()%num_vecinos;
			}
			if (j==2)
			{
				pos[2] = (int)rand()%num_vecinos;
				while(pos[2]==pos[0]||pos[2]==pos[1]) pos[2] = (int)rand()%num_vecinos;
			}
		}

		int tm[3] = {subp[i].grupo[pos[0]],subp[i].grupo[pos[1]],subp[i].grupo[pos[2]]};

		solucion m = subp[tm[0]].x + (subp[tm[1]].x - subp[tm[2]].x)*f;		//Mutación

		for(int j =0; j<m.dimensiones; j++)
		{
			if(m.vector[j]<li)
			{
				m.vector[j] = li;
			}
			else if(m.vector[j]>ls)
			{
				m.vector[j] = ls;
			}
		}

		solucion previa = subp[i].x;

		float trial_vector[previa.dimensiones];
		int delta = (int)rand()% previa.dimensiones;

		for (int j = 0; j < previa.dimensiones; ++j)			//Cruce según criterio
		{
			float rng = (float)(rand()%99)/100; 				//Número entre 0 y 1
			if(rng<=cr||j == delta){
				trial_vector[j] = m.vector[j];
			}else if(rng>cr||j != delta){
				trial_vector[j] = previa.vector[j];
			}
		}
		solucion trial_solution = solucion(trial_vector, previa.dimensiones);

		subproblema nuevo_sub = subproblema(subp[i].id, subp[i].grupo, num_vecinos, trial_solution);

		if(tchebycheff(nuevo_sub)<tchebycheff(subp[i]))	//Evaluación de la nueva solución con respecto a la anterior
		{
			subp[i].x = trial_solution;

			solucion temp = busquedaAobjetivo(subp[i].x);		//Recalcula z
			if(temp.vector[0]<z.vector[0])
			{
				float tempa[2] = {temp.vector[0], z.vector[1]};
				z = solucion(tempa,2);
			}
			if(temp.vector[1]<z.vector[1])
			{
				float tempa[2] = {z.vector[0], temp.vector[1]};
				z = solucion(tempa,2);
			}

			for (int j = 0; j < num_vecinos; j++)		//Comprueba si la solución encontrada también mejora a la de los vecinos
			{
				int indice = subp[i].grupo[j];
				if(tchebycheff(subp[indice])>tchebycheff(subp[i]))
				{
					subp[indice].x = subp[i].x;
				}
			}
		}
	}

	for(int i = 0; i < n; i++)		//[DEBUG]Imprime todas las soluciones
	{
		cout<< "Sub "<<i<<": ";
		for(int j = 0;j < 2;j++)
		{
			cout << busquedaAobjetivo(subp[i].x).vector[j] << ",";
		}
		cout << endl;
	}
}

float tchebycheff(subproblema subproblem)	//Nos devuelve un valor que es menor cuanto mas cerca nos encontremos de la solución
{
	float re;						//Variable de retorno
	float re2;
	solucion solu;
	if(subproblem.x.dimensiones>2)	//Hace válida la función para espacio de búsqueda y objetivos
	{
		solu = busquedaAobjetivo(subproblem.x);
	}else
	{
		solu = subproblem.x;
	}

	re = subproblem.id.vector[0]*abs(solu.vector[0]-z.vector[0]);	// lambda*|f1-z|
	re2 = subproblem.id.vector[1]*abs(solu.vector[1]-z.vector[1]);
	if(re<re2)
	{
		re = re2;
	}
	return re;
}

void imprimeSolucion(solucion s)
{
	solucion a = s;
	if(s.dimensiones!=2){
		a = busquedaAobjetivo(s);
	}
	for(int b = 0; b < a.dimensiones; b++)
	{
		archivoOut << a.vector[b] << "\t";
	}
	archivoOut << endl;
}

void imprimeNoDominadas(subproblema *subproblemas)	//Tiene que computar soluciones de objetivo
{
	int contador = 0;										//Servirá para almacenar las soluciones consecutivamente en el vector
	solucion * res = new solucion[n];						//Soluciones de respuesta no dominadas
	solucion * solus = new solucion[n];						//Almacenamos todas las soluciones del espacio de búsqueda
	for (int i = 0; i < n; i++) solus[i] = subproblemas[i].x;
	solucion * solusObj = busquedaAobjetivo(solus,n);		//Convertimos todas las soluciones al espacio de objetivo

	for(int i = 0;i < n;i++)								//Comprobamos una por una si la solución es dominada por alguna otra
	{
		for (int j = 0; j < n; j++)
		{
			if(solusObj[j].domina(solusObj[i]))
			{
				break;
			}
			if(j==(n-1))
			{
				res[contador] = solusObj[i];
				contador++;
			}
		}
	}
	for(int i = 0; i < contador; i++)
	{
		for(int b = 0; b < res[0].dimensiones; b++)
		{
			archivoBest << res[i].vector[b] << "\t";
		}
		archivoBest << endl;
	}


}

void imprimeSoluciones(solucion * soluciones){
	for(int i = 0; i < n; i++)
	{
		if(soluciones[i].dimensiones!=0)
		{
			imprimeSolucion(soluciones[i]);
		}else break;
	}
}

void imprimeSoluciones(subproblema * subproblemas){
	for(int i = 0; i < n; i++)
	{
		if(subproblemas[i].x.dimensiones!=0)
		{
			imprimeSolucion(subproblemas[i].x);
		}else break;
	}
}

void compruebaMejoresSubproblemas(){
	if(mejoresSubproblemas[0].x.dimensiones==0)
	{
		for (int i = 0; i < n; i++) mejoresSubproblemas[i] = subp[i];
		return;
	}
	for (int i = 0; i < n; i++)
	{
		if(tchebycheff(subp[i]) < tchebycheff(mejoresSubproblemas[i]))
		{
			mejoresSubproblemas[i] = subp[i];
		}
	}
}


