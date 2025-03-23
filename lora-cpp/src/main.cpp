#include <Arduino.h>
#include <LiquidCrystal.h>

#include <DHT.h>  // DHT sensor library
#include <DHT_U.h>  // DHT sensor library

// todo: add a button to start/stop the timer	
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);	// pines RS, E, D4, D5, D6, D7 de modulo 1602A


int dhtPin = 8; // pin DATA de DHT22 a pin digital 2
int dhtType = DHT11;
DHT dht(dhtPin, dhtType);

int temperature = 0;
int humidity = 0;


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

// String printElapsedTime(int elapsed_seconds) {
String printElapsedTime() {
	int elapsed_seconds = millis() / 1000;
	char time_fmt[9]; // Buffer to hold the formatted time string (8 chars + null terminator)
	sprintf(time_fmt, "%02d:%02d:%02d",  // * fmt time str -> HH:MM:SS
		(elapsed_seconds / 3600),
		(elapsed_seconds % 3600) / 60,
		(elapsed_seconds % 60)
	);
	return String(time_fmt); // Convert to String and return
}

// * Main code --------------------------------------------------------

// * Setup fn (runs once at the beginning)
void setup() {
	Serial.begin(9600);	// Set the baud rate to your computer's serial port
	lcd.begin(16, 2);	// inicializa a display de 16 columnas y 2 lineas
	dht.begin();	// inicializa el sensor DHT
	pinMode(LED_BUILTIN, OUTPUT);
}

// * Main loop
void loop() {
	// some log data...
	String time_fmt = printElapsedTime();
    setLCD(0, 0, time_fmt);
    Serial.println("Time: " + time_fmt);

	// set the temperature and humidity on the LCD
	temperature = dht.readTemperature();
	humidity = dht.readHumidity();
	setLCD(0, 1, "Temp: " + String(temperature) + "C");
	setLCD(8, 1, "Hum: " + String(humidity) + "%");

	delay(1000);
}
