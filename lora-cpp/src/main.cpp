#include <Arduino.h>
#include <LiquidCrystal.h>

#include <DHT.h>
#include <DHT_U.h>

#include <SoftwareSerial.h>
#include <TinyGPS++.h>


int RXpin = 8;
int TXpin = 9;
SoftwareSerial gpsSerial(RXpin, TXpin); // RX, TX
TinyGPSPlus gps;
float lattitude,longitude;

// * LCD setup
int RSpin = 7;  // Register Select pin
int Epin = 6;  // Enable pin
LiquidCrystal lcd(RSpin, Epin, 5, 4, 3, 2);	// pines RS, E, D4, D5, D6, D7 de modulo 1602A

int dhtPin = 12; // pin DATA de DHT22 a pin digital 2
DHT dht(dhtPin, DHT11);
int temperature, humidity;


// * Some fn's declarations...
void blinkPin(int pin, int onTime, int offTime) {
	for (int i = 0; i < 2; i++) {
		digitalWrite(pin, i % 2 == 0 ? HIGH : LOW);
		delay(i % 2 == 0 ? onTime : offTime);
	}
}

void setLCD(int col, int row, String msg) {
	lcd.setCursor(col, row);
	lcd.print(msg);
	if (msg.length() > 16) {
		lcd.scrollDisplayLeft();
	}
}

void setDHT(int col, int row) {
	temperature = dht.readTemperature();
	humidity = dht.readHumidity();
	setLCD(col, row, String(temperature) + "C");
	setLCD(col + 5, row, String(humidity) + "%");
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
	return String(time_fmt);
}

// * Main code --------------------------------------------------------

// * Setup fn (runs once at the beginning)
void setup() {
	Serial.begin(9600);	// Set the baud rate to your computer's serial port
	lcd.begin(16, 2);	// inicializa a display de 16 columnas y 2 lineas
	dht.begin();	// inicializa el sensor DHT
	pinMode(LED_BUILTIN, OUTPUT);

	// Start the software serial port at the GPS's default baud
	gpsSerial.begin(9600); 	// * Baud rate of the GPS

}

// * Main loop
void loop() {
	String time_fmt = printElapsedTime();
    setLCD(0, 0, time_fmt);
	setDHT(0, 1);
    // Serial.println("Time: " + time_fmt);

	// while (gpsSerial.available()) {
		int data = gpsSerial.read();
		Serial.print(data);
		// if (gps.encode(data)) {
			lattitude = (gps.location.lat());
			longitude = (gps.location.lng());
			Serial.println("Location: " + String(lattitude) + ", " + String(longitude));
		// }
	// }
	// delay(1000);
}
