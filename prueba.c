#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("Error al abrir el archivo"); // Informa si el archivo no puede abrirse
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
    scanf("%49s", nombre_usuario);
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
    scanf("%49s", nombre_usuario);
    MapPair *usuario_encontrado = map_search(mapa_usuarios, nombre_usuario);
    if(usuario_encontrado == NULL){
        puts("el usuario no existe, intente de nuevo");
    }
    else{
        puts("ingreso exitoso");
        *resultado = 0; 
    }
}

int main(){

    List *lista_clavesMasUsadas = cargarClavesMasUsadas(); 

    Map *mapa_usuarios = map_create(is_equal_str);
    int resultado= 1;

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

    // aca se abriria el menu 


    return 0;
}