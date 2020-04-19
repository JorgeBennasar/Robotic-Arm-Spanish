// ----------- MODULO UART (Universal Asynchronous Receiver Transmitter)
// Modulo encargado de gestionar las comunicaciones por el puerto serie.

#ifndef _UART_H
#define _UART_H

#include "config.h"
#include "p33FJ32MC202.h"

// Velocidad de transmision en baudios (bps)
#define BAUDRATE 38400

// Tamaño de los vectores y colas
#define TAM_TR_UART 200 // Cola de transmisión
#define TAM_REC_UART 200 // Cola de recepción
#define TAM_MENSAJE_REC 100 // Mensaje recibido

// Prioridad de las interrupciones (max. 7 - min. 1)
#define PR_INT_TX_UART 4
#define PR_INT_RX_UART 4

// --------------- DECLARACION DE LAS FUNCIONES PUBLICAS ---------

void InicializarUART(void);
int HayAlgoRecibido(void);
void Transmite(void);
void PonerEnColaTransmisionUART(unsigned char uc_caracter);
unsigned char SacarDeColaRecepcionUART();
void Transmite(void);

#endif