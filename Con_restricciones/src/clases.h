#include <stdio.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <math.h>



class peso
{
    public:
    float vector[2]; 			//Posición del peso
    peso(); 					//Constructor peso, inicializa a 0
    peso(float p0, float p1); 	//Constructor dado dos componentes del vector
};

class solucion
{
    public:
	float *vector; 									//Posición de la solución en el espacio
	int dimensiones;
    solucion();
    solucion(solucion *a);
    solucion(float vectorp[],int p);				//Constructor dado un vector y las dimensiones del mismo
    solucion(int dimensiones);	//Constructor de una solución aleatoria
    solucion operator - (solucion obj);				//Operador resta
    solucion operator + (solucion obj);				//Operador suma
    solucion operator + (float obj);				//Operador suma
    solucion operator * (float obj);				//Operador multiplicación escalar
    int domina(solucion s);							//Devuelve 1 si domina a la solución, 0 si no la domina y -1 si no corresponden las dimensiones
};

class subproblema
{
    public:
    peso id; 					//Peso al que pertenece
    int *grupo; 				//Grupo de pesos vecinos
    solucion x;					//Solución asociada a este subproblema
    subproblema();
    subproblema(peso id, int *grupo, int num_vecinos, solucion x);
    subproblema(int n, peso p, peso pesos[], float porcentaje_vecinos, int dimensiones);

};

float distanciaEuclidea(solucion s1, solucion s2);	//Distancia euclidea entre dos soluciones
float distanciaEuclidea(solucion s);				//Distancia euclidea entre una solucion y el centro
float floatAleatorio(int max, int min);				//Generador de número aleatorio en un rango
solucion busquedaAobjetivo(solucion busq);			//Transforma una solución del espacio de búsqueda al de objetivos
solucion * busquedaAobjetivo(solucion * busq, int longitud);

