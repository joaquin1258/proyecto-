# Maxima Seguridad

En esta carpeta podra ver como compilar y ejecutar el programa.

## Descripcion 

La aplicación "Maxima seguridad" es un sistema creado con el fin de proteger el acceso a las cuentas digitales de un usuario. Este gestor de contraseñas cuenta con un sistema de acceso seguro a través de una clave única creada por el usuario durante la generación de su perfil en el gestor. El programa ofrece al usuario funciones de creación, verificación y generación automática de contraseñas personalizadas y su almacenamiento cifrado seguro, además de la búsqueda en base a una cuenta o nombre de usuario de una contraseña.

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
1) `CargarcargarClavesMasUsadas`: A partir del archivo "comunes.txt" crea una lista con las 1000 claves mas usadas.
2) `recuperarDatos`: Carga los datos anteriormente ocupados en la aplicacion, guardados en el archivo "datos.dat".
3) `crear_perfil`: Crea un perfil para un usuario que ingresa por primera vez a la aplicacion, el cual se guarda en el mapa de perfiles.
4) `ingresar_perfil`: El usuario ingresa su perfil anteriormente usado en la aplicacion, el cual estaba guardado en el mapa de perfiles y verifica si en efecto el usuario creo un perfil anteriormente.
5) `funcionPBKDF2`: En base a la clave única y el salt correspondientes al perfil del usuario se utilizan funciones de la libreria bcrypt y la función PBKDF2 para generar una clave derivada.
6)  `funcionAES256Cifrar`: Toma la clave derivada generada por la función PBKDF2 junto con la contraseña que el usuario desea almacenar y utilizando funciones de la libreria bcrypt y la función AES-256 realiza el cifrado de la contraseña y su almacenamiento en una cadena.
7) `funcionAES256Descifrar`: A partir de la cadena con la contraseña cifrada realiza un descifrado utilizando la clave derivada y funciones de la libreria bcrypt.

Ya en el menu principales, la aplicacion implementa las siguiente funciones: 
1) `crearCuenta`: El usuario ingresa el nombre del servicio/cuenta, se le pregunta si quiere ingresar una clave por si mismo o que la aplicacion le elabore una nueva clave, asegurandose de que estas claves sean seguras, guardando toda esta informacion en el mapa de usuarios.
2) `asociarServicio`: En la funcion crearCuenta, se le pide al usuario ingresar el nombre del servicio asociado (DisneyPlus, Netflix, etc).
3) `claveAleatoria`: Tambien en la misma funcion, si el usuario quiere que el programa le elabore una clave segura, esta funcion se encarga de crearla.
4) `buscarContra`: Si el usuario quiere ver si una clave esta ocupada en algun servicio, esta funcion verifica que dicha clave se encuentra en uso, utilizando el mapa de usuarios.
5) `verificarClave`: Si el usuario quiere verificar si su clave es segura, la funcion revisa la clave verificando que cumpla con los principales criterios de seguridad (si es larga, tiene mayuscula, tiene minuscula y no es una clave comun).
6) `contrRepetida`: Si el usuario quiere saber cuantas veces a usado una clave en diferentes servicios, la funcion le indica cuantas veces a usado esa clave en los diferentes sevicios en el mapa de usuario.
7) `claves_mas_usadas`: Si el usuario quiere ver cuales son las claves mas comunes y vunerables, esta funcion le mostrara todas las contraseñas que son mas comunes, a partir de la lista de claves mas usadas.
8) `guardado`: al salir de la aplicacion, para evitar perder la informacion, se guardan los datos en el archivo "datos.dat".
9) `cambiar_clav`: Si el usuario quiere cambiar alguna clave ya usada, esta funcion se encarga de cambiar la informacion a partir del mapa de usuarios y verifica que esta funcion sea segura.

### Problemas conocidos:
1) No hay problemas conocidos. 

### A mejorar: 
1) Interfaz de usuario más amigable.
2) Permitir el almacenamiento de contraseñas en la nube.
3) Permitir el funcionamiento en diferentes sistemas operativos


## Ejemplo de uso 
...

## contribuciones

### Jorge Alfaro

    Desarrollo del informe
    Función de búsqueda y carga de contraseñas más usadas
    Función de modificar contraseñas
    Función de contraseñas repetidas
    Función de verificación de contraseñas
    Función de recuperación de datos
    Función de cifrado simétrico

### Denisse Godoy
    Desarrollo del informe
    Función de ingresar cuenta
    Función de búsqueda de contraseña
    Función de contraseñas repetidas
    Función de creación de contraseñas
    Función de derivación de claves
    Función de cifrado simétrico

### Joaquin Cortez
    Desarrollo del informe
    Función de ingresar cuenta
    Función de búsqueda de contraseña
    Función de contraseñas repetidas
    Función de creación de contraseñas
    Función de derivación de claves
    Función de cifrado simétrico
