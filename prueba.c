#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    char correo[100];
    char clave[50];
} cuenta_usuario;


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


int main(){

    List *lista_clavesMasUsadas = cargarClavesMasUsadas(); 

    printf("CACO EL PROYECTOOOO!!!!!");
    return 0;
}