// src/lib/stores/locationStore.svelte.ts
import type { Position, MapStyle } from '$lib/types';

// Configuration options
const CONFIG = {
  // Set to 0 for unlimited history points or any number for a limit
  MAX_HISTORY_POINTS: 0 
};

// Helper function to calculate distance between two points
function calculateDistance(pos1: Position, pos2: Position): number {
  const R = 6371e3; // Earth's radius in meters
  const φ1 = (pos1.lat * Math.PI) / 180;
  const φ2 = (pos2.lat * Math.PI) / 180;
  const Δφ = ((pos2.lat - pos1.lat) * Math.PI) / 180;
  const Δλ = ((pos2.lng - pos1.lng) * Math.PI) / 180;

  const a = Math.sin(Δφ / 2) * Math.sin(Δφ / 2) + 
            Math.cos(φ1) * Math.cos(φ2) * 
            Math.sin(Δλ / 2) * Math.sin(Δλ / 2);
  const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  return R * c; // in meters
}

class LocationStore {
  // Initial location (Mexico City)
  currentLocation = $state<Position>({
    lat: 19.4326,
    lng: -99.1332,
    timestamp: new Date().toISOString(),
    accuracy: 10,
    altitude: 2250,
    speed: 0
  });

  positionHistory = $state<Position[]>([]);
  
  mapSettings = $state({
    style: 'streets' as MapStyle,
    autoCenter: true,
    isFullscreen: false
  });
  
  updateInterval = $state<number | null>(null);
  
  // Derived values
  trailPoints = $derived(
    this.positionHistory.map(pos => [pos.lat, pos.lng] as [number, number])
  );
  
  totalDistance = $derived(
    this.positionHistory.reduce((total, pos, index) => {
      if (index === 0) return 0;
      return total + calculateDistance(this.positionHistory[index - 1], pos);
    }, 0)
  );
  
  constructor() {
    // Initialize history with current location
    this.positionHistory = [this.currentLocation];
  }
  
  // Move marker to simulate movement
  moveMarker(): void {
    const newLocation: Position = {
      lat: this.currentLocation.lat + (Math.random() - 0.5) * 0.01,
      lng: this.currentLocation.lng + (Math.random() - 0.5) * 0.01,
      timestamp: new Date().toISOString(),
      accuracy: Math.max(5, (this.currentLocation.accuracy || 0) + (Math.random() - 0.5) * 2),
      altitude: (this.currentLocation.altitude || 0) + (Math.random() - 0.5) * 5,
      speed: Math.max(0, (this.currentLocation.speed || 0) + (Math.random() - 0.5) * 2),
    };
    
    // Update current location
    this.currentLocation = newLocation;
    
    // Update history - either keep all points or limit to MAX_HISTORY_POINTS if set
    let updatedHistory = [...this.positionHistory, newLocation];
    
    if (CONFIG.MAX_HISTORY_POINTS > 0 && updatedHistory.length > CONFIG.MAX_HISTORY_POINTS) {
      updatedHistory = updatedHistory.slice(
        Math.max(0, updatedHistory.length - CONFIG.MAX_HISTORY_POINTS)
      );
    }
    
    this.positionHistory = updatedHistory;
  }
  
  // Clear all history except current location
  resetHistory(): void {
    this.positionHistory = [this.currentLocation];
  }
  
  // Toggle fullscreen mode
  toggleFullscreen = (): void => {
    this.mapSettings = {
      ...this.mapSettings,
      isFullscreen: !this.mapSettings.isFullscreen
    };
  }
  
  // Toggle auto-center
  toggleAutoCenter = (): void => {
    this.mapSettings = {
      ...this.mapSettings,
      autoCenter: !this.mapSettings.autoCenter
    };
  }
  
  // Set map style
  setMapStyle(style: MapStyle): void {
    this.mapSettings = {
      ...this.mapSettings,
      style
    };
  }
  
  // Format helpers
  formatTime(timestamp: string): string {
    return new Date(timestamp).toLocaleTimeString([], {
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
    });
  }
  
  formatDate(timestamp: string): string {
    return new Date(timestamp).toLocaleDateString([], {
      year: 'numeric',
      month: 'short',
      day: 'numeric',
    });
  }
}

// Create singleton instance
export const locationStore = new LocationStore();
