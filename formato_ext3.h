/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   formato_ext3.h
 * Author: nicte
 *
 * Created on 28 de agosto de 2017, 16:01
 */

#ifndef FORMATO_EXT3_H
#define FORMATO_EXT3_H

typedef struct {
    int n_inodos;
    int n_bloques;
    int tamano_bloque; // es 74
    int firma; //carnet

    int bloques_free;
    int indoso_free;

    char fecha[50];

    int bm_inodos; // apuntador al inicio del bm de los inodos en archivo
    int bm_bloques; // apuntador al inicio del bm de los bloques en el archivo

    int blo_inodos; //apuntador al inicio de los bloques en el inicio
    int blo_bloques; //apunador al inicio de los bloques en el inicio

    int blo_indirectos; // apuntador al inicio de los bloques inidirectos


    int primer_inodo; //primer bloque libre inodo BLOQUES;
    int primer_bloque; //primer bloque libre de bloque

    int primer_inodo_libre; //
    int primer_bloque_libre; //primer inodo libre  
    int primer_indircto_libre;






} super_bloque;

typedef struct {
    char fecha[50];
    char operacion[150];

} journaling;

typedef struct {
    int llave;
    int tamano; // tamaño en bytes;
    int bloues_asignados; //bloques asignados;
    int tipo; // inodo archivo carpeta o enlace;
    int ad1;
    int ad2; //apuntadores directos
    int ad3;
    int ad4;
    int ap_n1;
    int ap_n2;
    int permisos;


    char propietario[50];
    char fecha[50];
    char idchar[200];


} inodo;

typedef struct {
    char padre[25];
    char nombre[25];

    int hijo1; //1
    int hijo2; //2
    int hijo3; //3
    int hijo4; //4
    int hijo5; //5
    int hijo6; //6
    int appadre;
    int apactual;

} b_carpetas;

typedef struct {
    int appadre;
    int apactual;
    char padre[25];
    char nombre[25];
    char info[24];
    char sobra[2];

} b_archivos;

typedef struct {
    int pos; //pos del bloque
    int tipo;
    int ap1;
    int ap2;
    int ap3;
    int ap4;
    int ap5;
    int ap6;

} b_indirecto;


#endif /* FORMATO_EXT3_H */

