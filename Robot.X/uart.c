#include "uart.h"

static char uc_cola_transmision[TAM_TR_UART];
static char uc_cola_recepcion[TAM_REC_UART];
static unsigned int ui_icabeza_tr = 0;
static unsigned int ui_icola_tr = 0;
static unsigned int ui_icabeza_rec = 0;
static unsigned int ui_icola_rec = 0;

// ----------------------------------- FUNCIONES -----------------------

/* Nombre: InicializarUART
* Descripci�n: Funci�n de inicializaci�n del m�dulo UART.
* Argumentos: Ninguno
* Valor devuelto: Ninguno */

void InicializarUART(void) {
    
    TRISB |= 0x0020; // Configura RX como entrada
    TRISB &= 0xFFEF; // Configura TX como salida
    U1BRG = (FCY/BAUDRATE)/16 - 1;
    U1MODEbits.STSEL = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXISEL1 = 1;
    U1STAbits.URXISEL = 0;
    U1STAbits.UTXEN = 1;
    IPC3bits.U1TXIP = PR_INT_TX_UART;
    IPC2bits.U1RXIP = PR_INT_RX_UART;
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1TXIE = 1;
    IEC0bits.U1RXIE = 1;
    
}

// ---------------------------------------------------------------------

/* Nombre: PonerEnColaTransmisionUART
* Descripci�n: Funci�n que coloca un dato en la cola de transmisi�n y
* gestiona el rebose del �ndice.
* Argumentos:
* unsigned char uc_caracter: Caracter que se quiere poner en cola
* Valor devuelto: Ninguno */

void PonerEnColaTransmisionUART(unsigned char uc_caracter) {
    
    uc_cola_transmision[ui_icabeza_tr] = uc_caracter;
    ui_icabeza_tr++;
    if (ui_icabeza_tr == TAM_TR_UART)
        ui_icabeza_tr = 0;
    
}

// ---------------------------------------------------------------------

/* Nombre: SacarDeColaTransmision
* Descripci�n: Funci�n que obtiene un dato de la cola de transmisi�n y
* gestiona el rebose del �ndice.
* Argumentos: Ninguno
* Valor devuelto:
* unsigned char: Caracter recuperado de la cola */

unsigned char SacarDeColaTransmisionUART(void) {
    
    unsigned char uc_caracter;
    uc_caracter = uc_cola_transmision[ui_icola_tr];
    ui_icola_tr++;
    if (ui_icola_tr == TAM_TR_UART)
        ui_icola_tr = 0;
    return uc_caracter;
    
}

// ---------------------------------------------------------------------

/* Nombre: PonerEnColaRecepcionUART
* Descripci�n: Funci�n que coloca un dato en la cola de recepci�n y
* gestiona el rebose del �ndice.
* Argumentos:
* unsigned char uc_caracter: Caracter que se quiere poner en cola
* Valor devuelto: Ninguno */

void PonerEnColaRecepcionUART(unsigned char uc_caracter) {
    
    uc_cola_recepcion[ui_icabeza_rec] = uc_caracter;
    ui_icabeza_rec++;
    if (ui_icabeza_rec == TAM_REC_UART)
        ui_icabeza_rec = 0;
    
}

// ---------------------------------------------------------------------

/* Nombre: SacarDeColaRecepcionUART
* Descripci�n: Funci�n que obtiene un dato de la cola de recepci�n y
* gestiona el rebose del �ndice.
* Argumentos: Ninguno
* Valor devuelto:
* unsigned char: Caracter recuperado de la cola */

unsigned char SacarDeColaRecepcionUART() {
    
    unsigned char uc_caracter;
    uc_caracter = uc_cola_recepcion[ui_icola_rec];
    ui_icola_rec++;
    if (ui_icola_rec == TAM_REC_UART)
        ui_icola_rec = 0;
    return uc_caracter;

}

// ---------------------------------------------------------------------

/* Nombre: Transmite
* Descripci�n: Comienza a enviar la cola de transmisi�n
* Argumentos: Ninguno
* Valor devuelto: Ninguno*/

void Transmite(void) {
    
    IFS0bits.U1TXIF = 1; // Borrar la bandera de la interrupci�n
    
}

// ---------------------------------------------------------------------

/* Nombre: _U1TXInterrupt
* Descripci�n: Rutina de atenci�n a interrupci�n de transmisi�n.
* Valor devuelto: Ninguno */

void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void) {
    
    IFS0bits.U1TXIF = 0; // Borrar la bandera de la interrupci�n
    if (ui_icola_tr != ui_icabeza_tr)
        U1TXREG = SacarDeColaTransmisionUART();

}

// ---------------------------------------------------------------------

/* Nombre: _U1RXInterrupt
* Descripci�n: Rutina de atenci�n a interrupci�n de recepci�n.
* Valor devuelto: Ninguno */

void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void) {
    
    IFS0bits.U1RXIF = 0; // Borrar la bandera de la interrupci�n
    PonerEnColaRecepcionUART(U1RXREG);

}

// ---------------------------------------------------------------------

/* Nombre: HayAlgoRecibido
* Descripci�n: Indica si hay algo disponible en la cola de recepci�n
* Valor devuelto:
* int: distinto de 0 si hay algo, 0 si no hay nada */

int HayAlgoRecibido(void) {
    
    return (ui_icola_rec != ui_icabeza_rec);

}