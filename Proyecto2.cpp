#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
using namespace std;

struct Nodo {
    char dato;
    int prioridad;
    int contador;
    Nodo* siguiente;
};

void insertarCola(Nodo*& frente, Nodo*& fin, char dato, int prioridad, int contador) {
    Nodo* nuevo_nodo = new Nodo{ dato, prioridad, contador, NULL };
    if (frente == NULL) {
        frente = nuevo_nodo;
    }
    else {
        fin->siguiente = nuevo_nodo;
    }
    fin = nuevo_nodo;
}

void suprimirCola(Nodo*& frente, Nodo*& fin, char& dato, int& prioridad, int& contador) {
    Nodo* aux = frente;
    dato = aux->dato;
    prioridad = aux->prioridad;
    contador = aux->contador;
    frente = frente->siguiente;
    if (frente == NULL) {
        fin = NULL;
    }
    delete aux;
}

void mostrarCola(Nodo* frente) {
    cout << "\nMostrando los elementos de la cola: ";
    while (frente != NULL) {
        cout << frente->dato << "(" << frente->contador << ") ";
        frente = frente->siguiente;
    }
    cout << endl;
}

void generarElementos(Nodo*& frente, Nodo*& fin) {
    int i = 0;
    while (true) {
        char dato = 'A' + i;
        int prioridad = rand() % 3 + 1;
        int contador = rand() % 81 + 20;
        insertarCola(frente, fin, dato, prioridad, contador);
        mostrarCola(frente);
        this_thread::sleep_for(chrono::seconds(1));
        i++;
    }
}

void decrementarContador(Nodo*& frente) {
    while (true) {
        Nodo* aux = frente;
        while (aux != NULL) {
            aux->contador -= rand() % 5 + 1;
            if (aux->contador < 0) {
                aux->contador = 0;
            }
            aux = aux->siguiente;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    Nodo* frente = NULL;
    Nodo* fin = NULL;
    thread generador(generarElementos, ref(frente), ref(fin));
    thread decrementador(decrementarContador, ref(frente));
    generador.join();
    decrementador.join();
    return 0;
}