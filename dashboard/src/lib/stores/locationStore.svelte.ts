// file: dashboard/src/lib/stores/locationStore.svelte.ts
import { browser } from '$app/environment';
import type { Position, MapStyle } from '$lib/types';

const CONFIG = {
  MAX_HISTORY_POINTS: 1000 
};

function calculateDistance(pos1: Position, pos2: Position): number {
  if (!pos1 || !pos2) return 0;
  const R = 6371e3;
  const φ1 = (pos1.lat * Math.PI) / 180;
  const φ2 = (pos2.lat * Math.PI) / 180;
  const Δφ = ((pos2.lat - pos1.lat) * Math.PI) / 180;
  const Δλ = ((pos2.lng - pos1.lng) * Math.PI) / 180;
  const a = Math.sin(Δφ / 2) * Math.sin(Δφ / 2) + Math.cos(φ1) * Math.cos(φ2) * Math.sin(Δλ / 2) * Math.sin(Δλ / 2);
  const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  return R * c;
}

class LocationStore {
  // [REVERTED] Start with a valid, non-null object. This is the key fix.
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
  
  // [NEW] State for managing real-time fetching
  fetchIntervalId = $state<number | null>(null);
  
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
    // [REVERTED] Initialize history with the starting location.
    this.positionHistory = [this.currentLocation];
  }
  
  // [KEPT] The simulation logic is valuable for testing.
  moveMarker(): void {
    const newLocation: Position = {
      lat: this.currentLocation.lat + (Math.random() - 0.5) * 0.005,
      lng: this.currentLocation.lng + (Math.random() - 0.5) * 0.005,
      timestamp: new Date().toISOString(),
      accuracy: Math.max(5, (this.currentLocation.accuracy || 0) + (Math.random() - 0.5) * 2),
      altitude: (this.currentLocation.altitude || 0) + (Math.random() - 0.5) * 5,
      speed: Math.max(0, (this.currentLocation.speed || 0) + (Math.random() - 0.5) * 2),
    };
    this.updateLocation(newLocation);
  }

  // [NEW] Helper to avoid code duplication
  private updateLocation(newLocation: Position): void {
    this.currentLocation = newLocation;
    let updatedHistory = [...this.positionHistory, newLocation];
    if (CONFIG.MAX_HISTORY_POINTS > 0 && updatedHistory.length > CONFIG.MAX_HISTORY_POINTS) {
      updatedHistory = updatedHistory.slice(-CONFIG.MAX_HISTORY_POINTS);
    }
    this.positionHistory = updatedHistory;
  }

  // [NEW] Fetching logic for live data
  async fetchLatestLocation() {
    if (!browser) return;
    try {
      const response = await fetch('/api/location');
      if (!response.ok) return;
      const data: { currentLocation: Position } = await response.json();
      if (data.currentLocation && data.currentLocation.timestamp !== this.currentLocation.timestamp) {
        this.updateLocation(data.currentLocation);
      }
    } catch (error) {
      console.error("Fetch error:", error);
    }
  }

  startLiveUpdates(intervalMs = 3000) {
    if (!browser || this.fetchIntervalId) return;
    this.fetchLatestLocation(); // Fetch immediately
    this.fetchIntervalId = window.setInterval(() => this.fetchLatestLocation(), intervalMs);
  }

  stopLiveUpdates() {
    if (this.fetchIntervalId) {
      clearInterval(this.fetchIntervalId);
      this.fetchIntervalId = null;
    }
  }

  resetHistory(): void {
    this.positionHistory = [this.currentLocation];
  }
  
  toggleAutoCenter = (): void => { this.mapSettings = { ...this.mapSettings, autoCenter: !this.mapSettings.autoCenter }; }
  setMapStyle(style: MapStyle): void { this.mapSettings = { ...this.mapSettings, style }; }
  
  formatTime(timestamp: string): string { return new Date(timestamp).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit' }); }
  formatDate(timestamp: string): string { return new Date(timestamp).toLocaleDateString([], { year: 'numeric', month: 'short', day: 'numeric' }); }
}

export const locationStore = new LocationStore();