#include "config.h"
#include "ad.h"
#include "uart.h"
#include "servos.h"

// ----------------------------------------------------------------------------------
// ----------------------------- BITS DE CONFIGURACI?N ------------------------------
// ----------------------------------------------------------------------------------

// 1. Eliminar el segmento de arranque flash
// 2. Permitir la escritura del segmento de arranque flash
_FBS(BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF);

// 1. No proteger la memoria de programa contra escritura
// 2. No proteger el cÛdigo
_FGS(GWRP_OFF & GCP_OFF);

// 1. Utilizar el oscilador interno (FRC) en el arranque
// 2. Arrancar directamente con el oscilador seleccionado
_FOSCSEL(FNOSC_FRC & IESO_OFF);

// 1. Permitir la conmutaciÛn del reloj y deshabilitar la monitorizaciÛn de fallos
// 2. Desactivar el oscilador primario
// 3. Utilizar el oscilador secundario como entrada y salida digital
// 4. Permitir m?ltiples remapeos de los pines
_FOSC(FCKSM_CSECMD & POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF);

// 1. Permitir la deshabilitaciÛn del watchdog timer
//    poniendo a 0 el bit SWDTEN del registro RCON
_FWDT(FWDTEN_OFF);

// 1. Esperar 128 ms y resetear el microcontrolador al enchufar la alimentaciÛn
// 2. Controlar los pines de PWM desde el registro PORT al arrancar
// 3. Los pines PWM high est·n activos a nivel alto
// 4. Los pines PWM low est·n activos a nivel alto
// 5. Utilizar los pines est·ndar (SDA1 y SCL1) para el I2C
_FPOR(FPWRT_PWR128 & PWMPIN_ON & HPOL_ON & LPOL_ON & ALTI2C_OFF);

// 1. Programar y depurar a travÈs de los pines PGC1 y PGD1
// 2. Desactivar el interfaz para JTAG
_FICD(ICS_PGD1 & JTAGEN_OFF);

int ancho_pulso[4] = {6,6,6,6}; 
int contador_servos = 0, contador_pinza = 0;
int estado_actual = 1;
int PORT_AUX = 0x0F3F;

int main(void) { 
    
    InicializarReloj();
    RemapeaPerifericos();
    
    TRISB &= 0x0F3F;
    PORTB = 0x0F3F;
        
    InicializarUART();
    
    init_servos();
    init_ad(0x3F);

    int detector_objeto = 0;
    
    PonerEnColaTransmisionUART((unsigned char) estado_actual);
    Transmite();

    while(1) {
        
        if(estado_actual == 1) {
            
            detector_objeto = get_ad(4); // Selecciona el canal
            
            if((detector_objeto >> 6) <= 7) {
            
                estado_actual = 2;
                PORT_AUX |= 0x0040; // LED 1 encendido
                contador_pinza = 0;
                
                PonerEnColaTransmisionUART((unsigned char) estado_actual);
                Transmite();
            
            } 
            
        }
        
        if(estado_actual == 2 && contador_pinza >= 24000) { 
            
            estado_actual = 3;
            PORT_AUX &= 0xFFBF; // LED 1 apagado
            contador_pinza = 0;
            
            PonerEnColaTransmisionUART((unsigned char) estado_actual);
            Transmite();
            
        }
        
        if(estado_actual == 3 && contador_pinza >= 32000) { 
            
            estado_actual = 4;
            PORT_AUX |= 0x0080; // LED 2 encendido
            contador_pinza = 0;
            
            PonerEnColaTransmisionUART((unsigned char) estado_actual);
            Transmite();
            
        }
        
        if(estado_actual == 4 && contador_pinza >= 16000) { 
            
            estado_actual = 1;
            PORT_AUX &= 0xFF7F; // LED 2 apagado
            contador_pinza = 0;
            
            PonerEnColaTransmisionUART((unsigned char) estado_actual);
            Transmite();
            
        }
        
        ancho_pulso[0] = get_servo_1(estado_actual, contador_pinza);
        ancho_pulso[1] = get_servo_2(estado_actual, contador_pinza);
        ancho_pulso[2] = get_servo_3(estado_actual, contador_pinza);
        ancho_pulso[3] = get_servo_4(estado_actual, contador_pinza);
        
    }
   
    return 0;
   
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    
    IFS0 &= 0xFFF7;
    
    contador_servos++;

    if (contador_servos >= 160) {

       contador_servos = 0;

    }
            
    contador_pinza++;
    
    int i;
   
    for(i = 0; i < 4; i++) {
       
        if(contador_servos < ancho_pulso[i]) {
                
                PORT_AUX |= (0x1000 << i); 

            } else {

                PORT_AUX &= ~(0x1000 << i); 

            }
       
    }
    
    PORTB = PORT_AUX;

}