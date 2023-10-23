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
        this_thread::sleep_for(chrono::seconds(5));
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



void roundRobin(Nodo*& frente, Nodo*& fin) {
    while (true) {
        if (frente != nullptr) {
            Nodo* procesoActual = frente;
            // Realiza el despacho del proceso actual (simulado)
            cout << "Proceso en ejecución: " << procesoActual->dato << "(" << procesoActual->contador << ")" << endl;
            this_thread::sleep_for(chrono::seconds(1));  // Simula el tiempo de ejecución
            // Decrementa el contador del proceso y mueve al final de la cola si es necesario
            procesoActual->contador -= 1;
            if (procesoActual->contador < 0) {
                procesoActual->contador = 0;
            }
              // Simula el tiempo de ejecución
            // Mueve el proceso al final de la cola
            else{
            frente = frente->siguiente;
            insertarCola(frente, fin, procesoActual->dato, procesoActual->prioridad, procesoActual->contador);
            }
        }
        this_thread::sleep_for(chrono::seconds(1));  // Intervalo entre procesos
    }
}

void shortestJobFirst(Nodo*& frente, Nodo*& fin) {
    while (true) {
        if (frente != nullptr) {
            Nodo* procesoActual = frente;
            Nodo* procesoMasCorto = frente;
            Nodo* anterior = NULL;
            Nodo* anteriorProcesoMasCorto = NULL;

            // Search for the process with the shortest execution time
            while (procesoActual != NULL) {
                if (procesoActual->contador < procesoMasCorto->contador) {
                    procesoMasCorto = procesoActual;
                    anteriorProcesoMasCorto = anterior;
                }
                anterior = procesoActual;
                procesoActual = procesoActual->siguiente;
            }

            // Move the shortest job to the front of the queue
            if (anteriorProcesoMasCorto != nullptr) {
                anteriorProcesoMasCorto->siguiente = procesoMasCorto->siguiente;
                procesoMasCorto->siguiente = frente;
                frente = procesoMasCorto;
            }

            // Simulate the execution of the process
            cout << "Proceso en ejecución: " << frente->dato << "(" << frente->contador << ")" << endl;
            this_thread::sleep_for(chrono::seconds(1));  // Simulate execution time
            frente->contador -= 1;
            if (frente->contador < 0) {
                frente->contador = 0;
            }

            // If the process hasn't completed its execution, move it to the end of the queue
            if (frente->contador > 0) {
                if (anteriorProcesoMasCorto != NULL) {
                    anteriorProcesoMasCorto->siguiente = frente->siguiente;
                    frente->siguiente = NULL;
                    fin->siguiente = frente;
                    fin = frente;
                    frente = procesoMasCorto;
                }
            }
        }
        this_thread::sleep_for(chrono::seconds(1));  // Interval between processes
    }
}



int main() {
    int opcion;
    cout << "\n\tSeleccion de algoritmo de despacho" << endl;
    cout << "1.Cola con prioridades" << endl;
    cout << "2. Round Robin" << endl;
    cout << "3. Shortest Job First" << endl;
    cout << "Ingrese una opcion: ";
    cin >> opcion;
    Nodo* frente = NULL;
    Nodo *fin = NULL;
    char dato;
    int prioridad, contador;
    thread generador;
    thread decrementador;
    thread moverNodos;
    thread supresor;
 
    switch (opcion)
    {
        case 1:
        {
            Nodo* frente = NULL;
            Nodo *fin = NULL;
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
            break;
        }
        case 2:
        {
            Nodo* frente = NULL;
            Nodo *fin = NULL;
            char dato;
            int prioridad, contador;
            thread generador(generarElementos, ref(frente), ref(fin));
            thread rr(roundRobin, ref(frente), ref(fin));
            thread supresor(suprimirColaEnHilo, ref(frente), ref(fin), ref(dato), ref(prioridad), ref(contador));
            generador.join();
            rr.join();
            supresor.join();
            break;
        }
        case 3:
            Nodo* frente = NULL;
            Nodo *fin = NULL;
            char dato;
            int prioridad, contador;
            thread generador(generarElementos, ref(frente), ref(fin));
            thread sjf(shortestJobFirst, ref(frente), ref(fin));
            thread supresor(suprimirColaEnHilo, ref(frente), ref(fin), ref(dato), ref(prioridad), ref(contador));
            generador.join();
            sjf.join();
            supresor.join();
            break;
    }

    return 0;
}