#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include "clases.h"
#include <unistd.h>

using namespace std;


int n; 					//Número de subproblemas
int g; 					//Número de generaciones
int dim; 					//Número de dimensiones

float f; 				//Parámetro de mutación
float cr; 				//Parámetro de cruce

float vecindad; 				//Porcentaje de vecinos de cada subproblema

float ls; 					//Límite superior
float li; 					//Límite inferior

float semilla;

float compensacion_angular;

subproblema * subp;				//Puntero al vector de subproblemas (tamaño n)

solucion z;							//Mejor solución ideal encontrada
subproblema * mejoresSubproblemas;	//Mejores subproblemas encontrados

ofstream archivoOut;			//Declaración de archivo de salida con las soluciones de todas las iteraciones
ofstream archivoBest;			//Declaración del archivo de salida con las mejores soluciones
ofstream archivoZ;				//Declaración del archivo de salida con las mejores soluciones
ofstream iteracion;

void parametrosDeEntrada();
void inicializacion(int n, int dim, int max, int min);
void operacionED();
void ejecucion();
double tchebycheff(solucion soluc, peso peso);							//Devuelve la distancia de tchebycheff
void imprimeSolucion(solucion s);								//Escribe en el archivo de salida la solución(Si no es de objetivo lo convierte)
void imprimeNoDominadas(subproblema *subproblemas);				//Imprime el conjunto de soluciones no dominadas
void imprimeSoluciones(solucion * soluciones);					//Imprime un conjunto de soluciones
void imprimeSoluciones(subproblema * subproblemas);				//Imprime las soluciones de un conjunto de problemas
void compruebaMejoresSubproblemas();							//Actualiza la lista de mejores subproblemas
void graficaGNUplot();											//Grafica nuestras soluciones finales no dominadas
void graficaSalida();
void graficaIteracion(FILE *pipe2);
void graficaIteracion1(FILE *pipe2);



//Código principal
int main()
{
	archivoOut.open("salida.out");
	archivoZ.open("salidaZ.out");
	archivoBest.open("salidaTOP.out");
	iteracion.open("iteracion.out");

	parametrosDeEntrada();			//Recogida de parámetros de entrada
	srand(semilla);					//Inicialización de los números aleatorios con semilla
	inicializacion(n, dim, ls, li);	//Inicialización del problema
	ejecucion();
	imprimeNoDominadas(mejoresSubproblemas);
	graficaGNUplot();



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

    cout << "Compensación angular:";
	cin >> compensacion_angular;
	cout << "Ha seleccionado " << compensacion_angular << " como compensación angular.\n\n";

}

void inicializacion(int n, int dim, int max, int min)
{
    peso pesos[n]; 								//Inicialización de los vectores peso
    mejoresSubproblemas = new subproblema[n];	//Reserva de memoria para el puntero a mejoresSubproblemas
    float paso = (float)1/(float)(n-1);
    for(int i =0; i<n ;i++)
    {
        //pesos[i] = peso(1-paso*i,0+paso*i);
    	pesos[i] = peso(0+paso*i,1-paso*i);
    }

    /*[DEBUG] Muestra todos los pesos
    salidaDebug << "EJECUCIÓN CON 30 SUBPROBLEMAS 100 GENERACIONES Y 30 DIMENSIONES / F = 0.5 /CR = 0.5 /VECINDAD = 20%"<<endl;
    salidaDebug<<"===INICIALIZACIÓN==="<<endl;
    salidaDebug << "===PESOS==="<<endl;
    for (int i = 0; i < n; i++)
    {
		for (int j = 0; j < 2; ++j)
		{
			salidaDebug << pesos[i].vector[j] << "\t";
		}
		salidaDebug << endl;
	}*/


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
    salidaDebug << "===SUBPROBLEMAS==="<< endl << "VALORES ALEATORIOS PARA ESPACIO DE BÚSQUEDA" << endl;
    for (int i = 0; i < n; ++i)
    {
    	salidaDebug << "Subproblema "<<i<<": "<<endl;
		for (int j = 0; j < dim; ++j)
		{
			cout << subproblemas[i].x.vector[j] << ",";
			salidaDebug << subproblemas[i].x.vector[j] << "\t";
		}
		cout<<endl;
		salidaDebug<<endl<<endl;
	}*/

    int n_vecinos = (int)((vecindad/100)*n);
    /*[DEBUG]IMPRIME TODOS LOS VECINOS
    salidaDebug << "===VECINOS==="<< endl << "HACE REFERENCIA A SU POSICIÓN EN EL ARRAY" << endl << "DESE CUENTA DE QUE NO SALEN ORDENADOS"<<endl;
	for (int i = 0; i < n; ++i)
	{
		salidaDebug << "Subproblema "<<i<<": "<<endl;
		for (int j = 0; j < n_vecinos; ++j)
		{
			salidaDebug << subproblemas[i].grupo[j] << "\t";
		}
		salidaDebug<<endl;
	}*/
    /*[DEBUG]IMPRIME LAS SOLUCIONES DE TODOS LOS SUBPROBLEMAS OBJETIVO
	for (int i = 0; i < n; ++i)
	{
		solucion s = busquedaAobjetivo(subproblemas[i].x);
		cout << "Subproblema " << i << ":";
		for (int j = 0; j < dim; ++j)
		{
			cout << s.vector[j] << ",";
			archivoOut << s.vector[j] << "\t";
		}
		cout<<endl;
		archivoOut<<endl;
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

   /*[DEBUG] IMPRIME EL VECTOR Z
    salidaDebug << endl << "Solución Z primera iteración: " << z.vector[0] << "\t" << z.vector[1]<< endl << endl;
    graficaSalida();*/


}

void ejecucion(){

	//Se crea una tubería para acceder a GNUplot
	FILE *pipe2 = popen("gnuplot -persist","w");

	imprimeSoluciones(subp);
	graficaIteracion1(pipe2);
	compruebaMejoresSubproblemas();
	//salidaDebug << "=======EJECUCIÓN DEL PROGRAMA=======" <<endl;
	for (int i = 0; i < g; i++)
	{
		cout<< "GENERACIÓN: " << i <<endl;
		//salidaDebug<< "GENERACIÓN: " << i <<endl;
		operacionED();
		cout<<"z: "<<z.vector[0]<<","<<z.vector[1]<<endl;
		imprimeSoluciones(subp);
		compruebaMejoresSubproblemas();
		//graficaIteracion(pipe2);
		/*[DEBUG] IMPRIME SOLUCIONES DE TODOS LOS SUBPROBLEMAS
		salidaDebug << "===SUBPROBLEMAS==="<< endl;
		for (int i = 0; i < n; ++i)
		{
			cout << "Subproblema "<<i<<": "<<endl;
			//salidaDebug << "Subproblema "<<i<<": "<<endl;
			for (int j = 0; j < dim; ++j)
			{
				cout << subp[i].x.vector[j] << ",";
				//salidaDebug << subp[i].x.vector[j] << "\t";
			}
			cout<<endl;
			//salidaDebug<<endl<<endl;
		}*/
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

		for(int j =0; j<m.dimensiones; j++)													//Limitación de los componentes del nuevo vector entre el límite superior e inferior
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
	/*//[DEBUG]Imprime todas las soluciones
	for(int i = 0; i < n; i++)
	{
		cout<< "Sub "<<i<<": ";
		for(int j = 0;j < 2;j++)
		{
			cout << busquedaAobjetivo(subp[i].x).vector[j] << ",";
		}
		cout << endl;
	}*/
}

double tchebycheff(solucion soluc, peso peso)	//Dado un subproblema calcula la distancia de tchebycheff de su solución hasta la solución Z(La mejor ideal encontrada hasta ahora) para un peso.
{
	double componenteX = 0.0;	//Variable de retorno
	double componenteY = 0.0;	//Variable auxiliar
	solucion solucionObj;			//Variable auxiliar

	//A continuación trasladamos nuestra solución desde el espacio de búsqueda al espacio de objetivos.
	solucionObj = busquedaAobjetivo(soluc);

	//Cálculo de la función de tchebycheff para las dos dimensiones del espacio de objetivos
	componenteX = peso.vector[0] * abs(solucionObj.vector[0] - (z.vector[0] + compensacion_angular));	// lambda[0] * |f1 - z[0]|  coordenada x
	componenteY = peso.vector[1] * abs(solucionObj.vector[1] - z.vector[1]);	// lambda[1] * |f2 - z[1]|  coordenada y

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

void imprimeSolucion(solucion s)
{
	solucion a = s;

	a = busquedaAobjetivo(s);

	for(int b = 0; b < a.dimensiones; b++)
	{
		archivoOut << a.vector[b] << "\t";
		//salidaDebug << a.vector[b] << "\t";
	}
	archivoOut << endl;
	//salidaDebug << endl;
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

void compruebaMejoresSubproblemas()
{
	if(mejoresSubproblemas[0].x.dimensiones==0)
	{
		for (int i = 0; i < n; i++) mejoresSubproblemas[i] = subp[i];
		return;
	}
	for (int i = 0; i < n; i++)
	{
		if(tchebycheff(subp[i].x, subp[i].id) < tchebycheff(mejoresSubproblemas[i].x, mejoresSubproblemas[i].id))
		{
			mejoresSubproblemas[i] = subp[i];
		}
	}
}

void graficaGNUplot()
{
	//Se crea una tubería para acceder a GNUplot
	FILE *pipe = popen("gnuplot -persist","w");

	//Ajustes
	fprintf(pipe, "set palette gray negative\n");
	fprintf(pipe, "set title 'Mejores resultados del algoritmo'\n");

	//Graficar
	fprintf(pipe, "plot [0:1][-1:2] 'salidaTOP.out'\n");
	fflush(pipe);
}

void graficaSalida()
{
	archivoZ << z.vector[0] << "\t" << z.vector[1]<<endl;	//Guarda Z
	//Se crea una tubería para acceder a GNUplot
	FILE *pipe = popen("gnuplot -persist","w");

	//Ajustes
	fprintf(pipe, "set palette gray negative\n");
	fprintf(pipe, "set title 'Salida.out'\n");

	//Graficar
	fprintf(pipe, "plot 'salida.out'\n");
    fprintf(pipe, "replot 'salidaZ.out' lt rgb 'red'\n");
	fflush(pipe);
}

void graficaIteracion(FILE *pipe2)
{
	std::ofstream iteracion("iteracion.out", std::ofstream::trunc);
	for(int l = 0; l < n; l++){
		solucion th = busquedaAobjetivo(subp[l].x);
		for (int t = 0; t < 2; t++) {
			iteracion << th.vector[t]<<"\t";
		}
		iteracion << endl;
	}

	//Graficar
	fprintf(pipe2, "clear\n");
	fprintf(pipe2, "replot 'iteracion.out' lt rgb 'blueviolet' pointtype 13 notitle\n");
	fprintf(pipe2, "replot 'aComparar.out' lt rgb 'coral' pointtype 22 notitle\n");

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
		iteracion << endl;
	}

	//Ajustes
	fprintf(pipe2, "set palette gray negative\n");
	fprintf(pipe2, "set title 'Iteracion'\n");

	//Graficar
	fprintf(pipe2, "plot 'iteracion.out'\n");

	fflush(pipe2);
	usleep(300000);
}

