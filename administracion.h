/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   administracion.h
 * Author: nicte
 *
 * Created on 12 de septiembre de 2017, 12:24
 */

#ifndef ADMINISTRACION_H
#define ADMINISTRACION_H

#ifdef __cplusplus
//extern "C" {
#endif

void buscar_carpeta0(int apuntador, FILE *archivo, long int inicio, long int tamano, int tipo, arreglo carpetas);

void buscar_inodo00(int apuntador, FILE *archivo, long int inicio, long int tamano, arreglo carpetas);
void mostrar_contenido(int numero, idspart ids, arreglo carpetas);
void eliminar_architvo(int numero, idspart ids, arreglo carpetas);

void renombrar_archivo_directorio(int numero, idspart ids, arreglo carpetas);
void mover_archivo_o_carpeta(int numero, idspart ids, arreglo carpetas, arreglo carpetas2, int quehago);
int recuperar_usurios(int numero, idspart ids, arreglo carpetas);

#ifdef __cplusplus
//}
#endif

#endif /* ADMINISTRACION_H */

