/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include "mbr.h"
#include "dar_formato.h"
#include "general.h"
#include "formato_ext3.h"
#include "manejo_archivos.h"
#include "gruposusers.h"


int control;
int seguir;
int tipo0;
int addusers;
char quehueva[96];
//int total_C;

int contar_carpetas(arreglo part) {
    int contador = 0;
    for (int x = 0; x < 25; x++) {
        if (strcmp(part[x].nombre, "vacio") == 0) {
        } else {
            contador++;
        }
    }
    return contador;
}

long int c_n(long int tamano_part) {

    float n = 0;
    n = (tamano_part - sizeof (super_bloque));
    float l = (sizeof (journaling) + 47 + sizeof (inodo) + 8 * sizeof (b_indirecto) + 46 * sizeof (b_archivos));
    float f = n / l;
    //ver si agarra el valor mas pequeño
    return f;
}

void poner_princio(FILE *archivo, long int inicio) {
    fseek(archivo, 0, SEEK_SET);

    for (long int x = 0; x < inicio; x++) {
        fseek(archivo, 1, SEEK_CUR);
    }

}

void saltar_hastainodos(FILE *archivo, long int tamano) { //saltar bitacaroa desps del sb

    long int n = c_n(tamano);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);
    for (long int x = 0; x < n; x++) {
        fseek(archivo, sizeof (journaling), SEEK_CUR);
    }
    for (long int x = 0; x < n; x++) {
        fseek(archivo, 1, SEEK_CUR);
    }
    for (long int x = 0; x < (n * 46); x++) { //bm bloques
        fseek(archivo, 1, SEEK_CUR);
    }



}

void saltar_bmbloques(FILE *archivo, long int tamano) {
    long int n = c_n(tamano);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);
    for (long int x = 0; x < n; x++) {
        fseek(archivo, sizeof (journaling), SEEK_CUR);
    }
    for (long int x = 0; x < n; x++) { //bm inodos
        fseek(archivo, 1, SEEK_CUR);
    }

}

void saltar_carpetas(FILE *archivo, long int tamano) {

    long int n = c_n(tamano);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);
    for (long int x = 0; x < n; x++) {
        fseek(archivo, sizeof (journaling), SEEK_CUR);
    }
    for (long int x = 0; x < n; x++) {
        fseek(archivo, 1, SEEK_CUR);
    }
    for (long int x = 0; x < (n * 46); x++) { //bm bloques
        fseek(archivo, 1, SEEK_CUR);
    }

    for (long int x = 0; x < (n); x++) { //bm bloques
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }

    for (long int x = 0; x < (n * 8); x++) { //bm bloques
        fseek(archivo, sizeof (b_indirecto), SEEK_CUR);
    }


}

int escribir_carpeta2(FILE *archivo, long int inicio, long int tamano, long int ap_inodo, char *nombre) {


    poner_princio(archivo, inicio);
    long int n = c_n(tamano);

    fseek(archivo, sizeof (super_bloque), SEEK_CUR); //sb

    for (long int x = 0; x < n; x++) {

        fseek(archivo, sizeof (journaling), SEEK_CUR); //journaling
    }

    for (long int x = 0; x < n; x++) {
        fseek(archivo, 1, SEEK_CUR); //bminodos
    }

    char byte;
    char c = 'c';

    long int numero = 0;
    long int lugar = 0;
    for (long int x = 0; x < n * 46; x++) {
        fread(&byte, sizeof (byte), 1, archivo);
        lugar++;
        numero++; //nuevo apuntador
        if (byte == '\0') {
            lugar--;
            //escribir en el bitmpa
            fseek(archivo, -1, SEEK_CUR);
            fwrite(&c, sizeof (c), 1, archivo);
            break;

        }

    }

    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);
    for (long int x = 0; x < n; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);

    }
    for (long int x = 0; x < n * 8; x++) {
        fseek(archivo, sizeof (b_indirecto), SEEK_CUR);

    }

    for (long int x = 0; x < lugar; x++) {
        fseek(archivo, sizeof (b_carpetas), SEEK_CUR);

    }
    b_carpetas temporal;
    temporal.apactual = numero;
    temporal.appadre = ap_inodo;

    temporal.hijo1 = -1;
    temporal.hijo2 = -1;
    temporal.hijo3 = -1;

    temporal.hijo4 = -1;
    temporal.hijo5 = -1;
    temporal.hijo6 = -1;


    strcpy(temporal.nombre, nombre);
    strcpy(temporal.padre, nombre);

    fwrite(&temporal, sizeof (b_carpetas), 1, archivo);

    return numero;






}

void escribir_carpeta(FILE *archivo, long int indice, long int inicio, long int tamano, long int actual, long int padre, arreglo carpetas) {

    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano); //inodos o bmcarpetas?
    long int n = calcular_n(tamano);

    for (int x = 0; x < n; x++) { //pos inicial de los inodos
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }
    for (int x = 0; x < n * 8; x++) { //saltando los indiertos
        fseek(archivo, sizeof (b_indirecto), SEEK_CUR);
    }
    b_carpetas temporal;
    temporal.apactual = actual;
    temporal.appadre = padre;
    temporal.hijo1 = -1;
    temporal.hijo2 = -1;
    temporal.hijo3 = -1;
    temporal.hijo4 = -1;
    temporal.hijo5 = -1;
    temporal.hijo6 = -1;

    int t = contar_carpetas(carpetas);

    strcpy(temporal.nombre, carpetas[t - 1].nombre);
    strcpy(temporal.padre, carpetas[t - 1].nombre);
    // aca la cague t-2?






    for (int x = 0; x < indice; x++) { //escribir carpeta

        fseek(archivo, sizeof (b_carpetas), SEEK_CUR);
    }
    fwrite(&temporal, sizeof (b_carpetas), 1, archivo);

    poner_princio(archivo, inicio);
    saltar_bmbloques(archivo, tamano);

    char byte;
    for (int x = 0; x < n * 46; x++) {
        fread(&byte, sizeof (byte), 1, archivo);

        if (byte == '\0') {

            fseek(archivo, -1, SEEK_CUR);
            char c = 'c';
            fwrite(&c, sizeof (c), 1, archivo);
            break;
        }
    }



}

long int escrbir_archivo(FILE *archivo, int padre, char*informacion, long int tamano, long int inicio) {

    long int n = c_n(tamano);
    poner_princio(archivo, inicio);
    saltar_bmbloques(archivo, tamano);

    int contador = 0;
    char byte;
    for (int x = 0; x < n * 46; x++) {

        fread(&byte, sizeof (byte), 1, archivo);
        contador++; //id del bloque de archivo

        if (byte == '\0') {
            fseek(archivo, -1, SEEK_CUR);
            char b = 'a';
            fwrite(&b, sizeof (b), 1, archivo); // ya marque el bitmap
            break;
        }
    }

    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);
    for (int x = 0; x < n; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }
    for (int x = 0; x < n * 8; x++) {
        fseek(archivo, sizeof (b_indirecto), SEEK_CUR);
    }
    for (int x = 0; x < contador - 1; x++) {
        fseek(archivo, sizeof (b_archivos), SEEK_CUR);
    }

    b_archivos temporal;
    temporal.apactual = contador;
    temporal.appadre = padre;
    for (int x = 0; x < 24; x++) {
        temporal.info[x] = '\0';
    }
    strcpy(temporal.info, informacion);

    fwrite(&temporal, sizeof (b_archivos), 1, archivo); //escribir archivo


    for (int fj = 0; fj < 24; fj++) {
        // printf("%c", temporal.info[fj]);
    }
    //printf("\n");
    //escribir


    return contador;

}

long int escribir_inodo_texto(FILE *archivo, long int inicio, long int tamano, arreglo carptas) {

    long int n = c_n(tamano);

    poner_princio(archivo, inicio);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);

    for (int x = 0; x < n; x++) {
        fseek(archivo, sizeof (journaling), SEEK_CUR);
    }
    //-----------------------------------------------bm inodos
    char byte;
    char uno = '1';
    char uno2 = '1';
    char llave_inodo = 0;
    for (int x = 0; x < n; x++) {
        fread(&byte, sizeof (uno), 1, archivo);
        llave_inodo++;
        if (byte == '\0') { //escribir en el bm
            fseek(archivo, -1, SEEK_CUR);
            fwrite(&uno2, sizeof (uno2), 1, archivo);
            break;
        }

    }
    //-----------------------------------------------encontra el inodo libre
    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);

    for (int x = 0; x < llave_inodo - 1; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }
    long int l_para_inodo = ftell(archivo);
    //-------------------------------------lugar donde escribir el inodo


    inodo nuevo;

    nuevo.ad1 = -1;
    nuevo.ad2 = -1;
    nuevo.ad3 = -1;
    nuevo.ad4 = -1;

    nuevo.ap_n1 = -1;
    nuevo.ap_n2 = -1;


    char texto[94];
    for (int x = 0; x < 94; x++) {
        texto[x] = '\0';
    }
    if (addusers == 0) {
        printf("Escribir el contenido del archivo:  \n");
        scanf(" %[^\n]s", texto);

    } else if (addusers == 1) {
        strcpy(texto, quehueva);

    }

    int otro = strlen(texto);
    float veces = otro / 24.0;
    //------------------ fecha
    char fe[200];
    struct tm fecha = *localtime(&(time_t){time(NULL)});
    strcpy(fe, asctime(&fecha));
    //--------------------

    nuevo.bloues_asignados = veces;
    strcpy(nuevo.fecha, fe);
    int xxx = contar_carpetas(carptas);
    strcpy(nuevo.idchar, carptas[xxx - 1].nombre);

    nuevo.llave = llave_inodo;
    nuevo.permisos = 664;
    strcpy(nuevo.propietario, "vacio");
    nuevo.tamano = strlen(texto);
    nuevo.tipo = 1;



    int contador = 0;
    int totalcadena = strlen(texto);
    char auxiliar[24];



    for (int x = 0; x < veces; x++) {

        for (int l = 0; l < 24; l++) {
            auxiliar[l] = '\0';
        }

        for (int y = 0; y < 24; y++) {

            auxiliar[y] = texto[contador];
            contador++;

            if (contador >= totalcadena) {

                break;
            }

        }

        char otrossss[24];
        strcpy(otrossss, auxiliar); //tenia aglo comentado pero esta en el bacup


        int nuevopuntador = escrbir_archivo(archivo, llave_inodo, otrossss, tamano, inicio);

        if (nuevo.ad1 == -1) {
            nuevo.ad1 = nuevopuntador;
        } else if (nuevo.ad2 == -1) {
            nuevo.ad2 = nuevopuntador;
        } else if (nuevo.ad3 == -1) {
            nuevo.ad3 = nuevopuntador;
        } else if (nuevo.ad4 == -1) {
            nuevo.ad4 = nuevopuntador;
        }

    }
    fseek(archivo, 0, SEEK_SET);
    for (long int x = 0; x < l_para_inodo; x++) {
        fseek(archivo, 1, SEEK_CUR);
    }
    fwrite(&nuevo, sizeof (inodo), 1, archivo);




    return llave_inodo;

}

long int escribir_inodo(FILE *archivo, long int inicio, long int tamano, arreglo carptas) {

    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano); // hasta en inodos
    long int n = c_n(tamano);
    long int lugar_escribir = 0;

    int encontrad = 0;
    inodo temporal;
    long int contador = 0;
    for (long int x = 0; x < n; x++) {

        fread(&temporal, sizeof (inodo), 1, archivo);
        contador++; //nueva llave

        if (temporal.llave == -1) {
            encontrad = 1;
            fseek(archivo, sizeof (inodo)*-1, SEEK_CUR);
            lugar_escribir = ftell(archivo); //pos del inodo donde se va escribir
            break;
        }


    }


    poner_princio(archivo, inicio);
    saltar_bmbloques(archivo, tamano); /// por aca voy bm bloques

    int cbm = 0;

    char byte;
    for (int x = 0; x < n * 46; x++) { //buscar llave de la nueva carpeta

        fread(&byte, sizeof (byte), 1, archivo);
        cbm++;
        if (byte == '\0') {//el otro es la lalve aca son las posiciones
            cbm--; // de la carpeta
            fseek(archivo, sizeof (byte)*-1, SEEK_CUR);
            escribir_carpeta(archivo, cbm, inicio, tamano, cbm + 1, contador, carptas);
            break;
        }

    }

    inodo nuevo;
    nuevo.ad1 = cbm + 1;
    nuevo.ad2 = -1;
    nuevo.ad3 = -1;
    nuevo.ad4 = -1;

    nuevo.ap_n1 = -1;
    nuevo.ap_n2 = -1;

    int xxx = contar_carpetas(carptas);
    nuevo.bloues_asignados = 1;
    //----------------- fecha
    char fe[200];
    struct tm fecha = *localtime(&(time_t){time(NULL)});
    strcpy(fe, asctime(&fecha));
    //----------------
    strcpy(nuevo.fecha, fe);

    strcpy(nuevo.idchar, carptas[xxx - 1].nombre);

    printf("%s--\n", nuevo.idchar);
    nuevo.llave = contador;
    nuevo.permisos = 664;

    strcpy(nuevo.propietario, "vacio");
    nuevo.tamano = 0;
    nuevo.tipo = 0;


    fseek(archivo, 0, SEEK_SET);
    for (long int x = 0; x < lugar_escribir; x++) { //de los inodos

        fseek(archivo, 1, SEEK_CUR);
    }

    fwrite(&nuevo, sizeof (inodo), 1, archivo);

    //marcar bm inodos
    poner_princio(archivo, inicio);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);

    for (int x = 0; x < n; x++) {
        fseek(archivo, sizeof (journaling), SEEK_CUR);

    } //------------------bm de los inodos puta que vergueo el que me eche

    char byteoro;
    for (int x = 0; x < n; x++) {
        fread(&byteoro, sizeof (byteoro), 1, archivo);
        if (byteoro == '\0') {

            fseek(archivo, -1, SEEK_CUR);
            char uno = '1';
            fwrite(&uno, sizeof (uno), 1, archivo);

            break;
        }

    }





    //marcar bm carpetas

    return contador;

}

int buscar_carpeta(int apuntador, FILE *archivo, long int inicio, long int tamano, int tipo, arreglo carpetas) {
    int sdfasd = ftell(archivo);
    poner_princio(archivo, inicio);
    sdfasd = ftell(archivo);
    long int lugar_escribir = 0;
    int hacer = 0;

    saltar_hastainodos(archivo, tamano);
    long int n = c_n(tamano);

    sdfasd = ftell(archivo);

    for (long int x = 0; x < n; x++) {

        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }
    sdfasd = ftell(archivo);

    for (long int x = 0; x < (n * 8); x++) {
        fseek(archivo, sizeof (b_indirecto), SEEK_CUR);
    }
    sdfasd = ftell(archivo);
    // b_archivos ta;
    b_carpetas tc;

    int encontro = 0;

    int prono = 0;
    for (long int x = 0; x < (n * 46); x++) { //encontre carpeta

        fseek(archivo, sizeof (b_carpetas), SEEK_CUR);
        prono++;

        if (prono == apuntador) {


            fseek(archivo, sizeof (b_carpetas)*-1, SEEK_CUR);
            fread(&tc, sizeof (b_carpetas), 1, archivo);
            //----------------------------------------------------
            if (tc.apactual == apuntador) {
                fseek(archivo, sizeof (b_carpetas)*-1, SEEK_CUR);
                encontro = 1;
                lugar_escribir = ftell(archivo);
                break;
            }
        }

    }

    int totc = contar_carpetas(carpetas);

    if (control < totc - 1) { //


        if (tc.hijo1 != -1 && seguir == 1 && encontro == 1) { //-------------------------------

            buscar_inodo(tc.hijo1, archivo, inicio, tamano, carpetas);
            //------------------------asdfasdf

        }


        if (tc.hijo2 != -1 && seguir == 1 && encontro == 1) {

            buscar_inodo(tc.hijo2, archivo, inicio, tamano, carpetas);
        }

        if (tc.hijo3 != -1 && seguir == 1 && encontro == 1) {
            buscar_inodo(tc.hijo3, archivo, inicio, tamano, carpetas);

        }
        if (tc.hijo4 != -1 && seguir == 1 && encontro == 1) {
            buscar_inodo(tc.hijo4, archivo, inicio, tamano, carpetas);

        }

        if (tc.hijo5 != -1 && seguir == 1 && encontro == 1) {
            buscar_inodo(tc.hijo5, archivo, inicio, tamano, carpetas);

        }

        if (tc.hijo6 != -1 && seguir == 1 && encontro == 1) {
            buscar_inodo(tc.hijo6, archivo, inicio, tamano, carpetas);

        }

        if (tc.hijo1 == -1 && tc.hijo2 == -1 && tc.hijo3 == -1 && tc.hijo4 == -1 && tc.hijo5 == -1 && tc.hijo6 == -1) {

            printf("la carpeta esta vacia");

        }


    } else {


        if (encontro == 1) {
            char nigua[20];
            int tyoe = 0;

            for (int x = 0; x < 20; x++) {
                nigua[x] = '\0';
            }

            int xxx = contar_carpetas(carpetas);
            strcpy(nigua, carpetas[xxx - 1].nombre);
            int i = strlen(nigua);
            //-----------------------------
            for (int x = 0; x < strlen(nigua); x++) {

                if (nigua[x] == 46) {

                    tyoe = 1;
                    break;
                }



            }
            tipo0 = tyoe; //lo  acabo de agregar



            if (tc.hijo1 == -1 && encontro == 1) { //si un inodo de bloques o un inodo de 
                if (tyoe == 0) {
                    tc.hijo1 = escribir_inodo(archivo, inicio, tamano, carpetas);
                } else if (tyoe == 1) {

                    tc.hijo1 = escribir_inodo_texto(archivo, inicio, tamano, carpetas);
                    //---------------ACA AJKLÑDFJAÑSDKFASDFASD

                }
                seguir = 0;
            } else if (tc.hijo2 == -1 && encontro == 1) {
                if (tyoe == 0) {
                    tc.hijo2 = escribir_inodo(archivo, inicio, tamano, carpetas);
                } else if (tyoe == 1) {
                    tc.hijo2 = escribir_inodo_texto(archivo, inicio, tamano, carpetas);

                }
                seguir = 0;
            } else if (tc.hijo3 == -1 && encontro == 1) {
                if (tyoe == 0) {
                    tc.hijo3 = escribir_inodo(archivo, inicio, tamano, carpetas);
                } else if (tyoe == 1) {
                    tc.hijo3 = escribir_inodo_texto(archivo, inicio, tamano, carpetas);

                }
                seguir = 0;

            } else if (tc.hijo4 == -1 && encontro == 1) {
                if (tyoe == 0) {
                    tc.hijo4 = escribir_inodo(archivo, inicio, tamano, carpetas);
                } else if (tyoe == 1) {

                    tc.hijo4 = escribir_inodo_texto(archivo, inicio, tamano, carpetas);

                }
                seguir = 0;
            } else if (tc.hijo5 == -1 && encontro == 1) {
                if (tyoe == 0) {
                    tc.hijo5 = escribir_inodo(archivo, inicio, tamano, carpetas);
                } else if (tyoe == 1) {

                    tc.hijo5 = escribir_inodo_texto(archivo, inicio, tamano, carpetas);

                }
                seguir = 0;
            } else if (tc.hijo6 == -1 && encontro == 1) {
                if (tyoe == 0) {
                    tc.hijo6 = escribir_inodo(archivo, inicio, tamano, carpetas);
                } else if (tyoe == 1) {
                    tc.hijo6 = escribir_inodo_texto(archivo, inicio, tamano, carpetas);

                }
                seguir = 0;

            }


            //reescribir esta carpeta

            if (encontro == 1 && seguir == 0) {
                fseek(archivo, 0, SEEK_SET);
                for (int x = 0; x < lugar_escribir; x++) {
                    fseek(archivo, 1, SEEK_CUR);
                }
                fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                if (tc.hijo1 != -1 && tc.hijo2 != -1 && tc.hijo3 != -1 && tc.hijo4 != -1 && tc.hijo5 != -1 && tc.hijo6 != -1) {

                    hacer = 1;

                    //crear carpeta nueva para el otro apuntador del inodo
                }

            }

        }

    }



    return hacer;

}

void buscar_inodo(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas) {

    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);
    int encontrado = 0;
    long int n = c_n(tamano) - 1;
    inodo temporal;
    char nombre[20];
    strcpy(nombre, carpetas[control].nombre);
    control++;
    long int aca = 0;




    for (long int x = 0; x < n; x++) {

        fread(&temporal, sizeof (inodo), 1, archivo);

        if (temporal.llave == apuntador && strcmp(nombre, temporal.idchar) == 0) {
            encontrado = 1;
            fseek(archivo, sizeof (inodo)*-1, SEEK_CUR);
            aca = ftell(archivo);
            break;
        }

    }

    if (encontrado == 1) {

        if (temporal.ad1 != -1 && seguir == 1) {


            if (temporal.tipo == 0) {
                int ha = buscar_carpeta(temporal.ad1, archivo, inicio, tamano, temporal.tipo, carpetas);
                //crear carpeta
                if (ha == 1 && seguir == 0 && temporal.ad2 == -1) {
                    temporal.ad2 = escribir_carpeta2(archivo, inicio, tamano, apuntador, nombre);

                    fseek(archivo, 0, SEEK_SET);
                    for (long int l = 0; l < aca; l++) {

                        fseek(archivo, 1, SEEK_CUR);
                    }
                    fwrite(&temporal, sizeof (inodo), 1, archivo);
                }
            } else if (temporal.tipo == 1) {

            }
            //rescribir inodo



        }

        if (temporal.ad2 != -1 && seguir == 1) { //crear nueva carpeta


            if (temporal.tipo == 0) {
                int ha = buscar_carpeta(temporal.ad2, archivo, inicio, tamano, temporal.tipo, carpetas);
                if (ha == 1 && seguir == 0 && temporal.ad3 == -1) {
                    //--escribir carpeta
                    temporal.ad3 = escribir_carpeta2(archivo, inicio, tamano, apuntador, nombre);

                    fseek(archivo, 0, SEEK_SET);
                    for (long int l = 0; l < aca; l++) {

                        fseek(archivo, 1, SEEK_CUR);
                    }
                    fwrite(&temporal, sizeof (inodo), 1, archivo);

                }
            } else if (temporal.tipo == 1) {

            }

        }

        if (temporal.ad3 != -1 && seguir == 1) {

            if (temporal.tipo == 0) {
                int ha = buscar_carpeta(temporal.ad3, archivo, inicio, tamano, temporal.tipo, carpetas);
                if (ha == 1 && seguir == 0 && temporal.ad4 == -1) {
                    temporal.ad4 = escribir_carpeta2(archivo, inicio, tamano, apuntador, nombre);

                    fseek(archivo, 0, SEEK_SET);
                    for (long int l = 0; l < aca; l++) {
                        fseek(archivo, 1, SEEK_CUR);
                    }
                    fwrite(&temporal, sizeof (inodo), 1, archivo);
                }

            } else if (temporal.tipo == 1) {

            }
        }

        if (temporal.ad4 != -1 && seguir == 1) {

            if (temporal.tipo == 0) {

                int ha = buscar_carpeta(temporal.ad4, archivo, inicio, tamano, temporal.tipo, carpetas);
                if (ha == 1) {
                    printf("se acabaron los apuntadores directos del inodo \n");
                }
            } else if (temporal.tipo == 1) {

            }

        }

    } else {

        control--;
    }

}

void escribir_journaling(FILE *archivo, long int n, long int inicio, arreglo carpetas) {
    poner_princio(archivo, inicio);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);


    for (int x = 0; x < n; x++) {
        journaling temporal;
        fread(&temporal, sizeof (journaling), 1, archivo);
        if (strcmp(temporal.fecha, "vacio") == 0) {
            fseek(archivo, sizeof (journaling)*-1, SEEK_CUR);

            journaling temporal2;
            char fe[200];
            struct tm fecha = *localtime(&(time_t){time(NULL)});
            strcpy(fe, asctime(&fecha));
            if (tipo0 == 0) {
                strcpy(temporal2.fecha, fe);
                int xxx = contar_carpetas(carpetas);
                char nuevo[150];
                memset(nuevo, '\0', 150);
                sprintf(nuevo, "Se creo la carpeta: %s", carpetas[xxx - 1].nombre);
                strcpy(temporal2.operacion, nuevo);
                fwrite(&temporal2, sizeof (journaling), 1, archivo);
                printf("Se actualizo el journaling \n");

            } else if (tipo0 == 1) {
                strcpy(temporal2.fecha, fe);
                int xxx = contar_carpetas(carpetas);
                char nuevo[150];
                memset(nuevo, '\0', 150);
                sprintf(nuevo, "Se creo el archivo: %s", carpetas[xxx - 1].nombre);
                strcpy(temporal2.operacion, nuevo);
                fwrite(&temporal2, sizeof (journaling), 1, archivo);
                printf("Se actualizo el journaling \n");
            }


            break;
        }
    }


}

void crear_carpeta(int numero, idspart ids, arreglo carpetas) {

    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    super_bloque sb;
    inodo tinodo;
    b_carpetas tcapreta;
    b_archivos tarchivo;
    int total_carpetas = contar_carpetas(carpetas);


    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;


    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;

    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;

    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;

    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;

    }
    long int n = c_n(tamano);
    total_carpetas = contar_carpetas(carpetas) - 1;

    FILE *archivo = fopen(ids[numero].ruta, "r+b");
    if (archivo) {
        control = 0;
        seguir = 1;
        tipo0 = 0;
        addusers = 0;
        //control ++;
        buscar_inodo(1, archivo, inicio, tamano, carpetas);
        //journaling
        escribir_journaling(archivo, n, inicio, carpetas);


        fclose(archivo);

    }

}

int que_es(long int apuntador, long int tamano, long int inicio, FILE *disco) {
    poner_princio(disco, inicio);
    saltar_hastainodos(disco, tamano);
    calcular_n(tamano);
    inodo temporal;
    for (int x = 0; x < apuntador; x++) {
        fread(&temporal, sizeof (inodo), 1, disco);
    }

    if (temporal.tipo == 0) {
        return 0; //carpeta

    } else if (temporal.tipo == 1) {

        return 1;
    } else if (temporal.tipo == -1) {

        return -1;
    }

    return -1;
}

void agregarusuarios(int numero, idspart ids, arreglo carpetas, grupos gu, usuarios us) {

    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    int total_carpetas = contar_carpetas(carpetas);

    addusers = 1;
    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;


    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;

    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;

    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;

    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;

    }
    long int n = c_n(tamano);
    total_carpetas = contar_carpetas(carpetas) - 1;

    FILE *archivo = fopen(ids[numero].ruta, "r+b");
    if (archivo) {
        control = 0;
        seguir = 1;
        tipo0 = 0;
        addusers = 1;
        memset(quehueva, 96, '\0');
        printf("1. crear usuarios \n");
        printf("2. crear grupo \n");
        int dato = 0;
        scanf("%i", &dato);

        if (dato == 1) {

            char usuario[20];
            char grupo[20];
            char contra [20];
            memset(usuario, 20, '\0');
            memset(grupo, 20, '\0');
            memset(contra, 20, '\0');

            printf("1. Ingrese el nombre del grupo al que pertenecer: \n");
            scanf("%s", grupo);
            printf("2. Ingrese el nombre del usuario: \n");
            scanf("%s", usuario);
            printf("3. Ingrese la contrasena: \n");
            scanf("%s", contra);



            sprintf(quehueva, "1,U,%s,%s,%s:", usuario, grupo, contra);
            buscar_inodo(1, archivo, inicio, tamano, carpetas);
            for (int x = 0; x < 100; x++) {
                if (strcmp(us[x].contra, "vacio") == 0) {
                    strcpy(us[x].contra, contra);
                    strcpy(us[x].grupo, grupo);
                    strcpy(us[x].nombre, usuario);

                    break;
                }

            }

        } else if (dato == 2) {


            char usuario[20];
            memset(usuario, 20, '\0');
            printf("1. Ingrese el nombre del grupo: \n");
            scanf("%s", usuario);

            sprintf(quehueva, "1,G,%s:", usuario);

            buscar_inodo(1, archivo, inicio, tamano, carpetas);


            for (int x = 0; x < 100; x++) {
                if (strcmp(gu[x].nombre, "vacio") == 0) {

                    strcpy(gu[x].nombre, usuario);

                    break;
                }

            }

        }





        fclose(archivo);

    }



}


//-----------------------------------------------------------------------------------GRAFICAS

void bus_carpetar1(int apuntador, FILE *disco, long int inicio, long int tamango, FILE *grafica, int primero) {
    poner_princio(disco, inicio);
    saltar_hastainodos(disco, tamango);
    long int n = c_n(tamango);

    for (int x = 0; x < n; x++) {
        fseek(disco, sizeof (inodo), SEEK_CUR);
    }

    for (int x = 0; x < n * 8; x++) {
        fseek(disco, sizeof (b_indirecto), SEEK_CUR);
    }


    int prono = 0;
    b_carpetas temporal;
    for (int x = 0; x < n * 46; x++) {
        fseek(disco, sizeof (b_carpetas), SEEK_CUR);


        prono++;

        if (prono == apuntador) {

            fseek(disco, sizeof (b_carpetas)*-1, SEEK_CUR);
            fread(&temporal, sizeof (b_carpetas), 1, disco);

            if (temporal.hijo1 != -1 && que_es(temporal.hijo1, tamango, inicio, disco) == 0) {
                fprintf(grafica, "node%i->node%i;\n", primero, temporal.hijo1);
                //  b_inodor1(temporal.hijo1,disco, inicio, tamango);
            }
            if (temporal.hijo2 != -1 && que_es(temporal.hijo2, tamango, inicio, disco) == 0) {
                fprintf(grafica, "node%i->node%i;\n", primero, temporal.hijo2);
                // b_inodor1(temporal.hijo2,disco, inicio, tamango);

            }
            if (temporal.hijo3 != -1 && que_es(temporal.hijo3, tamango, inicio, disco) == 0) {
                fprintf(grafica, "node%i->node%i;\n", primero, temporal.hijo3);
                // b_inodor1(temporal.hijo3,disco, inicio, tamango);
            }
            if (temporal.hijo4 != -1 && que_es(temporal.hijo4, tamango, inicio, disco) == 0) {
                fprintf(grafica, "node%i->node%i;\n", primero, temporal.hijo4);
                //b_inodor1(temporal.hijo4,disco, inicio, tamango);
            }
            if (temporal.hijo5 != -1 && que_es(temporal.hijo5, tamango, inicio, disco) == 0) {
                fprintf(grafica, "node%i->node%i;\n", primero, temporal.hijo5);
                //  b_inodor1(temporal.hijo5,disco, inicio, tamango);

            }
            if (temporal.hijo6 != -1 && que_es(temporal.hijo6, tamango, inicio, disco) == 0) {
                fprintf(grafica, "node%i->node%i;\n", primero, temporal.hijo6);
                //   b_inodor1(temporal.hijo6,disco, inicio, tamango);

            }

            break;
        }

    }







}

void g_enlacesr1(FILE *disco, FILE *grafica, int inicio, int tamano) {

    fseek(disco, 0, SEEK_END);
    poner_princio(disco, inicio);
    saltar_hastainodos(disco, tamano);
    long int n = c_n(tamano);

    for (int x = 0; x < n; x++) {
        inodo temporal;
        fread(&temporal, sizeof (inodo), 1, disco);

        if (temporal.llave != -1 && temporal.tipo == 0) {
            // printf("%s\n", temporal.idchar);
            long int pos_actual = ftell(disco);
            if (temporal.ad1 != -1) {

                bus_carpetar1(temporal.ad1, disco, inicio, tamano, grafica, temporal.llave);

            }

            if (temporal.ad2 != -1) {
                bus_carpetar1(temporal.ad2, disco, inicio, tamano, grafica, temporal.llave);

            }
            if (temporal.ad3 != -1) {
                bus_carpetar1(temporal.ad3, disco, inicio, tamano, grafica, temporal.llave);

            }
            if (temporal.ad4 != -1) {

                bus_carpetar1(temporal.ad4, disco, inicio, tamano, grafica, temporal.llave);

            }

            fseek(disco, pos_actual, SEEK_SET);
        }

    }

}

void g_carpetasr1(FILE *disco, FILE *grafica, long int inicio, long int tamano) {
    fseek(disco, 0, SEEK_END);
    poner_princio(disco, inicio);
    saltar_hastainodos(disco, tamano);

    inodo temporal;
    long int n = c_n(tamano);

    for (int x = 0; x < n; x++) {

        fread(&temporal, sizeof (inodo), 1, disco);
        if (temporal.llave != -1 && temporal.tipo == 0) { //solo carpetas


            fprintf(grafica, "node%i[shape=none, margin=0, label=<\n", temporal.llave);
            fprintf(grafica, "<TABLE> \n");

            fprintf(grafica, "<TR> \n");

            if (strcmp(temporal.idchar, "/") == 0) {
                fprintf(grafica, "<TD>root</TD> \n");
            } else {
                fprintf(grafica, "<TD>%s</TD>\n", temporal.idchar);
            }

            fprintf(grafica, "</TR> \n");

            fprintf(grafica, "</TABLE>>]\n");
        }

    }

}

void grafiacr_carpetas_subcarpetasr1(int numero, idspart ids) {

    mbr temporal = recuperar_mbr(ids[numero].ruta);
    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;

    if (option == 1) {
        inicio = temporal.par[0].inicio;
        tamano = temporal.par[0].tamano;

    } else if (option == 2) {
        inicio = temporal.par[1].inicio;
        tamano = temporal.par[1].tamano;

    } else if (option == 3) {
        inicio = temporal.par[2].inicio;
        tamano = temporal.par[2].tamano;

    } else if (option == 4) {
        inicio = temporal.par[3].inicio;
        tamano = temporal.par[3].tamano;

    }

    FILE* file = fopen("juego.dot", "w");
    fprintf(file, "digraph juego { \n");
    fprintf(file, "node [shape=plaintext] \n");
    //fprintf(file, "rankdir=LR \n");

    FILE *disco = fopen(ids[numero].ruta, "r+b");

    if (disco) {

        g_carpetasr1(disco, file, inicio, tamano);
        g_enlacesr1(disco, file, inicio, tamano);
        // g_enlacesr1(disco,file, inicio, tamano);
    }

    fprintf(file, "}\n");
    fclose(file);
    fclose(disco);
    system("dot -Tpng juego.dot -o juego.png");
    system("xdg-open juego.png");

}

//--------------------------------------grafica de archivos

char antiguo[20];
int antiguallave;

b_archivos recu_bloques_archivos(int apuntador, FILE *archivo, long int inicio, long int tamano) {
    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);
    long int n = c_n(tamano);

    for (int x = 0; x < n; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }

    for (int x = 0; x < n * 8; x++) {
        fseek(archivo, sizeof (b_indirecto), SEEK_CUR);
    }


    for (int x = 0; x < apuntador - 1; x++) {
        fseek(archivo, sizeof (b_archivos), SEEK_CUR);
    }
    b_archivos temporal;
    fread(&temporal, sizeof (b_archivos), 1, archivo);
    //generar dot 
    return temporal;


}

inodo buscar_inodor3(int apuntador, FILE *disco, long int inicio, long int tamano) {
    inodo temporal;
    poner_princio(disco, inicio);
    saltar_hastainodos(disco, tamano);

    for (int x = 0; x < apuntador - 1; x++) {
        fseek(disco, sizeof (inodo), SEEK_CUR);
    }

    fread(&temporal, sizeof (inodo), 1, disco);

    if (temporal.llave == apuntador) {



        return temporal;
    }

    temporal.llave = -1;

    return temporal;
}

void buscar_carpetar3(int apuntador, FILE *disco, long int inicio, long int tamano, FILE *grafica, int nografica) {
    poner_princio(disco, inicio);
    saltar_carpetas(disco, tamano);
    for (int x = 0; x < apuntador - 1; x++) {
        fseek(disco, sizeof (b_carpetas), SEEK_CUR);

    }
    b_carpetas temporal;
    fread(&temporal, sizeof (b_carpetas), 1, disco);

    if (temporal.apactual == apuntador) {

        inodo t1;
        inodo t2;
        inodo t3;
        inodo t4;
        inodo t5;
        inodo t6;

        t1.tipo = -1;
        t2.tipo = -1;
        t3.tipo = -1;
        t4.tipo = -1;
        t5.tipo = -1;
        t6.tipo = -1;

        strcpy(t1.idchar, "vacio");
        strcpy(t2.idchar, "vacio");

        strcpy(t3.idchar, "vacio");
        strcpy(t4.idchar, "vacio");

        strcpy(t5.idchar, "vacio");
        strcpy(t6.idchar, "vacio");

        if (temporal.hijo1 != -1) {
            t1 = buscar_inodor3(temporal.hijo1, disco, inicio, tamano);
        }

        if (temporal.hijo2 != -1) {
            t2 = buscar_inodor3(temporal.hijo2, disco, inicio, tamano);
        }

        if (temporal.hijo3 != -1) {

            t3 = buscar_inodor3(temporal.hijo3, disco, inicio, tamano);
        }

        if (temporal.hijo4 != -1) {

            t4 = buscar_inodor3(temporal.hijo4, disco, inicio, tamano);
        }

        if (temporal.hijo5 != -1) {

            t5 = buscar_inodor3(temporal.hijo5, disco, inicio, tamano);
        }

        if (temporal.hijo6 != -1) {

            t6 = buscar_inodor3(temporal.hijo6, disco, inicio, tamano);
        }

        fprintf(grafica, "nodep%i[shape=none, margin=0, label=<\n", nografica);
        fprintf(grafica, "<TABLE>\n");
        fprintf(grafica, "<TR><TD>Contenido:</TD></TR>\n");
        fprintf(grafica, "<TR><TD>TIPO: %i</TD><TD>%s</TD></TR>\n", t1.tipo, t1.idchar);
        fprintf(grafica, "<TR><TD>TIPO: %i</TD><TD>%s</TD></TR>\n", t2.tipo, t2.idchar);
        fprintf(grafica, "<TR><TD>TIPO: %i</TD><TD>%s</TD></TR>\n", t3.tipo, t3.idchar);
        fprintf(grafica, "<TR><TD>TIPO: %i</TD><TD>%s</TD></TR>\n", t4.tipo, t4.idchar);
        fprintf(grafica, "<TR><TD>TIPO: %i</TD><TD>%s</TD></TR>\n", t5.tipo, t5.idchar);
        fprintf(grafica, "<TR><TD>TIPO: %i</TD><TD>%s</TD></TR>\n", t6.tipo, t6.idchar);
        fprintf(grafica, "</TABLE>>]\n");

    }


}

void g_existe_carpetar3(int apuntador, char*nombre, FILE *archivo, long int inicio, long int tamano, FILE *grafica) {


    /// se recibe un inodo
    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);



    for (int x = 0; x < apuntador - 1; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }

    inodo temporal;
    fread(&temporal, sizeof (inodo), 1, archivo);
    if (temporal.llave == apuntador && strcmp(temporal.idchar, nombre) == 0 && seguir == 1) {
        printf("Archivo encontrado: \n");
        strcpy(antiguo, temporal.idchar);
        antiguallave = temporal.llave;
        printf("%s \n", temporal.idchar);
        seguir = 0;

        //------------------------------------- nodo

        fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave);
        fprintf(grafica, "<TABLE> \n");
        fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
        fprintf(grafica, "</TABLE>>]\n");

        //-------------------------------------

        if (temporal.ad1 != -1) {
            buscar_carpetar3(temporal.ad1, archivo, inicio, tamano, grafica, 1);
            fprintf(grafica, "nodei%i->nodep%i;\n", temporal.llave, 1);


        }

        if (temporal.ad2 != -1) {

            buscar_carpetar3(temporal.ad2, archivo, inicio, tamano, grafica, 2);
            fprintf(grafica, "nodei%i->nodep%i;\n", temporal.llave, 2);

        }

        if (temporal.ad3 != -1) {

            buscar_carpetar3(temporal.ad3, archivo, inicio, tamano, grafica, 3);
            fprintf(grafica, "nodei%i->nodep%i;\n", temporal.llave, 3);
        }

        if (temporal.ad4 != -1) {

            buscar_carpetar3(temporal.ad4, archivo, inicio, tamano, grafica, 4);

            fprintf(grafica, "nodei%i->nodep%i;\n", temporal.llave, 4);
        }




    }
}

void existe_texto(int apuntador, char *nombre, FILE *archivo, long int inicio, long int tamano, FILE *grafica) {
    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);
    long int n = c_n(tamano);


    for (int x = 0; x < apuntador - 1; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }

    inodo temporal;
    fread(&temporal, sizeof (inodo), 1, archivo);
    if (temporal.llave == apuntador && strcmp(temporal.idchar, nombre) == 0 && seguir == 1) {
        printf("Archivo encontrado: \n");
        strcpy(antiguo, temporal.idchar);
        antiguallave = temporal.llave;
        printf("%s \n", temporal.idchar);
        seguir = 0;

        //-------------------------------------nodo dl antiguo


        fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave);
        fprintf(grafica, "<TABLE> \n");
        fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
        fprintf(grafica, "</TABLE>>]\n");

        //---------------------------------------


        b_archivos temporal1;
        b_archivos temporal2;
        b_archivos temporal3;
        b_archivos temporal4;

        temporal1.apactual = -1;
        temporal2.apactual = -1;
        temporal3.apactual = -1;
        temporal4.apactual = -1;


        if (temporal.ad1 != -1) {

            temporal1 = recu_bloques_archivos(temporal.ad1, archivo, inicio, tamano);

        }

        if (temporal.ad2 != -1) {
            temporal2 = recu_bloques_archivos(temporal.ad2, archivo, inicio, tamano);

        }

        if (temporal.ad3 != -1) {

            temporal3 = recu_bloques_archivos(temporal.ad3, archivo, inicio, tamano);

        }

        if (temporal.ad4 != -1) {
            temporal4 = recu_bloques_archivos(temporal.ad4, archivo, inicio, tamano);

        }

        fprintf(grafica, "nodet1[shape=none, margin=0, label=< \n");
        fprintf(grafica, "<TABLE> \n");
        fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);

        fprintf(grafica, "<TR><TD>Punteros</TD></TR>\n");

        fprintf(grafica, "<TR><TD>Directo</TD><TD PORT=\"uno\">%i</TD></TR>\n", temporal.ad1);
        fprintf(grafica, "<TR><TD>Directo</TD><TD PORT=\"dos\">%i</TD></TR>\n", temporal.ad2);
        fprintf(grafica, "<TR><TD>Directo</TD><TD PORT=\"tres\">%i</TD></TR>\n", temporal.ad3);
        fprintf(grafica, "<TR><TD>Directo</TD><TD PORT=\"cuatro\">%i</TD></TR>\n", temporal.ad4);

        fprintf(grafica, "<TR><TD>Indirecto n1</TD><TD>%i</TD></TR>\n", -1);
        fprintf(grafica, "<TR><TD>Indirecto n2</TD><TD>%i</TD></TR>\n", -1);

        fprintf(grafica, "</TABLE>>]\n");
        //---------------------------------------------------------------
        fprintf(grafica, "nodei%i->nodet1;\n", temporal.llave);

        if (temporal1.apactual != -1) {

            fprintf(grafica, "nodea1[shape=none, margin=0, label=< \n"); //si hago lo de los indirectors usar el numeros de llaves
            fprintf(grafica, "<TABLE> \n");
            fprintf(grafica, "<TR><TD>Informacion</TD></TR>\n");
            fprintf(grafica, "<TR><TD>Apuntador: %i</TD></TR>\n", temporal1.apactual);
            fprintf(grafica, "<TR><TD>");
            for (int x = 0; x < 24; x++) {

                if (temporal1.info[x] == '\0') {
                } else {
                    fprintf(grafica, "%c", temporal1.info[x]);
                }

            }
            fprintf(grafica, "</TD></TR>");
            fprintf(grafica, "</TABLE>>]\n");


            fprintf(grafica, "nodet1:uno->nodea1; \n");
        }


        if (temporal2.apactual != -1) {

            fprintf(grafica, "nodea2[shape=none, margin=0, label=< \n"); //si hago lo de los indirectors usar el numeros de llaves
            fprintf(grafica, "<TABLE> \n");
            fprintf(grafica, "<TR><TD>Informacion</TD></TR>\n");
            fprintf(grafica, "<TR><TD>Apuntador: %i</TD></TR>\n", temporal2.apactual);
            fprintf(grafica, "<TR><TD>");
            for (int x = 0; x < 24; x++) {

                if (temporal2.info[x] == '\0') {
                } else {
                    fprintf(grafica, "%c", temporal2.info[x]);
                }

            }
            fprintf(grafica, "</TD></TR>");
            fprintf(grafica, "</TABLE>>]\n");
            fprintf(grafica, "nodet1:dos->nodea2; \n");
        }


        if (temporal3.apactual != -1) {

            fprintf(grafica, "nodea3[shape=none, margin=0, label=< \n"); //si hago lo de los indirectors usar el numeros de llaves
            fprintf(grafica, "<TABLE> \n");
            fprintf(grafica, "<TR><TD>Informacion</TD></TR>\n");
            fprintf(grafica, "<TR><TD>Apuntador: %i</TD></TR>\n", temporal3.apactual);
            fprintf(grafica, "<TR><TD>");
            for (int x = 0; x < 24; x++) {

                if (temporal3.info[x] == '\0') {
                } else {
                    fprintf(grafica, "%c", temporal3.info[x]);
                }

            }
            fprintf(grafica, "</TD></TR>");
            fprintf(grafica, "</TABLE>>]\n");
            fprintf(grafica, "nodet1:tres->nodea3; \n");

        }


        if (temporal4.apactual != -1) {

            fprintf(grafica, "nodea4[shape=none, margin=0, label=< \n"); //si hago lo de los indirectors usar el numeros de llaves
            fprintf(grafica, "<TABLE> \n");
            fprintf(grafica, "<TR><TD>Informacion</TD></TR>\n");
            fprintf(grafica, "<TR><TD>Apuntador: %i</TD></TR>\n", temporal4.apactual);
            fprintf(grafica, "<TR><TD>");
            for (int x = 0; x < 24; x++) {

                if (temporal4.info[x] == '\0') {
                } else {
                    fprintf(grafica, "%c", temporal4.info[x]);
                }

            }
            fprintf(grafica, "</TD></TR>");
            fprintf(grafica, "</TABLE>>]\n");
            fprintf(grafica, "nodet1:cuatro->nodea4; \n");



        }
    }

}

int g_carpetar2(int apuntador, FILE *archivo, long int inicio, long int tamano, int tipo, arreglo carpetas, FILE *grafica) {
    int sdfasd = ftell(archivo);
    poner_princio(archivo, inicio);
    sdfasd = ftell(archivo);
    long int lugar_escribir = 0;
    int hacer = 0;

    saltar_hastainodos(archivo, tamano);
    long int n = c_n(tamano);

    sdfasd = ftell(archivo);

    for (long int x = 0; x < n; x++) {

        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }

    for (long int x = 0; x < (n * 8); x++) {
        fseek(archivo, sizeof (b_indirecto), SEEK_CUR);
    }


    b_carpetas tc;

    int encontro = 0;

    int prono = 0;
    for (long int x = 0; x < (n * 46); x++) { //encontre carpeta

        fseek(archivo, sizeof (b_carpetas), SEEK_CUR);
        prono++;

        if (prono == apuntador) {


            fseek(archivo, sizeof (b_carpetas)*-1, SEEK_CUR);
            fread(&tc, sizeof (b_carpetas), 1, archivo);

            if (tc.apactual == apuntador) {
                fseek(archivo, sizeof (b_carpetas)*-1, SEEK_CUR);
                encontro = 1;
                lugar_escribir = ftell(archivo);
                break;
            }
        }

    }

    int totc = contar_carpetas(carpetas);

    if (control < totc - 1) { //


        if (tc.hijo1 != -1 && seguir == 1 && encontro == 1) { //-------------------------------

            g_inodor2(tc.hijo1, archivo, inicio, tamano, carpetas, grafica);


        }


        if (tc.hijo2 != -1 && seguir == 1 && encontro == 1) {

            g_inodor2(tc.hijo2, archivo, inicio, tamano, carpetas, grafica);
        }

        if (tc.hijo3 != -1 && seguir == 1 && encontro == 1) {
            g_inodor2(tc.hijo3, archivo, inicio, tamano, carpetas, grafica);

        }
        if (tc.hijo4 != -1 && seguir == 1 && encontro == 1) {
            g_inodor2(tc.hijo4, archivo, inicio, tamano, carpetas, grafica);

        }

        if (tc.hijo5 != -1 && seguir == 1 && encontro == 1) {
            g_inodor2(tc.hijo5, archivo, inicio, tamano, carpetas, grafica);

        }

        if (tc.hijo6 != -1 && seguir == 1 && encontro == 1) {
            g_inodor2(tc.hijo6, archivo, inicio, tamano, carpetas, grafica);

        }



    } else {


        if (encontro == 1) {
            char nigua[20];
            int tyoe = 0;
            memset(nigua, '\0', 20);
            //habia un for que lo limpiaba
            int xxx = contar_carpetas(carpetas);
            strcpy(nigua, carpetas[xxx - 1].nombre);


            for (int x = 0; x < strlen(nigua); x++) {

                if (nigua[x] == 46) {

                    tyoe = 1;
                    break;
                }



            }



            if (tc.hijo1 != -1 && encontro == 1 && seguir == 1) { //si un inodo de bloques o un inodo de 
                if (tyoe == 0) {

                    g_existe_carpetar3(tc.hijo1, nigua, archivo, inicio, tamano, grafica);


                } else if (tyoe == 1) {

                    existe_texto(tc.hijo1, nigua, archivo, inicio, tamano, grafica);


                }

            }

            if (tc.hijo2 != -1 && encontro == 1 && seguir == 1) {
                if (tyoe == 0) {
                    g_existe_carpetar3(tc.hijo2, nigua, archivo, inicio, tamano, grafica);


                } else if (tyoe == 1) {

                    existe_texto(tc.hijo2, nigua, archivo, inicio, tamano, grafica);
                }

            }

            if (tc.hijo3 != -1 && encontro == 1 && seguir == 1) {
                if (tyoe == 0) {
                    g_existe_carpetar3(tc.hijo3, nigua, archivo, inicio, tamano, grafica);

                } else if (tyoe == 1) {
                    existe_texto(tc.hijo3, nigua, archivo, inicio, tamano, grafica);

                }


            }

            if (tc.hijo4 != -1 && encontro == 1 && seguir == 1) {
                if (tyoe == 0) {
                    g_existe_carpetar3(tc.hijo4, nigua, archivo, inicio, tamano, grafica);

                } else if (tyoe == 1) {

                    existe_texto(tc.hijo4, nigua, archivo, inicio, tamano, grafica);
                }

            }

            if (tc.hijo5 != -1 && encontro == 1 && seguir == 1) {
                if (tyoe == 0) {
                    g_existe_carpetar3(tc.hijo5, nigua, archivo, inicio, tamano, grafica);

                } else if (tyoe == 1) {

                    existe_texto(tc.hijo5, nigua, archivo, inicio, tamano, grafica);
                }

            }

            if (tc.hijo6 != -1 && encontro == 1 && seguir == 1) {
                if (tyoe == 0) {
                    g_existe_carpetar3(tc.hijo6, nigua, archivo, inicio, tamano, grafica);

                } else if (tyoe == 1) {

                    existe_texto(tc.hijo6, nigua, archivo, inicio, tamano, grafica);
                }

            }
        }
    }
    return hacer;

}

void g_inodor2(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas, FILE *grafica) {

    poner_princio(archivo, inicio);
    saltar_hastainodos(archivo, tamano);
    int encontrado = 0;
    long int n = c_n(tamano) - 1;
    inodo temporal;
    char nombre[20];
    strcpy(nombre, carpetas[control].nombre);
    control++;
    long int aca = 0;




    for (long int x = 0; x < n; x++) {

        fread(&temporal, sizeof (inodo), 1, archivo);

        if (temporal.llave == apuntador && strcmp(nombre, temporal.idchar) == 0) {
            encontrado = 1;
            fseek(archivo, sizeof (inodo)*-1, SEEK_CUR);
            aca = ftell(archivo);
            break;
        }

    }

    if (encontrado == 1) {

        if (temporal.ad1 != -1 && seguir == 1) {


            if (temporal.tipo == 0) {
                g_carpetar2(temporal.ad1, archivo, inicio, tamano, temporal.tipo, carpetas, grafica);
                if (seguir == 0) {

                    fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave);
                    fprintf(grafica, "<TABLE> \n");
                    fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
                    fprintf(grafica, "</TABLE>>]\n");

                    fprintf(grafica, "nodei%i->nodei%i;\n", temporal.llave, antiguallave);




                    //printf("%s\n", temporal.idchar);
                    //printf("%s->%s\n", temporal.idchar, antiguo);

                    memset(antiguo, '\0', 20);
                    strcpy(antiguo, temporal.idchar);
                    antiguallave = temporal.llave;

                }

            } else if (temporal.tipo == 1) {

            }




        }

        if (temporal.ad2 != -1 && seguir == 1) { //crear nueva carpeta


            if (temporal.tipo == 0) {
                g_carpetar2(temporal.ad2, archivo, inicio, tamano, temporal.tipo, carpetas, grafica);

                if (seguir == 0) {


                    fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave);
                    fprintf(grafica, "<TABLE> \n");
                    fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
                    fprintf(grafica, "</TABLE>>]\n");

                    fprintf(grafica, "nodei%i->nodei%i;\n", temporal.llave, antiguallave);




                    // printf("%s->%s\n", temporal.idchar, antiguo);

                    //printf("%s\n", temporal.idchar);
                    memset(antiguo, '\0', 20);
                    strcpy(antiguo, temporal.idchar);
                    antiguallave = temporal.llave;
                }


            } else if (temporal.tipo == 1) {

            }

        }

        if (temporal.ad3 != -1 && seguir == 1) {

            if (temporal.tipo == 0) {

                g_carpetar2(temporal.ad3, archivo, inicio, tamano, temporal.tipo, carpetas, grafica);

                if (seguir == 0) {


                    fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave);
                    fprintf(grafica, "<TABLE> \n");
                    fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
                    fprintf(grafica, "</TABLE>>]\n");

                    fprintf(grafica, "nodei%i->nodei%i\n", temporal.llave, antiguallave);




                    // printf("%s->%s\n", temporal.idchar, antiguo);
                    //  printf("%s\n", temporal.idchar);
                    memset(antiguo, '\0', 20);
                    strcpy(antiguo, temporal.idchar);
                    antiguallave = temporal.llave;
                }
            } else if (temporal.tipo == 1) {

            }
        }

        if (temporal.ad4 != -1 && seguir == 1) {

            if (temporal.tipo == 0) {

                g_carpetar2(temporal.ad4, archivo, inicio, tamano, temporal.tipo, carpetas, grafica);

                if (seguir == 0) {


                    fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave);
                    fprintf(grafica, "<TABLE> \n");
                    fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
                    fprintf(grafica, "</TABLE>>]\n");

                    fprintf(grafica, "nodei%i->nodei%i\n", temporal.llave, antiguallave);

                    // printf("%s->%s\n", temporal.idchar, antiguo);
                    //printf("%s\n", temporal.idchar);
                    memset(antiguo, '\0', 20);
                    strcpy(antiguo, temporal.idchar);
                    antiguallave = temporal.llave;
                }
            } else if (temporal.tipo == 1) {

            }

        }

    } else {

        control--;
    }

}

//----------------------------------------------graficas de directorios y archivos de texto

void graficar_archivos(int numero, idspart ids, arreglo carpetas, int cual, int pedo) {

    memset(antiguo, '\0', 20);
    antiguallave = 0;
    mbr tmbr = recuperar_mbr(ids[numero].ruta);


    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;

    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;

    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;

    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;

    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;

    }



    if (cual == 3) {

        FILE *archivo = fopen(ids[numero].ruta, "r+b");
        FILE *grafica = fopen("juego2.dot", "w");
        if (archivo) {
            control = 0;
            seguir = 1;
            fprintf(grafica, "digraph juego2 { \n");
            fprintf(grafica, "node [shape=plaintext] \n");
            fprintf(grafica, "rankdir=LR \n");



            g_inodor2(1, archivo, inicio, tamano, carpetas, grafica);
            //printf("%i", seguir);

            fprintf(grafica, "}");
            fclose(archivo);
            fclose(grafica);

            system("dot -Tpng juego2.dot -o juego2.png");
            system("xdg-open juego2.png");

        }

    } else if (cual == 5 && pedo == 1) {
        if (strcmp(carpetas[0].nombre, "/") == 0) {
            FILE *archivo = fopen(ids[numero].ruta, "r+b");
            FILE *grafica = fopen("juego2.dot", "w");
            if (archivo) {
                control = 0;
                seguir = 1;
                fprintf(grafica, "digraph juego2 { \n");
                fprintf(grafica, "node [shape=plaintext] \n");
                fprintf(grafica, "rankdir=LR \n");



                g_existe_carpetar3(1, "/", archivo, inicio, tamano, grafica);


                fprintf(grafica, "}");
                fclose(archivo);
                fclose(grafica);

                system("dot -Tpng juego2.dot -o juego2.png");
                system("xdg-open juego2.png");

            }



        } else {
            printf("ruta incorrecta \n");

        }

    } else if (cual == 5) {

        FILE *archivo = fopen(ids[numero].ruta, "r+b");
        FILE *grafica = fopen("juego4.dot", "w");
        if (archivo) {
            control = 0;
            seguir = 1;
            fprintf(grafica, "digraph juego4 { \n");
            fprintf(grafica, "node [shape=plaintext] \n");
            fprintf(grafica, "rankdir=LR \n");

            g_inodor2(1, archivo, inicio, tamano, carpetas, grafica);

            fprintf(grafica, "}");
            fclose(archivo);
            fclose(grafica);

            system("dot -Tpng juego4.dot -o juego4.png");
            system("xdg-open juego4.png");

        }


    }


}

//-----------------grafica general

long int que_sosr4(long int inicio, long int tamano, FILE *disco, int llave) {

    poner_princio(disco, inicio);
    saltar_bmbloques(disco, tamano);
    char bit = '\0';
    for (long int x = 0; x <= llave; x++) {
        fread(&bit, sizeof (bit), 1, disco);

    }

    if (bit == 'c') {
        return 0;

    } else if (bit == 'a') {
        return 1;

    } else if (bit == '1') {
        printf("que rayos");
    } else if (bit == '\0') {
        return -1;
    }

    return -1;
}

long int c_pos_disco_carpetasr4(long int puntero, long int inicio, long int tamano) {
    if (puntero == -1) {

        return -1;
    }

    long int n = c_n(tamano);
    long total = inicio + sizeof (super_bloque) + n * sizeof (journaling) + n + n * 46 + n * sizeof (inodo) + n * 8 * sizeof (b_indirecto) + sizeof (b_carpetas)*(puntero - 1);
    return total;
}

long int c_pos_inodor4(long int puntero, long int inicio, long int tamano) {

    if (puntero == -1) {

        return -1;
    }

    long int n = c_n(tamano);
    long total = inicio + sizeof (super_bloque) + n * sizeof (journaling) + n + n * 46 + sizeof (inodo)*(puntero - 1);

    return total;
}

void generar_enlazesr4(FILE *disco, FILE *grafica, long int inicio, long int tamano) {
    poner_princio(disco, inicio);
    saltar_hastainodos(disco, tamano);
    long int n = c_n(tamano);

    for (int x = 0; x < n; x++) {
        inodo temporal;
        fread(&temporal, sizeof (inodo), 1, disco);
        if (temporal.llave != -1) {
            if (temporal.ad1 != -1) {
                if (temporal.tipo == 0) {
                    fprintf(grafica, "nodei%i:uno->nodec%i;\n", temporal.llave, temporal.ad1);
                } else if (temporal.tipo == 1) {
                    fprintf(grafica, "nodei%i:uno->nodea%i;\n", temporal.llave, temporal.ad1);


                }

            }

            if (temporal.ad2 != -1) {
                if (temporal.tipo == 0) {
                    fprintf(grafica, "nodei%i:dos->nodec%i;\n", temporal.llave, temporal.ad2);

                } else if (temporal.tipo == 1) {
                    fprintf(grafica, "nodei%i:dos->nodea%i;\n", temporal.llave, temporal.ad2);

                }


            }

            if (temporal.ad3 != -1) {
                if (temporal.tipo == 0) {
                    fprintf(grafica, "nodei%i:tres->nodec%i;\n", temporal.llave, temporal.ad3);

                } else if (temporal.tipo == 1) {
                    fprintf(grafica, "nodei%i:tres->nodea%i;\n", temporal.llave, temporal.ad3);


                }



            }

            if (temporal.ad4 != -1) {
                if (temporal.tipo == 0) {
                    fprintf(grafica, "nodei%i:cuatro->nodec%i;\n", temporal.llave, temporal.ad4);

                } else if (temporal.tipo == 1) {
                    fprintf(grafica, "nodei%i:cuatro->nodea%i;\n", temporal.llave, temporal.ad4);


                }


            }

        }

    }

    for (int x = 0; x < n * 8; x++) {

        fseek(disco, sizeof (b_indirecto), SEEK_CUR);
    }

    for (int x = 0; x < n * 46; x++) {

        long int pos_acutal = ftell(disco);
        int que = que_sosr4(inicio, tamano, disco, x);
        fseek(disco, pos_acutal, SEEK_SET);

        if (que == 0) {
            b_carpetas temporal;
            fread(&temporal, sizeof (b_carpetas), 1, disco);

            if (temporal.hijo1 != -1) {

                fprintf(grafica, "nodec%i:uno->nodei%i;\n", temporal.apactual, temporal.hijo1);

            }


            if (temporal.hijo2 != -1) {

                fprintf(grafica, "nodec%i:dos->nodei%i;\n", temporal.apactual, temporal.hijo2);

            }

            if (temporal.hijo3 != -1) {

                fprintf(grafica, "nodec%i:tres->nodei%i;\n", temporal.apactual, temporal.hijo3);

            }

            if (temporal.hijo4 != -1) {
                fprintf(grafica, "nodec%i:cuatro->nodei%i;\n", temporal.apactual, temporal.hijo4);

            }

            if (temporal.hijo5 != -1) {

                fprintf(grafica, "nodec%i:cinco->nodei%i;\n", temporal.apactual, temporal.hijo5);

            }

            if (temporal.hijo6 != -1) {

                fprintf(grafica, "nodec%i:seis->nodei%i;\n", temporal.apactual, temporal.hijo6);

            }


        } else {

            fseek(disco, sizeof (b_carpetas), SEEK_CUR);
        }



        if (x == 200) {
            break;
        }
    }


}

void genrar_nodosr4(FILE *disco, FILE *grafica, long int inicio, long int tamano) {


    poner_princio(disco, inicio);
    saltar_hastainodos(disco, tamano);
    long int n = c_n(tamano);


    for (int x = 0; x < n; x++) {
        inodo temporal;
        long int pos_puntero = ftell(disco);
        fread(&temporal, sizeof (inodo), 1, disco);

        if (temporal.llave != -1) {

            fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave); //i de inodos
            fprintf(grafica, "<TABLE> \n");


            fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
            fprintf(grafica, "<TR><TD>TIPO</TD><TD>%i</TD></TR>\n", temporal.tipo);
            fprintf(grafica, "<TR><TD>ID</TD><TD>%i</TD></TR>\n", temporal.llave);
            fprintf(grafica, "<TR><TD>Puntero</TD><TD>%ld</TD></TR>\n", pos_puntero);
            fprintf(grafica, "<TR><TD>Permisos</TD><TD>%i</TD></TR>\n", temporal.permisos);
            fprintf(grafica, "<TR><TD>Propietario</TD><TD>%s</TD></TR>\n", temporal.propietario);
            fprintf(grafica, "<TR><TD>Fecha</TD><TD>%s</TD></TR>\n", temporal.fecha);
            fprintf(grafica, "<TR><TD>Tamano</TD><TD>%i</TD></TR>\n", temporal.tamano);


            fprintf(grafica, "<TR><TD>P1</TD><TD PORT=\"uno\">%i (%ld)</TD></TR>\n", temporal.ad1, c_pos_disco_carpetasr4(temporal.ad1, inicio, tamano));

            fprintf(grafica, "<TR><TD>P2</TD><TD PORT=\"dos\">%i (%ld)</TD></TR>\n", temporal.ad2, c_pos_disco_carpetasr4(temporal.ad2, inicio, tamano));
            fprintf(grafica, "<TR><TD>P3</TD><TD PORT=\"tres\">%i (%ld)</TD></TR>\n", temporal.ad3, c_pos_disco_carpetasr4(temporal.ad3, inicio, tamano));

            fprintf(grafica, "<TR><TD>P4</TD><TD PORT=\"cuatro\">%i (%ld)</TD></TR>\n", temporal.ad4, c_pos_disco_carpetasr4(temporal.ad4, inicio, tamano));

            fprintf(grafica, "<TR><TD>AP1</TD><TD>%i (%i)</TD></TR>\n", -1, -1);

            fprintf(grafica, "<TR><TD>AP2</TD><TD>%i (%i)</TD></TR>\n", -1, -1);

            fprintf(grafica, "</TABLE>>]\n");


        }


    }

    for (int x = 0; x < n * 8; x++) { //indirecto
        fseek(disco, sizeof (b_indirecto), SEEK_CUR);
    }

    //que es y ponerme ne los inodos
    // printf("n: %ld \n", n);
    for (long int x = 0; x < n * 46; x++) {
        // printf("n: %ld \n", x);
        long int pos_acutal = ftell(disco);
        int que = que_sosr4(inicio, tamano, disco, x);
        fseek(disco, pos_acutal, SEEK_SET);

        if (que == 0) {
            b_carpetas temporal;
            fread(&temporal, sizeof (b_carpetas), 1, disco);

            fprintf(grafica, "nodec%i[shape=none, margin=0, label=< \n", temporal.apactual); //i de inodos
            fprintf(grafica, "<TABLE> \n");

            fprintf(grafica, "<TR><TD>Actual</TD><TD>%i (%ld)</TD></TR>\n", temporal.apactual, c_pos_disco_carpetasr4(temporal.apactual, inicio, tamano));
            fprintf(grafica, "<TR><TD>Inodo Padre</TD><TD>%i (%ld)</TD></TR>\n", temporal.appadre, c_pos_inodor4(temporal.appadre, inicio, tamano));
            fprintf(grafica, "<TR><TD>P1</TD><TD PORT=\"uno\">%i (%ld)</TD></TR>\n", temporal.hijo1, c_pos_inodor4(temporal.hijo1, inicio, tamano));
            fprintf(grafica, "<TR><TD>P2</TD><TD PORT=\"dos\">%i (%ld)</TD></TR>\n", temporal.hijo2, c_pos_inodor4(temporal.hijo2, inicio, tamano));
            fprintf(grafica, "<TR><TD>P3</TD><TD PORT=\"tres\">%i (%ld)</TD></TR>\n", temporal.hijo3, c_pos_inodor4(temporal.hijo3, inicio, tamano));
            fprintf(grafica, "<TR><TD>P4</TD><TD PORT=\"cuatro\">%i (%ld)</TD></TR>\n", temporal.hijo4, c_pos_inodor4(temporal.hijo4, inicio, tamano));
            fprintf(grafica, "<TR><TD>P5</TD><TD PORT=\"cinco\">%i (%ld)</TD></TR>\n", temporal.hijo5, c_pos_inodor4(temporal.hijo5, inicio, tamano));
            fprintf(grafica, "<TR><TD>P6</TD><TD PORT=\"seis\">%i (%ld)</TD></TR>\n", temporal.hijo6, c_pos_inodor4(temporal.hijo6, inicio, tamano));

            fprintf(grafica, "</TABLE>>]\n");


        } else if (que == 1) {
            b_archivos temporal;
            fread(&temporal, sizeof (b_archivos), 1, disco);

            fprintf(grafica, "nodea%i[shape=none, margin=0, label=< \n", temporal.apactual); //i de inodos
            fprintf(grafica, "<TABLE> \n");
            fprintf(grafica, "<TR><TD>Actual</TD><TD>%i (%ld)</TD></TR>\n", temporal.apactual, c_pos_disco_carpetasr4(temporal.apactual, inicio, tamano));
            fprintf(grafica, "<TR><TD>Inodo Padre</TD><TD>%i (%ld)</TD></TR>\n", temporal.appadre, c_pos_inodor4(temporal.appadre, inicio, tamano));

            fprintf(grafica, "<TR><TD>Contenido</TD><TD>");
            for (int x = 0; x < 24; x++) {
                if (temporal.info[x] == '\0') {

                } else {
                    fprintf(grafica, "%c", temporal.info[x]);
                }
            }
            fprintf(grafica, "</TD></TR>\n");
            fprintf(grafica, "</TABLE>>]\n");


        } else if (que == -1) {
            fseek(disco, sizeof (b_carpetas), SEEK_CUR);
        }

        if (x == 200) {
            break;
        }

    }
    //- saltar los indirectos



}

void grafica_general(int numero, idspart ids) {
    memset(antiguo, '\0', 20);
    antiguallave = 0;
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;

    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;

    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;

    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;

    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;

    }
    FILE *archivo = fopen(ids[numero].ruta, "r+b");
    FILE *grafica = fopen("juego3.dot", "w");
    if (archivo) {
        fprintf(grafica, "digraph juego3 { \n");
        fprintf(grafica, "node [shape=plaintext] \n");
        fprintf(grafica, "rankdir=LR \n");
        genrar_nodosr4(archivo, grafica, inicio, tamano);
        generar_enlazesr4(archivo, grafica, inicio, tamano);
        fprintf(grafica, "}");
        fclose(archivo);
        fclose(grafica);
        system("dot -Tpng juego3.dot -o juego3.png");
        system("xdg-open juego3.png");
    }
}

void reporte_bm_inodos(int numero, idspart ids) {

    mbr tmbr = recuperar_mbr(ids[numero].ruta);


    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;
    char nombre[50];
    char ndisco[50];
    memset(nombre, '\0', 50);
    memset(ndisco, '\0', 50);
    strcpy(ndisco, ids[numero].nombre_disco);

    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;
        strcpy(nombre, tmbr.par[0].nombre);


    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;
        strcpy(nombre, tmbr.par[1].nombre);

    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;
        strcpy(nombre, tmbr.par[2].nombre);
    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;
        strcpy(nombre, tmbr.par[3].nombre);
    }

    FILE *disco = fopen(ids[numero].ruta, "r+b");
    FILE *reporte = fopen("bm_inodos.txt", "w");
    if (disco) {

        fprintf(reporte, "Disco: %s\n", ndisco);
        fprintf(reporte, "Nombre Particion: %s\n", nombre);
        fprintf(reporte, "Reporte de inodos\n");
        //-----------------
        char fe[200];
        struct tm fecha = *localtime(&(time_t){time(NULL)});
        strcpy(fe, asctime(&fecha));
        //-----------------generar fecha
        fprintf(reporte, "Fecha del reporte: %s\n", fe);
        fprintf(reporte, "-------------------------------\n");
        poner_princio(disco, inicio);
        long int n = c_n(tamano);
        fseek(disco, sizeof (super_bloque), SEEK_CUR);
        fseek(disco, sizeof (journaling) * n, SEEK_CUR);
        char carater = '\0';
        int hasta20 = 0;
        for (int x = 0; x < n; x++, hasta20++) {
            fread(&carater, sizeof (carater), 1, disco);
            if (carater == '1') {

                fprintf(reporte, "|%c", carater);
            } else {
                fprintf(reporte, "|0");
            }
            if (hasta20 == 19) {
                fprintf(reporte, "|");
                hasta20 = -1;
                fprintf(reporte, "\n");
            }

            if (x == n - 1) {
                fprintf(reporte, "|");
            }
        }
        fclose(reporte);
        fclose(disco);
        system("xdg-open bm_inodos.txt");
    }
}

void reporte_bm_bdatos(int numero, idspart ids) {
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;
    char nombre[50];
    char ndisco[50];
    memset(nombre, '\0', 50);
    memset(ndisco, '\0', 50);
    strcpy(ndisco, ids[numero].nombre_disco);
    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;
        strcpy(nombre, tmbr.par[0].nombre);
    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;
        strcpy(nombre, tmbr.par[1].nombre);
    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;
        strcpy(nombre, tmbr.par[2].nombre);
    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;
        strcpy(nombre, tmbr.par[3].nombre);
    }
    FILE *disco = fopen(ids[numero].ruta, "r+b");
    FILE *reporte = fopen("bm_bloques.txt", "w");
    if (disco) {
        fprintf(reporte, "Disco: %s\n", ndisco);
        fprintf(reporte, "Nombre Particion: %s\n", nombre);
        fprintf(reporte, "Reporte de bloques\n");
        //-----------------
        char fe[200];
        struct tm fecha = *localtime(&(time_t){time(NULL)});
        strcpy(fe, asctime(&fecha));
        //-----------------generar fecha
        fprintf(reporte, "Fecha del reporte: %s\n", fe);
        fprintf(reporte, "-------------------------------\n");
        poner_princio(disco, inicio);
        long int n = c_n(tamano);
        fseek(disco, sizeof (super_bloque), SEEK_CUR);
        fseek(disco, sizeof (journaling) * n, SEEK_CUR);
        fseek(disco, 1 * n, SEEK_CUR);
        char carater = '\0';
        int hasta20 = 0;
        for (int x = 0; x < n * 46; x++, hasta20++) {
            fread(&carater, sizeof (carater), 1, disco);
            if (carater == '\0') {
                fprintf(reporte, "|0");
            } else {
                fprintf(reporte, "|%c", carater);
            }
            if (hasta20 == 19) {
                fprintf(reporte, "|");
                hasta20 = -1;
                fprintf(reporte, "\n");
            }
            if (x == n - 1) {
                fprintf(reporte, "|");
            }
        }
        fclose(reporte);
        fclose(disco);
        system("xdg-open bm_bloques.txt");
    }
}

void reporte_de_inodos(int numero, idspart ids) {
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;
    char nombre[50];
    char ndisco[50];
    memset(nombre, '\0', 50);
    memset(ndisco, '\0', 50);
    strcpy(ndisco, ids[numero].nombre_disco);
    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;
        strcpy(nombre, tmbr.par[0].nombre);
    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;
        strcpy(nombre, tmbr.par[1].nombre);
    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;
        strcpy(nombre, tmbr.par[2].nombre);
    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;
        strcpy(nombre, tmbr.par[3].nombre);
    }
    FILE *disco = fopen(ids[numero].ruta, "r+b");
    FILE *grafica = fopen("reporte_inodos.dot", "w");
    if (disco) {
        fprintf(grafica, "digraph reporte_inodos { \n");
        fprintf(grafica, "node [shape=plaintext] \n");
        fprintf(grafica, "rankdir=LR \n");
        long int n = c_n(tamano);
        poner_princio(disco, inicio);
        saltar_hastainodos(disco, tamano);
        int kush = 0;
        for (int x = 0; x < n; x++) {
            inodo temporal;
            long int pos_puntero = ftell(disco);
            fread(&temporal, sizeof (inodo), 1, disco);
            if (temporal.llave != -1) {
                fprintf(grafica, "nodei%i[shape=none, margin=0, label=< \n", temporal.llave); //i de inodos
                fprintf(grafica, "<TABLE> \n");
                fprintf(grafica, "<TR><TD>INODO %i</TD></TR>\n", temporal.llave);
                fprintf(grafica, "<TR><TD>%s</TD></TR>\n", temporal.idchar);
                fprintf(grafica, "<TR><TD>TIPO</TD><TD>%i</TD></TR>\n", temporal.tipo);
                fprintf(grafica, "<TR><TD>ID</TD><TD>%i</TD></TR>\n", temporal.llave);
                fprintf(grafica, "<TR><TD>Puntero</TD><TD>%ld</TD></TR>\n", pos_puntero);
                fprintf(grafica, "<TR><TD>Permisos</TD><TD>%i</TD></TR>\n", temporal.permisos);
                fprintf(grafica, "<TR><TD>Propietario</TD><TD>%s</TD></TR>\n", temporal.propietario);
                fprintf(grafica, "<TR><TD>Fecha</TD><TD>%s</TD></TR>\n", temporal.fecha);
                fprintf(grafica, "<TR><TD>Tamano</TD><TD>%i</TD></TR>\n", temporal.tamano);
                fprintf(grafica, "<TR><TD>P1</TD><TD PORT=\"uno\">%i (%ld)</TD></TR>\n", temporal.ad1, c_pos_disco_carpetasr4(temporal.ad1, inicio, tamano));
                fprintf(grafica, "<TR><TD>P2</TD><TD PORT=\"dos\">%i (%ld)</TD></TR>\n", temporal.ad2, c_pos_disco_carpetasr4(temporal.ad2, inicio, tamano));
                fprintf(grafica, "<TR><TD>P3</TD><TD PORT=\"tres\">%i (%ld)</TD></TR>\n", temporal.ad3, c_pos_disco_carpetasr4(temporal.ad3, inicio, tamano));
                fprintf(grafica, "<TR><TD>P4</TD><TD PORT=\"cuatro\">%i (%ld)</TD></TR>\n", temporal.ad4, c_pos_disco_carpetasr4(temporal.ad4, inicio, tamano));
                fprintf(grafica, "<TR><TD>AP1</TD><TD>%i (%i)</TD></TR>\n", -1, -1);
                fprintf(grafica, "<TR><TD>AP2</TD><TD>%i (%i)</TD></TR>\n", -1, -1);
                fprintf(grafica, "</TABLE>>]\n");
                if (kush == 0) {
                    kush = temporal.llave;
                } else {
                    fprintf(grafica, "nodei%i->nodei%i;\n", kush, temporal.llave);
                    kush = temporal.llave;
                }
            }
        }
        fprintf(grafica, "}\n");
        fclose(disco);
        fclose(grafica);
    }
    system("dot -Tpng reporte_inodos.dot -o reporte_inodos.png");
    system("xdg-open reporte_inodos.png");
}

void reporte_de_bloques(int numero, idspart ids) {
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;
    char nombre[50];
    char ndisco[50];
    memset(nombre, '\0', 50);
    memset(ndisco, '\0', 50);
    strcpy(ndisco, ids[numero].nombre_disco);
    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;
        strcpy(nombre, tmbr.par[0].nombre);
    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;
        strcpy(nombre, tmbr.par[1].nombre);
    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;
        strcpy(nombre, tmbr.par[2].nombre);
    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;
        strcpy(nombre, tmbr.par[3].nombre);
    }
    FILE *disco = fopen(ids[numero].ruta, "r+b");
    FILE *grafica = fopen("reportes_bloques.dot", "w");
    if (disco) {
        fprintf(grafica, "digraph reporte_inodos { \n");
        fprintf(grafica, "node [shape=plaintext] \n");
        fprintf(grafica, "rankdir=LR \n");
        long int n = c_n(tamano);
        poner_princio(disco, inicio);
        saltar_hastainodos(disco, tamano);
        for (int x = 0; x < n; x++) {
            fseek(disco, sizeof (inodo), SEEK_CUR);
        }
        for (int x = 0; x < n * 8; x++) {
            fseek(disco, sizeof (b_indirecto), SEEK_CUR);
        }
        for (long int x = 0; x < n * 46; x++) {
            long int pos_acutal = ftell(disco);
            int que = que_sosr4(inicio, tamano, disco, x);
            fseek(disco, pos_acutal, SEEK_SET);
            if (que == 1) {
                b_archivos temporal;
                fread(&temporal, sizeof (b_archivos), 1, disco);
                pos_acutal = ftell(disco);
                inodo it = buscar_inodor3(temporal.appadre, disco, inicio, tamano);
                fseek(disco, pos_acutal, SEEK_SET);
                fprintf(grafica, "nodea%i[shape=none, margin=0, label=< \n", temporal.apactual); //i de inodos
                fprintf(grafica, "<TABLE> \n");
                fprintf(grafica, "<TR><TD>Bloque %i (%ld)</TD></TR>\n", temporal.apactual, c_pos_disco_carpetasr4(temporal.apactual, inicio, tamano));
                fprintf(grafica, "<TR><TD>Archivo: %s</TD></TR>\n", it.idchar);
                fprintf(grafica, "<TR><TD>");
                for (int x = 0; x < 24; x++) {
                    if (temporal.info[x] == '\0') {
                    } else {
                        fprintf(grafica, "%c", temporal.info[x]);
                    }
                }
                fprintf(grafica, "</TD></TR>\n");
                fprintf(grafica, "</TABLE>>]\n");
            } else {
                fseek(disco, sizeof (b_carpetas), SEEK_CUR);
            }
            if (x == 200) {
                break;
            }
        }
        fprintf(grafica, "}\n");
        fclose(disco);
        fclose(grafica);
        system("dot -Tpng reportes_bloques.dot -o reportes_bloques.png");
        system("xdg-open reportes_bloques.png");
    }
}

void reporte_journaling(int numero, idspart ids) {
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    int option = ids[numero].numero;
    long int inicio = 0;
    long int tamano = 0;
    char nombre[50];
    char ndisco[50];
    memset(nombre, '\0', 50);
    memset(ndisco, '\0', 50);
    strcpy(ndisco, ids[numero].nombre_disco);
    if (option == 1) {
        inicio = tmbr.par[0].inicio;
        tamano = tmbr.par[0].tamano;
        strcpy(nombre, tmbr.par[0].nombre);
    } else if (option == 2) {
        inicio = tmbr.par[1].inicio;
        tamano = tmbr.par[1].tamano;
        strcpy(nombre, tmbr.par[1].nombre);
    } else if (option == 3) {
        inicio = tmbr.par[2].inicio;
        tamano = tmbr.par[2].tamano;
        strcpy(nombre, tmbr.par[2].nombre);
    } else if (option == 4) {
        inicio = tmbr.par[3].inicio;
        tamano = tmbr.par[3].tamano;
        strcpy(nombre, tmbr.par[3].nombre);
    }
    FILE *disco = fopen(ids[numero].ruta, "r+b");
    FILE *reporte = fopen("reporte_journaling.txt", "w");
    if (disco) {
        fprintf(reporte, "Disco: %s\n", ndisco);
        fprintf(reporte, "Nombre Particion: %s\n", nombre);
        fprintf(reporte, "Reporte Journaling\n");
        //-----------------
        char fe[200];
        struct tm fecha = *localtime(&(time_t){time(NULL)});
        strcpy(fe, asctime(&fecha));
        fprintf(reporte, "Fecha de reporte:%s\n", fe);
        fprintf(reporte, "-------------------------------------------\n");
        poner_princio(disco, inicio);
        fseek(disco, sizeof (super_bloque), SEEK_CUR);
        long int n = c_n(tamano);

        for (int x = 0; x < n; x++) {

            journaling temporal;
            fread(&temporal, sizeof (journaling), 1, disco);
            if (strcmp(temporal.fecha, "vacio") == 0) {

            } else {

                fprintf(reporte, "Fecha: %s\n", temporal.fecha);
                fprintf(reporte, "Descripcion: %s\n", temporal.operacion);
                fprintf(reporte, "-------------------------------------------\n");
            }


        }



        fclose(reporte);
        fclose(disco);
        system("xdg-open reporte_journaling.txt");
    }




}





