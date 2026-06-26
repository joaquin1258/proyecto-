# Maxima Seguridad

En esta carpeta podra ver como compilar y ejecutar el programa.

## Descripcion 

aplicacion para almacenar claves de manera segura.

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
5) `crearCuenta`: el usuario ingresa el nombre del servicio/cuenta, se le pregunta si quiere ingresar una clave por si mismo o que la aplicacion le elabore una nueva clave, asegurandose de que estas claves sean seguras, guardando toda esta informacion en el mapa de usuarios
6) `asociarServicio`: en la funcion crearCuenta, se le pide al usuario ingresar el nombre del servicio asociado (DisneyPlus, Netflix, etc)
7) `claveAleatoria`: tambien en la misma funcion, si el usuario quiere que el programa le elabore una clave segura, esta funcion se encarga de crearla
8) `buscarContra`: si el usuario quiere ver si una clave esta ocupada en algun servicio, esta funcion verifica que dicha clave se encuentra en uso, utilizando el mapa de usuarios
9) `verificarClave`: si el usuario quiere verificar si su clave es segura, la funcion revisa la clave verificando que cumpla con los principales criterios de seguridad (si es larga, tiene mayuscula, tiene minuscula y no es una clave comun)
10) `contrRepetida`: si el usuario quiere saber cuantas veces a usado una clave en diferentes servicios, la funcion le indica cuantas veces a usado esa clave en los diferentes sevicios en el mapa de usuario
11) `claves_mas_usadas`: si el usuario quiere ver cuales son las claves mas comunes y vunerables, esta funcion le mostrara todas las contraseñas que son mas comunes, a partir de la lista de claves mas usadas
12) `guardado`: al salir de la aplicacion, para evitar perder la informacion, se guardan los datos en el archivo "datos.dat"


### contribuciones

#### Jorge Alfaro

    Desarrollo del informe
    Función de búsqueda y carga de contraseñas más usadas
    Función de modificar contraseñas
    Función de contraseñas repetidas
    Función de verificación de contraseñas
    Función de recuperación de datos
    Función de cifrado simétrico

#### Denisse Godoy
    Desarrollo del informe
    Función de ingresar cuenta
    Función de búsqueda de contraseña
    Función de contraseñas repetidas
    Función de creación de contraseñas
    Función de derivación de claves
    Función de cifrado simétrico

#### Joaquin Cortez
    Desarrollo del informe
    Función de ingresar cuenta
    Función de búsqueda de contraseña
    Función de contraseñas repetidas
    Función de creación de contraseñas
    Función de derivación de claves
    Función de cifrado simétrico
