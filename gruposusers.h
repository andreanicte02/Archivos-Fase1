/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gruposusers.h
 * Author: nicte
 *
 * Created on 2 de octubre de 2017, 0:29
 */

#ifndef GRUPOSUSERS_H
#define GRUPOSUSERS_H

#ifdef __cplusplus
//extern "C" {
#endif
typedef struct{
    char nombre[50];   
}grupos [100];

typedef struct{
    
    char grupo[50];   
    char nombre[50];  
    char contra[50];  
    
}usuarios [100];





#ifdef __cplusplus
//}
#endif

#endif /* GRUPOSUSERS_H */

