#![no_std]
#![no_main]

use panic_halt as _;

// Function to blink a pin with specified on and off times
fn blink_pin<P: arduino_hal::port::PinOps>(
    pin: &mut arduino_hal::port::Pin<arduino_hal::port::mode::Output, P>,
    on_time: u16,
    off_time: u16,
) {
    for i in 0..2 {
        if i % 2 == 0 {
            pin.set_high();
            arduino_hal::delay_ms(on_time);
        } else {
            pin.set_low();
            arduino_hal::delay_ms(off_time);
        }
    }
}

#[arduino_hal::entry]
fn main() -> ! {
    let dp = arduino_hal::Peripherals::take().unwrap();
    let pins = arduino_hal::pins!(dp);

    // Set up the built-in LED (pin D13 on Arduino Uno R3)
    let mut led = pins.d13.into_output();
    
    loop {
        // Blink the LED (500ms on, 100ms off)
        // blink_pin(&mut led, 500, 100);
        blink_pin(&mut led, 50, 50);
    }
}
