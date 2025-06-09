<!-- dashboard/src/components/MapView.svelte -->
<script lang="ts">
  import { onMount, onDestroy } from 'svelte';
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { Layers, Navigation, MapPin, ChevronDown, LocateFixed, RefreshCw, Power, PowerOff } from 'lucide-svelte';
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

  // UI Functions
  function toggleStyleSelector(): void { showStyleSelector = !showStyleSelector; }
  function handleStyleSelection(style: MapStyle): void { locationStore.setMapStyle(style); showStyleSelector = false; }
  function handleClickOutsideStyleSelector(event: MouseEvent): void {
    const target = event.target as HTMLElement;
    if (showStyleSelector && !target?.closest('.style-selector-container') && !target?.closest('.style-selector-button')) {
      showStyleSelector = false;
    }
  }

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
    locationStore.stopLiveUpdates();
  });

  function initMap(): void {
    if (!mapElement || !L || map) return;
    
    map = L.map(mapElement, { zoomControl: false }).setView([locationStore.currentLocation.lat, locationStore.currentLocation.lng], 15);
    L.control.zoom({ position: 'bottomright' }).addTo(map);
    
    updateMapStyle(locationStore.mapSettings.style);
    
    marker = L.marker([locationStore.currentLocation.lat, locationStore.currentLocation.lng], { icon: markerIcon }).addTo(map);
    
    polyline = L.polyline(locationStore.trailPoints, {
      color: '#0ea5e9',
      weight: 3,
      opacity: 0.7,
    }).addTo(map);
    
    // This is still important to handle resizing
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

  // Effects for updating the map after it's been created
  $effect(() => {
    if (map && marker) {
      const latLng = L.latLng(locationStore.currentLocation.lat, locationStore.currentLocation.lng);
      marker.setLatLng(latLng);
      marker.bindPopup(`...`); // Popup content
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
  <div class="flex flex-wrap gap-2 justify-between items-center mb-2">
    
    <!-- Style Selector Dropdown -->
    <div class="relative z-30 style-selector-container">
      <button onclick={toggleStyleSelector} class="style-selector-button flex items-center gap-1.5 h-9 px-4 py-2 bg-white rounded-md text-sm font-medium border shadow-sm hover:bg-gray-50">
        <Layers class="h-4 w-4" />
        <span class="capitalize">{locationStore.mapSettings.style}</span>
        <ChevronDown class="h-3 w-3 opacity-50 ml-1" />
      </button>
      {#if showStyleSelector}
        <!-- Dropdown content here -->
      {/if}
    </div>

    <!-- Main Action Buttons -->
    <div class="flex gap-2 items-center">
      <!-- Button to Start/Stop LIVE data -->
      <button
        onclick={() => locationStore.fetchIntervalId ? locationStore.stopLiveUpdates() : locationStore.startLiveUpdates()}
        class="h-9 px-3 text-sm flex items-center gap-1.5 rounded-md font-medium shadow {locationStore.fetchIntervalId ? 'bg-destructive text-destructive-foreground hover:bg-destructive/90' : 'bg-green-600 text-white hover:bg-green-700'}">
        {#if locationStore.fetchIntervalId}
          <PowerOff class="h-4 w-4" /> Stop Live
        {:else}
          <Power class="h-4 w-4" /> Start Live
        {/if}
      </button>
      
      <!-- Button for SIMULATION -->
      <button 
        onclick={() => locationStore.moveMarker()}
        class="h-9 px-3 text-sm bg-blue-600 text-white rounded-md font-medium shadow hover:bg-blue-700">
        Simulate Movement
      </button>

      <button 
        onclick={() => locationStore.resetHistory()}
        class="h-9 px-3 text-sm bg-background rounded-md font-medium border border-input shadow-sm hover:bg-accent hover:text-accent-foreground">
        Reset History
      </button>

      <button 
        onclick={locationStore.toggleAutoCenter}
        class="flex items-center justify-center h-9 w-9 bg-white rounded-md font-medium border shadow-sm hover:bg-gray-50 {locationStore.mapSettings.autoCenter ? 'text-primary ring-2 ring-primary/50' : ''}">
        <LocateFixed class="h-4 w-4" />
      </button>
    </div>
  </div>
  
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