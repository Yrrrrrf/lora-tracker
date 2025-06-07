// src/routes/api/location/+server.ts
import { json } from '@sveltejs/kit';
import type { RequestHandler } from './api/location/$types';
import type { Position } from '$lib/types';

// In-memory storage as a temporary database
// In a real application, you would use a proper database
let locationHistory: Position[] = [];
let currentLocation: Position = {
  lat: 19.4326,
  lng: -99.1332,
  timestamp: new Date().toISOString(),
  accuracy: 10,
  altitude: 2250,
  speed: 0
};

// POST endpoint to receive location updates
export const POST: RequestHandler = async ({ request }) => {
  try {
    const data = await request.json();
    
    // Validate required fields
    if (typeof data.lat !== 'number' || typeof data.lng !== 'number') {
      return json({ error: 'Invalid location data. Required fields: lat, lng as numbers' }, { status: 400 });
    }
    
    // Create new position object with current timestamp
    const newPosition: Position = {
      lat: data.lat,
      lng: data.lng,
      timestamp: new Date().toISOString(),
      accuracy: data.accuracy !== undefined ? Number(data.accuracy) : undefined,
      altitude: data.altitude !== undefined ? Number(data.altitude) : undefined,
      speed: data.speed !== undefined ? Number(data.speed) : undefined
    };
    
    // Update current location
    currentLocation = newPosition;
    
    // Add to history
    locationHistory.push(newPosition);
    
    // Optional: Limit history size (e.g., keep last 1000 points)
    if (locationHistory.length > 1000) {
      locationHistory = locationHistory.slice(-1000);
    }
    
    return json({ success: true, position: newPosition });
  } catch (error) {
    console.error('Error processing location update:', error);
    return json({ error: 'Invalid request data' }, { status: 400 });
  }
};

// GET endpoint to retrieve current location and history
export const GET: RequestHandler = async ({ url }) => {
  // Check if client only wants current position or full history
  const historyParam = url.searchParams.get('history');
  const includeHistory = historyParam === 'true';
  
  return json({
    currentLocation,
    ...(includeHistory ? { locationHistory } : {})
  });
};