# Recursive Hello Exercise

## Enunciado
Copie su `examples/pthreads/hello` a `exercises/pthreads/recursive_hello`.  Renombre `exercises/pthreads/recursive_hello/src/hello.c` a `recursive_hello.c` .

Modifique a `greet()` para recibir un número de tipo `size_t`. Si este número es 0, `greet()` imprime una despedida y el número recibido. Luego termina su ejecución.

Si el número que `greet()` recibe es mayor que cero, imprime un saludo y el número recibido. Luego crea un hilo para que invoque a `greet()` pero con un número menor que el recibido por parámetro.

Modifique el procedimiento `main()` para crear una variable local de tipo `size_t` inicializada en 2. Envíe este número como parámetro de la subrutina `greet()` que correrá en el hilo secundario.

Recuerde copiar este enunciado en una sección de su documeneto de análisis `exercises/pthreads/recursive_hello/readme.md` y darle el formato apropiado. En otra sección haga una predicción de la salida de su programa antes de corerlo. Compílelo y córralo con el Makefile. Copie la salida de su programa y péguela en un bloque de código debajo de su predicción. ¿Acertó la predicción?

## Prediccion
Dado que una funcion recursiva necesita que la siguiente instancia llegue a su punto de retorno antes de poder retornar su valor, los hilos se crearan y se cerraran de manera secuencial (serial), sin ningun paralelismo. 

## Salida 
Al correr el programa hecho este nos da la siguiente salida en todas las corridas realizadas. 
```
Output:

Hello from main thread
Saludo desde: 1 
Saludo desde: 0 
Hasta luego desde: 0 
```
Esto cumple con la prediccion hecha con respecto a la naturaleza recursiva en la que se estan creando los hilos.

## Rastreo de memoria
![Rastreo de memoria recursive_hello](./trace/Rastreo_Mem_recursive_hello.drawio.svg)