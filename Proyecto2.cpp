#include<iostream>
#include<thread>
#include<chrono>
#include<stdlib.h>
using namespace std;

struct Nodo{
    char dato;
    int prioridad;
    int contador; // nuevo campo contador
    Nodo *siguiente;
};

void menu();
void insertarCola(Nodo *&,Nodo *&,char,int,int); // agregar el nuevo parámetro contador
bool cola_vacia(Nodo *);
void suprimirCola(Nodo *&,Nodo *&,char &,int &,int &); // agregar el nuevo parámetro contador
void generarElementos(Nodo *&, Nodo *&, bool &);
void mostrarCola(Nodo *);
void borrarElemento(Nodo *&, Nodo *&);
void borrarElementoPorContador(Nodo *&, Nodo *&);
void decrementarContador(Nodo *&);

int main(){
    Nodo *frente = NULL;
    Nodo *fin = NULL;
    bool mostrar = false;

    thread generador(generarElementos, ref(frente), ref(fin), ref(mostrar)); // Crear un hilo para generar elementos

    thread decrementador(decrementarContador, ref(frente)); // Crear un hilo para decrementar el contador de los elementos

    while (true) {
        if (mostrar) {
            mostrarCola(frente); // mostrar la cola si se ha agregado un elemento
            mostrar = false; // resetear la variable
        }
        if (!cola_vacia(frente)) {
            if (frente->contador == 0) {
                thread borrador(borrarElementoPorContador, ref(frente), ref(fin)); // Crear un hilo para borrar el primer elemento de la cola si su contador es 0
                borrador.detach(); // Desconectar el hilo para que se ejecute en segundo plano
            }
        }
    }

    generador.join(); // Esperar a que el hilo de generación de elementos termine
    decrementador.join(); // Esperar a que el hilo de decremento de contadores termine
}

void insertarCola(Nodo *&frente, Nodo *&fin, char dato, int prioridad, int contador){ // agregar el nuevo parámetro contador
    Nodo *nuevo_nodo = new Nodo{dato, prioridad, contador, NULL}; // asignar el valor del contador al nuevo nodo

    if (cola_vacia(frente))
    {
        frente = nuevo_nodo;
        fin = nuevo_nodo;
    }
    else if (prioridad > frente->prioridad)
    {
        nuevo_nodo->siguiente = frente;
        frente = nuevo_nodo;
    }
    else
    {
        Nodo *aux = frente;
        while (aux->siguiente != NULL && prioridad <= aux->siguiente->prioridad)
        {
            aux = aux->siguiente;
        }
        nuevo_nodo->siguiente = aux->siguiente;
        aux->siguiente = nuevo_nodo;
        if (nuevo_nodo->siguiente == NULL)
        {
            fin = nuevo_nodo;
        }
    }
}

void suprimirCola(Nodo *&frente, Nodo *&fin, char &dato, int &prioridad, int &contador){ // agregar el nuevo parámetro contador
    dato = frente->dato;
    prioridad = frente->prioridad;
    contador = frente->contador; // asignar el valor del contador a la variable de salida
    if (frente == fin)
    {
        frente = NULL;
        fin = NULL;
    }
    else
    {
        frente = frente->siguiente;
    }
    delete frente;
}

bool cola_vacia(Nodo *frente){
    return (frente == NULL);
}

void generarElementos(Nodo *&frente, Nodo *&fin, bool &mostrar){
    int i = 0;
    while (true) {
        char dato = 'A' + i; // generar un nuevo elemento
        int prioridad = rand() % 3 + 1; // generar una prioridad aleatoria
        int contador = rand() % 81 + 20; // generar un contador aleatorio entre 20 y 100
        insertarCola(frente, fin, dato, prioridad, contador); // insertar el elemento en la cola
        i++; // incrementar el contador
        mostrar = true; // indicar que se debe mostrar la cola
        this_thread::sleep_for(chrono::seconds(1)); // esperar un segundo antes de generar el siguiente elemento
    }
}

void mostrarCola(Nodo *frente) {
    cout << "\nMostrando los elementos de la cola: ";
    while (frente != NULL)
    {
        cout << frente->dato << "(" << frente->contador << ") "; // mostrar el valor del contador junto con el elemento
        frente = frente->siguiente;
    }
    cout << endl;
}

void borrarElemento(Nodo *&frente, Nodo *&fin) {
    char dato;
    int prioridad, contador;
    suprimirCola(frente, fin, dato, prioridad, contador); // borrar el primer elemento de la cola
    cout << "Se ha borrado el elemento " << dato << "(" << contador << ")" << endl; // mostrar el elemento que se ha borrado
}

void borrarElementoPorContador(Nodo *&frente, Nodo *&fin) {
    char dato;
    int prioridad, contador;
    suprimirCola(frente, fin, dato, prioridad, contador); // borrar el primer elemento de la cola
    cout << "Se ha borrado el elemento " << dato << "(" << contador << ")" << " porque su contador ha llegado a 0" << endl; // mostrar el elemento que se ha borrado
}

void decrementarContador(Nodo *&frente) {
    while (true) {
        Nodo *aux = frente;
        while (aux != NULL) {
            aux->contador -= rand() % 5 + 1; // decrementar el contador de cada elemento en un valor aleatorio entre 1 y 5
            aux = aux->siguiente;
        }
        this_thread::sleep_for(chrono::seconds(1)); // esperar un segundo antes de volver a decrementar los contadores
    }
}