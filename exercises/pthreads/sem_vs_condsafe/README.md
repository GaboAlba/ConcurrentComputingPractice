## Resultados

### HELLO_ORDER_COND_SAFE

* 0.000860619s
* 0.000339014s
* 0.000661002s


### HELLO_ORDER_SEMAPHOR

* 0.000681239s
* 0.000653851s
* 0.001143334s


### Analisis

Como se puede ver el algoritmo con seguridad condicional termina siendo mas veloz, esto se debe a que el algoritmo que utiiza semaforos, aunque sea por un tiempo minimo detienen la ejecucion de otros hilos. Esto causa pequenos atrasos que se van acumulando y por lo tanto representan un tiempo mayor de ejecucion. Esto en un programa mas complejo se podria ver de manera mas dramatica y por lo tanto siempre que sea posible hay que asegurarse de tener seguridad condicional antes de semaforos. 