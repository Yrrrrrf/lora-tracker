import { SerialPort } from 'serialport'; // Main export should be the stream class
import { ReadlineParser } from '@serialport/parser-readline';
import type { PortInfo } from 'serialport';


// --- Configuration ---
const ARDUINO_PORT_PATH: string | null = 'COM5'; // <<<<====== CHANGE THIS if needed, or set to null
const BAUD_RATE = 9600;

const ARDUINO_MANUFACTURERS = [
  'arduino',
  'wch.cn',
  '1a86',
  'Adafruit',
  'SparkFun',
  'Prolific Technology Inc.'
];

async function findArduinoPort(): Promise<string | null> {
  try {
    const ports: PortInfo[] = await SerialPort.list(); // Static method from SerialPort
    console.log('Available serial ports:');
    ports.forEach(port => {
      console.log(`- Path: ${port.path}, Manufacturer: ${port.manufacturer ?? 'N/A'}, pnpId: ${port.pnpId ?? 'N/A'}`);
    });

    for (const port of ports) {
      if (port.manufacturer) {
        for (const manu of ARDUINO_MANUFACTURERS) {
          if (port.manufacturer.toLowerCase().includes(manu.toLowerCase())) {
            console.log(`Found potential Arduino at: ${port.path} (Manufacturer: ${port.manufacturer})`);
            return port.path;
          }
        }
      }
      if (port.pnpId && (port.pnpId.includes('VID_2341') || port.pnpId.includes('PID_0043') || port.pnpId.includes('VID_1A86'))) {
         console.log(`Found potential Arduino at: ${port.path} (pnpId: ${port.pnpId})`);
         return port.path;
      }
    }
    console.log('No Arduino port automatically detected.');
    return null;
  } catch (err) {
    console.error('Error listing serial ports:', err);
    return null;
  }
}

async function main() {
  let portPath: string | null = ARDUINO_PORT_PATH;

  if (!portPath) {
    console.log('ARDUINO_PORT_PATH not set, attempting to auto-detect...');
    portPath = await findArduinoPort();
  }

  if (!portPath) {
    console.error(
      'Failed to determine Arduino port. Please specify ARDUINO_PORT_PATH manually in the script.'
    );
    process.exitCode = 1; // Set exit code for Bun
    return;
  }

  console.log(`Attempting to connect to Arduino on port: ${portPath} at ${BAUD_RATE} baud.`);

  // The SerialPort class from 'serialport' is already a stream
  const port = new SerialPort({
    path: portPath,
    baudRate: BAUD_RATE,
    // autoOpen: true, // default
  });

  const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

  port.on('open', () => {
    console.log(`Serial port ${portPath} opened successfully.`);
  });

  parser.on('data', (line: string) => {
    const trimmedLine = line.trim();
    if (trimmedLine) {
      console.log(`Raw data: [${trimmedLine}]`);
      try {
        const jsonData = JSON.parse(trimmedLine);
        console.log('Parsed JSON:', jsonData);
        // TODO: Send jsonData to your SvelteKit API endpoint
        // e.g., fetch('/api/location', { method: 'POST', body: JSON.stringify(jsonData), headers: {'Content-Type': 'application/json'}})
      } catch (e) {
        console.error(`Error parsing JSON from line: "${trimmedLine}"`, e);
      }
    }
  });

  port.on('error', (err: Error) => {
    console.error(`Serial port error on ${portPath}:`, err.message);
  });

  port.on('close', () => {
    console.log(`Serial port ${portPath} closed.`);
  });

  // Graceful shutdown
  const cleanup = () => {
    console.log('\nClosing serial port before exit...');
    if (port && port.isOpen) {
      port.close(err => {
        if (err) {
          console.error('Error closing port during shutdown:', err.message);
        } else {
          console.log('Port closed gracefully.');
        }
        process.exit(err ? 1 : 0);
      });
    } else {
      process.exit(0);
    }
  };

  process.on('SIGINT', cleanup); // Ctrl+C
  process.on('SIGTERM', cleanup); // Termination signal
}

main().catch(err => {
  console.error("Unhandled error in main:", err);
  process.exitCode = 1;
});