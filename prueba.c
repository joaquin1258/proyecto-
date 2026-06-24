#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"


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
    char password[20] ;
} cuenta ;



int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

List *cargarClavesMasUsadas() {
    List *lista = list_create(); 
    if(lista == NULL) exit(EXIT_FAILURE); 

    FILE *archivo = fopen("comunes.txt", "r"); 
    if (archivo == NULL) {
        printf("Error al abrir el archivo");
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

void crear_usuario(Map *mapa_usuarios) {
    char nombre_usuario[50];
    puts("ingrese el nombre del nuevo usuario: ");
    scanf(" %49s", nombre_usuario);
    if(map_search(mapa_usuarios, nombre_usuario) != NULL){
        puts("el usuario ya existe, intente con otro nombre");
    }else{
        usuario *nuevo_usuario = (usuario *) malloc(sizeof(usuario));

        strcpy(nuevo_usuario->nombre, nombre_usuario);
        nuevo_usuario->mapa_claves = map_create(is_equal_str);

        map_insert(mapa_usuarios, nuevo_usuario->nombre, nuevo_usuario);
        puts("se a creado el nuevo usuario");
    }
}

void ingresar_usuario(Map *mapa_usuarios, int *resultado) {
    char nombre_usuario[50];
    puts("ingrese el nombre del usuario: ");
    scanf(" %49s", nombre_usuario);
    MapPair *usuario_encontrado = map_search(mapa_usuarios, nombre_usuario);
    if(usuario_encontrado == NULL){
        puts("el usuario no existe, intente de nuevo");
    }
    else{
        puts("ingreso exitoso");
        *resultado = 0; 
    }
}

void asociarServicio(MapPair *par) {
    char servicio[50] ;
    char opcion ;
    printf("Ingrese nombre del servicio: ") ;
    scanf(" %49s", servicio) ;

    printf("¿Desea generar una contraseña para el servicio? s/n: ") ;
    scanf(" %c", &opcion) ;
    if (opcion=='s') {
        printf("generar contraseña\n") ;
    }
    else {
        printf("Ingresar contraseña\n") ;
    }
}

void crearCuenta(Map *cuentas) {
    printf("Ingrese nombre de usuario: ") ;
    char nombreCuenta[50] ;
    scanf("%49s", nombreCuenta) ;

    MapPair *valor=map_search(cuentas, nombreCuenta) ;
    if (valor!=NULL) {
        char opcion ;
        printf("¡Nombre de usuario ya en uso!\n") ;
        printf("¿Desea asociar un nuevo servicio al usuario ingresado? s/n: ") ;
        scanf(" %c", &opcion) ;
        if (opcion=='s') {
            asociarServicio(valor) ;
        }
        else printf("Intente ingresar una cuenta nuevamente\n") ;
    }
    else {
        char servicio[50] ;
        char opcion2 ;
        char contrasena_ingresada[50] = "Clave123";
        usuario *nuevo=(usuario*) malloc(sizeof(usuario)) ;

        strcpy(nuevo->nombre, nombreCuenta) ;
        nuevo->mapa_claves=map_create(is_equal_str) ;

        printf("Ingrese nombre del servicio: ") ;
        scanf("%49s", servicio) ;

        printf("¿Desea generar una contraseña para el servicio? s/n: ") ;
        scanf(" %c", &opcion2) ;
        if (opcion2=='s') {
            printf("generar contraseña\n") ;
        }
        else {
            printf("Ingresar contraseña\n") ;
            printf("Ingrese la contraseña: ");
            scanf("%49s", contrasena_ingresada);
        }
        printf("Cuenta ingresada correctamente\n") ;
        
        map_insert(nuevo->mapa_claves, servicio, contrasena_ingresada) ; 
    }
}

void buscarContra(Map *nombresUsuarios) {
    char opcion ;
    printf("¿Desea buscar por nombre de usuario o cuenta?") ;
    printf("1. Usuario\n2. Cuenta\n") ;
    scanf("%c", &opcion) ;

    if (opcion=='1') {
        char user[50] ;
        printf("Ingrese nombre de usuario: ") ;
        scanf("%49s", user) ;
        MapPair *usuario=map_search(nombresUsuarios, user) ;
        if (usuario==NULL) {
            printf("Nombre de usuario no registrado, intente nuevamente.\n") ;
        }
        else {
            printf("Cuentas asociadas al nombre de usuario: \n") ;
            Map *mapaServicios=usuario->value ;
            MapPair *cuenta=map_first(mapaServicios) ;
            while (cuenta!=NULL) {
                printf("%s\n", cuenta->key) ;
                cuenta=map_next(mapaServicios) ;
            }
            printf("¿A cuál cuenta desearía ver la clave?\n") ;
            char servicio[50] ;
            scanf("%s", servicio) ;
            MapPair *par=map_search(mapaServicios, servicio) ;
            if (par!=NULL) {
                char clave[20] ;
                strcpy(clave, par->value) ;
                // funcion de cifrado para descifrar
                printf("Nombre de usuario: %s\n", user) ;
                printf("Cuenta: %s\n", servicio) ;
                printf("Clave: %s\n", clave) ;
            }
        }
    }
    if (opcion=='2') {
        char cuenta[50] ;
        unsigned short aux=0 ;
        printf("Ingrese cuenta: ") ;
        scanf("%s, cuenta") ;
        
        MapPair *usuarios=map_first(nombresUsuarios) ;
        while (usuarios!=NULL) {
            Map *servicios=usuarios->value ;
            MapPair *servicio=map_search(servicios, cuenta) ;
            if (servicio==NULL) {
                aux=1 ;
            }
            else {
                char clave[20] ;
                strcpy(clave, servicio->value) ;
                //funcion cifrado para descifrar
                printf("Nombre de usuario: %s\n", usuarios->key) ;
                printf("Cuenta: %s\n", servicio->key) ;
                printf("Clave de la cuenta: %s", clave) ;
                aux=0 ;
                break ;
            }
            usuarios=map_next(nombresUsuarios) ;
        }
        if (aux==1) {
            printf("Cuenta no encontrada.\n") ;
        }
    }
    else {
        printf("ERROR, opción inválida.\n") ;
    }
}


void verificarClave(char *clave, List *lista_clavesMasUsadas) {
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
                return;
            }
            clave_comun = (char *)list_next(lista_clavesMasUsadas);
        }
    }

    if(largo < 12) {
        printf("La clave es demasiado corta y no es segura.\n");
        return;
    }

    for(int i = 0; i < largo; i++){
        if(isupper(clave[i])) mayuscula += 1;
        if(islower(clave[i])) minuscula += 1;
        if(isdigit(clave[i])) numero += 1;
        if(ispunct(clave[i])) simbolo += 1;

    }

    if(mayuscula < 1 || minuscula < 1 || numero < 1 || simbolo < 1) {
        printf("La clave no cumple con los requisitos de seguridad.\n");
        return;
    }

    printf("La clave es segura.\n");
}

int main(){
    printf("Bienvenido al gestor de claves\n");
    printf("Cargando claves mas usadas...\n");

    Map *mapaUsuarios=map_create(is_equal_str) ;

    List *lista_clavesMasUsadas = cargarClavesMasUsadas(); 
    Map *mapa_usuarios = map_create(is_equal_str);
    int resultado = 1;
    char opcion;

    do{
        char respuesta[9];
        puts("desea ingresar o crear un nuevo usuario? (ingresar/crear)"); 
        scanf("%8s", respuesta);
        
        if (strcmp(respuesta, "crear")==0){
            crear_usuario(mapa_usuarios);
            resultado = 0;
        }
        else if (strcmp(respuesta, "ingresar")==0){
            ingresar_usuario(mapa_usuarios, &resultado);
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
        puts("4) salir");
        puts("5) verificar clave");
        
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1':
                crearCuenta(mapaUsuarios);
                break;
            case '2':
                printf("Opcion 2 seleccionada\n");
                break;
            case '3':
                printf("Opcion 3 seleccionada\n");
                break;
            case '4':
                printf("Saliendo...\n");
                break;
            case '5':

                printf("Opcion 5 seleccionada\n");
                char clave[50];
                printf("Ingrese la clave a verificar: ");
                scanf("%49s", clave);
                verificarClave(clave, lista_clavesMasUsadas);
                break;

            default:
                printf("Opcion no válida\n");
                break;
        } 
        
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }
        
    } while (opcion != '4');

    return 0;
}