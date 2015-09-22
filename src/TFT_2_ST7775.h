#ifndef TFT_2_ST7775
#define TFT_2_ST7775

#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif
#include <Adafruit_GFX.h>

#include <avr/pgmspace.h>

namespace ST7775 {
	const unsigned char WIDTH = 176, HEIGHT = 220;

	class Display : public Adafruit_GFX {
		void setWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
		void writeWrReg(uint8_t command);
		void writeWrData(uint16_t data);
		void writeWr(uint8_t command, uint16_t data);

		uint8_t cs;
		uint8_t wr;
		uint8_t rs;

		volatile uint8_t *csPort;
		volatile uint8_t *pmwPort;
		volatile uint8_t *wrPort;
		volatile uint8_t *rsPort;
		volatile uint8_t *restPort;

		uint8_t csPinMask;
		uint8_t pmwPinMask;
		uint8_t wrPinMask;
		uint8_t rsPinMask;
		uint8_t restPinMask;

		volatile uint8_t data[8];
		volatile uint8_t dataPinMask[8];
		volatile uint8_t* dataPort[8];

	public:
		Display(uint8_t, uint8_t, uint8_t);
		void writeSpi(const uint8_t);
		void drawPixel(int16_t, int16_t, uint16_t);
		void setPins(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
		void fillRect(int16_t, int16_t, int16_t, int16_t, uint16_t);

		void begin();
	};

	namespace Utility {
		uint16_t RGB_565(uint8_t, uint8_t, uint8_t);
	}
}

#endif
