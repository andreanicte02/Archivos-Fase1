/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mbr.h
 * Author: nicte
 *
 * Created on 26 de agosto de 2017, 18:37
 */

#ifndef MBR_H
#define MBR_H

typedef struct{

    int numero; // cambia la particion
    char letra; // letra cambia el disco
    char id[50]; //vdletrainumero
    char ruta[200]; //ruta del disco 
    char nombre_disco[50]; //nombre
    int montado;
    int formato; //si esta particion ya tiene formato 0 es no 1 es si
    
    
}idspart [20];


typedef struct{
    char nombre[50];
    int no_partition;
    long int tamano; //en bits el final de la particion
    long int inicio; //creo que tambien puedo manejar el estado con el inicio
    int estado;//ocupada o no
    char tipo[1];
    
}partiton [4];


typedef struct{

     char nombre[50];
     char path [200];
     long int tamano;
     int particiones;
     long int libre;//espacio libre en bytes
     partiton par;
    
     
}mbr;


typedef struct{
    char nombre[20];
    int valido;
}arreglo[25];

#endif /* MBR_H */

