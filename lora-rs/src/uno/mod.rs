use arduino_hal::{delay_ms, port::PinOps, port::Pin, port::mode::Output, port::mode::Input};

/// Toggle a pin's state
pub fn toggle_pin<P: PinOps>(pin: &mut Pin<Output, P>) {
    if pin.is_set_high() {
        pin.set_low();
    } else {
        pin.set_high();
    }
}

/// Delay for specified milliseconds
pub fn delay(ms: u16) {
    delay_ms(ms);
}

/// Get pin state as a boolean
pub fn pin_state<P: PinOps>(pin: &Pin<Input, P>) -> bool {
    pin.is_high()
}

/// Perform software reset via watchdog
pub fn reset() {
    unsafe {
        let wdt = &*arduino_hal::pac::WDT::ptr();
        wdt.wdtcsr.write(|w| w.wdce().set_bit().wde().set_bit());
        wdt.wdtcsr.write(|w| w.wde().set_bit());
        loop {}
    }
}

/// Wait for a pin to reach a specific state
pub fn wait_for_pin<P: PinOps>(pin: &Pin<Input, P>, state: bool, timeout_ms: u16) -> bool {
    let mut elapsed: u16 = 0;
    let increment: u16 = 10;
    
    while pin.is_high() != state {
        delay(increment);
        elapsed += increment;
        
        if elapsed >= timeout_ms {
            return false;
        }
    }
    
    true
}

/// Simple non-blocking timer
pub struct Timer {
    last_time: u16,
    interval: u16,
}

impl Timer {
    pub fn new(interval_ms: u16) -> Self {
        Timer {
            last_time: 0,
            interval: interval_ms,
        }
    }
    
    pub fn check(&mut self, current_ms: u16) -> bool {
        if current_ms.wrapping_sub(self.last_time) >= self.interval {
            self.last_time = current_ms;
            true
        } else {
            false
        }
    }
    
    pub fn reset(&mut self, current_ms: u16) {
        self.last_time = current_ms;
    }
}

/// Software PWM implementation
pub struct SoftPWM<P: PinOps> {
    pin: Pin<Output, P>,
    duty: u8,
    cycle_timer: Timer,
    current_value: bool,
    cycle_position: u8,
}

impl<P: PinOps> SoftPWM<P> {
    /// Create a new software PWM with default settings
    /// freq_hz: PWM frequency in Hz (typically 50-1000)
    pub fn new(pin: Pin<Output, P>, freq_hz: u16) -> Self {
        // Calculate cycle time in ms (1000 / frequency)
        let cycle_time = 1000 / freq_hz;
        
        // Create with pin low
        let mut pwm_pin = pin;
        pwm_pin.set_low();
        
        SoftPWM {
            pin: pwm_pin,
            duty: 0,
            cycle_timer: Timer::new(cycle_time / 255),
            current_value: false,
            cycle_position: 0,
        }
    }
    
    /// Set duty cycle (0-255)
    pub fn set_duty(&mut self, duty: u8) {
        self.duty = duty;
    }
    
    /// Update PWM output (call this frequently in main loop)
    pub fn update(&mut self, current_ms: u16) {
        if self.cycle_timer.check(current_ms) {
            // Increment position in cycle
            self.cycle_position = self.cycle_position.wrapping_add(1);
            
            // Calculate if output should be high or low
            let new_state = self.cycle_position < self.duty;
            
            // Update pin if state changed
            if new_state != self.current_value {
                self.current_value = new_state;
                if new_state {
                    self.pin.set_high();
                } else {
                    self.pin.set_low();
                }
            }
        }
    }
}
