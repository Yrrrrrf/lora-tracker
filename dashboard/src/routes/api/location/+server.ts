// file: dashboard/src/routes/api/location/+server.ts
import { json } from '@sveltejs/kit';
import type { RequestHandler } from './$types';
import type { Position } from '$lib/types';

// --- Default starting position if no data has been received yet ---
const initialPosition: Position = {
  lat: 19.4326,
  lng: -99.1332,
  timestamp: new Date().toISOString(), 
  accuracy: 100,
  altitude: 2250,
  speed: 0
};

// In-memory storage
let locationHistory: Position[] = [];
let currentLocation: Position | null = null;

// POST: Receives data from the serialBridge
export const POST: RequestHandler = async ({ request }) => {
    try {
        const data = await request.json();

        if (typeof data.lat !== 'number' || typeof data.lng !== 'number') {
            return json({ error: 'Invalid location data' }, { status: 400 });
        }
        
        // The bridge should now be adding the timestamp, but we can do it here as a fallback
        const newPosition: Position = {
            lat: data.lat,
            lng: data.lng,
            timestamp: new Date().toISOString(), // Generate timestamp on arrival
            accuracy: data.acc_hdop, // Assuming 'acc_hdop' from your mock
            altitude: data.alt,
            speed: data.spd,
        };

        currentLocation = newPosition;
        locationHistory.push(newPosition);

        console.log('Received new location:', newPosition);

        if (locationHistory.length > 1000) {
            locationHistory = locationHistory.slice(-1000);
        }

        return json({ success: true });
    } catch (error) {
        return json({ error: 'Invalid request data' }, { status: 400 });
    }
};

// GET: Provides data to the frontend
export const GET: RequestHandler = async ({ url }) => {
    const includeHistory = url.searchParams.get('history') === 'true';

    const response = {
        currentLocation: currentLocation ?? initialPosition,
        ...(includeHistory ? { locationHistory } : {})
    };

    return json(response);
};