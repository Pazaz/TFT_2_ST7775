#include "TFT_2_ST7775.h"

namespace ST7775 {
	Display::Display(uint8_t cs, uint8_t wr, uint8_t rs) : Adafruit_GFX(WIDTH, HEIGHT) {
		cs = cs;
		wr = wr;
		rs = rs;

		pinMode(cs, OUTPUT);
		pinMode(wr, OUTPUT);
		pinMode(rs, OUTPUT);

		csPort = portOutputRegister(digitalPinToPort(cs));
		wrPort = portOutputRegister(digitalPinToPort(wr));
		rsPort = portOutputRegister(digitalPinToPort(rs));

		csPinMask = digitalPinToBitMask(cs);
		wrPinMask = digitalPinToBitMask(wr);
		rsPinMask = digitalPinToBitMask(rs);

		*(csPort) |= csPinMask;
		*(wrPort) |= wrPinMask;
		*(rsPort) |= rsPinMask;
	}

	void Display::writeSpi(const uint8_t data) {
		uint8_t bit = 1, i = 0;
		do {
			if (data & bit) {
				*(dataPort[i]) |= dataPinMask[i];
			} else {
				*(dataPort[i]) &= (~dataPinMask[i]);
			}
			++i;
			bit <<= 1;
		} while (bit);
	}

	void Display::setPins(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7) {
		data[0] = b0;
		data[1] = b1;
		data[2] = b2;
		data[3] = b3;
		data[4] = b4;
		data[5] = b5;
		data[6] = b6;
		data[7] = b7;

		uint8_t i = 0;
		do {
			pinMode(data[i], OUTPUT);
			dataPort[i] = portOutputRegister(digitalPinToPort(data[i]));
			dataPinMask[i] = digitalPinToBitMask(data[i]);
			++i;
		} while (i < 8);
	}

	void Display::writeWrReg(uint8_t command) {
		*(rsPort) &= (~rsPinMask);
		*(csPort) &= (~csPinMask);
		writeSpi(command >> 8);
		*(wrPort) &= (~wrPinMask);
		*(wrPort) |= wrPinMask;
		writeSpi(command & 0xFF);
		*(wrPort) &= (~wrPinMask);
		*(wrPort) |= wrPinMask;
		*(csPort) |= csPinMask;
	}

	void Display::writeWrData(uint16_t data) {
		*(rsPort) |= rsPinMask;
		*(csPort) &= (~csPinMask);
		writeSpi(data >> 8);
		*(wrPort) &= (~wrPinMask);
		*(wrPort) |= wrPinMask;
		writeSpi(data & 0xFF);
		*(wrPort) &= (~wrPinMask);
		*(wrPort) |= wrPinMask;
		*(csPort) |= csPinMask;
	}

	void Display::writeWr(uint8_t command, uint16_t data) {
		writeWrReg(command);
		writeWrData(data);
	}

	void Display::setWindow(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
		writeWr(0x37, x1);
		writeWr(0x36, x2);
		writeWr(0x39, y1);
		writeWr(0x38, y2);
		writeWr(0x20, x1);
		writeWr(0x21, y1);
	}

	void Display::drawPixel(int16_t x, int16_t y, uint16_t color) {
		setWindow(x, y, x, y);
		writeWr(0x22, color);
	}

	void Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
		setWindow(x, y, x + w - 1, y + h - 1);
		writeWrReg(0x22);
		int i = w * h;
		while (i--) {
			writeWrData(color);
		}
	}

	void Display::begin() {
		writeWr(0x01, 0x011C);
		writeWr(0x02, 0x0100);
		writeWr(0x03, 0x1030);
		writeWr(0x08, 0x0808);
		writeWr(0x0C, 0x0000);
		writeWr(0x0F, 0x0E01);

		writeWr(0x10, 0x0A00);
		writeWr(0x11, 0x1038);
		writeWr(0xFF, 0x0003);
		writeWr(0xB0, 0x1411);
		writeWr(0xB1, 0x0202);
		writeWr(0xB2, 0x0313);

		writeWr(0x30, 0x0000);
		writeWr(0x31, 0x00db);
		writeWr(0x32, 0x0000);
		writeWr(0x33, 0x0000);
		writeWr(0x34, 0x00db);
		writeWr(0x35, 0x0000);
		writeWr(0x36, 0x00AF);
		writeWr(0x37, 0x0000);
		writeWr(0x38, 0x00DB);
		writeWr(0x39, 0x0000);

		writeWr(0xFF, 0x0003);
		writeWr(0x50, 0x0000);
		writeWr(0x51, 0x0300);
		writeWr(0x52, 0x0103);
		writeWr(0x53, 0x2011);
		writeWr(0x54, 0x0703);
		writeWr(0x55, 0x0000);
		writeWr(0x56, 0x0400);
		writeWr(0x57, 0x0107);
		writeWr(0x58, 0x2011);
		writeWr(0x59, 0x0703);
		writeWr(0x20, 0x0000);
		writeWr(0x21, 0x0000);
		writeWr(0x07, 0x1017);
	}

	namespace Utility {
		uint16_t RGB_565(uint8_t red, uint8_t green, uint8_t blue) {
			return (red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3);
		}
	}
}