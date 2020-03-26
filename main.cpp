#include <iostream>
#include <vector>
#include <string>
#include "clases.h"

using namespace std;

static int n; //Número de subproblemas
int g; //Número de generaciones
int dim; //Número de dimensiones

float f; //Parámetro de mutación
float cr; //Parámetro de cruce
float vecindad;
float ls; //Límite superior
float li; //Límite inferior

void parametrosDeEntrada();
void inicializacion();

//Código principal
int main()
{
    inicializacion();
    parametrosDeEntrada();


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
}

void inicializacion()
{
    //Inicialización de los vectores peso
    peso pesos[n];
    float paso = (float)1/(float)(n-1);
    for(int i =0; i<n ;i++)
    {
        pesos[i] = peso(1-paso*i,0+paso*i);
    }
}