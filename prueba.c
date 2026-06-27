#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "tdas/extra.h"
#undef N              


#include "tdas/list.h"
#include "tdas/map.h"
#include <time.h>
#include <windows.h>


// --- TIPOS DE FUNCIONES PARA CARGAR DE FORMA DINÁMICA DE REEMPLAZO A BCRYPT.H ---
typedef LONG (WINAPI *pfnBCryptOpenAlgorithmProvider)(HANDLE*, LPCWSTR, LPCWSTR, ULONG);
typedef LONG (WINAPI *pfnBCryptDeriveKeyPBKDF2)(HANDLE, PUCHAR, ULONG, PUCHAR, ULONG, ULONGLONG, PUCHAR, ULONG, ULONG);
typedef LONG (WINAPI *pfnBCryptCloseAlgorithmProvider)(HANDLE, ULONG);
typedef LONG (WINAPI *pfnBCryptSetProperty)(HANDLE, LPCWSTR, PUCHAR, ULONG, ULONG);
typedef LONG (WINAPI *pfnBCryptGenerateSymmetricKey)(HANDLE, HANDLE*, PUCHAR, ULONG, PUCHAR, ULONG, ULONG);
typedef LONG (WINAPI *pfnBCryptEncrypt)(HANDLE, PUCHAR, ULONG, void*, PUCHAR, ULONG, PUCHAR, ULONG, ULONG*, ULONG);
typedef LONG (WINAPI *pfnBCryptDecrypt)(HANDLE, PUCHAR, ULONG, void*, PUCHAR, ULONG, PUCHAR, ULONG, ULONG*, ULONG);
typedef LONG (WINAPI *pfnBCryptDestroyKey)(HANDLE);


// Punteros de función globales
pfnBCryptOpenAlgorithmProvider fn_BCryptOpenAlgorithmProvider = NULL;
pfnBCryptDeriveKeyPBKDF2 fn_BCryptDeriveKeyPBKDF2 = NULL;
pfnBCryptCloseAlgorithmProvider fn_BCryptCloseAlgorithmProvider = NULL;
pfnBCryptSetProperty fn_BCryptSetProperty = NULL;
pfnBCryptGenerateSymmetricKey fn_BCryptGenerateSymmetricKey = NULL;
pfnBCryptEncrypt fn_BCryptEncrypt = NULL;
pfnBCryptDecrypt fn_BCryptDecrypt = NULL;
pfnBCryptDestroyKey fn_BCryptDestroyKey = NULL;


int inicializarCriptografia() {
    HMODULE hBCrypt = LoadLibraryA("bcrypt.dll");
    if (!hBCrypt) return 0;
   
    fn_BCryptOpenAlgorithmProvider = (pfnBCryptOpenAlgorithmProvider)GetProcAddress(hBCrypt, "BCryptOpenAlgorithmProvider");
    fn_BCryptDeriveKeyPBKDF2 = (pfnBCryptDeriveKeyPBKDF2)GetProcAddress(hBCrypt, "BCryptDeriveKeyPBKDF2");
    fn_BCryptCloseAlgorithmProvider = (pfnBCryptCloseAlgorithmProvider)GetProcAddress(hBCrypt, "BCryptCloseAlgorithmProvider");
    fn_BCryptSetProperty = (pfnBCryptSetProperty)GetProcAddress(hBCrypt, "BCryptSetProperty");
    fn_BCryptGenerateSymmetricKey = (pfnBCryptGenerateSymmetricKey)GetProcAddress(hBCrypt, "BCryptGenerateSymmetricKey");
    fn_BCryptEncrypt = (pfnBCryptEncrypt)GetProcAddress(hBCrypt, "BCryptEncrypt");
    fn_BCryptDecrypt = (pfnBCryptDecrypt)GetProcAddress(hBCrypt, "BCryptDecrypt");
    fn_BCryptDestroyKey = (pfnBCryptDestroyKey)GetProcAddress(hBCrypt, "BCryptDestroyKey");


    return (fn_BCryptOpenAlgorithmProvider && fn_BCryptDeriveKeyPBKDF2 && fn_BCryptCloseAlgorithmProvider &&
            fn_BCryptSetProperty && fn_BCryptGenerateSymmetricKey && fn_BCryptEncrypt && fn_BCryptDecrypt && fn_BCryptDestroyKey);
}


typedef struct {
    char nombrePerfil[50] ;
    char salt[16] ;
    char resultado[50] ;
} perfil ;


typedef struct {
    char nombre[50];
    Map *mapa_claves;
} usuario;


typedef struct {
    char nombreCuenta[50] ;
    char password[68] ;
} cuenta ;


int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}


void claveAleatoria(char *clave, int largo){
    // se crea un banco de caracteres
    const char caracteres[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$&*()_+-/=?";
    int cantidad_caracteres = sizeof(caracteres) - 1;
    int es_segura = 0;

    // se itera para confirma que la clave creada sea segura
    do {
        int mayuscula = 0;
        int minuscula = 0;
        int numero = 0;
        int simbolo = 0;

        // se eligen datos al azar para crear la clave
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

        // si cumple con los requerimientos es segura
        if (mayuscula >= 1 && minuscula >= 1 && numero >= 1 && simbolo >= 1) {
            es_segura = 1;
        }


    } while (es_segura==0);
}


void funcionPBKDF2(char *claveUnica, char *salt, unsigned char *claveDerivada) {
    HANDLE handle=NULL ;
    fn_BCryptOpenAlgorithmProvider(&handle, L"PBKDF2", NULL, 0) ;
    fn_BCryptDeriveKeyPBKDF2(handle, (PUCHAR) claveUnica, strlen(claveUnica), (PUCHAR) salt, strlen(salt), 10000, claveDerivada, 32, 0) ;
    fn_BCryptCloseAlgorithmProvider(handle, 0) ;
}


void funcionAES256Cifrar(unsigned char *claveDerivada, char *password, unsigned char *contraCifrada) {
    HANDLE handle=NULL ;
    fn_BCryptOpenAlgorithmProvider(&handle, L"AES", NULL, 0) ;
   
    fn_BCryptSetProperty(handle, L"ChainingMode", (PUCHAR) L"ChainingModeCBC", sizeof(L"ChainingModeCBC"), 0) ;


    HANDLE handleLLave=NULL ;
    fn_BCryptGenerateSymmetricKey(handle, &handleLLave, NULL, 0, (PUCHAR) claveDerivada, 32, 0) ;


    unsigned char iv[16] = {0};
    ULONG cantBytes=0 ;
   
    fn_BCryptEncrypt(handleLLave, (PUCHAR) password, 64, NULL, iv, 16, contraCifrada, 64, &cantBytes, 0) ;


    fn_BCryptDestroyKey(handleLLave) ;
    fn_BCryptCloseAlgorithmProvider(handle, 0) ;
}


void funcionAES256Descifrar(unsigned char *claveDerivada, unsigned char *contraCifrada, char *passwordOriginal) {
    HANDLE handle=NULL ;
    fn_BCryptOpenAlgorithmProvider(&handle, L"AES", NULL, 0) ;
   
    fn_BCryptSetProperty(handle, L"ChainingMode", (PUCHAR) L"ChainingModeCBC", sizeof(L"ChainingModeCBC"), 0) ;


    HANDLE handleLLave=NULL ;
    fn_BCryptGenerateSymmetricKey(handle, &handleLLave, NULL, 0, (PUCHAR) claveDerivada, 32, 0) ;


    unsigned char iv[16] = {0};
    ULONG cantBytes=0 ;
   
    fn_BCryptDecrypt(handleLLave, (PUCHAR) contraCifrada, 64, NULL, iv, 16, (PUCHAR) passwordOriginal, 64, &cantBytes, 0) ;


    if (cantBytes < 64) {
        passwordOriginal[cantBytes] = '\0';
    } else {
        passwordOriginal[63] = '\0';
    }


    fn_BCryptDestroyKey(handleLLave) ;
    fn_BCryptCloseAlgorithmProvider(handle, 0) ;
}


List *cargarClavesMasUsadas() {
    List *lista = list_create();
    // se crea una lista de claves
    if(lista == NULL) exit(EXIT_FAILURE);

    // se abre el archivo de claves mas usadas
    FILE *archivo = fopen("comunes.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo comunes.txt\n");
        return NULL;
    }


    char palabra[50];

    // se itera en cada palabra para alamacenarla en la lista 
    while(fscanf(archivo, "%49s", palabra) != EOF) {
        char *Nodo = (char *) malloc(sizeof(char) * 50);
        if(Nodo != NULL){
            strcpy(Nodo, palabra);
            list_pushBack(lista, Nodo);
        }
    }

    // se cierra el archivo y se retorna la lista
    fclose(archivo);
    return lista;
}


Map *mapa_salts = NULL;


void crear_perfil(Map *mapa_perfiles, unsigned char *claveDerivada, unsigned char *valido) {
    char nombre_perfil[50];
    char clave[50];
    puts("Ingrese el nombre del nuevo perfil: ");
    scanf(" %49s", nombre_perfil);
    
    // recibe un nombre de perfil y verifica que no exista
    if(map_search(mapa_perfiles, nombre_perfil) != NULL){
        puts("El perfil ya existe, intente con otro nombre");
    } else {
        // se crea el nuevo perfil y se le da su nombre
        perfil *nuevo_perfil = (perfil *) malloc(sizeof(perfil));
        strcpy(nuevo_perfil->nombrePerfil, nombre_perfil);

        printf("Ingrese la clave para el nuevo perfil: ");
        scanf(" %49s", clave);
        // se crea la contraseña del perfil y se ingresa junto a su salt
        char *salt_aux = (char *) malloc(16 * sizeof(char));
        claveAleatoria(salt_aux, 15);
        map_insert(mapa_salts, nuevo_perfil->nombrePerfil, salt_aux);

        // se crea su clave derivada
        funcionPBKDF2(clave, salt_aux, claveDerivada);
        
        char palabra_secreta[64] = "valido";
        funcionAES256Cifrar(claveDerivada, palabra_secreta, valido);
        // se  comprueba que funcione la clave derivada
        memset(nuevo_perfil->resultado, 0, sizeof(nuevo_perfil->resultado));
        memset(nuevo_perfil->resultado, valido, 64);

        // se ingresa el perfil en el mapa de perfiles
        map_insert(mapa_perfiles, nuevo_perfil->nombrePerfil, nuevo_perfil);
        puts("Se ha creado el nuevo perfil de forma segura");
    }
}


usuario *ingresar_perfil(Map *mapa_perfiles, int *resultado, unsigned char *claveDerivada) {
    char nombre_perfil[50];
    puts("Ingrese el nombre del perfil: ");
    scanf(" %49s", nombre_perfil);
    // se recibe un nombre de perfil y se comprueba de que exista
    MapPair *perfil_encontrado = map_search(mapa_perfiles, nombre_perfil);
    if(perfil_encontrado == NULL){
        puts("El perfil no existe, intente de nuevo");
        return NULL;
    } else {

        // se recibe la clave del perfil 
        char clave[50];
        puts("Ingrese la clave del perfil: ");
        scanf(" %49s", clave);
        // se referencia el perfil actual
        perfil *perfil_actual = (perfil *)perfil_encontrado->value;
       
        // descifra el contenido del perfil para validar si la clave maestra es correcta
        char guardado[65] = {0};
        char auxiliar[50] = "valido";

        funcionAES256Descifrar(claveDerivada, perfil_actual->resultado, guardado);
        
        // se compara para ver si es correcta la clave
        printf("%s", guardado);
        if (strncmp(guardado, auxiliar, 6)!=0){
            puts("clave incorrecta, intentelo de nuevo");
            *resultado=1;
            return NULL;
        }
        // si es correcta se ingresa 
        puts("Ingreso exitoso");
        *resultado = 0;
        return perfil_actual;
    }
}


int contrRepetida(char *clave, Map *usuarios, unsigned char *claveDerivada) {
    int cont = 0;
    MapPair *aux = map_first(usuarios);
   
    while(aux != NULL) {
       
        Map *mapaServicios = (Map *)aux->value;
        MapPair *aux_cuenta = map_first(mapaServicios);
       
        while(aux_cuenta != NULL) {
            cuenta *c = (cuenta *)aux_cuenta->value;
            char des_password[68] = {0};
           
            funcionAES256Descifrar(claveDerivada, (unsigned char*)c->password, des_password);
           
            if (strcmp(clave, des_password) == 0) {
                cont++;
            }
            aux_cuenta = map_next(mapaServicios);
        }
        aux = map_next(usuarios);
    }
    return cont;
}


void buscarContra(Map *nombresUsuarios, unsigned char *claveDerivada) {
    char opcion ;
    printf("¿Desea buscar por nombre de usuario o cuenta?\n") ;
    printf("1. Usuario\n") ;
    printf("2. Cuenta\n") ;
    scanf(" %c", &opcion) ;


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
            scanf("%49s", servicio) ;
            MapPair *par=map_search(mapaServicios, servicio) ;
            if (par!=NULL) {
                char clave[65] ;
                memset(clave, 0, sizeof(clave)) ;
                funcionAES256Descifrar(claveDerivada, par->value, clave) ;
                printf("Nombre de usuario: %s\n", user) ;
                printf("Cuenta: %s\n", par->key) ;
                printf("Clave: %s\n", clave) ;
            }
            else printf("Servicio no encontrado.\n") ;
        }
    }
    if (opcion=='2') {
        char cuenta[50] ;
        unsigned short aux=1 ;
        printf("Ingrese cuenta: ") ;
        scanf("%49s", cuenta) ;
        MapPair *usuarios=map_first(nombresUsuarios) ;
        while (usuarios!=NULL) {
            Map *servicios=usuarios->value ;
            MapPair *servicio=map_search(servicios, cuenta) ;
            if (servicio!=NULL) {
                char clave[65] ;
                memset(clave, 0, sizeof(clave)) ;
                funcionAES256Descifrar(claveDerivada, servicio->value, clave) ;
                for (int i=0; i<64; i++) {
                    if (clave[i]<32 || clave[i]>126) {
                        clave[i]='\0' ;
                        break ;
                    }
                }
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
    else if (opcion!='1' && opcion!='2') {
        printf("ERROR, opcion invalida.\n") ;
    }
}


int verificarClave(char *clave, List *lista_clavesMasUsadas) {
    int largo = strlen(clave);
    int mayuscula = 0;
    int minuscula = 0;
    int numero = 0;
    int simbolo = 0;

    // recorre la lista de claves mas usadas para ver si la contraseña esta entre esas
    if(lista_clavesMasUsadas != NULL) {
        char *clave_comun = (char *)list_first(lista_clavesMasUsadas);


        while(clave_comun != NULL) {
            if(strcmp(clave, clave_comun) == 0) {
                printf("La clave es demasiado comun y no es segura.\n");
                return 0;
            }
            clave_comun = (char *)list_next(lista_clavesMasUsadas);
        }
    }

    // si la clave es menor a 12 caracterez se informa de eso
    if(largo < 12) {
        printf("La clave es demasiado corta y no es segura.\n");
        return 0;
    }

    // si la contraseña contiene al menos 1 mayuscula, 1 minuscula, 1 numero y 1 simbolo se avisa de eso
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

    // si no retorno con lo anterior se confirma que la contraseña es segura
    printf("La clave es segura.\n");
    return 1;
}


void asociarServicio(MapPair *par, unsigned char *claveDerivada, List *lista) {
    char servicio[50] ;
    char opcion ;
    char clave[50] ;
    printf("Ingrese nombre del servicio: ") ;
    scanf(" %49s", servicio) ;


    cuenta *nueva=(cuenta*) malloc(sizeof(cuenta)) ;
    strcpy(nueva->nombreCuenta, servicio) ;


    printf("¿Desea generar una clave para el servicio? s/n: ") ;
    scanf(" %c", &opcion) ;
    if (opcion=='s') {
        claveAleatoria(clave, 16) ;
        printf("Clave generada: %s\n", clave) ;
    }
    else {
        do {
        printf("Ingrese una clave: ");
        scanf("%49s", clave);
        } while (verificarClave(clave, lista)==0) ;
    }
    unsigned char contraCifrada[64] = {0};
    funcionAES256Cifrar(claveDerivada, clave, contraCifrada) ;
    memcpy(nueva->password, contraCifrada, 64) ;
    map_insert(par->value, nueva->nombreCuenta, nueva->password) ;
}


void crearUsuario(Map *usuarios, List *lista, unsigned char *claveDerivada) {
    printf("Ingrese nombre de usuario: ") ;
    char nombreCuenta[50] ;
    scanf("%49s", nombreCuenta) ;
    MapPair *valor=map_search(usuarios, nombreCuenta) ;
    if (valor!=NULL) {
        char opcion ;
        printf("¡Nombre de usuario ya en uso!\n") ;
        printf("¿Desea asociar una nueva cuenta al usuario ingresado? s/n: ") ;
        scanf(" %c", &opcion) ;
        if (opcion=='s') {
            asociarServicio(valor, claveDerivada, lista) ;
        }
        else printf("Intente ingresar una cuenta nuevamente\n") ;
    }
    else {
        char servicio[50] ;
        char opcion2 ;
        char clave[50] ;
        usuario *nuevo=(usuario*) malloc(sizeof(usuario)) ;
        strcpy(nuevo->nombre, nombreCuenta) ;
        nuevo->mapa_claves=map_create(is_equal_str) ;


        printf("Ingrese nombre del servicio: ") ;
        scanf("%49s", servicio) ;


        cuenta *nueva=(cuenta*) malloc(sizeof(cuenta)) ;
        strcpy(nueva->nombreCuenta, servicio) ;


        printf("¿Desea generar una clave para el servicio? s/n: ") ;
        scanf(" %c", &opcion2) ;
        if (opcion2=='s') {
            claveAleatoria(clave, 16) ;
            printf("Clave generada: %s\n", clave) ;
            strcpy(nueva->password, clave) ;
        }
        else {


            do {
                printf("Ingrese una clave: ");
                scanf("%49s", clave);
            } while (verificarClave(clave, lista)==0) ;
            strcpy(nueva->password, clave) ;
        }


        // --- PROCESO DE CIFRADO ---
        unsigned char contraCifrada[64] = {0};
        funcionAES256Cifrar(claveDerivada, clave, contraCifrada);
        memcpy(nueva->password, contraCifrada, 64);


        printf("Cuenta ingresada y cifrada correctamente\n") ;
        map_insert(nuevo->mapa_claves, nueva->nombreCuenta, nueva->password) ;
        map_insert(usuarios, nuevo->nombre, nuevo->mapa_claves) ;
    }
}


void claves_mas_usadas(List *lista) {
    // recibe la lista de claves mas usadas
    printf("claves mas usadas:\n");
    char *clave = (char *)list_first(lista);
    // se usa un auxiliar para poder recorrerla y pasar al siguiente
    while (clave != NULL) {
        printf("%s\n", clave);
        clave = (char *)list_next(lista);
    }
}


int guardado(const char *nombreArchivo, Map* mapa_perfiles) {
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar.\n");
        return 0;
    }


    int total_perfiles = 0;
    MapPair *aux_contar = map_first(mapa_perfiles);
    while (aux_contar != NULL) {
        total_perfiles++;
        aux_contar = map_next(mapa_perfiles);
    }


    fwrite(&total_perfiles, sizeof(int), 1, archivo);
    MapPair *pair_perfil = map_first(mapa_perfiles);


    while(pair_perfil != NULL){
        usuario *user = (usuario *) pair_perfil->value;
        if (user != NULL){
            fwrite(user->nombre, sizeof(char), 50, archivo);
           
            MapPair *salt_pair = map_search(mapa_salts, user->nombre);
            char *salt_guardar = (salt_pair != NULL) ? (char*)salt_pair->value : "000000000000000";
            fwrite(salt_guardar, sizeof(char), 16, archivo);
           
            int total_usuarios_internos = 0;
            MapPair *aux_u = map_first(user->mapa_claves);
            while (aux_u != NULL) {
                total_usuarios_internos++;
                aux_u = map_next(user->mapa_claves);
            }
            fwrite(&total_usuarios_internos, sizeof(int), 1, archivo);


            // Re-iteramos de forma lineal y limpia
            MapPair *pair_u_interno = map_first(user->mapa_claves);
            while(pair_u_interno != NULL){
                usuario *u_int = (usuario *)pair_u_interno->value;
                fwrite(u_int->nombre, sizeof(char), 50, archivo);
               
                int total_cuentas = 0;
                MapPair *aux_c = map_first(u_int->mapa_claves);
                while(aux_c != NULL) { total_cuentas++; aux_c = map_next(u_int->mapa_claves); }
            fwrite(&total_cuentas, sizeof(int), 1, archivo);


                MapPair *pair_cuenta = map_first(u_int->mapa_claves);
                while(pair_cuenta != NULL) {
                    cuenta *c = (cuenta *)pair_cuenta->value;
                fwrite(c->nombreCuenta, sizeof(char), 50, archivo);
                    fwrite(c->password, sizeof(char), 64, archivo);
                    pair_cuenta = map_next(u_int->mapa_claves);
                }
                // CORRECCIÓN AQUÍ: Avanzamos con la variable del ciclo correcto
                pair_u_interno = map_next(user->mapa_claves);
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
        printf("Base de datos vacia.\n");
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
       
        char *salt_recup = (char *) malloc(sizeof(char) * 16);
        fread(salt_recup, sizeof(char), 16, archivo);
        map_insert(mapa_salts, user->nombre, salt_recup);
       
        user->mapa_claves = map_create(is_equal_str);


        int total_usuarios_internos = 0;
        fread(&total_usuarios_internos, sizeof(int), 1, archivo);


        for (int j = 0; j < total_usuarios_internos; j++) {
            usuario *u_int = (usuario *) malloc(sizeof(usuario));
            fread(u_int->nombre, sizeof(char), 50, archivo);
            u_int->mapa_claves = map_create(is_equal_str);


        int total_cuentas = 0;
        fread(&total_cuentas, sizeof(int), 1, archivo);


            for(int k = 0; k < total_cuentas; k++) {
            cuenta *nueva = (cuenta *) malloc(sizeof(cuenta));
            fread(nueva->nombreCuenta, sizeof(char), 50, archivo);
                fread(nueva->password, sizeof(char), 64, archivo);
                map_insert(u_int->mapa_claves, nueva->nombreCuenta, nueva->password);
        }
            map_insert(user->mapa_claves, u_int->nombre, u_int);
        }
        map_insert(mapa_perfiles, user->nombre, user);
    }


    fclose(archivo);
    return 1;
}


void cambiar_clave(Map *cuentas, unsigned char *claveDerivada, List *lista) {
    char nombre_u[50], servicio[50];
    printf("Ingrese el nombre de usuario asociado a la cuenta: ");
    scanf("%49s", nombre_u);
    MapPair *pair_u = map_search(cuentas, nombre_u);
    if (pair_u != NULL){
        usuario *u_int = (usuario *)pair_u->value;
        printf("Ingrese el nombre del servicio / cuenta: ");
        scanf("%49s", servicio);
        MapPair *pair_c = map_search(u_int->mapa_claves, servicio);
        if(pair_c != NULL) {
            cuenta *cuenta_auxi = (cuenta *)pair_c->value;
        int opcion;
        char nueva_clave[50];
       
        printf("desea generar una nueva clave aleatoria o ingresar una nueva clave? \n");
        printf("1. Generar clave aleatoria\n");
        printf("2. Ingresar nueva clave\n");
        scanf("%d", &opcion);
        if (opcion == 1){
            claveAleatoria(nueva_clave, 20);
            printf("Nueva clave generada: %s\n", nueva_clave);
        }
        else if (opcion == 2){
            printf("Ingrese la nueva clave: ");
            scanf("%49s", nueva_clave);
            verificarClave(nueva_clave, lista);
        }
        else{
            printf("Opcion no valida, no se realizo ningun cambio.\n");
            return;
        }


        unsigned char contraCifrada[64] = {0};
        funcionAES256Cifrar(claveDerivada, nueva_clave, contraCifrada);


            memset(cuenta_auxi->password, 0, sizeof(cuenta_auxi->password));
        memcpy(cuenta_auxi->password, contraCifrada, 64);
            printf("Clave cambiada con éxito.\n");
        } else {
            printf("Servicio no encontrado.\n");
        }
    }
    else{
        printf("Usuario no encontrado.\n");
    }
}


int main(){
    limpiarPantalla();
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);


    if (!inicializarCriptografia()) {
        printf("Error al cargar los modulos de seguridad de Windows.\n");
        return 1;
    }


    unsigned char claveDerivada[64] = {0};
    unsigned char valido[64] = {0};



    printf("========================================\n");
    printf("     Bienvenido a MAXIMA SEGURIDAD\n");
    printf("========================================\n");


    Map *mapaUsuarios = NULL;
    List *lista_clavesMasUsadas = cargarClavesMasUsadas();
    Map *mapa_perfiles = map_create(is_equal_str);
    mapa_salts = map_create(is_equal_str);
    usuario *perfil_activo = NULL;
    int resultado = 1;
    char opcion;
    srand(time(NULL));


    int estado = recuperarDatos("datos.dat", mapa_perfiles);


    if (estado == 1){
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
            crear_perfil(mapa_perfiles, claveDerivada, valido);
           
            puts("Por favor, confirma el ingreso al perfil que acabas de crear:");
            perfil_activo = ingresar_perfil(mapa_perfiles, &resultado, claveDerivada);
            if (perfil_activo != NULL) {
                mapaUsuarios = perfil_activo->mapa_claves;
               
                
            }
        }
        else if (strcmp(respuesta, "ingresar")==0){
            perfil_activo = ingresar_perfil(mapa_perfiles, &resultado, claveDerivada);
            if (perfil_activo != NULL) {
                mapaUsuarios = perfil_activo->mapa_claves;
               
            }
        }
        else{
            puts("respuesta no valida, intente de nuevo");
        }


    }while(resultado != 0);


    limpiarPantalla();


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
                crearUsuario(mapaUsuarios, lista_clavesMasUsadas, claveDerivada);
                break;
            case '2':
                buscarContra(mapaUsuarios, claveDerivada) ;
                break;
            case '3':
                cambiar_clave(mapaUsuarios, claveDerivada, lista_clavesMasUsadas);
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
                int repetidas = contrRepetida(clave_rep, mapaUsuarios, claveDerivada);
                printf("La clave se repite %d veces en este perfil.\n", repetidas);
                break;
            }
            case '6':
                claves_mas_usadas(lista_clavesMasUsadas);
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
