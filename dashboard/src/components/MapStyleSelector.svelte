<!-- MapStyleSelector.svelte -->
<script lang="ts">
    import { locationStore } from '$lib/stores/locationStore.svelte';
    import { Layers, MapPin, Navigation } from 'lucide-svelte';
    import type { MapStyle } from '$lib/types';

    // Handle style selection
    function selectStyle(style: MapStyle): void {
        locationStore.setMapStyle(style);
    }
</script>

<div class="absolute top-20 left-3 z-30 bg-white shadow-md rounded-md overflow-hidden">
    <div class="flex flex-col divide-y">
        <button 
        class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'streets' ? 'bg-gray-100' : ''}"
        onclick={() => selectStyle('streets')}
        >
        <MapPin class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'streets' ? 'text-primary' : ''}" />
        <span class="text-sm">Streets</span>
        </button>
        
        <button 
        class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'satellite' ? 'bg-gray-100' : ''}"
        onclick={() => selectStyle('satellite')}
        >
        <Layers class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'satellite' ? 'text-primary' : ''}" />
        <span class="text-sm">Satellite</span>
        </button>
        
        <button 
        class="flex items-center px-4 py-2 hover:bg-gray-100 {locationStore.mapSettings.style === 'terrain' ? 'bg-gray-100' : ''}"
        onclick={() => selectStyle('terrain')}
        >
        <Navigation class="h-4 w-4 mr-2 {locationStore.mapSettings.style === 'terrain' ? 'text-primary' : ''}" />
        <span class="text-sm">Terrain</span>
        </button>
    </div>
</div>