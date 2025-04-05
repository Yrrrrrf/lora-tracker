<!-- MapControls.svelte -->
<script lang="ts">
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { Layers, MapPin, Navigation, ChevronDown, LocateFixed } from 'lucide-svelte';
  import type { MapStyle } from '$lib/types';
  
  let isDropdownOpen = $state(false);
  
  function toggleDropdown(): void {
    isDropdownOpen = !isDropdownOpen;
  }

  // Close dropdown when clicking outside
  function handleClickOutside(event: MouseEvent): void {
    const target = event.target as HTMLElement;
    if (isDropdownOpen && !target?.closest('.dropdown-container')) {
      isDropdownOpen = false;
    }
  }

  // Close dropdown when selecting a style
  function selectMapStyle(style: MapStyle): void {
    locationStore.setMapStyle(style);
    isDropdownOpen = false;
  }
</script>

<svelte:window onclick={handleClickOutside} />

<div class="flex justify-between items-center mb-2 relative z-20">
  <!-- Map style dropdown -->
  <div class="dropdown-container relative">
    <button 
      onclick={toggleDropdown}
      class="flex items-center gap-1.5 h-9 px-4 py-2 bg-background rounded-md text-sm font-medium border border-input shadow-sm hover:bg-accent hover:text-accent-foreground"
    >
      <Layers class="h-4 w-4" />
      {#if locationStore.mapSettings.style === 'streets'}
        Streets
      {:else if locationStore.mapSettings.style === 'satellite'}
        Satellite
      {:else}
        Terrain
      {/if}
      <ChevronDown class="h-3 w-3 opacity-50" />
    </button>
    
    {#if isDropdownOpen}
      <div class="absolute top-full left-0 mt-1 w-36 rounded-md border bg-white shadow-md z-50 overflow-hidden">
        <div class="py-1">
          <button 
            class="flex w-full items-center px-3 py-2 text-sm hover:bg-gray-100"
            onclick={() => selectMapStyle('streets')}
          >
            <MapPin class="h-4 w-4 mr-2" />
            Streets
          </button>
          <button 
            class="flex w-full items-center px-3 py-2 text-sm hover:bg-gray-100"
            onclick={() => selectMapStyle('satellite')}
          >
            <Layers class="h-4 w-4 mr-2" />
            Satellite
          </button>
          <button 
            class="flex w-full items-center px-3 py-2 text-sm hover:bg-gray-100"
            onclick={() => selectMapStyle('terrain')}
          >
            <Navigation class="h-4 w-4 mr-2" />
            Terrain
          </button>
        </div>
      </div>
    {/if}
  </div>

  <!-- Auto-center toggle -->
  <button 
    onclick={locationStore.toggleAutoCenter}
    class="flex items-center gap-1.5 h-9 px-4 py-2 bg-background rounded-md text-sm font-medium border border-input shadow-sm hover:bg-accent hover:text-accent-foreground {locationStore.mapSettings.autoCenter ? 'bg-primary/10' : ''}"
  >
    <LocateFixed class="h-4 w-4 mr-2 {locationStore.mapSettings.autoCenter ? 'text-primary' : ''}" />
    Auto-center
  </button>
</div>