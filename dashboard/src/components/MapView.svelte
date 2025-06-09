<!-- dashboard/src/components/MapView.svelte -->
<script lang="ts">
  import { onMount, onDestroy } from 'svelte';
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { Layers, Navigation, MapPin, ChevronDown, LocateFixed, RefreshCw, Power } from 'lucide-svelte';
  import type { LeafletMap, LeafletMarker, LeafletPolyline, LeafletIcon, MapStyle } from '$lib/types';
  import type * as Leaflet from 'leaflet';

  let map: LeafletMap | null = $state(null);
  let mapElement: HTMLElement | null = $state(null);
  let marker: LeafletMarker | null = $state(null);
  let polyline: LeafletPolyline | null = $state(null);
  let mapLoaded = $state(false);
  let showStyleSelector = $state(false);

  let L: any;
  let markerIcon: LeafletIcon;

  // --- UI Functions ---
  function toggleStyleSelector(): void {
    showStyleSelector = !showStyleSelector;
  }
  
  function handleStyleSelection(style: MapStyle): void {
    locationStore.setMapStyle(style);
    showStyleSelector = false;
  }

  function handleClickOutsideStyleSelector(event: MouseEvent): void {
    const target = event.target as HTMLElement;
    if (showStyleSelector && !target?.closest('.style-selector-container') && !target?.closest('.style-selector-button')) {
      showStyleSelector = false;
    }
  }

  // --- Map Initialization and Cleanup ---
  onMount(async () => {
    const leafletModule = await import('leaflet');
    L = leafletModule.default;
    await import('leaflet/dist/leaflet.css');
    
    markerIcon = L.icon({
      iconUrl: "https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon.png",
      iconRetinaUrl: "https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon-2x.png",
      shadowUrl: "https://unpkg.com/leaflet@1.7.1/dist/images/marker-shadow.png",
      iconSize: [25, 41],
      iconAnchor: [12, 41],
      popupAnchor: [1, -34],
      shadowSize: [41, 41],
    });
    
    initMap();
    mapLoaded = true;
  });

  onDestroy(() => {
    if (map) map.remove();
    // Ensure we stop fetching when the component is destroyed
    locationStore.stopLiveUpdates();
  });

  function initMap(): void {
    if (!mapElement || !L || map) return; // Guard against re-initialization
    
    map = L.map(mapElement, { zoomControl: false }).setView([locationStore.currentLocation.lat, locationStore.currentLocation.lng], 15);
    L.control.zoom({ position: 'bottomright' }).addTo(map);
    
    updateMapStyle(locationStore.mapSettings.style);
    
    marker = L.marker([locationStore.currentLocation.lat, locationStore.currentLocation.lng], { icon: markerIcon }).addTo(map);
    
    polyline = L.polyline(locationStore.trailPoints, {
      color: '#0ea5e9',
      weight: 3,
      opacity: 0.7,
    }).addTo(map);

    // Invalidate size to fix potential rendering issues in flex/grid layouts
    setTimeout(() => (map as any)?.invalidateSize(), 100);
  }

  function updateMapStyle(style: MapStyle): void {
    if (!map || !L) return;
    
    map.eachLayer((layer) => {
      if (layer instanceof L.TileLayer) map?.removeLayer(layer);
    });
    
    let newLayer: Leaflet.TileLayer | null = null;
    if (style === 'streets') newLayer = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { attribution: '© OpenStreetMap' });
    else if (style === 'satellite') newLayer = L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', { attribution: '© Esri' });
    else if (style === 'terrain') newLayer = L.tileLayer('https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png', { attribution: '© OpenTopoMap' });
    
    if (newLayer) newLayer.addTo(map);
  }

  // --- Reactive Effects to Update Map from Store ---
  $effect(() => {
    if (map && marker) {
      const latLng = L.latLng(locationStore.currentLocation.lat, locationStore.currentLocation.lng);
      marker.setLatLng(latLng);
      marker.bindPopup(`
        <div style="font-weight: 500;">Current Location</div>
        <div style="font-size: 12px; color: #666; margin-top: 4px;">
          ${locationStore.currentLocation.lat.toFixed(6)}, ${locationStore.currentLocation.lng.toFixed(6)}
        </div>
      `);
      if (locationStore.mapSettings.autoCenter) {
        map.setView(latLng);
      }
    }
  });

  $effect(() => {
    if (map && polyline) {
      const latLngs = locationStore.trailPoints.map(p => L.latLng(p[0], p[1]));
      polyline.setLatLngs(latLngs);
    }
  });

  $effect(() => {
    if (map) updateMapStyle(locationStore.mapSettings.style);
  });
</script>

<svelte:window onclick={handleClickOutsideStyleSelector} />

<div class="flex flex-col h-full">
  <!-- Top Controls -->
  <div class="flex flex-wrap gap-2 justify-between items-center mb-2">
    <!-- Style Selector Dropdown -->
    <div class="relative z-30 style-selector-container">
      <button 
        onclick={toggleStyleSelector}
        class="style-selector-button flex items-center gap-1.5 h-9 px-4 py-2 bg-white rounded-md text-sm font-medium border shadow-sm hover:bg-gray-50"
      >
        <Layers class="h-4 w-4" />
        <span class="capitalize">{locationStore.mapSettings.style}</span>
        <ChevronDown class="h-3 w-3 opacity-50 ml-1" />
      </button>

      {#if showStyleSelector}
        <div class="absolute top-full left-0 z-40 bg-white shadow-md rounded-md overflow-hidden mt-1 w-36">
          <div class="flex flex-col divide-y">
            <button class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'streets' ? 'bg-gray-100' : ''}" onclick={() => handleStyleSelection('streets')}>
              <MapPin class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'streets' ? 'text-primary' : ''}" />
              <span class="text-sm">Streets</span>
            </button>
            <button class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'satellite' ? 'bg-gray-100' : ''}" onclick={() => handleStyleSelection('satellite')}>
              <Layers class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'satellite' ? 'text-primary' : ''}" />
              <span class="text-sm">Satellite</span>
            </button>
            <button class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'terrain' ? 'bg-gray-100' : ''}" onclick={() => handleStyleSelection('terrain')}>
              <Navigation class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'terrain' ? 'text-primary' : ''}" />
              <span class="text-sm">Terrain</span>
            </button>
          </div>
        </div>
      {/if}
    </div>

    <!-- Main Action Buttons -->
    <div class="flex gap-2 items-center">
      <button
        onclick={() => locationStore.isLive ? locationStore.stopLiveUpdates() : locationStore.startLiveUpdates()}
        class="h-9 px-3 text-sm flex items-center gap-1.5 rounded-md font-medium shadow transition-colors {locationStore.isLive ? 'bg-destructive text-destructive-foreground hover:bg-destructive/90' : 'bg-green-600 text-white hover:bg-green-700'}"
      >
        {#if locationStore.isLive}
          <RefreshCw class="h-4 w-4 animate-spin" />
          <span>Stop Live</span>
        {:else}
          <Power class="h-4 w-4" />
          <span>Start Live</span>
        {/if}
      </button>

      <button 
        onclick={() => locationStore.moveMarker()}
        class="h-9 px-3 text-sm bg-blue-600 text-white rounded-md font-medium shadow hover:bg-blue-700">
        Simulate
      </button>

      <button 
        onclick={() => locationStore.resetHistory()}
        class="h-9 px-3 text-sm bg-background rounded-md font-medium border border-input shadow-sm hover:bg-accent hover:text-accent-foreground">
        Reset
      </button>

      <button 
        onclick={locationStore.toggleAutoCenter}
        title="Toggle Auto-center"
        class="flex items-center justify-center h-9 w-9 bg-white rounded-md font-medium border shadow-sm hover:bg-gray-50 {locationStore.mapSettings.autoCenter ? 'text-primary ring-2 ring-primary/50' : ''}">
        <LocateFixed class="h-4 w-4" />
      </button>
    </div>
  </div>
  
  <!-- Map Container -->
  <div class="relative h-[450px] md:h-[500px] rounded-lg overflow-hidden border flex-grow">
    <div bind:this={mapElement} class="h-full w-full absolute inset-0 z-10">
       {#if !mapLoaded}
        <div class="flex items-center justify-center h-full bg-muted/50">
          <RefreshCw class="h-6 w-6 animate-spin text-muted-foreground" />
          <span class="ml-2 text-muted-foreground">Loading Map...</span>
        </div>
      {/if}
    </div>

  </div>
</div>