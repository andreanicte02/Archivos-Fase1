/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mbr.h"



void rebovinar(FILE *archivo){
    
    fseek(archivo,0, SEEK_SET);
}

void inicio_particion(FILE *archivo, long int inicio){


}

mbr recuperar_mbr(char *path) {

    FILE *archivo = fopen(path, "r+b");
    if (archivo != NULL) {

        rewind(archivo);

        mbr temporal;
        fread(&temporal, sizeof (mbr), 1, archivo);

        //printf("------ %s \n", temporal.nombre);


        fclose(archivo);
        return temporal;

    }

    mbr t;
    return t;

}


