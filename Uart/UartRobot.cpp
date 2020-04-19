#include "serial.h";
#include <string>;
#include <iostream>;

int main(void) {

	CSerial port;
	unsigned char buffer[10];
	unsigned long resultado;

	while (1) {

		port.PortRead(10, buffer, &resultado);

		if(resultado == 1) {

			std::cout << "Estado actual: " << (int) buffer[0] << std::endl;

		} 

	}

	return 0;

}
