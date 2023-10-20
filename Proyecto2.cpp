#include<iostream>
#include<thread>
#include<chrono>
#include<stdlib.h>
using namespace std;

struct Nodo{
    char dato;
    int prioridad;
    Nodo *siguiente;
};

void insertarCola(Nodo *&,Nodo *&,char,int);
bool cola_vacia(Nodo *);
void suprimirCola(Nodo *&,Nodo *&,char &,int &);
void generarElementos(Nodo *&, Nodo *&, bool &);
void mostrarCola(Nodo *);

int main(){
    Nodo *frente = NULL;
    Nodo *fin = NULL;
    bool mostrar = false;

    int n = 15; // number of elements to add
    int i = 0; // counter for loop

    while (i < n) {
        char dato = 'A' + i; // generate a character to add to the queue
        int prioridad = rand() % 3 + 1; // generate a random priority
        insertarCola(frente, fin, dato, prioridad); // insert the element into the queue
        i++; // increment the counter
    }

    thread generador(generarElementos, ref(frente), ref(fin), ref(mostrar)); // Crear un hilo para generar elementos

    while (true) {
        if (mostrar) {
            mostrarCola(frente); // mostrar la cola si se ha agregado un elemento
            mostrar = false; // resetear la variable
        }
    }

    generador.join(); // Esperar a que el hilo termine
}

void insertarCola(Nodo *&frente, Nodo *&fin, char dato, int prioridad){
    Nodo *nuevo_nodo = new Nodo{dato, prioridad, NULL};

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

void suprimirCola(Nodo *&frente, Nodo *&fin, char &dato, int &prioridad){
    dato = frente->dato;
    prioridad = frente->prioridad;
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
        insertarCola(frente, fin, dato, prioridad); // insertar el elemento en la cola
        i++; // incrementar el contador
        mostrar = true; // indicar que se debe mostrar la cola
        this_thread::sleep_for(chrono::seconds(1)); // esperar un segundo antes de generar el siguiente elemento
    }
}

void mostrarCola(Nodo *frente) {
    cout << "\nMostrando los elementos de la cola: ";
    while (frente != NULL)
    {
        cout << frente->dato << " ";
        frente = frente->siguiente;
    }
    cout << endl;
}