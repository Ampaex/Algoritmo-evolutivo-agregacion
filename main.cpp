#include <iostream>
#include <vector>
#include <string>

using namespace std;

int N; //Número de subproblemas
int G; //Número de generaciones

int opEv;//Operador evolutivo


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
    
    cout << "=AJUSTES DE EVOLUCIÓN=\n"<<endl;
    cout << "Tipos de recombinación:"<<endl;
    cout << "1: Recombinación aritmétrica \n2: Recombinación binaria[en desarrollo]\n"<<endl;
    cout << "Indique el número del tipo deseado:";
    cin >> opEv;
    if(opEv<0 || opEv>=2){ cout << "El tipo especificado no existe"<<endl; return 1;};//Comprueba si es válido el tipo introducido
    cout << "Ha seleccionado la recombinación tipo " << opEv<<"\n\n";



    return 0;
}
