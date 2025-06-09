/**
 * Software PWM Implementation for Arduino
 * Equivalent to the Rust version
 */
#include <Arduino.h>

/**
 * Simple non-blocking timer class
 */
class Timer {
	private:
	unsigned long lastTime;
	unsigned int interval;

	public:
	Timer() {
		interval = 0;
		lastTime = 0;
	}

	void begin(unsigned int intervalMs) {
		interval = intervalMs;
		lastTime = millis();
	}

	bool check(unsigned long currentMs) {
		if (currentMs - lastTime >= interval) {
		lastTime = currentMs;
		return true;
		}
		return false;
	}

	void reset(unsigned long currentMs) {
		lastTime = currentMs;
	}
};

/**
 * Software PWM implementation
 * Allows PWM on any digital pin
 */
class SoftPWM {
	private:
		uint8_t pin;
		uint8_t duty;
		Timer cycleTimer;
		bool currentValue;
		uint8_t cyclePosition;

	public:
	SoftPWM() {
		pin = 0;
		duty = 0;
		currentValue = false;
		cyclePosition = 0;
	}

	/**
	* Initialize the PWM output
	* @param pinNumber Arduino pin number
	* @param freqHz PWM frequency in Hz (typically 50-1000)
	*/
	void begin(uint8_t pinNumber, uint16_t freqHz) {
		pin = pinNumber;
		
		// Calculate cycle interval
		unsigned int cycleInterval = 1000 / freqHz / 255;
		cycleInterval = max(1, cycleInterval); // Ensure at least 1ms
		
		cycleTimer.begin(cycleInterval);
		
		// Initialize pin
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}

	/**
	* Set PWM duty cycle
	* @param newDuty Duty cycle (0-255)
	*/
	void setDuty(uint8_t newDuty) {
		duty = newDuty;
	}

/**
	* Update PWM output - call frequently from main loop
	* @param currentMs Current time in milliseconds
	*/
void update(unsigned long currentMs) {
	if (cycleTimer.check(currentMs)) {
	// Increment position in cycle
	cyclePosition++;
	
	// Calculate if output should be high or low
	bool newState = cyclePosition < duty;
	
	// Update pin if state changed
	if (newState != currentValue) {
		currentValue = newState;
		digitalWrite(pin, newState ? HIGH : LOW);
	}
	}
}
};

// Global variables
SoftPWM ledPwm;
SoftPWM ledPwm2;
unsigned int count = 0;
uint8_t fadeValue = 0;
bool fadeUp = true;

void setup() {
// Initialize PWM objects
ledPwm.begin(9, 100);  // Pin 9, 100Hz
ledPwm2.begin(10, 100); // Pin 10, 100Hz

// Set initial duty cycles
ledPwm.setDuty(128);  // 50% brightness
ledPwm2.setDuty(64);  // 25% brightness
}

void loop() {
// Get current time
unsigned long currentTime = millis();

// Update both PWM outputs
ledPwm.update(currentTime);
ledPwm2.update(currentTime);

// Increment counter
count++;

// Every 1000 cycles, adjust the PWM duty cycles
if (count >= 1000) {
	count = 0;
	
	// Update fade value (0-255)
	if (fadeUp) {
	fadeValue += 5;
	if (fadeValue >= 250) {
		fadeUp = false;
	}
	} else {
	fadeValue -= 5;
	if (fadeValue <= 5) {
		fadeUp = true;
	}
	}
	
	// Update PWM duty cycles
	ledPwm.setDuty(fadeValue);
	ledPwm2.setDuty(255 - fadeValue); // Inverse fade
}

// Small delay to keep loop from running too fast
delay(1);
}