#![no_std]
#![no_main]

use panic_halt as _;

mod uno;

#[arduino_hal::entry]
fn main() -> ! {
    let dp = arduino_hal::Peripherals::take().unwrap();
    let pins = arduino_hal::pins!(dp);

    // Set up LED
    let mut led = pins.d13.into_output();
    
    // Initialize serial
    let mut serial = arduino_hal::default_serial!(dp, pins, 57600);
    
    // Main loop
    loop {
        // Toggle LED to show system is alive
        uno::toggle_pin(&mut led);
        uno::delay(50);
    }
}
