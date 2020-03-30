#include <math.h>
#include <algorithm>
#include <array>
#include <cmath>



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
    solucion(const solucion *a);
    solucion(float vectorp[],int p);						//Constructor dado un vector
    solucion(int max, int min, int dimensiones);	//Constructor de una solución aleatoria
    solucion operator - (solucion const &obj);		//Operador resta
    solucion operator + (solucion const &obj);		//Operador suma
    solucion operator * (float const &obj);			//Operador multiplicación escalar
};

class subproblema
{
    public:
    peso id; 					//Peso al que pertenece
    int *grupo; 				//Grupo de pesos vecinos
    solucion x;					//Solución asociada a este subproblema
    subproblema();
    subproblema(peso id, int *grupo, int num_vecinos, solucion x);
    subproblema(int n, peso p, peso pesos[], float porcentaje_vecinos, int dimensiones, int max, int min);

};

float distanciaEuclidea(solucion s1, solucion s2); //Distancia euclidea entre dos soluciones
float distanciaEuclidea(solucion s);				//Distancia euclidea entre una solucion y el centro


