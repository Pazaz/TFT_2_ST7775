#include "TFT_2_ST7775.h"

namespace ST7775 {
	Display::Display(uint8_t cs, uint8_t wr, uint8_t rs) : Adafruit_GFX(176, 220) {
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
		uint8_t bit, i;
		for (bit = 0x01, i = 0; bit; ++i, bit <<= 1) {
			if (data & bit) {
				*(dataPort[i]) |= dataPinMask[i];
			} else {
				*(dataPort[i]) &= (~dataPinMask[i]);
			}
		}
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
		for (uint8_t i = 0; i < 8; ++i) {
			pinMode(data[i], OUTPUT);
			dataPort[i] = portOutputRegister(digitalPinToPort(data[i]));
			dataPinMask[i] = digitalPinToBitMask(data[i]);
		}
	}

	void Display::writeWrReg(uint16_t data) {
		*(rsPort) &= (~rsPinMask);
		*(csPort) &= (~csPinMask);
		writeSpi(data >> 8);
		*(wrPort) &= (~wrPinMask);
		*(wrPort) |= wrPinMask;
		writeSpi((data & 0x00ff));
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
		writeSpi((data & 0x00ff));
		*(wrPort) &= (~wrPinMask);
		*(wrPort) |= wrPinMask;
		*(csPort) |= csPinMask;
	}

	void Display::writeWr(uint16_t cmd, uint16_t data) {
		writeWrReg(cmd);
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

	void Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
		setWindow(x, y, x + w - 1, y + h - 1);
		writeWrReg(0x22);
		int i = w*h;
		while (i--) {
			writeWrData(color);
		}
	}

	void Display::begin() {
		writeWr(0x0001, 0x011C);
		writeWr(0x0002, 0x0100);
		writeWr(0x0003, 0x1030);
		writeWr(0x0008, 0x0808);
		writeWr(0x000C, 0x0000);

		writeWr(0x000F, 0x0e01);
		writeWr(0x0010, 0x0A00);
		writeWr(0x0011, 0x1038);
		writeWr(0x00ff, 0x0003);
		writeWr(0x00b0, 0x1411);
		writeWr(0x00b1, 0x0202);
		writeWr(0x00b2, 0x0313);

		writeWr(0x0030, 0x0000);
		writeWr(0x0031, 0x00db);
		writeWr(0x0032, 0x0000);
		writeWr(0x0033, 0x0000);
		writeWr(0x0034, 0x00db);
		writeWr(0x0035, 0x0000);
		writeWr(0x0036, 0x00AF);
		writeWr(0x0037, 0x0000);
		writeWr(0x0038, 0x00DB);
		writeWr(0x0039, 0x0000);

		writeWr(0x00ff, 0x0003);
		writeWr(0x0050, 0x0000);
		writeWr(0x0051, 0x0300);
		writeWr(0x0052, 0x0103);
		writeWr(0x0053, 0x2011);
		writeWr(0x0054, 0x0703);
		writeWr(0x0055, 0x0000);
		writeWr(0x0056, 0x0400);
		writeWr(0x0057, 0x0107);
		writeWr(0x0058, 0x2011);
		writeWr(0x0059, 0x0703);
		writeWr(0x0020, 0x0000);
		writeWr(0x0021, 0x0000);
		writeWr(0x0007, 0x1017);
	}

	namespace Utility {
		uint16_t RGB_565(uint8_t red, uint8_t green, uint8_t blue) {
			return (red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3);
		}
	}
}