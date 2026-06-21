#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"

typedef struct {
    char nombre[50];
    Map *mapa_claves;
} usuario;

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

int main(){
    printf("Bienvenido al gestor de claves\n");
    printf("Cargando claves mas usadas...\n");

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
        puts("            MENÚ DE OPCIONES");
        puts("========================================");
        
        puts("1) crear cuenta");
        puts("2) buscar contraseña");
        puts("3) cambiar contraseña");
        puts("4) salir");
        
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1':
                crearCuenta(mapa_usuarios);
                break;
            case '2':
                printf("Opción 2 seleccionada\n");
                break;
            case '3':
                printf("Opción 3 seleccionada\n");
                break;
            case '4':
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida\n");
                break;
        } 
        
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }
        
    } while (opcion != '4');

    return 0;
}