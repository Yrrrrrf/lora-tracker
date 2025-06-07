<!-- dashboard/src/components/map/MapView.svelte -->
<script lang="ts">
  import { onMount, onDestroy } from 'svelte';
  import type { LeafletMap, LeafletMarker, LeafletPolyline, LeafletIcon, LatLngExpression, LeafletTileLayer } from '$lib/types';
  import type { MapStyle } from '$lib/types';
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { Layers, Navigation, MapPin, ChevronDown, LocateFixed, RefreshCw } from 'lucide-svelte';
  // Removed imports for LocationInfo and PositionHistory
  
  let map: LeafletMap | null = $state(null);
  let mapElement: HTMLElement | null = $state(null);
  let leaflet: any;
  let marker: LeafletMarker | null = $state(null);
  let polyline: LeafletPolyline | null = $state(null);
  let mapLoaded = $state(false);
  let showStyleSelector = $state(false);

  let L: any;
  let markerIcon: LeafletIcon;

  function toggleStyleSelector(): void {
    showStyleSelector = !showStyleSelector;
  }
  
  // Close style selector when clicking outside or selecting
  function handleStyleSelection(style: MapStyle): void {
    locationStore.setMapStyle(style);
    showStyleSelector = false; // Ensure it closes
  }

  function handleClickOutsideStyleSelector(event: MouseEvent): void {
    const target = event.target as HTMLElement;
    if (showStyleSelector && !target?.closest('.style-selector-container') && !target?.closest('.style-selector-button')) {
      showStyleSelector = false;
    }
  }


  onMount(async () => {
    leaflet = await import('leaflet');
    L = leaflet.default;
    await import('leaflet/dist/leaflet.css');
    
    markerIcon = new L.Icon({
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

  function initMap(): void {
    if (!mapElement || !L) return;
    
    map = L.map(mapElement).setView([locationStore.currentLocation.lat, locationStore.currentLocation.lng], 15);
    updateMapStyle(locationStore.mapSettings.style);
    
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
    
    polyline = L.polyline(locationStore.trailPoints, {
      color: '#0ea5e9',
      weight: 3,
      opacity: 0.7,
      dashArray: '5, 5'
    }).addTo(map);
    
    L.control.zoom({ position: 'bottomright' }).addTo(map);
  }

  function updateMapStyle(style: MapStyle): void {
    if (!map || !L) return;
    
    map.eachLayer((layer: any) => {
      if (layer instanceof L.TileLayer) {
        if (map) {
          map.removeLayer(layer);
        }
      }
    });
    
    let newLayer: LeafletTileLayer | null = null;
    if (style === 'streets') {
      newLayer = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '© <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
      });
    } else if (style === 'satellite') {
      newLayer = L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
        attribution: '© <a href="https://www.esri.com">Esri</a>'
      });
    } else if (style === 'terrain') {
      newLayer = L.tileLayer('https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png', {
        attribution: '© <a href="https://www.opentopomap.org">OpenTopoMap</a>'
      });
    }
    if (newLayer) newLayer.addTo(map);
  }

  $effect(() => {
    if (map && marker && L) {
      const latLng = L.latLng(locationStore.currentLocation.lat, locationStore.currentLocation.lng);
      marker.setLatLng(latLng);
      marker.getPopup()?.setContent(`
        <div style="padding: 4px">
          <div style="font-weight: 500">Current Location</div>
          <div style="font-size: 12px; color: #666; margin-top: 4px">
            ${locationStore.currentLocation.lat.toFixed(6)}, ${locationStore.currentLocation.lng.toFixed(6)}
          </div>
        </div>
      `);
      if (locationStore.mapSettings.autoCenter) {
        map.setView(latLng);
      }
    }
  });

  $effect(() => {
    if (map && polyline && L && locationStore.trailPoints.length > 0) {
        const latLngs = locationStore.trailPoints.map(p => L.latLng(p[0], p[1]));
        polyline.setLatLngs(latLngs);
    } else if (map && polyline && L && locationStore.trailPoints.length === 0) {
        polyline.setLatLngs([]); // Clear polyline if no points
    }
  });

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

  
  let intervalId = $state<number | null>(null);
  
  // Toggle automatic updates
  function toggleAutomaticUpdates(): void {
    if (locationStore.updateInterval) {
      clearInterval(intervalId as number);
      intervalId = null;
      locationStore.updateInterval = null;
    } else {
      const interval = 3000; // 3 seconds
      intervalId = window.setInterval(() => locationStore.moveMarker(), interval) as unknown as number;
      locationStore.updateInterval = interval;
    }
  }
  
  // Clean up interval on component destruction
  onDestroy(() => {
    if (intervalId) {
      clearInterval(intervalId);
    }
  });


</script>

<svelte:window onclick={handleClickOutsideStyleSelector} />

<div class="flex flex-col h-full">
  <!-- Top Controls for the map -->
  <div class="flex justify-between items-center mb-2 relative z-30 style-selector-container">
    <button 
      onclick={toggleStyleSelector}
      class="style-selector-button flex items-center gap-1.5 h-9 px-4 py-2 bg-white rounded-md text-sm font-medium border shadow-sm hover:bg-gray-50"
    >
      <Layers class="h-4 w-4" />
      {#if locationStore.mapSettings.style === 'streets'}Streets
      {:else if locationStore.mapSettings.style === 'satellite'}Satellite
      {:else}Terrain{/if}
      <ChevronDown class="h-3 w-3 opacity-50 ml-1" />
    </button>

    {#if showStyleSelector}
      <div class="absolute top-full left-0 z-40 bg-white shadow-md rounded-md overflow-hidden mt-1 w-36">
        <div class="flex flex-col divide-y">
          <button 
            class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'streets' ? 'bg-gray-100' : ''}"
            onclick={() => handleStyleSelection('streets')}
          >
            <MapPin class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'streets' ? 'text-primary' : ''}" />
            <span class="text-sm">Streets</span>
          </button>
          <button 
            class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'satellite' ? 'bg-gray-100' : ''}"
            onclick={() => handleStyleSelection('satellite')}
          >
            <Layers class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'satellite' ? 'text-primary' : ''}" />
            <span class="text-sm">Satellite</span>
          </button>
          <button 
            class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'terrain' ? 'bg-gray-100' : ''}"
            onclick={() => handleStyleSelection('terrain')}
          >
            <Navigation class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'terrain' ? 'text-primary' : ''}" />
            <span class="text-sm">Terrain</span>
          </button>
        </div>
      </div>
    {/if}


  
  <div class="flex gap-2">
    <button 
      onclick={() => locationStore.resetHistory()}
      class="h-8 px-3 text-xs bg-background rounded-md font-medium border border-input shadow-sm hover:bg-accent hover:text-accent-foreground"
    >
      Reset History
    </button>
    
    <button 
      onclick={() => locationStore.moveMarker()}
      class="h-8 px-3 text-xs bg-primary text-primary-foreground rounded-md font-medium shadow hover:bg-primary/90"
    >
      Simulate Movement
    </button>
    
    <button 
      onclick={toggleAutomaticUpdates}
      class="h-8 px-3 text-xs flex items-center gap-1.5 rounded-md font-medium shadow {locationStore.updateInterval ? 'bg-destructive text-destructive-foreground hover:bg-destructive/90' : 'bg-primary text-primary-foreground hover:bg-primary/90'}"
    >
      <RefreshCw class="h-3 w-3 {locationStore.updateInterval ? 'animate-spin' : ''}" />
      {locationStore.updateInterval ? 'Stop Updates' : 'Auto Update'}
    </button>
  </div>

    <button 
      onclick={locationStore.toggleAutoCenter}
      class="flex items-center gap-1.5 h-9 px-4 py-2 bg-white rounded-md text-sm font-medium border shadow-sm hover:bg-gray-50 {locationStore.mapSettings.autoCenter ? 'bg-primary/10' : ''}"
    >
      <LocateFixed class="h-4 w-4 {locationStore.mapSettings.autoCenter ? 'text-primary' : ''}" />
      Auto-center
    </button>
  </div>
  
  <!-- Map container -->
  <div class="relative h-[450px] md:h-[500px] rounded-lg overflow-hidden border flex-grow">
    <div bind:this={mapElement} class="h-full w-full absolute inset-0 z-10">
        {#if !mapLoaded}Loading Map...{/if}
    </div>
  </div>
</div>