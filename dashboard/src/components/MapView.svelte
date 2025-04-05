<!-- MapView.svelte -->
<script lang="ts">
  import { onMount, onDestroy } from 'svelte';
  import type { LeafletMap, LeafletMarker, LeafletPolyline, LeafletIcon, LatLngExpression, LeafletTileLayer } from '$lib/types';
  import type { MapStyle } from '$lib/types';
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { Layers, Navigation, MapPin, ChevronDown, LocateFixed } from 'lucide-svelte';
  import LocationInfo from './LocationInfo.svelte';
  import PositionHistory from './PositionHistory.svelte';
  
  // We'll need to load Leaflet dynamically in onMount as it requires browser APIs
  let map: LeafletMap | null = $state(null);
  let mapElement: HTMLElement | null = $state(null);
  let leaflet: any;
  let marker: LeafletMarker | null = $state(null);
  let polyline: LeafletPolyline | null = $state(null);
  let mapLoaded = $state(false);
  let showStyleSelector = $state(false);

  // These will be needed for map initialization (use browser-only code in onMount)
  let L: any;
  let markerIcon: LeafletIcon;

  // Toggle the map style selector panel
  function toggleStyleSelector(): void {
    showStyleSelector = !showStyleSelector;
  }

  onMount(async () => {
    // Dynamically import leaflet (browser-only)
    leaflet = await import('leaflet');
    L = leaflet.default;
    
    // Import CSS
    await import('leaflet/dist/leaflet.css');
    
    // Create custom icon
    markerIcon = new L.Icon({
      iconUrl: "https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon.png",
      iconRetinaUrl: "https://unpkg.com/leaflet@1.7.1/dist/images/marker-icon-2x.png",
      shadowUrl: "https://unpkg.com/leaflet@1.7.1/dist/images/marker-shadow.png",
      iconSize: [25, 41],
      iconAnchor: [12, 41],
      popupAnchor: [1, -34],
      shadowSize: [41, 41],
    });
    
    // Initialize map
    initMap();
    mapLoaded = true;
  });

  // Initialize the map
  function initMap(): void {
    if (!mapElement) return;
    
    map = L.map(mapElement).setView([locationStore.currentLocation.lat, locationStore.currentLocation.lng], 15);
    
    // Add tile layer based on current style
    updateMapStyle(locationStore.mapSettings.style);
    
    // Add current position marker
    marker = L.marker([locationStore.currentLocation.lat, locationStore.currentLocation.lng], { icon: markerIcon })
      .addTo(map)
      .bindPopup(`
        <div style="padding: 4px">
          <div style="font-weight: 500">Current Location</div>
          <div style="font-size: 12px; color: #666; margin-top: 4px">
            ${locationStore.currentLocation.lat.toFixed(6)}, ${locationStore.currentLocation.lng.toFixed(6)}
          </div>
        </div>
      `);
    
    // Add polyline for trail
    polyline = L.polyline(locationStore.trailPoints, {
      color: '#0ea5e9',
      weight: 3,
      opacity: 0.7,
      dashArray: '5, 5'
    }).addTo(map);
    
    // Add zoom control
    L.control.zoom({ position: 'bottomright' }).addTo(map);
  }

  // Update map when style changes
  function updateMapStyle(style: MapStyle): void {
    if (!map) return;
    
    // Remove existing layers
    map.eachLayer((layer: any) => {
      if (layer instanceof L.TileLayer) {
        if (map) {
          map.removeLayer(layer);
        }
      }
    });
    
    // Add new layer based on selected style
    if (style === 'streets') {
      L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
      }).addTo(map);
    } else if (style === 'satellite') {
      L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
        attribution: '&copy; <a href="https://www.esri.com">Esri</a>'
      }).addTo(map);
    } else if (style === 'terrain') {
      L.tileLayer('https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="https://www.opentopomap.org">OpenTopoMap</a>'
      }).addTo(map);
    }
    
    // Close the style selector after selecting
    showStyleSelector = false;
  }

  // Update marker position and trail when location changes
  $effect(() => {
    if (map && marker) {
      marker.setLatLng([locationStore.currentLocation.lat, locationStore.currentLocation.lng]);
      
      // Update popup content
      marker.getPopup()?.setContent(`
        <div style="padding: 4px">
          <div style="font-weight: 500">Current Location</div>
          <div style="font-size: 12px; color: #666; margin-top: 4px">
            ${locationStore.currentLocation.lat.toFixed(6)}, ${locationStore.currentLocation.lng.toFixed(6)}
          </div>
        </div>
      `);
      
      // Center map if auto-center is enabled
      if (locationStore.mapSettings.autoCenter) {
        map.setView([locationStore.currentLocation.lat, locationStore.currentLocation.lng]);
      }
    }
  });

  // Update polyline when trail points change
  $effect(() => {
    if (map && polyline && locationStore.trailPoints.length > 1) {
      polyline.setLatLngs(locationStore.trailPoints);
    }
  });

  // Update map style when it changes
  $effect(() => {
    if (map) {
      updateMapStyle(locationStore.mapSettings.style);
    }
  });

  onDestroy(() => {
    if (map) {
      map.remove();
    }
  });
</script>

<div class="w-full h-full grid grid-cols-1 lg:grid-cols-3 gap-4">
  <div class="lg:col-span-2 flex flex-col h-full">
    <!-- Top Controls -->
    <div class="flex justify-between items-center mb-2 relative z-30">
      <!-- Map style selector button -->
      <button 
        onclick={toggleStyleSelector}
        class="flex items-center gap-1.5 h-9 px-4 py-2 bg-white rounded-md text-sm font-medium border shadow-sm hover:bg-gray-50"
      >
        <Layers class="h-4 w-4" />
        {#if locationStore.mapSettings.style === 'streets'}
          Streets
        {:else if locationStore.mapSettings.style === 'satellite'}
          Satellite
        {:else}
          Terrain
        {/if}
        <ChevronDown class="h-3 w-3 opacity-50 ml-1" />
      </button>

      <!-- Auto-center button -->
      <button 
        onclick={locationStore.toggleAutoCenter}
        class="flex items-center gap-1.5 h-9 px-4 py-2 bg-white rounded-md text-sm font-medium border shadow-sm hover:bg-gray-50 {locationStore.mapSettings.autoCenter ? 'bg-primary/10' : ''}"
      >
        <LocateFixed class="h-4 w-4 {locationStore.mapSettings.autoCenter ? 'text-primary' : ''}" />
        Auto-center
      </button>
    </div>
    
    <!-- Map container with styles selector -->
    <div class="relative h-[400px] rounded-lg overflow-hidden border flex-grow">
      <!-- Map style selector panel -->
      {#if showStyleSelector}
        <div class="absolute top-0 left-0 z-30 bg-white shadow-md rounded-md overflow-hidden mt-2 ml-2">
          <div class="flex flex-col divide-y">
            <button 
              class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'streets' ? 'bg-gray-100' : ''}"
              onclick={() => locationStore.setMapStyle('streets')}
            >
              <MapPin class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'streets' ? 'text-primary' : ''}" />
              <span class="text-sm">Streets</span>
            </button>
            
            <button 
              class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'satellite' ? 'bg-gray-100' : ''}"
              onclick={() => locationStore.setMapStyle('satellite')}
            >
              <Layers class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'satellite' ? 'text-primary' : ''}" />
              <span class="text-sm">Satellite</span>
            </button>
            
            <button 
              class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'terrain' ? 'bg-gray-100' : ''}"
              onclick={() => locationStore.setMapStyle('terrain')}
            >
              <Navigation class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'terrain' ? 'text-primary' : ''}" />
              <span class="text-sm">Terrain</span>
            </button>
          </div>
        </div>
      {/if}
      
      <!-- Map element -->
      <div bind:this={mapElement} class="h-full w-full absolute inset-0 z-10"></div>
    </div>
    
    <!-- Location info cards -->
    <LocationInfo />
  </div>
  
  <div class="lg:col-span-1 h-full">
    <!-- Position history panel -->
    <PositionHistory />
  </div>
</div>
