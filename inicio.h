/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   inicio.h
 * Author: nicte
 *
 * Created on 22 de agosto de 2017, 18:55
 */
#include "mbr.h"
#ifndef INICIO_H
#define INICIO_H

#ifdef __cplusplus
//extern "C" {
#endif

void menu_darformato(idspart part, char*id);
void menu_montar(idspart part, char *id);
void menu_desmontar(idspart part, char *id);
void menudico1(char *nombre, char* path, idspart part) ;
void menu();




#ifdef __cplusplus
//}
#endif

#endif /* INICIO_H */

