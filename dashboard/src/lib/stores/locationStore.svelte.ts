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
  currentLocation = $state<Position>({
    lat: 19.4326,
    lng: -99.1332,
    timestamp: new Date().toISOString(),
    accuracy: 10,
    altitude: 2250,
    speed: 0
  });

  positionHistory = $state<Position[]>([]);
  mapSettings = $state({ style: 'streets' as MapStyle, autoCenter: true });
  
  // [NEW] Connection and fetching state
  fetchIntervalId = $state<number | null>(null);
  isFetching = $state(false);
  connectionState = $state<'disconnected' | 'connecting' | 'connected'>('disconnected');
  
  // [NEW] Derived state to make UI code cleaner
  isLive = $derived(this.fetchIntervalId !== null);

  // Derived values for trip summary
  trailPoints = $derived(this.positionHistory.map(pos => [pos.lat, pos.lng] as [number, number]));
  totalDistance = $derived(
    this.positionHistory.reduce((total, pos, index) => {
      if (index === 0) return 0;
      return total + calculateDistance(this.positionHistory[index - 1], pos);
    }, 0)
  );

  
  constructor() {
    this.positionHistory = [this.currentLocation];
  }

  // --- Core Methods ---
  private updateLocation(newLocation: Position): void {
    this.currentLocation = newLocation;
    let updatedHistory = [...this.positionHistory, newLocation];
    if (CONFIG.MAX_HISTORY_POINTS > 0 && updatedHistory.length > CONFIG.MAX_HISTORY_POINTS) {
      updatedHistory = updatedHistory.slice(-CONFIG.MAX_HISTORY_POINTS);
    }
    this.positionHistory = updatedHistory;
  }

  async fetchLatestLocation() {
    if (!browser || this.isFetching) return;
    this.isFetching = true;
    // Set to connecting only if we are in live mode
    if(this.isLive) this.connectionState = 'connecting';

    try {
      const response = await fetch('/api/location');
      if (!response.ok) {
        this.connectionState = 'disconnected';
        throw new Error(`API fetch failed: ${response.statusText}`);
      }
      const data: { currentLocation: Position } = await response.json();
      
      this.connectionState = 'connected';

      if (data.currentLocation && data.currentLocation.timestamp !== this.currentLocation.timestamp) {
        this.updateLocation(data.currentLocation);
      }
    } catch (error) {
      console.error("Fetch error:", error);
      this.connectionState = 'disconnected';
    } finally {
      this.isFetching = false;
    }
  }

  startLiveUpdates(intervalMs = 3000) {
    if (!browser || this.fetchIntervalId) return;
    this.fetchIntervalId = window.setInterval(() => this.fetchLatestLocation(), intervalMs);
    this.fetchLatestLocation(); // Fetch immediately
  }

  stopLiveUpdates() {
    if (this.fetchIntervalId) {
      clearInterval(this.fetchIntervalId);
      this.fetchIntervalId = null;
    }
    this.connectionState = 'disconnected';
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

  resetHistory(): void {
    this.positionHistory = [];
  }
  
  toggleAutoCenter = (): void => { this.mapSettings = { ...this.mapSettings, autoCenter: !this.mapSettings.autoCenter }; }
  setMapStyle(style: MapStyle): void { this.mapSettings = { ...this.mapSettings, style }; }
  
  formatTime(timestamp: string): string { return new Date(timestamp).toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit' }); }
  formatDate(timestamp: string): string { return new Date(timestamp).toLocaleDateString([], { year: 'numeric', month: 'short', day: 'numeric' }); }
}

export const locationStore = new LocationStore();