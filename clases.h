#include <math.h>
#include <algorithm>
#include <array>



class peso
{
    public:
    float vector[2]; //Posición del peso
    peso(); //Constructor peso, inicializa a 0
    peso(float p0, float p1); //Constructor dado dos componentes del vector
};

class solucion
{
    public:
	float *vector; //Posición de la solución en el espacio
    solucion();
    solucion(float vectorp[]);
};

class subproblema
{
    public:
    peso id; //Peso al que pertenece
    int *grupo; //Grupo de pesos vecinos
    subproblema();
    subproblema(int n, peso p, peso pesos[], float porcentaje_vecinos);

};
