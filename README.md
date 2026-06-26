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
1) `cargarClavesMasUsadas`: A partir del archivo "comunes.txt" crea una lista con las 1000 claves mas usadas.
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

### Paso 1 Crear Perfil 

Se le pide al usuario que ingrese una de las dos opciones, si ingresar un perfil ya creado o crear un perfil nuevo.
```
========================================
     Bienvenido a MAXIMA SEGURIDAD
========================================
Bienvenido de nuevo, se han cargado los datos previamente guardados.
desea ingresar o crear un nuevo perfil? (ingresar/crear)
crear
ingrese el nombre del nuevo perfil: 
Joaquin
se ha creado el nuevo perfil de forma segura
Por favor, confirma el ingreso al perfil que acabas de crear:
ingrese el nombre del perfil: 
Joaquin
```

En este caso, se crea un nuevo perfil y se le pide al usuario que confirme el nombre con el cual va a trabajar con la aplicacion (aparece la frase de "Bienvenido de nuevo", porque ya habiamos trabajado antes en la aplicacion para probarla).

### Paso 2 Crear cuenta

El usuario, con su perfil ya creado, entra al menu principal y elige crear una nueva cuenta.
```
========================================
             MENU DE OPCIONES
========================================
1) crear cuenta
2) buscar clave
3) cambiar clave
4) verificar clave
5) clave repetida
6) claves mas usadas
7) salir
Ingrese su opcion: 1
Ingrese nombre de cuenta: JoaquinDisney
Ingrese nombre del servicio: Disney
¿Desea generar una clave para el servicio? s/n: s
Clave generada: )Ng7PnK-)TJ&*u9W
Cuenta ingresada y cifrada correctamente
Presione una tecla para continuar...
```

El usuario crea una nueva cuenta y pide crear una nueva contraseña aleatoria y segura. 

### Paso 3 Cambiar clave

El usuario se arrepiente de usar una contraseña aleatoria y decide cambiarla por una que el mismo desarrollo.

```
========================================
            MENU DE OPCIONES
========================================
1) crear cuenta
2) buscar clave
3) cambiar clave
4) verificar clave
5) clave repetida
6) claves mas usadas
7) salir
Ingrese su opcion: 3
Ingrese el nombre de usuario asociado a la cuenta: JoaquinDisney
Ingrese el nombre del servicio / cuenta: Disney
desea generar una nueva clave aleatoria o ingresar una nueva clave? 
1. Generar clave aleatoria
2. Ingresar nueva clave
2
Ingrese la nueva clave: JOAQUINcortez2006$
La clave es segura.
Clave cambiada con éxito.
Presione una tecla para continuar...
```

Como se puede ver el usuario ingresa los datos de el servicio asociado para poder cambiar la contraseña antes generada.

### Paso 4 salir y guardar

Ya el usuario ocupo la aplicacion y ahora quiere guardar los datos, lo que hace es salir de la aplicacion.
```
========================================
            MENU DE OPCIONES
========================================
1) crear cuenta
2) buscar clave
3) cambiar clave
4) verificar clave
5) clave repetida
6) claves mas usadas
7) salir
Ingrese su opcion: 7
Saliendo del programa y guardando...
```
Aunque no se vea, los datos de la cuenta creada, ya estan guardadas en un archivo.

### Paso 5 Volver a la aplicacion 

El usuario vuelve a probar la aplicacion, pero esta vez, ingresara con su perfil ya creado
```
========================================
     Bienvenido a MAXIMA SEGURIDAD
========================================
Bienvenido de nuevo, se han cargado los datos previamente guardados.
desea ingresar o crear un nuevo perfil? (ingresar/crear)
ingresar
ingrese el nombre del perfil: 
Joaquin
```

Esto lo llevara directamente al menu principal de la aplicacion.

### Paso 6 Claves mas usadas

El usuario ingresa la opcion de claves mas usadas, y el sistema le mostrara las claves mas usadas y vulnerables. 
```
Ingrese su opcion: 6
claves mas usadas:
123456
123456789
password
qwerty
12345678
12345
123123
111111
(...)
Presione una tecla para continuar...
```

### Paso 7 verificar clave 

Ya en el menu principal, el usuario ingresa la opcion de verificar clave.
```
========================================
            MENU DE OPCIONES
========================================
1) crear cuenta
2) buscar clave
3) cambiar clave
4) verificar clave
5) clave repetida
6) claves mas usadas
7) salir
Ingrese su opcion: 4
Ingrese la clave a verificar: Joaquin
La clave es demasiado corta y no es segura.
Presione una tecla para continuar...

Ingrese su opcion: 4
Ingrese la clave a verificar: JoaquinCortez2026
La clave no cumple con los requisitos de seguridad.
Presione una tecla para continuar...

Ingrese su opcion: 4
Ingrese la clave a verificar: !JoaquinCortez2026¡
La clave es segura.
Presione una tecla para continuar...
```

Se puede ver que se intentan con varias contraseñas, pero el sistema le dice que no es muy larga o no cumple con los requisitos de seguridad, solo cuando cumple con los requisitos (que sea larga, con mayuscula, con minuscula y que no sea una contraseña comun).

### Paso 8 Cerrar y terminar de usar

El usuario ingres la opcion de salir de la aplicacion despues de haber probado multiples de las opciones.
```
========================================
            MENU DE OPCIONES
========================================
1) crear cuenta
2) buscar clave
3) cambiar clave
4) verificar clave
5) clave repetida
6) claves mas usadas
7) salir
Ingrese su opcion: 7
Saliendo del programa y guardando...
```


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
