#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include <random>
#include "clases.h"
#include <unistd.h>


#define SIGNO(x) (x > 0 ? 1.0 : -1.0)

using namespace std;


int n ; 					//Número de subproblemas
int g; 					//Número de generaciones
int dim; 					//Número de dimensiones

float f; 					//Parámetro de mutación 0.75
float cr;				//Parámetro de cruce 0.479

float vecindad; 			//Porcentaje de vecinos de cada subproblema

float semilla;

float compensacion_angular;//0.00403

float intensidad;

int iter = 0;

subproblema * subp;					//Puntero al vector de subproblemas (tamaño n)

solucion z;							//Mejor solución ideal encontrada
subproblema * mejoresSubproblemas;	//Mejores subproblemas encontrados

ofstream archivoOut;			//Declaración de archivo de salida con las soluciones de todas las iteraciones
ofstream iteracion;


void parametrosDeEntrada();
void inicializacion();
double tchebycheff(solucion soluc, peso peso);
void operacionED();
void ejecucion();
void imprimeSolucion(solucion s);								//Escribe en el archivo de salida la solución
void imprimeSoluciones(solucion * soluciones);					//Imprime un conjunto de soluciones
void imprimeSoluciones(subproblema * subproblemas);				//Imprime las soluciones de un conjunto de problemas
void graficaIteracion(FILE *pipe2);
void graficaIteracion1(FILE *pipe2);
void mutacionGaussiana(solucion * x);							//Aplica una mutación gaussiana a una solución



//Código principal
int main()
{
		archivoOut.open("salidaRes.out");
		iteracion.open("iteracion.out");

		parametrosDeEntrada();			//Recogida de parámetros de entrada
		srand(semilla);					//Inicialización de los números aleatorios con semilla
		inicializacion();
		ejecucion();
		return 0;
}

//Recogida de parámetros de entrada
void parametrosDeEntrada()
{
    cout << "=====BIENVENIDO AL ALGORITMO EVOLUTIVO MULTIOBJETIVO BASADO EN AGREGACIÓN CON RESTRICCIONES===== \n \n";
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

    cout << "Semilla de aleatoriedad:";
    cin >> semilla;
    cout << "Ha seleccionado " << semilla << " como semilla.\n\n";

    cout << "Compensación angular:";
	cin >> compensacion_angular;
	cout << "Ha seleccionado " << compensacion_angular << " como compensación angular.\n\n";

	cout << "Intensidad:";
	cin >> intensidad;
	cout << "Ha seleccionado " << intensidad << " como intensidad.\n\n";

}


void inicializacion()
{
    peso pesos[n]; 								//Inicialización de los vectores peso
    mejoresSubproblemas = new subproblema[n];	//Reserva de memoria para el puntero a mejoresSubproblemas
    float paso = (float)1/(float)(n-1);
    for(int i =0; i<n ;i++)
    {
    	pesos[i] = peso(0+paso*i,1-paso*i);
    }


    //Creación de subproblemas y asignación de pesos
    //Durante la creación de cada subproblema se asignan sus vecinos
    subproblema subproblemas[n];
    for (int i = 0; i < n; i++)
    {
        subproblemas[i] = subproblema(n, pesos[i], pesos, vecindad, dim);
    }

    //Copiamos todas las soluciones en un puntero para no perderlas
    subp = new subproblema[n];
    for(int i = 0; i<n ; i++)
    {
    	subp[i] = subproblemas[i];
    }


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
}

void ejecucion(){

	//Se crea una tubería para acceder a GNUplot
	FILE *pipe2 = popen("gnuplot -persist","w");
	graficaIteracion1(pipe2);
	imprimeSoluciones(subp);

	for (int i = 0; i < g; i++, iter++)
	{
		cout<< "GENERACIÓN: " << i <<endl;
		operacionED();
		cout<<"z: "<<z.vector[0]<<","<<z.vector[1]<<endl;
		imprimeSoluciones(subp);
		//graficaIteracion(pipe2);
	}
	graficaIteracion(pipe2);



}

void operacionED()	//Mutación y cruce de evolución diferencial
{
	//----------------------------------------------CÁLCULOS PREVIOS Y GENERACIÓN DE LAS POSICIONES ALEATORIAS PARA LA MUTACIÓN----------------------------------------

	//Calcula cuantos vecinos tiene cada subproblema
	int num_vecinos = (int)((vecindad/100)*n);
	//Envía un aviso si no hay un suficiente número de vecinos
	if(num_vecinos < 3)
	{
		cout << "No existen suficientes vecinos para ejecutar la evolución diferencial(3), aumente el número de subproblemas o el porcentaje de vecindad";
		return;
	}

	//Genera 3 enteros aleatorios para poder escoger las 3 soluciones para la mutación
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

		//-----------------------------------------RECOGIDA DE LAS SOLUCIONES QUE HAN TOCADO ACTUAR EN LA MUTACIÓN Y MUTACIÓN----------------------------------------

		int tm[3] = {subp[i].grupo[pos[0]],subp[i].grupo[pos[1]],subp[i].grupo[pos[2]]};	//Va al grupo de vecinos y coge las posiciones aleatorias que nos han salido

		solucion m = subp[tm[0]].x + (subp[tm[1]].x - subp[tm[2]].x)*f;						//Función de mutación

		//mutacionGaussiana(&m);

		for(int j =0; j<m.dimensiones; j++)													//Limitación de los componentes del nuevo vector entre el límite superior e inferior
		{
			if(j == 0 && m.vector[j] < 0)													//Para j = 0  |  0 <= Xi <= 1
			{
				m.vector[j] = 0;
			}
			else if(j == 0 && m.vector[j] > 1)
			{
				m.vector[j] = 1;
			}

			if( j > 0 && m.vector[j] < -2)													//Para j > 0  |  -2 <= Xi <= 2
			{
				m.vector[j] = -2;
			}
			else if(j > 0 && m.vector[j] > 2)
			{
				m.vector[j] = 2;
			}
		}

		//-----------------------------------------------------CREACIÓN DE VARIABLES TEMPORALES, CRUCE Y ALMACENAMIENTO---------------------------------------------------

		solucion previa = subp[i].x;							//Solución original.

		float trial_vector[previa.dimensiones];					//Array para ir guardando los valores de la solución final.
		int delta = (int)rand()% previa.dimensiones;			//El valor delta del cruce, un entero aleatorio con rango 0 hasta el número de dimensiones del espacio de búsqueda.

		for (int j = 0; j < previa.dimensiones; ++j)			//Cruce según criterio.
		{
			float rng = (float)(rand()%99)/100; 				//Número aleatorio entre 0 y 1.

			if(rng<=cr||j == delta)
			{
				trial_vector[j] = m.vector[j];
			}
			else if(rng>cr||j != delta)
			{
				trial_vector[j] = previa.vector[j];
			}
		}

		solucion trial_solution = solucion(trial_vector, previa.dimensiones);							//Creación de la nueva solución con los valores cruzados.


		//----------------------------------------EVALUACIÓN DE LA NUEVA SOLUCIÓN, ACTUALIZACIÓN DE Z, Y EVALUACIÓN PARA LOS SUBPROBLEMAS VECINOS-----------------------

		//Comparación de la nueva solución con la anterior (Van dentro de un subproblema para que tchebycheff pueda coger su vector peso)
		if(tchebycheff(trial_solution, subp[i].id) < tchebycheff(previa, subp[i].id))
		{
			for(int g = 0; g < dim; g++)
			{
				subp[i].x.vector[g] = trial_solution.vector[g];	//Si lo mejora, sustituye la solución de este subproblema por la nueva mejorada
			}
		}

		//Comprobación de Z
		solucion temp = busquedaAobjetivo(trial_solution);	//Trasladamos la nueva solución al espacio de objetivos y la guardamos en temp
		if(temp.vector[0]<z.vector[0])						//Comprueba si mejora a la coordenada 'x' de z, si es así lo sustituye
		{
			float tempz[2] = {temp.vector[0], z.vector[1]};	//Almacenamiento previo en un array de la nueva X y la antigua Y de Z
			z = solucion(tempz,2);							//Construye y guarda la nueva solución Z, dado el array y las dimensiones del array
		}
		if(temp.vector[1]<z.vector[1])						//Comprueba si mejora a la coordenada 'y' de z, si es así lo sustituye
		{
			float tempz[2] = {z.vector[0], temp.vector[1]};	//Almacenamiento previo en un array de la antigua X y la nueva Y de Z
			z = solucion(tempz,2);							//Construye y guarda la nueva solución Z, dado el array y las dimensiones del array
		}


		//Comprueba si la solución encontrada también mejora a la de los vecinos, recorre todos los vecinos almacenados en el grupo de vecinos del subproblema.
		for (int j = 0; j < num_vecinos; j++)
		{
			int indice = subp[i].grupo[j];							//Posición del subproblema a comprobar, grupo es un puntero a el array de vecinos.
			if(tchebycheff(trial_solution, subp[indice].id) < tchebycheff(subp[indice].x, subp[indice].id))	//Comparación para saber si la nueva solución también mejora a la de ese vecino
			{
				for(int g = 0; g < dim; g++)						//Si lo mejora, su solución es sustituída por la nueva. Bucle para copiar todos los componentes
				{
					subp[indice].x.vector[g] = trial_solution.vector[g];
				}

			}
		}

	}
}

double tchebycheff(solucion soluc, peso peso)	//Dado un subproblema calcula la distancia de tchebycheff de su solución hasta la solución Z(La mejor ideal encontrada hasta ahora) para un peso.
{
	double componenteX = 0.0;		//Variable de retorno
	double componenteY = 0.0;		//Variable auxiliar
	solucion solucionObj;			//Variable auxiliar

	//A continuación trasladamos nuestra solución desde el espacio de búsqueda al espacio de objetivos.
	solucionObj = busquedaAobjetivo(soluc);

	//====PENALIZACIÓN sobre f1 y f2 si no cumplen las restricciones====
	float x1 = soluc.vector[0]; float x2 = soluc.vector[1]; float x4 = soluc.vector[3];
	float penalty[2];
	int num = soluc.dimensiones;
	float penalizacion=0.0;

	//Cálculo de las funciones que definen las restricciones: penalty debe ser >= 0
	penalty[0] = x2 - 0.8*x1*sin(6.0*M_PI*x1 + 2.0*M_PI/num) - SIGNO(0.5*(1.0-x1)-pow(1.0-x1,2))*sqrt(fabs(0.5*(1.0-x1)-pow(1.0-x1,2)));
	penalty[1] = x4 - 0.8*x1*sin(6.0*M_PI*x1 + 4.0*M_PI/num) - SIGNO(0.25*sqrt(1-x1)-0.5*(1.0-x1))*sqrt(fabs(0.25*sqrt(1-x1)-0.5*(1.0-x1)));

	//penalty[0] = x2-0.8*x1*sin(6.0*x1*M_PI+2.0*M_PI/num) - SIGNO((x1-0.5)*(1.0-x1)) * sqrt(fabs((x1-0.5)*(1.0-x1)));
	//penalty[1] = x4-0.8*x1*sin(6.0*x1*M_PI+4.0*M_PI/num) - SIGNO(0.25*sqrt(1-x1)-0.5*(1.0-x1)) * sqrt(fabs(0.25*sqrt(1-x1)-0.5*(1.0-x1)));

	//Si incumplen alguna restricción se les suma una penalización(donde iter = número de iteración actual) --> f(*) = f(*) + (-penalty1*-penalty2*iter*0.1)^2
	//Se barajan todas las posibilidades
	if(penalty[0] < 0 && penalty[1] < 0)
	{
		penalizacion =  (-penalty[0]-penalty[1]) * pow(intensidad*iter, 4);
	}
	else if(penalty[0] < 0)
	{
		penalizacion =  -penalty[0] * pow(intensidad*iter, 4);
	}
	else if(penalty[1] < 0)
	{
		penalizacion =  -penalty[1] * pow(intensidad*iter, 4);

	}

	solucionObj + penalizacion;

	//Cálculo de la función de tchebycheff para las dos dimensiones del espacio de objetivos
	componenteX = peso.vector[0] * abs(solucionObj.vector[0] - (z.vector[0] + compensacion_angular));	// lambda[0] * |f1 - z[0]|  coordenada x
	componenteY = peso.vector[1] * abs(solucionObj.vector[1] - z.vector[1]);							// lambda[1] * |f2 - z[1]|  coordenada y

	//Se devuelve el componente que sea mayor
	if(componenteX > componenteY)
	{
		return componenteX;
	}
	else
	{
		return componenteY;
	}
}

float cumpleRestricciones(solucion solu)
{
	float x1 = solu.vector[0]; float x2 = solu.vector[1]; float x4 = solu.vector[3];
	float penalty[2];
	int num = solu.dimensiones;
	penalty[0] = x2-0.8*x1*sin(6.0*x1*M_PI+2.0*M_PI/num) - SIGNO((x1-0.5)*(1.0-x1)) * sqrt(fabs((x1-0.5)*(1.0-x1)));
	penalty[1] = x4-0.8*x1*sin(6.0*x1*M_PI+4.0*M_PI/num) - SIGNO(0.25*sqrt(1-x1)-0.5*(1.0-x1)) * sqrt(fabs(0.25*sqrt(1-x1)-0.5*(1.0-x1)));

	//Si incumplen alguna restricción se les suma una penalización(donde iter = número de iteración actual) --> f(*) = f(*) + (-penalty1*-penalty2*iter*0.1)^2
	//Se barajan todas las posibilidades
	float pn = 0.0;
	if(penalty[0] < 0 && penalty[1] < 0)
	{
		pn =  -penalty[0]-penalty[1];
	}
	else if(penalty[0] < 0)
	{
		pn =  -penalty[0];
	}
	else if(penalty[1] < 0)
	{
		pn =  -penalty[1];

	}
	return pn;

}

void mutacionGaussiana(solucion * x){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<float> d(0, ((float)x[0].dimensiones-1.0)/20.0);

	for (int i = 0; i < x[0].dimensiones; ++i)
	{
		x[i] = x[i] + d(gen);
	}
}

void imprimeSolucion(solucion s)
{
	solucion a = s;

	a = busquedaAobjetivo(s);

	for(int b = 0; b < a.dimensiones; b++)
	{
		archivoOut << a.vector[b] << "\t";
	}
	archivoOut << cumpleRestricciones(s)<< endl;
}

void imprimeSoluciones(solucion * soluciones)
{
	for(int i = 0; i < n; i++)
	{
		if(soluciones[i].dimensiones!=0)
		{
			imprimeSolucion(soluciones[i]);
		}else break;
	}
}

void imprimeSoluciones(subproblema * subproblemas)
{
	for(int i = 0; i < n; i++)
	{
		if(subproblemas[i].x.dimensiones!=0)
		{
			imprimeSolucion(subproblemas[i].x);
		}else break;
	}
}


void graficaIteracion(FILE *pipe2)
{
	std::ofstream iteracion("iteracion.out", std::ofstream::trunc);
	for(int l = 0; l < n; l++){
		solucion th = busquedaAobjetivo(subp[l].x);
		for (int t = 0; t < 2; t++) {
			iteracion << th.vector[t]<<"\t";
		}
		iteracion << cumpleRestricciones(subp[l].x) << endl;
	}

	//Graficar
	fprintf(pipe2, "clear\n");
	fprintf(pipe2, "set palette model RGB defined ( 0 'coral', 1 'red', 2 'dark-red', 3 'blue' )\n");
	fprintf(pipe2, "replot 'PF.dat' linecolor rgb 'light-green' linetype 5 notitle\n");
	fprintf(pipe2, "replot 'aComparar.out' lt rgb 'gray20' pointtype 22 notitle\n");
	fprintf(pipe2, "replot 'iteracion.out' u 1:2:3 with points lt palette pointtype 13 notitle\n");


	fflush(pipe2);
	usleep(300000);
}

void graficaIteracion1(FILE *pipe2)
{
	std::ofstream iteracion("iteracion.out", std::ofstream::trunc);
	for(int l = 0; l < n; l++){
		solucion th = busquedaAobjetivo(subp[l].x);
		for (int t = 0; t < 2; t++) {
			iteracion << th.vector[t]<<"\t";
		}
		iteracion << cumpleRestricciones(subp[l].x) << endl;
	}

	//Ajustes
	fprintf(pipe2, "set palette gray negative\n");
	fprintf(pipe2, "set title 'Iteracion'\n");

	//Graficar
	fprintf(pipe2, "set palette model RGB defined ( 0 'coral', 1 'red', 2 'dark-red', 3 'blue' )\n");
	//fprintf(pipe2, "unset autoscale\n");
	//fprintf(pipe2, "set cbrange[0:0.1]\n");
	fprintf(pipe2, "set cbrange[0 < * < 1000]\n");
	fprintf(pipe2, "set grid\n");
	fprintf(pipe2, "plot [0:1][0:1] 'iteracion.out' u 1:2:3 with points lt palette pointtype 13\n");

	fflush(pipe2);
	usleep(300000);
}
