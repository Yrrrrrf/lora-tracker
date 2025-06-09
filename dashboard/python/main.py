# file: dashboard/serialBridge.py
import serial
import requests
import time
import json
import sys

# --- Configuration ---
PORT_NAME = "COM5"
BAUD_RATE = 9600
API_ENDPOINT = "http://localhost:5173/api/location"
# API_ENDPOINT = "http://127.0.0.1:5173/api/location"

print("--- Starting Python Serial Bridge ---")
print(f"Attempting to connect to {PORT_NAME} at {BAUD_RATE} baud...")
# Ensure your SvelteKit dev server is running before starting this script.

try:
    # The 'with' statement ensures the port is closed automatically on exit/error
    with serial.Serial(PORT_NAME, BAUD_RATE, timeout=1) as ser:
        print(f"✅ Port {PORT_NAME} opened successfully. Waiting for data...")
        
        while True:
            try:
                # Read one line from the serial port
                line = ser.readline().decode('utf-8').strip()

                # If the line is not empty, process it
                if line:
                    print(f"⬅️  Received: {line}")
                    
                    # Parse the JSON data
                    try:
                        data = json.loads(line)
                    except json.JSONDecodeError:
                        print("Error: Could not parse JSON. Skipping line.")
                        continue
                    
                    # Forward the data to the SvelteKit API
                    try:
                        response = requests.post(API_ENDPOINT, json=data, timeout=5)
                        if response.ok:
                            print("➡️  Forwarded to API: Success")
                        else:
                            print(f"API Error: {response.status_code} {response.text}")
                    except requests.exceptions.RequestException as e:
                        print(f"Fetch failed. Is SvelteKit server running? Error: {e}")

            except KeyboardInterrupt:
                print("\nShutdown signal received. Closing port.")
                break
            except Exception as e:
                print(f"An unexpected error occurred: {e}")
                time.sleep(2)

except serial.SerialException as e:
    print(f"--- PORT ERROR ---: Could not open port {PORT_NAME}. Error: {e}")
    print("Please check the port name and ensure no other program (like PIO Monitor) is using it.")
    sys.exit(1)