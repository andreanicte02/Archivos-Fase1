/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   manejo_archivos.h
 * Author: nicte
 *
 * Created on 4 de septiembre de 2017, 0:57
 */

#ifndef MANEJO_ARCHIVOS_H
#define MANEJO_ARCHIVOS_H

#include "gruposusers.h"


#ifdef __cplusplus
//extern "C" {
#endif


int contar_carpetas(arreglo part);
long int c_n(long int tamano_part);
void poner_princio(FILE *archivo, long int inicio);

void saltar_hastainodos(FILE *archivo, long int tamano);
int buscar_carpeta(int apuntador, FILE *archivo, long int inicio, long int tamano, int tipo, arreglo carpetas);

void buscar_inodo(int apuntador, FILE *archivo, long int inicio, long int tamano,  arreglo carpetas);
void crear_carpeta(int numero, idspart ids, arreglo carpetas);
void agregarusuarios(int numero, idspart ids, arreglo carpetas,grupos gu, usuarios us);

void g_carpetasr1(FILE *disco, FILE *grafica, long int inicio, long int tamano);
void grafiacr_carpetas_subcarpetasr1(int numero, idspart ids);

int g_carpetar2(int apuntador, FILE *archivo, long int inicio, long int tamano, int tipo, arreglo carpetas, FILE *grafica);
void g_inodor2(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas, FILE *grafica);
void graficar_archivos(int numero, idspart ids, arreglo carpetas, int cual, int pedo);
void grafica_general(int numero, idspart ids);
void reporte_bm_inodos(int numero, idspart ids);
void reporte_bm_bdatos(int numero, idspart ids);
void reporte_de_inodos(int numero, idspart ids);
void reporte_de_bloques(int numero, idspart ids);
void reporte_journaling(int numero, idspart ids);
#ifdef __cplusplus
//}
#endif

#endif /* MANEJO_ARCHIVOS_H */

