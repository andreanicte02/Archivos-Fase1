/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   creacion_discos.h
 * Author: nicte
 *
 * Created on 22 de agosto de 2017, 19:29
 */
#include "mbr.h"

#ifndef CREACION_DISCOS_H
#define CREACION_DISCOS_H

#ifdef __cplusplus
//extern "C" {
#endif
void mostrar(idspart part);
void ids(char *nombre, idspart part, int n_par, char *ruta);
int hay_espacio(long int tamano_disco, long int inicio, long int tamano_particion);
void particionar(char *nombre_disco, char *path, long int tamano, idspart part);
void create_mbr(char *nombre, long int tamano, char *path);
int existe_disco(char *path);
int es_multiplo(long int numero);
void c_disco();
void el_disco();
void aumentar_disco();



#ifdef __cplusplus
//}
#endif

#endif /* CREACION_DISCOS_H */

