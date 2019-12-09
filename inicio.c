/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "creacion_discos.h"
#include "mbr.h"
#include "inicio.h"
#include "dar_formato.h"
#include "general.h"
#include "formato_ext3.h"
#include "manejo_archivos.h"
#include "administracion.h"
#include "gruposusers.h"

usuarios lu;
grupos lg;

void iniciar() {

    for (int x = 0; x < 100; x++) {
        strcpy(lu[x].contra, "vacio");
        strcpy(lu[x].grupo, "vacio");
        strcpy(lu[x].nombre, "vacio");
        strcpy(lg[x].nombre, "vacio");
    }
}

void menu_darformato(idspart part, char*id) {


    for (int x = 0; x < 20; x++) {

        if (strcmp(part[x].id, id) == 0 && part[x].montado == 1) {

            iniciar();
            strcpy(lu[0].contra, "201404104");
            strcpy(lu[0].nombre, "root");
            strcpy(lu[0].grupo, "root");

            strcpy(lg[0].nombre, "root");


            printf("%s --direc del dsico\n", part[x].ruta);
            formato_ext3(part, x);
            part[x].formato = 1;



            break;
        }

        if (x == 19) {
            printf("la particion no se encuentra montada en el sistema\n");
        }

    }


}

void menu_montar(idspart part, char *id) {

    for (int x = 0; x < 20; x++) {

        /*long int a = sizeof (journaling);
        long int b = sizeof (inodo);
        long int c = sizeof (b_indirecto);
        long int d = sizeof (b_archivos);
        long int e = sizeof (b_carpetas);
        long int sad = sizeof (super_bloque);*/
        if (strcmp(part[x].id, id) == 0 && part[x].montado == 0) {
            part[x].montado = 1;
            printf("se monto la particion %s \n", id);


            break;
        }

        if (x == 19) {

            printf("No se encontro la particion que %s", id);
        }


    }



}

void menu_desmontar(idspart part, char *id) {

    for (int x = 0; x < 20; x++) {

        if (strcmp(part[x].id, id) == 0 && part[x].montado == 1) {
            part[x].montado = 0;
            printf("se desmonto monto la particion %s \n", id);




            break;
        }

        if (x == 19) {

            printf("No se encontro la particion %s o no se encuentra montada", id);
        }


    }


}

void menudico1(char *nombre, char* path, idspart part) {


    int i = 0;
    // printf("%s---------",part[0].id);
    //strcpy(part[0].id,"hey");
    while (i == 0) {

        printf("1. Crear Particion: \n");
        int option;
        scanf("%i", &option);

        if (option == 1) {
            int tamano;
            printf("Ingresar el tamaño de la particion: \n");
            scanf("%i", &tamano);
            particionar(nombre, path, tamano * 1024 * 1024, part);




        } else {

            i = 1;
        }




    }
}

void despedazar(arreglo pedaso, char* ruta, int tope, char *nombre) {
    strcpy(pedaso[0].nombre, "/");
    char otro[20];
    int contador = 0;
    int contador2 = 1;


    for (int f = 0; f < 20; f++) {
        otro[f] = '\0';
    }

    for (int x = 1; x < tope; x++, contador++) {

        if (ruta[x] == '/') {
            //printf("%s- \n", otro);
            //printf("%i \n", contador2);


            strcpy(pedaso[contador2].nombre, otro);
            contador2++;


            for (int f = 0; f < 20; f++) {
                otro[f] = '\0';
            }
            contador = -1;


        } else {
            otro[contador] = ruta[x];
        }

    }

    // strcpy(pedaso[contador2].nombre, nombre);





}

void otromenu(idspart part) {

    char id[50];
    printf("Ingrese el id a la particion que va manejar: \n");
    scanf("%s", id);
    arreglo pedaso;
    arreglo b_archivo;

    for (int x = 0; x < 25; x++) {

        strcpy(pedaso[x].nombre, "vacio");
    }


    for (int x = 0; x < 25; x++) {

        strcpy(b_archivo[x].nombre, "vacio");
    }




    for (int x = 0; x < 20; x++) {

        if (strcmp(id, part[x].id) == 0 && part[x].montado == 1 && part[x].formato == 1) {

            int i = 0;

            //-----------------------logearse
            char users0[10];
            char contra0[15];
            memset(users0, '\0', 10);
            memset(contra0, '\0', 15);
            printf("Ingrese el usuario:");
            scanf("%s", users0);
            printf("Ingrese el la contrasena:");
            scanf("%s", contra0);
            arreglo poc;

            for (int x = 0; x < 25; x++) {

                strcpy(poc[x].nombre, "vacio");
            }
            char otra[20] = "/users.txt/";
            char p[20];
            despedazar(poc, otra, strlen(otra), p);


            if (strcmp(users0, "root") == 0 && strcmp(contra0, "201404104") == 0 && recuperar_usurios(x, part, poc) == 0) {
                //printf("si funicona \n");



                while (i == 0) {
                    int option;
                    printf("----------Administrar Archivos------- \n");
                    printf("1. Crear carpeta \n");
                    printf("2. Graficar directorios \n");
                    printf("3. Graficar archivos \n");
                    printf("4. Graficar general \n");
                    printf("5. Reporte grafico de los archivos de un direcotrio \n"); //aca me quede
                    printf("6. Reporte bm inodos \n");
                    printf("7. Reporte bm de bloques de datos \n");
                    printf("8. Reporte de inodos \n");
                    printf("9. Reporte de bloques \n");
                    printf("10. Reporte de journaling \n");
                    printf("11. Mostrar archivo: \n");
                    printf("12. Eliminar archivo: \n");
                    printf("13. Renombrar archivo/directorio: \n");
                    printf("14. Mover archivo/directorio: \n");
                    printf("15. Agregar grupos/usuarios:\n");
                    printf("16. Sincronziar\n");
                    printf("17. salir");


                    scanf("%i", &option);

                    if (option == 1) {

                        char nuevo[20];
                        char ruta[50];
                        printf("Ingrese el destino \n");
                        scanf("%s", ruta);
                        strcat(ruta, "/");

                        despedazar(pedaso, ruta, strlen(ruta), nuevo);
                        crear_carpeta(x, part, pedaso);

                        for (int x = 0; x < 25; x++) {

                            strcpy(pedaso[x].nombre, "vacio");
                        }


                        //----------------------------------------------------------------------crear carpeta y texto
                    } else if (option == 2) {
                        grafiacr_carpetas_subcarpetasr1(x, part);

                    } else if (option == 3 || option == 5) {

                        char nuevo[20];
                        char ruta[50];
                        printf("Ingrese el destino \n");
                        scanf("%s", ruta);

                        int pedo = 0;
                        if (strlen(ruta) == 1) {
                            pedo = 1;
                        }

                        strcat(ruta, "/");

                        despedazar(b_archivo, ruta, strlen(ruta), nuevo);
                        graficar_archivos(x, part, b_archivo, option, pedo);

                        for (int x = 0; x < 25; x++) {

                            strcpy(b_archivo[x].nombre, "vacio");
                        }



                    } else if (option == 4) {
                        grafica_general(x, part);

                    } else if (option == 6) {
                        reporte_bm_inodos(x, part);

                    } else if (option == 7) {
                        reporte_bm_bdatos(x, part);
                    } else if (option == 8) {
                        reporte_de_inodos(x, part);

                    } else if (option == 9) {
                        reporte_de_bloques(x, part);
                    } else if (option == 10) {

                        reporte_journaling(x, part);
                    } else if (option == 11) {
                        char nuevo[20];
                        char ruta[50];
                        printf("Ingrese el archivo que desea mostrar:\n");
                        scanf("%s", ruta);
                        strcat(ruta, "/");

                        despedazar(pedaso, ruta, strlen(ruta), nuevo);
                        mostrar_contenido(x, part, pedaso);

                        for (int x = 0; x < 25; x++) {

                            strcpy(pedaso[x].nombre, "vacio");
                        }


                    } else if (option == 12) {

                        char nuevo[20];
                        char ruta[50];
                        printf("Ingrese el archivo que desea eliminar:\n");
                        scanf("%s", ruta);
                        strcat(ruta, "/");

                        despedazar(pedaso, ruta, strlen(ruta), nuevo);
                        eliminar_architvo(x, part, pedaso);
                        for (int x = 0; x < 25; x++) {

                            strcpy(pedaso[x].nombre, "vacio");
                        }

                    } else if (option == 13) {

                        char nuevo[20];
                        char ruta[50];
                        printf("Ingrese el archivo/directorio que desea renombrar:\n");
                        scanf("%s", ruta);
                        strcat(ruta, "/");

                        despedazar(pedaso, ruta, strlen(ruta), nuevo);
                        renombrar_archivo_directorio(x, part, pedaso);
                        for (int x = 0; x < 25; x++) {

                            strcpy(pedaso[x].nombre, "vacio");
                        }


                    } else if (option == 14) {

                        char nuevo[20];
                        char ruta[50];
                        char ruta2[50];
                        printf("Ingrese el archivo/directorio que desea mover:\n");
                        scanf("%s", ruta);
                        strcat(ruta, "/");

                        printf("Ingrese la nueva ruta a donde se va amover mover:\n");
                        scanf("%s", ruta2);
                        int pedo = 0;
                        pedo = strlen(ruta2);


                        strcat(ruta2, "/");

                        despedazar(pedaso, ruta, strlen(ruta), nuevo);
                        despedazar(b_archivo, ruta2, strlen(ruta2), nuevo);

                        if (pedo == 1) {
                            strcpy(b_archivo[0].nombre, "/");
                            for (int x = 1; x < 25; x++) {

                                strcpy(b_archivo[x].nombre, "vacio");

                            }

                        }



                        mover_archivo_o_carpeta(x, part, pedaso, b_archivo, pedo);

                        for (int x = 0; x < 25; x++) {

                            strcpy(pedaso[x].nombre, "vacio");
                        }


                        for (int x = 0; x < 25; x++) {

                            strcpy(b_archivo[x].nombre, "vacio");
                        }


                    } else if (option == 15) { //aca

                        char nuevo[20];
                        char ruta[50] = "/users.txt/";
                        despedazar(pedaso, ruta, strlen(ruta), nuevo);
                        agregarusuarios(x, part, pedaso, lg, lu);

                        for (int x = 0; x < 25; x++) {

                            strcpy(pedaso[x].nombre, "vacio");
                        }


                    } else if (option == 16) {

                        for (int y = 0; y < 100; y++) {

                            if (strcmp(lu[y].contra, "vacio") == 0) {

                            } else {
                                printf("%s %s %s \n", lu[y].nombre, lu[y].grupo, lu[y].contra);
                            }


                        }

                        for (int y = 0; y < 100; y++) {

                            if (strcmp(lg[y].nombre, "vacio") == 0) {

                            } else {

                                printf("%s\n", lg[y].nombre);

                            }



                        }
                        //ACA VOY A CREAR EL ARCHIVO
                        FILE *carga = fopen("usuario.txt", "w");
                        if (carga) {
                            int ccarnet = 0;




                            for (int y = 0; y < 100; y++) {

                                if (strcmp(lu[y].contra, "vacio") == 0) {

                                } else {

                                    for (int x1 = 0; x1 < 4; x1++) {
                                        if(x1 ==0){
                                            
                                            char cadnea2[10];
                                            char cadnea[10];
                                            memset(cadnea,'\0',10);
                                            memset(cadnea2,'\0',10);
                                            sprintf(cadnea,"%i",ccarnet);
                                            //fprintf(carga,"%s",cadnea);
                                            int rwtr= sizeof(cadnea);
                                            int paso=0;
                                            for(int x2=strlen(cadnea);x2<10;x2++){
                                                if(paso==0){
                                                    paso =1;
                                                    strcpy(cadnea2,"0");
                                                }else{
                                                    strcat(cadnea2,"0");
                                                }
                                                //fprintf(carga," ");
                                            }
                                            fprintf(carga,"%s",cadnea2);
                                            fprintf(carga,"%s",cadnea);
                                            fprintf(carga,"%c",44);
                                            ccarnet++;
                                        }else if(x1== 1){
                                           
                                            fprintf(carga,"%s",lu[y].nombre);
                                            for(int x2 =strlen(lu[y].nombre);x2<50;x2++){
                                                    fprintf(carga," ");
                                            }   
                                            
                                            fprintf(carga,"%c",44);
                                           
                                            
                                        }else if(x1 == 2)
                                        {
                                        
                                            
                                            fprintf(carga,"%s",lu[y].contra);
                                            for(int x2 =strlen(lu[y].contra);x2<50;x2++){
                                                    fprintf(carga," ");
                                            }
                                            
                                            fprintf(carga,"%c",44);
                                           
                                            //contraseña
                                        }else if(x1==3){
                                        
                                            char cadnea[50];
                                            memset(cadnea,'\0',50);
                                            
                                            if(strcmp(lu[y].grupo,"root")==0){
                                                strcpy(cadnea,"root");
                                            }else{
                                                strcpy(cadnea,"cliente");
                                            
                                            }
                                            
                                            fprintf(carga,"%s",cadnea);
                                            for(int x2=strlen(cadnea);x2<50;x2++){
                                                fprintf(carga," ");
                                            }
                                            fprintf(carga,"\n");
                                            
                                            
                                        }
                                        
                                        
                                        //tipo
                                    }

                                }



                            }

                            fclose(carga);
                            //----llamar de cosnola
                            system("sudo su");
                            //system("./ejecutable.sh");
                        }





                    } else if (option == 17) {
                        i = 1;

                    } else {

                        for (int x = 0; x < 25; x++) {

                            strcpy(pedaso[x].nombre, "vacio");
                        }


                        for (int x = 0; x < 25; x++) {

                            strcpy(b_archivo[x].nombre, "vacio");
                        }



                        i = 1;
                    }

                }



            }
            break;
        }

        if (x == 19) {

            printf("la particion no se ecuentra montada, o no tiene formato o no existe \n");
        }

    }



}

void menu() {

    idspart ids; //escribir en un archivo
    iniciar(); //ddar formato al disco
    char pbtiacora[150] = "/home/nicte/Escritorio/discos/bitacora.disk";

    FILE *bit = fopen(pbtiacora, "r+b");

    if (bit) { //si  existe

        fseek(bit, 0, SEEK_SET);
        fread(&ids, sizeof (idspart), 1, bit);
        printf("%s \n", ids[0].id);



        fclose(bit);
    } else {

        char pbit[150] = "/home/nicte/Escritorio/discos/bitacora.disk";

        FILE *porno = fopen(pbit, "w+b");
        for (int gaby = 0; gaby < 20; gaby++) {
            ids[gaby].letra = 96;
            strcpy(ids[gaby].nombre_disco, "vacio");
            strcpy(ids[gaby].id, "vacio");
            strcpy(ids[gaby].ruta, "vacio");
            ids[gaby].numero = 0;
            ids[gaby].montado = 0;
            ids[gaby].formato = 0;

        }
        fwrite(&ids, sizeof (idspart), 1, porno);
        fclose(porno);


    }
    int i = 0;
    while (i == 0) {

        printf("---MENU----\n");
        printf("1. Crear disco \n");
        printf("2. Eliminar disco \n");
        printf("3. Aumentar disco \n");
        printf("4. Montar Disco \n");
        printf("5. Montar Particion: \n");
        printf("6. Desmontar particion: \n");
        printf("7. Ingrese id de la particion a la que le va asignar el formato: \n");
        printf("8. Menu opciones");

        int option;
        scanf("%i", &option);

        if (option == 1) {
            c_disco();
        } else if (option == 2) {
            el_disco();
        } else if (option == 3) {
            aumentar_disco();

        } else if (option == 4) {
            char nombre[50];
            char nombre2[50];
            char path[150];
            long int tamano;

            printf("Ingrese el nombre del disco: \n");
            scanf(" %[^\n]s", nombre);
            strcpy(nombre2, nombre);

            printf("Ingrese la ruta del disco: \n");
            scanf(" %[^\n]s", path);
            strcat(path, "/");
            strcat(path, nombre2);
            strcat(path, ".disk");
            printf("%s\n", path);

            if (existe_disco(path) == 1) {

                menudico1(nombre2, path, ids);
                //printf("%s---------",ids[0].id);
                //printf("%s---------",ids[1].id);

            } else {

                printf("el disco no existe \n");
            }





        } else if (option == 5) {

            char id[50];
            printf("Ingrese el id de la particion que va a montar: \n");
            scanf("%s", id);
            menu_montar(ids, id);

        } else if (option == 6) {

            char id[50];
            printf("Ingrese el id de la particion que va a desmontar: \n");
            scanf("%s", id);
            menu_desmontar(ids, id);
        } else if (option == 7) {

            char id[50];
            printf("Ingrese el id de la particion a que le va dar formato: \n");
            scanf("%s", id);

            menu_darformato(ids, id);
        } else if (option == 8) {
            otromenu(ids);
        }


        char pbtiacora[150] = "/home/nicte/Escritorio/discos/bitacora.disk";
        FILE *bit = fopen(pbtiacora, "r+b");
        if (bit) { //si  existe
            idspart otros;
            for (int x = 0; x < 20; x++) {
                otros[x].formato = ids[x].formato;
                strcpy(otros[x].id, ids[x].id);
                otros[x].letra = ids[x].letra;
                otros[x].montado = ids[x].montado;
                strcpy(otros[x].nombre_disco, ids[x].nombre_disco);
                otros[x].numero = ids[x].numero;
                strcpy(otros[x].ruta, ids[x].ruta);
            }
            fseek(bit, 0, SEEK_SET);
            fwrite(&otros, sizeof (idspart), 1, bit);
            fclose(bit);
        }



    }
}

