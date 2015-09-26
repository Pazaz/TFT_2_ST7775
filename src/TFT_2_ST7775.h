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
		volatile uint8_t *wrPort;
		volatile uint8_t *rsPort;

		uint8_t csPinMask;
		uint8_t wrPinMask;
		uint8_t rsPinMask;

		volatile uint8_t data[8];
		volatile uint8_t dataPinMask[8];
		volatile uint8_t *dataPort[8];

	public:
		Display(uint8_t cs, uint8_t wr, uint8_t rs);
		void writeSpi(const uint8_t data);
		void drawPixel(int16_t, int16_t, uint16_t);
		void setPins(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);
		void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

		void begin();
	};

	namespace Utility {
		uint16_t RGB_565(uint8_t red, uint8_t green, uint8_t blue);
	}
}

#endif
