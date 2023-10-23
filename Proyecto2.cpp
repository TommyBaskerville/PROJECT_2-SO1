#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <mutex>
using namespace std;
mutex colaMutex;


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
    if (frente != NULL) {
        Nodo* temp = frente;
        dato = frente->dato;
        prioridad = frente->prioridad;
        contador = frente->contador;
        frente = frente->siguiente;
        if (frente == NULL) {
            fin = NULL;
        }
        delete temp;
    }
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


void suprimirColaEnHilo(Nodo*& frente, Nodo*& fin, char& dato, int& prioridad, int& contador) {
    while (true) {
        colaMutex.lock();
        if (frente != nullptr && frente->contador == 0) {
            suprimirCola(frente, fin, dato, prioridad, contador);
            cout << "Elemento eliminado: " << dato << "(" << contador << ")" << endl;
        }
        colaMutex.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }
}


void moverNodosAlPrincipio(Nodo*& frente) {
    while (true) {
        Nodo* aux = frente;
        Nodo* anterior = NULL;
        
        while (aux != NULL) {
            if (aux->contador == 0) {
                // Mover el nodo al principio de la cola
                if (anterior != NULL) {
                    anterior->siguiente = aux->siguiente;
                    aux->siguiente = frente;
                    frente = aux;
                    aux = anterior->siguiente;
                }
            } else {
                anterior = aux;
                aux = aux->siguiente;
            }
        }

        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    Nodo* frente = NULL;
    Nodo* fin = NULL;
    char dato;
    int prioridad, contador;
    thread generador(generarElementos, ref(frente), ref(fin));
    thread decrementador(decrementarContador, ref(frente));
    thread moverNodos(moverNodosAlPrincipio, ref(frente));
    thread supresor(suprimirColaEnHilo, ref(frente), ref(fin), ref(dato), ref(prioridad), ref(contador));
    generador.join();
    decrementador.join();
    moverNodos.join();
    supresor.join();
    return 0;
}