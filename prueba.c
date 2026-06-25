#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <time.h>


typedef struct {
    char nombrePerfil[50] ;
    char salt[10] ;
} perfil ;

typedef struct {
    char nombre[50];
    Map *mapa_claves;
} usuario;

typedef struct {
    char nombreCuenta[50] ;
    char password[50] ; 
} cuenta ;



int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

List *cargarClavesMasUsadas() {
    List *lista = list_create(); 
    if(lista == NULL) exit(EXIT_FAILURE); 

    FILE *archivo = fopen("comunes.txt", "r"); 
    if (archivo == NULL) {
        printf("Error al abrir el archivo comunes.txt\n");
        return NULL;
    }

    char palabra[50]; 

    while(fscanf(archivo, "%49s", palabra) != EOF) { 
        char *Nodo = (char *) malloc(sizeof(char) * 50);
        if(Nodo != NULL){
            strcpy(Nodo, palabra); 
            list_pushBack(lista, Nodo);
        } 
    }

    fclose(archivo); 
    printf("Claves cargadas exitosamente en la lista.\n");
    return lista;
}

void crear_perfil(Map *mapa_perfiles) {
    char nombre_perfil[50];
    puts("ingrese el nombre del nuevo perfil: ");
    scanf(" %49s", nombre_perfil);
    if(map_search(mapa_perfiles, nombre_perfil) != NULL){
        puts("el perfil ya existe, intente con otro nombre");
    }else{
        usuario *nuevo_usuario = (usuario *) malloc(sizeof(usuario));

        strcpy(nuevo_usuario->nombre, nombre_perfil);
        nuevo_usuario->mapa_claves = map_create(is_equal_str);

        map_insert(mapa_perfiles, nuevo_usuario->nombre, nuevo_usuario);
        puts("se a creado el nuevo perfil");
    }
}


usuario* ingresar_perfil(Map *mapa_perfiles, int *resultado) {
    char nombre_perfil[50];
    puts("ingrese el nombre del perfil: ");
    scanf(" %49s", nombre_perfil);
    MapPair *perfil_encontrado = map_search(mapa_perfiles, nombre_perfil);
    if(perfil_encontrado == NULL){
        puts("el perfil no existe, intente de nuevo");
        return NULL;
    }
    else{
        puts("ingreso exitoso");
        *resultado = 0; 
        return (usuario *)perfil_encontrado->value;
    }
}


int contrRepetida(char *clave, Map *usuarios) {
    int cont=0 ;
    MapPair *aux=map_first(usuarios) ;
    while(aux!=NULL) {
        // CORRECCIÓN: aux->value es un struct cuenta*, no un mapa directamente
        cuenta *c = (cuenta *)aux->value;
        if (strcmp(clave, c->password)==0) {
            cont++ ;
        }
        aux=map_next(usuarios) ;
    }
    return cont ;
}

void buscarContra(Map *nombresUsuarios) {
    char opcion ;
    printf("¿Desea buscar por nombre de usuario o cuenta?\n") ;
    printf("1. Usuario\n") ;
    printf("2. Cuenta\n") ;
    scanf(" %c", &opcion) ;

    if (opcion=='1') {
        char user[50] ;
        printf("Ingrese nombre de usuario (servicio): ") ;
        scanf("%49s", user) ;
        MapPair *par=map_search(nombresUsuarios, user) ;
        if (par!=NULL) {
            cuenta *c = (cuenta *)par->value;
            printf("Cuenta/Servicio: %s\n", c->nombreCuenta) ;
            printf("Clave: %s\n", c->password) ;
        }
        else printf("Servicio no encontrado.\n") ;
    }
    if (opcion=='2') {
        char cuenta_buscada[50] ;
        unsigned short aux=1 ;
        printf("Ingrese cuenta: ") ;
        scanf("%49s", cuenta_buscada) ;
        
        MapPair *par=map_search(nombresUsuarios, cuenta_buscada) ;
        if (par!=NULL) {
            cuenta *c = (cuenta *)par->value;
            printf("Cuenta/Servicio: %s\n", c->nombreCuenta) ;
            printf("Clave de la cuenta: %s\n", c->password) ;
            aux=0 ;
        }
        if (aux==1) {
            printf("Cuenta no encontrada.\n") ;
        }
    }
    else if (opcion != '1' && opcion != '2') {
        printf("ERROR, opción inválida.\n") ;
    }
}

int verificarClave(char *clave, List *lista_clavesMasUsadas) {
    int largo = strlen(clave);
    int mayuscula = 0; 
    int minuscula = 0;
    int numero = 0;
    int simbolo = 0;

    if(lista_clavesMasUsadas != NULL) {
        char *clave_comun = (char *)list_first(lista_clavesMasUsadas);

        while(clave_comun != NULL) {
            if(strcmp(clave, clave_comun) == 0) {
                printf("La clave es demasiado común y no es segura.\n");
                return 0;
            }
            clave_comun = (char *)list_next(lista_clavesMasUsadas);
        }
    }

    if(largo < 12) {
        printf("La clave es demasiado corta y no es segura.\n");
        return 0;
    }

    for(int i = 0; i < largo; i++){
        if(isupper(clave[i])) mayuscula += 1;
        if(islower(clave[i])) minuscula += 1;
        if(isdigit(clave[i])) numero += 1;
        if(ispunct(clave[i])) simbolo += 1;
    }

    if(mayuscula < 1 || minuscula < 1 || numero < 1 || simbolo < 1) {
        printf("La clave no cumple con los requisitos de seguridad.\n");
        return 0;
    }

    printf("La clave es segura.\n");
    return 1;
}

void claveAleatoria(char *clave, int largo){
    const char caracteres[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$&*()_+-/=?";
    int cantidad_caracteres = sizeof(caracteres) - 1;
    int es_segura = 0; 

    do {
        int mayuscula = 0;
        int minuscula = 0;
        int numero = 0; 
        int simbolo = 0;

        for (int i = 0; i < largo; i++){
            int caracter = rand() % cantidad_caracteres;
            clave[i] = caracteres[caracter];
        }
        clave[largo] = '\0'; 

        for (int i = 0; i < largo; i++){
            if (isupper(clave[i])) mayuscula++;
            if (islower(clave[i])) minuscula++;
            if (isdigit(clave[i])) numero++;
            if (ispunct(clave[i])) simbolo++;
        }

        if (mayuscula >= 1 && minuscula >= 1 && numero >= 1 && simbolo >= 1) {
            es_segura = 1; 
        }

    } while (es_segura==0);
}

int guardado(const char *nombreArchivo, Map* mapa_perfiles) {
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar.\n");
        return 0;
    }

    int total_perfiles = map_count(mapa_perfiles);
    fwrite(&total_perfiles, sizeof(int), 1, archivo);
    MapPair *pair_perfil = map_first(mapa_perfiles);

    while(pair_perfil != NULL){
        usuario *user = (usuario *) pair_perfil->value;
        if (user != NULL){
            fwrite(user->nombre, sizeof(char), 50, archivo);
            int total_cuentas = map_count(user->mapa_claves);
            fwrite(&total_cuentas, sizeof(int), 1, archivo);

            MapPair *pair_cuenta = map_first(user->mapa_claves);
            while(pair_cuenta != NULL){
                fwrite(pair_cuenta->key, sizeof(char), 50, archivo);
                fwrite(pair_cuenta->value, sizeof(char), 50, archivo); 
                pair_cuenta = map_next(user->mapa_claves);
            }
        }
        pair_perfil = map_next(mapa_perfiles);
    }
    fclose(archivo);
    return 1;
}

int recuperarDatos(const char *nombre, Map *mapa_perfiles) {
    FILE *archivo = fopen(nombre, "rb");
    if (archivo == NULL) {
        printf("Base de datos vacía.\n");
        return 0;
    }

    int total_perfiles = 0;
    if (fread(&total_perfiles, sizeof(int), 1, archivo) != 1) {
        fclose(archivo);
        return 0;
    }

    for (int i = 0; i < total_perfiles; i++) {
        usuario *user = (usuario *) malloc(sizeof(usuario));
        if (user == NULL) exit(EXIT_FAILURE);

        fread(user->nombre, sizeof(char), 50, archivo);
        
        user->mapa_claves = map_create(is_equal_str);

        int total_cuentas = 0;
        fread(&total_cuentas, sizeof(int), 1, archivo);

        for (int j = 0; j < total_cuentas; j++) {
            cuenta *nueva = (cuenta *) malloc(sizeof(cuenta));
            if (nueva == NULL) exit(EXIT_FAILURE);

            fread(nueva->nombreCuenta, sizeof(char), 50, archivo);
            fread(nueva->password, sizeof(char), 50, archivo);

            map_insert(user->mapa_claves, nueva->nombreCuenta, nueva);
        }

        map_insert(mapa_perfiles, user->nombre, user);
    }

    fclose(archivo);
    printf("Datos recuperados exitosamente desde '%s'.\n", nombre);
    return 1;
}

void asociarServicio(MapPair *par) {
    char servicio[50] ;
    char opcion ;
    char clave[50] ;
    printf("Ingrese nombre del servicio: ") ;
    scanf(" %49s", servicio) ;

    cuenta *nueva=(cuenta*) malloc(sizeof(cuenta)) ;
    strcpy(nueva->nombreCuenta, servicio) ;

    printf("¿Desea generar una contraseña para el servicio? s/n: ") ;
    scanf(" %c", &opcion) ;
    if (opcion=='s') {
        claveAleatoria(clave, 16) ;
        printf("Clave generada: %s\n", clave) ;
    }
    else {
        printf("Ingrese una contraseña: ");
        scanf("%49s", clave);
    }

    strcpy(nueva->password, clave) ;
    map_insert(par->value, nueva->nombreCuenta, nueva);
}

void crearCuenta(Map *cuentas, List *lista) {
    printf("Ingrese nombre del servicio / cuenta: ") ;
    char nombreCuenta[50] ;
    scanf("%49s", nombreCuenta) ;
    MapPair *valor=map_search(cuentas, nombreCuenta) ;
    if (valor!=NULL) {
        char opcion ;
        printf("¡Nombre de cuenta ya en uso!\n") ;
        printf("¿Desea asociar una nueva contraseña al servicio ingresado? s/n: ") ;
        scanf(" %c", &opcion) ;
        if (opcion=='s') {
            asociarServicio(valor) ;
        }
        else printf("Intente ingresar una cuenta nuevamente\n") ;
    }
    else {
        char opcion2 ;
        char clave[50] ;
        cuenta *nueva=(cuenta*) malloc(sizeof(cuenta)) ;
        strcpy(nueva->nombreCuenta, nombreCuenta) ;

        printf("¿Desea generar una contraseña para el servicio? s/n: ") ;
        scanf(" %c", &opcion2) ;
        if (opcion2=='s') {
            claveAleatoria(clave, 16) ;
            printf("Clave generada: %s\n", clave) ;
            strcpy(nueva->password, clave) ;
        }
        else {
            printf("Ingrese una contraseña: ");
            scanf("%49s", clave);
            verificarClave(clave, lista) ;
            strcpy(nueva->password, clave) ;
        }
        printf("Cuenta ingresada correctamente\n") ;

        map_insert(cuentas, nueva->nombreCuenta, nueva) ;
    }
}


int main(){
    printf("Bienvenido al gestor de claves\n");
    printf("Cargando claves mas usadas...\n");

    Map *mapaUsuarios = NULL; 
    List *lista_clavesMasUsadas = cargarClavesMasUsadas(); 
    Map *mapa_perfiles = map_create(is_equal_str);
    usuario *perfil_activo = NULL;
    int resultado = 1;
    char opcion;
    srand(time(NULL));

    int estado = recuperarDatos("datos.dat", mapa_perfiles);

    if (estadoCarga == 1){
        printf("Bienvenido de nuevo, se han cargado los datos previamente guardados.\n");
    }
    else{
        printf("Parece que es tu primera vez usando MAXIMA SEGURIDAD, crea un perfil para comenzar.\n");
    }

    do{
        char respuesta[9];
        puts("desea ingresar o crear un nuevo perfil? (ingresar/crear)"); 
        scanf("%8s", respuesta);
        
        if (strcmp(respuesta, "crear")==0){
            crear_perfil(mapa_perfiles);
        }
        else if (strcmp(respuesta, "ingresar")==0){
            perfil_activo = ingresar_perfil(mapa_perfiles, &resultado);
            if (perfil_activo != NULL) {
                mapaUsuarios = perfil_activo->mapa_claves; 
            }
        }
        else{
            puts("respuesta no valida, intente de nuevo");
        }

    }while(resultado != 0);

    do {
        puts("========================================");
        puts("            MENU DE OPCIONES");
        puts("========================================");
        
        puts("1) crear cuenta");
        puts("2) buscar clave");
        puts("3) cambiar clave");
        puts("4) verificar clave");
        puts("5) clave repetida");
        puts("6) claves mas usadas");
        puts("7) salir");
        
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1':
                crearCuenta(mapaUsuarios, lista_clavesMasUsadas);
                break;
            case '2':
                buscarContra(mapaUsuarios) ;
                break;
            case '3':
                printf("Opcion 3 seleccionada\n");
                break;
            case '4': {
                char clave[50];
                printf("Ingrese la clave a verificar: ");
                scanf("%49s", clave);
                verificarClave(clave, lista_clavesMasUsadas);
                break;
            }
            case '5': {
                char clave_rep[50];
                printf("Ingrese clave para buscar repetidas: ");
                scanf("%49s", clave_rep);
                int repes = contrRepetida(clave_rep, mapaUsuarios);
                printf("La clave se repite %d veces en este perfil.\n", repes);
                break;
            }
            case '6':
                printf("Opcion 6 seleccionada\n");
                break;
            case '7':
                printf("Saliendo del programa y guardando...\n");
                guardado("datos.dat", mapa_perfiles);
                break;
            default:
                printf("Opcion no válida\n");
                break;
        } 
        
        if (opcion != '7') { 
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }
        
    } while (opcion != '7'); 

    return 0;
}