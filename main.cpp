#include <iostream>
#include <vector>
#include <string>

using namespace std;

int N; //Número de subproblemas
int G; //Número de generaciones

float alpha; //Parámetro de recombinación
int recomb; //Número de valores a recombinar
int mutar; //Número de valores a mutar

int main()
{
    
    //Recogida de parámetros de entrada
    cout << "=====BIENVENIDO AL ALGORITMO EVOLUTIVO MULTIOBJETIVO BASADO EN AGREGACIÓN2===== \n \n";
    cout << "Indica el NÚMERO DE SUBPROBLEMAS: ";
    cin >> N;
    cout << "Ha seleccionado " << N << " subproblemas.\n\n";

    cout << "Indica el NÚMERO DE GENERACIONES: ";
    cin >> G;
    cout << "Ha seleccionado " << G << " generaciones.\n\n";
    
    cout << "=AJUSTES DE EVOLUCIÓN=\n" << endl;

    cout << "-Recombinación aritmétrica-" << endl;
    cout << "Seleccione un valor alpha (a*x - (a-1)*y):";
    cin >> alpha;
    cout << "¿Cuántos valores se recombinan?";
    cin >> recomb;

    cout << "-Mutación-" << endl;
    cout << "¿Cuantos valores han de mutar?(aleatoriamente)";
    cin >> mutar;


    return 0;
}
