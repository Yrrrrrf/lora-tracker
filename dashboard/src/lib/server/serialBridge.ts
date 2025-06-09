// file: dashboard/src/lib/server/serialBridge.ts
import { SerialPort } from 'serialport';
import { ReadlineParser } from '@serialport/parser-readline';

// --- Configuration ---
const API_ENDPOINT = 'http://localhost:5173/api/location'; 
const BAUD_RATE = 9600;
const RETRY_CONNECT_INTERVAL_MS = 5000;

let port: SerialPort | null = null;
let isShuttingDown = false;

// --- [REVISED] Helper to find the Arduino port automatically ---
async function findArduinoPort(): Promise<string | null> {
    console.log('Searching for serial ports...');
    try {
        const ports = await SerialPort.list();

        // 1. First-pass: Look for a port with a known Arduino manufacturer
        const explicitArduinoPort = ports.find(p => p.manufacturer?.toLowerCase().includes('arduino'));
        if (explicitArduinoPort) {
            console.log(`Found explicit Arduino port: ${explicitArduinoPort.path}`);
            return explicitArduinoPort.path;
        }

        // 2. Second-pass: Look for common path patterns
        const commonPathPort = ports.find(p => p.path.includes('tty.usbmodem') || p.path.includes('ttyACM'));
        if (commonPathPort) {
            console.log(`Found port with common Arduino path: ${commonPathPort.path}`);
            return commonPathPort.path;
        }
        
        // 3. Fallback: If there's only one port available, assume it's the one.
        if (ports.length === 1) {
            console.log(`Only one port found (${ports[0].path}). Assuming it's the Arduino.`);
            return ports[0].path;
        }

        // 4. If we still can't decide, log the options and fail.
        console.warn('Could not automatically determine the Arduino port.');
        if (ports.length > 0) {
            console.log('Please specify a port from this list:', ports.map(p => p.path).join(', '));
        } else {
            console.log('No serial ports found.');
        }
        return null;
    } catch (err) {
        console.error("Error listing serial ports:", err);
        return null;
    }
}

// --- Main connection and reading logic ---
function connectAndRead(portPath: string) {
    if (isShuttingDown) return;
    console.log(`Attempting to connect to ${portPath}...`);

    port = new SerialPort({ path: portPath, baudRate: BAUD_RATE });
    const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

    port.on('open', () => console.log(`✅ Port ${portPath} opened.`));

    parser.on('data', async (line: string) => {
        try {
            const jsonData = JSON.parse(line);
            console.log('⬅️  Received:', line);

            // Forward the data to the SvelteKit backend
            const response = await fetch(API_ENDPOINT, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(jsonData),
            });

            if (response.ok) {
                console.log('➡️  Forwarded to API: Success');
            } else {
                console.error('API Error:', response.status, response.statusText);
            }
        } catch (err) {
            console.error('Error parsing JSON or fetching:', err);
        }
    });

    port.on('error', (err) => {
        console.error(`Port ${portPath} error:`, err.message);
        scheduleReconnect(portPath);
    });

    port.on('close', () => {
        console.log(`Port ${portPath} closed.`);
        if (!isShuttingDown) scheduleReconnect(portPath);
    });
}

// --- Reconnection logic ---
function scheduleReconnect(portPath: string) {
    if (port?.isOpen) port.close();
    port = null;
    console.log(`Will attempt to reconnect in ${RETRY_CONNECT_INTERVAL_MS / 1000}s...`);
    setTimeout(() => connectAndRead(portPath), RETRY_CONNECT_INTERVAL_MS);
}

// --- Graceful shutdown ---
function gracefulShutdown() {
    console.log('\nShutting down serial bridge...');
    isShuttingDown = true;
    if (port?.isOpen) {
        port.close(() => {
            console.log('Port closed.');
            process.exit(0);
        });
    } else {
        process.exit(0);
    }
}

process.on('SIGINT', gracefulShutdown); // Ctrl+C
process.on('SIGTERM', gracefulShutdown);

// --- Start the process ---
async function main() {
    const portPath = await findArduinoPort();
    if (portPath) {
        connectAndRead(portPath);
    } else {
        // If not found, retry finding it after an interval
        console.log(`Retrying port search in ${RETRY_CONNECT_INTERVAL_MS / 1000}s...`);
        setTimeout(main, RETRY_CONNECT_INTERVAL_MS);
    }
}

main();