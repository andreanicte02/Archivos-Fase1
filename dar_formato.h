/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   dar_formato.h
 * Author: nicte
 *
 * Created on 30 de agosto de 2017, 16:58
 */

#ifndef DAR_FORMATO_H
#define DAR_FORMATO_H

#ifdef __cplusplus
//extern "C" {
#endif


long int calcular_n(long int tamano_part);
long int inicio_bm_inodos(long int inicio, long int n) ;
long int inicio_bm_bloques(long int incio, long int n);
long int inicio_inodos(long int inicio, long int n);
long int inicio_bloques(long int inicio, long int n);
void inicio(FILE *archivo, long int tamano);
void crearraiz(char *path, long int inicio_particion, long int tamano);
void formato_ext3(idspart part, int numero);

#ifdef __cplusplus
//}
#endif

#endif /* DAR_FORMATO_H */

