#ifndef SERIAL_H

#define SERIAL_H

#include <Windows.h>

class CSerial {

	private:

		HANDLE m_h_Port; // Serial port handle

	public:

		CSerial();
		~CSerial();
		bool PortRead(unsigned long numero_char_lectura, unsigned char *lectura, unsigned long *nEle);

};

#endif