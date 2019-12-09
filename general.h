/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   general.h
 * Author: nicte
 *
 * Created on 26 de agosto de 2017, 20:35
 */
#include "mbr.h"

#ifndef GENERAL_H
#define GENERAL_H

#ifdef __cplusplus
//extern "C" {
#endif
void rebovinar(FILE *archivo);
void inicio_particion (FILE *archivo, long int inicio);
mbr recuperar_mbr(char *path);


#ifdef __cplusplus
//}
#endif

#endif /* GENERAL_H */

