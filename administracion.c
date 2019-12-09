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
#include "administracion.h"
#include <math.h>
#include <sys/stat.h>
#include <time.h>



int control0;
int seguir0;
int action;
int ap_anterior;
char r_usuarios[96];
int imponiendoalgo;

int contar_carpetas0(arreglo part) {
    int contador = 0;
    for (int x = 0; x < 25; x++) {
        if (strcmp(part[x].nombre, "vacio") == 0) {
        } else {
            contador++;
        }
    }
    return contador;

}

long int c_n0(long int tamano_part) {

    float n = 0;
    n = (tamano_part - sizeof (super_bloque));
    float l = (sizeof (journaling) + 47 + sizeof (inodo) + 8 * sizeof (b_indirecto) + 46 * sizeof (b_archivos));
    float f = n / l;
    //ver si agarra el valor mas pequeño
    return f;
}

void poner_princio0(FILE *archivo, long int inicio) {
    fseek(archivo, 0, SEEK_SET);

    for (long int x = 0; x < inicio; x++) {
        fseek(archivo, 1, SEEK_CUR);
    }

}

void saltar_hastainodos0(FILE *archivo, long int tamano) { //saltar bitacaroa desps del sb

    long int n = c_n0(tamano);
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

void saltar_bmbloques0(FILE *archivo, long int tamano) {
    long int n = c_n0(tamano);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);
    for (long int x = 0; x < n; x++) {
        fseek(archivo, sizeof (journaling), SEEK_CUR);
    }
    for (long int x = 0; x < n; x++) { //bm inodos
        fseek(archivo, 1, SEEK_CUR);
    }

}

void saltar_carpetas0(FILE *archivo, long int tamano) {

    long int n = c_n0(tamano);
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

void acutalizar_jour(long int inicio, FILE *archivo, int tipo, arreglo carpetas, long int tamano) {
    int xxx = contar_carpetas0(carpetas);
    poner_princio0(archivo, inicio);
    fseek(archivo, sizeof (super_bloque), SEEK_CUR);

    long int n = c_n0(tamano);
    char fe[200];
    struct tm fecha = *localtime(&(time_t){time(NULL)});
    strcpy(fe, asctime(&fecha));

    for (int x = 0; x < n; x++) {
        journaling temp;
        fread(&temp, sizeof (journaling), 1, archivo);
        if (strcmp(temp.fecha, "vacio") == 0) {

            fseek(archivo, sizeof (journaling)*-1, SEEK_CUR);
            break;
        }
    }

    journaling temp2;
    if (tipo == 1) {

        strcpy(temp2.fecha, fe);
        char nuevo[150];
        memset(nuevo, '\0', 150);
        sprintf(nuevo, "Se elimino la carpeta: %s", carpetas[xxx - 1].nombre);
        strcpy(temp2.operacion, nuevo);
        fwrite(&temp2, sizeof (journaling), 1, archivo);

    } else if (tipo == 2) {

        strcpy(temp2.fecha, fe);
        char nuevo[150];
        memset(nuevo, '\0', 150);
        sprintf(nuevo, "Se renombro la carpeta/directorio: %s", carpetas[xxx - 1].nombre);
        strcpy(temp2.operacion, nuevo);
        fwrite(&temp2, sizeof (journaling), 1, archivo);

    } else if (tipo == 3) {

        strcpy(temp2.fecha, fe);
        char nuevo[150];
        memset(nuevo, '\0', 150);
        sprintf(nuevo, "Se movieron carpetas de un directorio a otro");
        strcpy(temp2.operacion, nuevo);
        fwrite(&temp2, sizeof (journaling), 1, archivo);


    }

}

b_carpetas recuperar_carpeta(int apuntador, long int inicio, long int tamano, FILE *archivo) {
    b_carpetas temporal;
    temporal.apactual = -1;
    poner_princio0(archivo, inicio);
    saltar_carpetas0(archivo, tamano);

    for (int x = 0; x < apuntador - 1; x++) {

        fseek(archivo, sizeof (b_carpetas), SEEK_CUR);

    }

    fread(&temporal, sizeof (b_carpetas), 1, archivo);
    if (apuntador == temporal.apactual) {

        return temporal;
    }

    return temporal;
}

int selleno(FILE *archivo, int padre, long int tamano, long int inicio) {

    poner_princio0(archivo, inicio);
    saltar_bmbloques0(archivo, tamano);
    long int n = c_n0(tamano);
    char by;
    int cota = 0; //nueva llave
    for (int x = 0; x < n * 46; x++) {
        fread(&by, sizeof (by), 1, archivo);
        cota++;
        if (by == '\0') {

            fseek(archivo, -1, SEEK_CUR);
            char bota = 'c';
            fwrite(&bota, sizeof (bota), 1, archivo);

            break;
        }
    }

    poner_princio0(archivo, inicio);
    saltar_carpetas0(archivo, tamano);

    for (int x = 0; x < cota - 1; x++) {
        fseek(archivo, sizeof (b_carpetas), SEEK_CUR);
    }
    b_carpetas tec;
    tec.apactual = cota;
    tec.appadre = padre;
    tec.hijo1 = -1;
    tec.hijo2 = -1;
    tec.hijo3 = -1;
    tec.hijo4 = -1;
    tec.hijo5 = -1;
    tec.hijo6 = -1;

    return cota;

}

void m6(int apuntador, FILE *archivo, long int inicio, long int tamno, arreglo carpetas) {
    poner_princio0(archivo, inicio);
    saltar_hastainodos0(archivo, tamno);

    for (int x = 0; x < apuntador - 1; x++) {

        fseek(archivo, sizeof (inodo), SEEK_CUR);

    }
    inodo temporal;
    long int pos_actual = ftell(archivo);
    fread(&temporal, sizeof (inodo), 1, archivo);
    int xxx = contar_carpetas0(carpetas);
    if (temporal.llave == apuntador && strcmp(temporal.idchar, carpetas[xxx - 1].nombre) == 0) {

        int constancia = 1;
        if (temporal.ad1 != -1 && constancia == 1) {
            b_carpetas tc = recuperar_carpeta(temporal.ad1, inicio, tamno, archivo);

            if (tc.hijo1 == -1) {
                tc.hijo1 = ap_anterior;
                constancia = 0;

            } else if (tc.hijo2 == -1) {
                tc.hijo2 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo3 == -1) {
                tc.hijo3 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo4 == -1) {
                tc.hijo4 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo5 == -1) {
                tc.hijo5 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo6 == -1) {
                tc.hijo6 = ap_anterior;
                constancia = 0;
            }

            if (constancia == 0) {
                poner_princio0(archivo, inicio);
                saltar_carpetas0(archivo, tamno);
                for (int x = 0; x < temporal.ad1 - 1; x++) {

                    fseek(archivo, sizeof (b_carpetas), SEEK_CUR);

                }
                fwrite(&tc, sizeof (b_carpetas), 1, archivo);

            }

            if (tc.hijo1 != -1 && tc.hijo2 != -1 && tc.hijo3 != -1 && tc.hijo4 != -1 && tc.hijo5 != -1 && tc.hijo6 != -1 && constancia == 0 && temporal.ad2 == -1) {

                temporal.ad2 = selleno(archivo, temporal.llave, tamno, inicio);
            }

        }
        if (temporal.ad2 != -1 && constancia == 1) {
            b_carpetas tc = recuperar_carpeta(temporal.ad2, inicio, tamno, archivo);

            if (tc.hijo1 == -1) {
                tc.hijo1 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo2 == -1) {
                tc.hijo2 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo3 == -1) {
                tc.hijo3 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo4 == -1) {
                tc.hijo4 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo5 == -1) {
                tc.hijo5 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo6 == -1) {
                tc.hijo6 = ap_anterior;
                constancia = 0;
            }

            if (constancia == 0) {
                poner_princio0(archivo, inicio);
                saltar_carpetas0(archivo, tamno);
                for (int x = 0; x < temporal.ad2 - 1; x++) {

                    fseek(archivo, sizeof (b_carpetas), SEEK_CUR);

                }
                fwrite(&tc, sizeof (b_carpetas), 1, archivo);

            }

            if (tc.hijo1 != -1 && tc.hijo2 != -1 && tc.hijo3 != -1 && tc.hijo4 != -1 && tc.hijo5 != -1 && tc.hijo6 != -1 && constancia == 0 && temporal.ad3 == -1) {

                temporal.ad3 = selleno(archivo, temporal.llave, tamno, inicio);
            }

        }
        if (temporal.ad3 != -1 && constancia == 1) {
            b_carpetas tc = recuperar_carpeta(temporal.ad3, inicio, tamno, archivo);


            if (tc.hijo1 == -1) {
                tc.hijo1 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo2 == -1) {
                tc.hijo2 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo3 == -1) {
                tc.hijo3 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo4 == -1) {
                tc.hijo4 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo5 == -1) {
                tc.hijo5 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo6 == -1) {
                tc.hijo6 = ap_anterior;
                constancia = 0;
            }

            if (constancia == 0) {
                poner_princio0(archivo, inicio);
                saltar_carpetas0(archivo, tamno);
                for (int x = 0; x < temporal.ad3 - 1; x++) {

                    fseek(archivo, sizeof (b_carpetas), SEEK_CUR);

                }
                fwrite(&tc, sizeof (b_carpetas), 1, archivo);

            }


            if (tc.hijo1 != -1 && tc.hijo2 != -1 && tc.hijo3 != -1 && tc.hijo4 != -1 && tc.hijo5 != -1 && tc.hijo6 != -1 && constancia == 0 && temporal.ad4 == -1) {

                temporal.ad4 = selleno(archivo, temporal.llave, tamno, inicio);
            }
        }
        if (temporal.ad4 != -1 & constancia == 1) {

            b_carpetas tc = recuperar_carpeta(temporal.ad4, inicio, tamno, archivo);

            if (tc.hijo1 == -1) {
                tc.hijo1 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo2 == -1) {
                tc.hijo2 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo3 == -1) {
                tc.hijo3 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo4 == -1) {
                tc.hijo4 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo5 == -1) {
                tc.hijo5 = ap_anterior;
                constancia = 0;
            } else if (tc.hijo6 == -1) {
                tc.hijo6 = ap_anterior;
                constancia = 0;
            }

            if (constancia == 0) {
                poner_princio0(archivo, inicio);
                saltar_carpetas0(archivo, tamno);
                for (int x = 0; x < temporal.ad4 - 1; x++) {

                    fseek(archivo, sizeof (b_carpetas), SEEK_CUR);

                }
                fwrite(&tc, sizeof (b_carpetas), 1, archivo);

            }

        }





        fseek(archivo, pos_actual, SEEK_SET);
        fwrite(&temporal, sizeof (inodo), 1, archivo);
        seguir0 = 0;
    }


}

void m5(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas) {
    poner_princio0(archivo, inicio);
    saltar_hastainodos0(archivo, tamano);
    for (int x = 0; x < apuntador - 1; x++) {

        fseek(archivo, sizeof (inodo), SEEK_CUR);

    }

    inodo temporal;
    fread(&temporal, sizeof (inodo), 1, archivo);
    int xxx = contar_carpetas0(carpetas);
    if (temporal.llave == apuntador && strcmp(temporal.idchar, carpetas[xxx - 1].nombre) == 0) {

        ap_anterior = temporal.llave;
        seguir0 = 0;
    }

}

void r_archivo_directorio(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpeta) {

    poner_princio0(archivo, inicio);
    saltar_hastainodos0(archivo, tamano);
    for (int x = 0; x < apuntador - 1; x++) {

        fseek(archivo, sizeof (inodo), SEEK_CUR);

    }
    inodo temporal;
    fread(&temporal, sizeof (inodo), 1, archivo);
    int xxx = contar_carpetas0(carpeta);
    if (temporal.llave == apuntador && strcmp(temporal.idchar, carpeta[xxx - 1].nombre) == 0) {

        char nombre[200];
        memset(nombre, '\0', 200);
        printf("escribir el nuevo nombre de la carpeta:\n");
        scanf("%s", nombre);

        strcpy(temporal.idchar, nombre);
        fseek(archivo, sizeof (inodo)*-1, SEEK_CUR);
        fwrite(&temporal, sizeof (inodo), 1, archivo);
        seguir0 = 0;


    }


}

void el_archivo(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas, int acutalc, int hijo) {

    poner_princio0(archivo, inicio);
    saltar_hastainodos0(archivo, tamano);
    for (int x = 0; x < apuntador - 1; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }
    inodo temporal;
    long int pactual = ftell(archivo);
    fread(&temporal, sizeof (temporal), 1, archivo);
    int xxx = contar_carpetas0(carpetas);
    //rescribir el inodo

    //rescribir los bitmpas
    if (temporal.llave == apuntador && strcmp(carpetas[xxx - 1].nombre, temporal.idchar) == 0) {

        if (temporal.ad1 != -1) {
            poner_princio0(archivo, inicio);
            saltar_bmbloques0(archivo, tamano);
            char byte = '\0';
            for (int x = 0; x < temporal.ad1 - 1; x++) {

                fseek(archivo, sizeof (byte), SEEK_CUR);
            }
            fwrite(&byte, sizeof (byte), 1, archivo);

        }

        if (temporal.ad2 != -1) {

            poner_princio0(archivo, inicio);
            saltar_bmbloques0(archivo, tamano);
            char byte = '\0';
            for (int x = 0; x < temporal.ad2 - 1; x++) {

                fseek(archivo, sizeof (byte), SEEK_CUR);
            }
            fwrite(&byte, sizeof (byte), 1, archivo);

        }

        if (temporal.ad3 != -1) {
            poner_princio0(archivo, inicio);
            saltar_bmbloques0(archivo, tamano);
            char byte = '\0';
            for (int x = 0; x < temporal.ad3 - 1; x++) {

                fseek(archivo, sizeof (byte), SEEK_CUR);
            }
            fwrite(&byte, sizeof (byte), 1, archivo);


        }

        if (temporal.ad4 != -1) {

            poner_princio0(archivo, inicio);
            saltar_bmbloques0(archivo, tamano);
            char byte = '\0';
            for (int x = 0; x < temporal.ad4 - 1; x++) {

                fseek(archivo, sizeof (byte), SEEK_CUR);
            }
            fwrite(&byte, sizeof (byte), 1, archivo);


        }





        seguir0 = 0;
        fseek(archivo, pactual, SEEK_SET);
        inodo t_inodos;
        t_inodos.llave = -1;
        strcpy(t_inodos.fecha, "vacio");
        t_inodos.tamano = -1;
        t_inodos.bloues_asignados = -1;
        t_inodos.tipo = -1;


        t_inodos.ad1 = -1;
        t_inodos.ad2 = -1;
        t_inodos.ad3 = -1;
        t_inodos.ad4 = -1;
        t_inodos.ap_n1 = -1;
        t_inodos.ap_n2 = -1;
        strcpy(t_inodos.propietario, "vacio");
        t_inodos.permisos = -1;
        strcpy(t_inodos.idchar, "vacio");


        fwrite(&t_inodos, sizeof (inodo), 1, archivo);
        long int n = c_n0(tamano);
        //bm inodos
        poner_princio0(archivo, inicio);
        fseek(archivo, sizeof (super_bloque), SEEK_CUR);
        fseek(archivo, sizeof (journaling) * n, SEEK_CUR);
        for (int x = 0; x < temporal.llave - 1; x++) { //bm inodos

            fseek(archivo, 1, SEEK_CUR);
        }
        char b = '\0';
        fwrite(&b, sizeof (b), 1, archivo);

        poner_princio0(archivo, inicio);
        saltar_carpetas0(archivo, tamano);

        for (int x = 0; x < acutalc - 1; x++) {
            fseek(archivo, sizeof (b_carpetas), SEEK_CUR);
        }


        b_carpetas temporalcarpeta;
        fread(&temporalcarpeta, sizeof (b_carpetas), 1, archivo);

        if (hijo == 1) {
            temporalcarpeta.hijo1 = -1;
        } else if (hijo == 2) {
            temporalcarpeta.hijo2 = -1;
        } else if (hijo == 3) {
            temporalcarpeta.hijo3 = -1;

        } else if (hijo == 4) {
            temporalcarpeta.hijo4 = -1;
        } else if (hijo == 5) {
            temporalcarpeta.hijo5 = -1;
        } else if (hijo == 6) {
            temporalcarpeta.hijo6 = -1;

        }

        fseek(archivo, sizeof (b_carpetas)*-1, SEEK_CUR);
        fwrite(&temporalcarpeta, sizeof (b_carpetas), 1, archivo);

        //----------------bm inodos
    }



}

void m_contenido(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas) {

    //recuperar inodo
    poner_princio0(archivo, inicio);
    saltar_hastainodos0(archivo, tamano);
    long int n = c_n0(tamano);
    inodo temporal;
    for (int x = 0; x < apuntador - 1; x++) {
        fseek(archivo, sizeof (inodo), SEEK_CUR);
    }
    fread(&temporal, sizeof (inodo), 1, archivo);
    int xxx = contar_carpetas0(carpetas);

    if (temporal.llave == apuntador && strcmp(temporal.idchar, carpetas[xxx - 1].nombre) == 0) {

        printf("Contenido: \n");

        int contadndo = 0;
        if (temporal.ad1 != -1) {
            poner_princio0(archivo, inicio);
            saltar_carpetas0(archivo, tamano);
            for (int x = 0; x < temporal.ad1 - 1; x++) {

                fseek(archivo, sizeof (b_archivos), SEEK_CUR);

            }
            b_archivos texto;
            fread(&texto, sizeof (b_archivos), 1, archivo);
            if (texto.apactual = temporal.ad1 && texto.appadre == temporal.llave) {

                for (int x = 0; x < 24; x++, contadndo++) {
                    printf("%c", texto.info[x]);
                    r_usuarios[contadndo] = texto.info[x];

                }

            }



        }

        if (temporal.ad2 != -1) {

            poner_princio0(archivo, inicio);
            saltar_carpetas0(archivo, tamano);
            for (int x = 0; x < temporal.ad2 - 1; x++) {

                fseek(archivo, sizeof (b_archivos), SEEK_CUR);

            }
            b_archivos texto;
            fread(&texto, sizeof (b_archivos), 1, archivo);
            if (texto.apactual = temporal.ad2 && texto.appadre == temporal.llave) {

                for (int x = 0; x < 24; x++, contadndo++) {
                    printf("%c", texto.info[x]);
                    r_usuarios[contadndo] = texto.info[x];
                }

            }



        }

        if (temporal.ad3 != -1) {

            poner_princio0(archivo, inicio);
            saltar_carpetas0(archivo, tamano);
            for (int x = 0; x < temporal.ad3 - 1; x++) {

                fseek(archivo, sizeof (b_archivos), SEEK_CUR);

            }
            b_archivos texto;
            fread(&texto, sizeof (b_archivos), 1, archivo);
            if (texto.apactual = temporal.ad3 && texto.appadre == temporal.llave) {

                for (int x = 0; x < 24; x++, contadndo++) {
                    printf("%c", texto.info[x]);
                    r_usuarios[contadndo] = texto.info[x];
                }

            }

        }

        if (temporal.ad4 != -1) {

            poner_princio0(archivo, inicio);
            saltar_carpetas0(archivo, tamano);
            for (int x = 0; x < temporal.ad4 - 1; x++) {

                fseek(archivo, sizeof (b_archivos), SEEK_CUR);

            }
            b_archivos texto;
            fread(&texto, sizeof (b_archivos), 1, archivo);
            if (texto.apactual = temporal.ad4 && texto.appadre == temporal.llave) {

                for (int x = 0; x < 24; x++, contadndo++) {
                    printf("%c", texto.info[x]);
                    r_usuarios[contadndo] = texto.info[x];
                }

            }

        }
        printf("\n");
        int pela;
        scanf("%i", &pela);

        seguir0 = 0;
    }


}

void buscar_carpeta0(int apuntador, FILE *archivo, long int inicio, long int tamano, int tipo, arreglo carpetas) {
    poner_princio0(archivo, inicio);
    long int lugar_escribir = 0;
    long int lugar_reescribircarp;


    saltar_hastainodos0(archivo, tamano);
    long int n = c_n0(tamano);


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
            //----------------------------------------------------
            if (tc.apactual == apuntador) {
                fseek(archivo, sizeof (b_carpetas)*-1, SEEK_CUR);
                encontro = 1;
                lugar_reescribircarp = ftell(archivo);
                lugar_escribir = ftell(archivo);
                break;
            }
        }

    }

    int totc = contar_carpetas0(carpetas);

    if (control0 < totc - 1) { //


        if (tc.hijo1 != -1 && seguir0 == 1 && encontro == 1) { //-------------------------------

            buscar_inodo00(tc.hijo1, archivo, inicio, tamano, carpetas);
            //------------------------asdfasdf

        }


        if (tc.hijo2 != -1 && seguir0 == 1 && encontro == 1) {

            buscar_inodo00(tc.hijo2, archivo, inicio, tamano, carpetas);
        }

        if (tc.hijo3 != -1 && seguir0 == 1 && encontro == 1) {
            buscar_inodo00(tc.hijo3, archivo, inicio, tamano, carpetas);

        }
        if (tc.hijo4 != -1 && seguir0 == 1 && encontro == 1) {
            buscar_inodo00(tc.hijo4, archivo, inicio, tamano, carpetas);

        }

        if (tc.hijo5 != -1 && seguir0 == 1 && encontro == 1) {
            buscar_inodo00(tc.hijo5, archivo, inicio, tamano, carpetas);

        }

        if (tc.hijo6 != -1 && seguir0 == 1 && encontro == 1) {
            buscar_inodo00(tc.hijo6, archivo, inicio, tamano, carpetas);

        }



    } else {


        if (encontro == 1) {
            char nigua[20];
            int tyoe = 0;

            for (int x = 0; x < 20; x++) {
                nigua[x] = '\0';
            }

            int xxx = contar_carpetas0(carpetas);
            strcpy(nigua, carpetas[xxx - 1].nombre);

            //-----------------------------
            for (int x = 0; x < strlen(nigua); x++) {
                if (nigua[x] == 46) {
                    tyoe = 1;
                    break;
                }
            }



            if (tc.hijo1 != -1 && encontro == 1 && seguir0 == 1) { //si un inodo de bloques o un inodo de 
                if (tyoe == 0) {
                    if (action == 3) {

                        r_archivo_directorio(tc.hijo1, archivo, inicio, tamano, carpetas);


                    } else if (action == 4) {

                        m5(tc.hijo1, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo1 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    } else if (action == 5) {
                        m6(tc.hijo1, archivo, inicio, tamano, carpetas);
                    }

                } else if (tyoe == 1) {

                    if (action == 1) {

                        m_contenido(tc.hijo1, archivo, inicio, tamano, carpetas);

                    } else if (action == 2) {
                        el_archivo(tc.hijo1, archivo, inicio, tamano, carpetas, tc.apactual, 1);

                    } else if (action == 3) {


                        r_archivo_directorio(tc.hijo1, archivo, inicio, tamano, carpetas);


                    } else if (action == 4) {


                        m5(tc.hijo1, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo1 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }

                    }



                }

            }
            if (tc.hijo2 != -1 && encontro == 1 && seguir0 == 1) {
                if (tyoe == 0) {

                    if (action == 3) {

                        r_archivo_directorio(tc.hijo2, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {


                        m5(tc.hijo2, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo2 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    } else if (action == 5) {
                        m6(tc.hijo2, archivo, inicio, tamano, carpetas);
                    }


                } else if (tyoe == 1) {

                    if (action == 1) {

                        m_contenido(tc.hijo2, archivo, inicio, tamano, carpetas);

                    } else if (action == 2) {
                        el_archivo(tc.hijo2, archivo, inicio, tamano, carpetas, tc.apactual, 2);

                    } else if (action == 3) {

                        r_archivo_directorio(tc.hijo2, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {

                        m5(tc.hijo2, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo2 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }

                    }


                }

            }

            if (tc.hijo3 != -1 && encontro == 1 && seguir0 == 1) {
                if (tyoe == 0) {
                    if (action == 3) {

                        r_archivo_directorio(tc.hijo3, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {

                        m5(tc.hijo3, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo3 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    } else if (action == 5) {
                        m6(tc.hijo3, archivo, inicio, tamano, carpetas);
                    }


                } else if (tyoe == 1) {
                    if (action == 1) {

                        m_contenido(tc.hijo3, archivo, inicio, tamano, carpetas);

                    } else if (action == 2) {
                        el_archivo(tc.hijo3, archivo, inicio, tamano, carpetas, tc.apactual, 3);

                    } else if (action == 3) {

                        r_archivo_directorio(tc.hijo3, archivo, inicio, tamano, carpetas);
                    } else if (action == 4) {


                        m5(tc.hijo3, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo3 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    }


                }


            }
            if (tc.hijo4 != -1 && encontro == 1 && seguir0 == 1) {
                if (tyoe == 0) {
                    if (action == 3) {

                        r_archivo_directorio(tc.hijo4, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {


                        m5(tc.hijo4, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo4 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    } else if (action == 5) {
                        m6(tc.hijo4, archivo, inicio, tamano, carpetas);
                    }


                } else if (tyoe == 1) {

                    if (action == 1) {

                        m_contenido(tc.hijo4, archivo, inicio, tamano, carpetas);

                    } else if (action == 2) {
                        el_archivo(tc.hijo4, archivo, inicio, tamano, carpetas, tc.apactual, 4);

                    } else if (action == 3) {


                        r_archivo_directorio(tc.hijo4, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {


                        m5(tc.hijo4, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo4 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    }


                }

            }

            if (tc.hijo5 != -1 && encontro == 1 && seguir0 == 1) {
                if (tyoe == 0) {
                    if (action == 3) {

                        r_archivo_directorio(tc.hijo5, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {

                        m5(tc.hijo5, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo5 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    } else if (action == 5) {
                        m6(tc.hijo5, archivo, inicio, tamano, carpetas);
                    }


                } else if (tyoe == 1) {
                    if (action == 1) {

                        m_contenido(tc.hijo5, archivo, inicio, tamano, carpetas);

                    } else if (action == 2) {
                        el_archivo(tc.hijo5, archivo, inicio, tamano, carpetas, tc.apactual, 5);

                    } else if (action == 3) {

                        r_archivo_directorio(tc.hijo5, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {

                        m5(tc.hijo5, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo5 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    }


                }

            }

            if (tc.hijo6 != -1 && encontro == 1 && seguir0 == 1) {
                if (tyoe == 0) {
                    if (action == 3) {

                        r_archivo_directorio(tc.hijo6, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {


                        m5(tc.hijo6, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo6 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }

                    } else if (action == 5) {
                        m6(tc.hijo6, archivo, inicio, tamano, carpetas);
                    }



                } else if (tyoe == 1) {

                    if (action == 1) {

                        m_contenido(tc.hijo6, archivo, inicio, tamano, carpetas);

                    } else if (action == 2) {
                        el_archivo(tc.hijo6, archivo, inicio, tamano, carpetas, tc.apactual, 6);

                    } else if (action == 3) {

                        r_archivo_directorio(tc.hijo6, archivo, inicio, tamano, carpetas);

                    } else if (action == 4) {


                        m5(tc.hijo6, archivo, inicio, tamano, carpetas);
                        if (seguir0 == 0) {
                            tc.hijo6 = -1;
                            fseek(archivo, lugar_escribir, SEEK_SET);
                            fwrite(&tc, sizeof (b_carpetas), 1, archivo);

                        }
                    }


                }


            }

        }

    }


}

void buscar_inodo00(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas) {

    poner_princio0(archivo, inicio);
    saltar_hastainodos0(archivo, tamano);
    int encontrado = 0;
    long int n = c_n0(tamano) - 1;
    inodo temporal;
    char nombre[20];
    strcpy(nombre, carpetas[control0].nombre);
    control0++;
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

        if (temporal.ad1 != -1 && seguir0 == 1) {


            if (temporal.tipo == 0) {
                buscar_carpeta0(temporal.ad1, archivo, inicio, tamano, temporal.tipo, carpetas);
                //crear carpeta

            } else if (temporal.tipo == 1) {

            }
            //rescribir inodo



        }

        if (temporal.ad2 != -1 && seguir0 == 1) { //crear nueva carpeta


            if (temporal.tipo == 0) {
                buscar_carpeta0(temporal.ad2, archivo, inicio, tamano, temporal.tipo, carpetas);

            } else if (temporal.tipo == 1) {

            }

        }

        if (temporal.ad3 != -1 && seguir0 == 1) {

            if (temporal.tipo == 0) {
                buscar_carpeta0(temporal.ad3, archivo, inicio, tamano, temporal.tipo, carpetas);


            } else if (temporal.tipo == 1) {

            }
        }

        if (temporal.ad4 != -1 && seguir0 == 1) {

            if (temporal.tipo == 0) {

                buscar_carpeta0(temporal.ad4, archivo, inicio, tamano, temporal.tipo, carpetas);

            } else if (temporal.tipo == 1) {

            }

        }

    } else {

        control0--;
    }

}

void mostrar_contenido(int numero, idspart ids, arreglo carpetas) {

    action = 0;
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    action = 1;

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
    long int n = c_n0(tamano);

    FILE *archivo = fopen(ids[numero].ruta, "r+b");
    if (archivo) {
        control0 = 0;
        seguir0 = 1;
        //        tipo00 = 0;

        buscar_inodo00(1, archivo, inicio, tamano, carpetas);


        fclose(archivo);

    }



}

void eliminar_architvo(int numero, idspart ids, arreglo carpetas) {

    action = 0;
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    action = 2;

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
    long int n = c_n0(tamano);

    FILE *archivo = fopen(ids[numero].ruta, "r+b");
    if (archivo) {
        control0 = 0;
        seguir0 = 1;
        //        tipo00 = 0;

        buscar_inodo00(1, archivo, inicio, tamano, carpetas);
        if (seguir0 == 0) {
            acutalizar_jour(inicio, archivo, 1, carpetas, tamano);
        }
        fclose(archivo);

    }


}

void renombrar_archivo_directorio(int numero, idspart ids, arreglo carpetas) {
    action = 0;
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    action = 3;

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
    long int n = c_n0(tamano);

    FILE *archivo = fopen(ids[numero].ruta, "r+b");
    if (archivo) {
        control0 = 0;
        seguir0 = 1;
        //        tipo00 = 0;

        buscar_inodo00(1, archivo, inicio, tamano, carpetas);
        if (seguir0 == 0) {
            acutalizar_jour(inicio, archivo, 2, carpetas, tamano);
        }
        fclose(archivo);

    }

}

void mover_archivo_o_carpeta(int numero, idspart ids, arreglo carpetas, arreglo carpetas2, int quehago) {


    action = 0;
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    action = 4;
    ap_anterior = -1;


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
    long int n = c_n0(tamano);

    FILE *archivo = fopen(ids[numero].ruta, "r+b");
    if (archivo) {
        control0 = 0;
        seguir0 = 1;
        //        tipo00 = 0;

        buscar_inodo00(1, archivo, inicio, tamano, carpetas);
        printf("apuntador a nodo que se va a mover %i \n", ap_anterior);
        action = 5;


        if (quehago == 1) {
            m6(1, archivo, inicio, tamano, carpetas2);
            acutalizar_jour(inicio, archivo, 3, carpetas, tamano);


        } else if (ap_anterior != -1 && seguir0 == 0 && quehago != 1) {
            seguir0 = 1;
            control0 = 0;
            buscar_inodo00(1, archivo, inicio, tamano, carpetas2);
            acutalizar_jour(inicio, archivo, 3, carpetas, tamano); //se lo acabo de agregar
        }
        fclose(archivo);

    }

}

int recuperar_usurios(int numero, idspart ids, arreglo carpetas) {
    memset(r_usuarios, '\0', 96);




    action = 0;
    mbr tmbr = recuperar_mbr(ids[numero].ruta);
    action = 1;

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
    if (archivo) {
        control0 = 0;
        seguir0 = 1;

        buscar_inodo00(1, archivo, inicio, tamano, carpetas);
        return seguir0;


    }


}









