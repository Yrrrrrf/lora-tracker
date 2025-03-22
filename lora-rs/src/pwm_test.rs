#![no_std]
#![no_main]

use panic_halt as _;

mod uno;
use uno::{SoftPWM, delay};

// Simple function to get "current time"
// In a real application, you'd have a proper time source
fn get_current_ms() -> u16 {
    static mut COUNTER: u16 = 0;
    unsafe {
        COUNTER = COUNTER.wrapping_add(1);
        COUNTER
    }
}

#[arduino_hal::entry]
fn main() -> ! {
    let dp = arduino_hal::Peripherals::take().unwrap();
    let pins = arduino_hal::pins!(dp);

    // Create a software PWM on pin D9
    let pwm_pin = pins.d9.into_output();
    let mut led_pwm = SoftPWM::new(pwm_pin, 100); // 100Hz PWM frequency
    
    // Optional: create another PWM on pin D10
    let pwm_pin2 = pins.d10.into_output();
    let mut led_pwm2 = SoftPWM::new(pwm_pin2, 100);
    
    // Set initial duty cycles
    led_pwm.set_duty(128);  // 50% brightness
    led_pwm2.set_duty(64);  // 25% brightness
    
    let mut count = 0;
    let mut fade_value = 0;
    let mut fade_up = true;
    
    loop {
        // Get current time
        let current_time = get_current_ms();
        
        // Update both PWM outputs
        led_pwm.update(current_time);
        led_pwm2.update(current_time);
        
        // Increment counter
        count += 1;
        
        // Every 1000 cycles, adjust the PWM duty cycles
        if count >= 1000 {
            count = 0;
            
            // Update fade value (0-255)
            if fade_up {
                fade_value = fade_value.saturating_add(5);
                if fade_value >= 250 {
                    fade_up = false;
                }
            } else {
                fade_value = fade_value.saturating_sub(5);
                if fade_value <= 5 {
                    fade_up = true;
                }
            }
            
            // Update PWM duty cycles
            led_pwm.set_duty(fade_value);
            led_pwm2.set_duty(255 - fade_value); // Inverse fade
        }
        
        // Small delay to keep loop from running too fast
        delay(1);
    }
}