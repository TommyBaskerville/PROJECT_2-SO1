#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
#include <condition_variable>

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

void procesarElementos(queue<Nodo>& cola, mutex& mtx, condition_variable& cv) {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&cola] { return !cola.empty(); });
        Nodo nodo = cola.front();
        cola.pop();
        lock.unlock();
        suprimirCola(nodo.dato, nodo.prioridad, nodo.contador);
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void generarElementos(queue<Nodo>& cola, mutex& mtx, condition_variable& cv) {
    int i = 0;
    while (true) {
        char dato = 'A' + i;
        int prioridad = rand() % 3 + 1;
        int contador = rand() % 81 + 20;
        Nodo nodo{ dato, prioridad, contador, NULL };
        {
            lock_guard<mutex> lock(mtx);
            cola.push(nodo);
        }
        mostrarCola(cola.front());
        cv.notify_one();
        this_thread::sleep_for(chrono::seconds(1));
        i++;
    }
}

int main() {
    queue<Nodo> cola;
    mutex mtx;
    condition_variable cv;
    thread hilo_procesar(procesarElementos, ref(cola), ref(mtx), ref(cv));
    thread hilo_generar(generarElementos, ref(cola), ref(mtx), ref(cv));
    hilo_procesar.join();
    hilo_generar.join();
    return 0;
}