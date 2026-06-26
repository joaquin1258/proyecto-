# Maxima Seguridad

En esta carpeta podra ver como compilar y ejecutar el programa.

## Descripcion 

aplicacion para almacenar contraseñas de manera segura.

## Como compilar y ejecutar tarea3

Este programa fue desarrollado en lenguaje C y puede ejecutarse utilizando VScode

### Requisitos previos 

- Tener una cuenta en VScode y tener el compilador de C/C++.

### Pasos para compilar y ejecutar

Primero, para compilar el programa (en VScode), se debe ir a la terminal y colocar la siguiente linea: 

````
gcc prueba.c tdas/list.c tdas/extra.c tdas/map.c -o programa
````

Despues en la misma terminal, para ejecutar el programa ingresas la siguiente linea: 

````
./programa
````

## Funcionalidades 

### Funcionando correctamente:

El programa implementa varias funcionalidades antes de entrar al menu principal, la cuales son: 
1) `CargarcargarClavesMasUsadas`: a partir del archivo "comunes.txt" crea una lista con las 1000 claves mas usadas 
2) `recuperarDatos`: carga los datos anteriormente ocupados en la aplicacion, guardados en el archivo "datos.dat" 
3) `crear_perfil`: crea un perfil para un usuario que ingresa por primera vez a la aplicacion, el cual se guarda en el mapa de perfiles
4) `ingresar_perfil`: el usuario ingresa su perfil anteriormente usado en la aplicacion, el cual estaba guardado en el mapa de perfiles y verifica si en efecto el usuario creo un perfil anteriormente

Ya en el menu principales, la aplicacion implementa las siguiente funciones: 
