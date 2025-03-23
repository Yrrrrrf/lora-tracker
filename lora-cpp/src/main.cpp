#include <Arduino.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(7, 6, 5, 4, 3, 2);	// pines RS, E, D4, D5, D6, D7 de modulo 1602A

// * Some fn's declarations...
void blinkPin(int pin, int onTime, int offTime) {
	for (int i = 0; i < 2; i++) {
		digitalWrite(pin, i % 2 == 0 ? HIGH : LOW);
		delay(i % 2 == 0 ? onTime : offTime);
	}
}

void setLCD(int col, int row, String msg) {
// conut message, it its more than 16 chars, then use the scroll fn
	lcd.setCursor(col, row);
	lcd.print(msg);
	if (msg.length() > 16) {
		lcd.scrollDisplayLeft();
	}
}

// * Main code --------------------------------------------------------

// * Setup fn (runs once at the beginning)
void setup() {
	Serial.begin(9600);  // Set the baud rate to your computer's serial port
	pinMode(LED_BUILTIN, OUTPUT);
	lcd.begin(16, 2);			// inicializa a display de 16 columnas y 2 lineas
}

// * Main loop
void loop() {
	// blinkPin(LED_BUILTIN, 100, 100);
	// setLCD(0, 0, String(millis() / 1000) + " s");
	Serial.println("Hello World!");
	delay(1000);
}

