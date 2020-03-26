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
    float posicion; //Posición de la solución en el espacio

};

class subprob 
{
public:
    int n; //Número de subproblemas total
    peso grupo; //Peso al que pertenece
    solucion z; //Mejor solución
    int b[];
    subprob(int _n);

};