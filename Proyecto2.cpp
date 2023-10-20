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

void menu();
void insertarCola(Nodo *&,Nodo *&,char,int);
bool cola_vacia(Nodo *);
void suprimirCola(Nodo *&,Nodo *&,char &,int &);
void generarElementos(Nodo *&, Nodo *&);

int main(){
    int contador = rand() % 81 + 20; // Generar un número aleatorio entre 20 y 100
    cout << "El contador es: " << contador << endl;
    menu();
    return 0;
}

void menu(){
    int opc, prioridad;
    char dato;

    Nodo *frente = NULL;
    Nodo *fin = NULL;

    thread generador(generarElementos, ref(frente), ref(fin)); // Crear un hilo para generar elementos

    do{
        system("clear"); // Limpiar la pantalla en Linux
        cout<<"\t.:Menu:.\n";
        cout<<"1. Insertar un caracter en la cola"<<endl;
        cout<<"2. Mostrar elementos de una cola "<<endl;
        cout<<"3. Salir"<<endl;
        cout<<"Opcion: ";
        cin>>opc;

        switch (opc)
        {
        case 1:
            cout<<"Ingrese el caracter para agregar a la cola: ";
            cin>>dato;
            prioridad = rand() % 3 + 1; // Generar un número aleatorio entre 1 y 3 para la prioridad
            insertarCola(frente,fin,dato,prioridad);
            break;
        case 2:
            cout<<"\n Mostrando los elementos de la cola: ";
            while (frente != NULL)
            {
                suprimirCola(frente,fin,dato,prioridad);
                if (frente != NULL)
                    cout<<dato<<" ";
            }
            cout<<endl;
            system("read -p 'Presione Enter para continuar...' var"); // Pausar la ejecución en Linux
            break;
        case 3:
            break;
        default:
            cout<<"Opcion no valida"<<endl;
            system("read -p 'Presione Enter para continuar...' var"); // Pausar la ejecución en Linux
            break;
        }
    } while (opc != 3);

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

void generarElementos(Nodo *&frente, Nodo *&fin){
    int i = 0;
    while (true) {
        char dato = 'A' + i; // generar un nuevo elemento
        int prioridad = rand() % 3 + 1; // generar una prioridad aleatoria
        insertarCola(frente, fin, dato, prioridad); // insertar el elemento en la cola
        i++; // incrementar el contador
        this_thread::sleep_for(chrono::seconds(1)); // esperar un segundo antes de generar el siguiente elemento
    }
}