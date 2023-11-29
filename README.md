# PROJECT_2-SO1
La cola comenzará con n elementos (n>10) generados de forma aleatoria. Estos elementos tendrán un contador inicializado en un número entre 20 y 100 y una prioridad donde 0 es la prioridad más baja y tres es la prioridad más alta (asignada de forma aleatoria).

Un hilo se encargará de generar nuevos elementos que se irán agregando a la cola con prioridades (QUEUE). Cada vez que se agregue un elemento, se debe imprimir la cola.

Otro hilo se encargará de eliminar el elemento inicial de la cola siempre y cuando su contador esté en cero. Si no, no hará nada.
Finalmente, Otro hilo se encargará de decrementar de forma aleatoria el contador de cada elemento en un número entre 1 y 5. cuando el contador llegue a cero, el elemento deberá moverse a la posición inicial de la cola.

Utilizar dos diferentes algoritmos de despacho para el funcionamiento del hilo que maneja el contador, aparte de la funcionalidad mencionada anteriormente:
Round robin
Cualquier otro excepto fcfs y por prioridad
Su simulación deberá solicitar al usuario que seleccione el algoritmo que se utilizará para el manejo del hilo, y correr el proceso infinitamente hasta que el usuario presione una tecla.

Seleccionar dos métricas para comparar el rendimiento de los algoritmos
tiempo de despacho: El tiempo que tarda un elemento en que su contador comience a decrementar.
tiempo de salida: El tiempo que tarda un elemento en salir de la cola (que su contador llegue a cero)
Througput: elementos sacados de la cola por unidad de tiempo.
Justicia: número de veces que el contador es decrementado entre el valor del contador.
Etcétera
