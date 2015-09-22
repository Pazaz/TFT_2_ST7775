#include <Adafruit_GFX.h>
#include <TFT_2_ST7775.h>

ST7775::Display display(11, 10, 12);

void setup() {
	Serial.begin(9600);
	randomSeed(analogRead(0));
	display.setPins(9, 8, 7, 6, 5, 4, 3, 2);
	display.begin();
}

void loop() {
	static int currentTime = millis(), lastTime = 0;
	display.fillRect(0, 0, ST7775::WIDTH, ST7775::HEIGHT, ST7775::Utility::RGB_565(random(), random(), random()));
	lastTime = currentTime;
	currentTime = millis();
	Serial.print(F("Took ")); Serial.print(currentTime - lastTime); Serial.println(F("ms to render 176x220 pixels"));
}
