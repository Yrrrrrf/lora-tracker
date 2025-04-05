<!-- Main Page Component -->
<script lang="ts">
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { LocateFixed, Maximize2 } from 'lucide-svelte';
  import MapView from '../components/MapView.svelte';
  import TripSummary from '../components/TripSummary.svelte';
  import CurrentLocationDetails from '../components/CurrentLocationDetails.svelte';
  import CompletePositionLog from '../components/CompletePositionLog.svelte';
  import Controls from '../components/Controls.svelte';
  
  let activeTab = $state('map');
</script>

<div 
  class="transition-all duration-300 {locationStore.mapSettings.isFullscreen ? 'fixed inset-0 z-50 bg-background p-4' : 'w-full max-w-5xl mx-auto bg-card rounded-xl shadow-lg'}"
>
  <div class="border-0 shadow-none h-full flex flex-col">
    <!-- Header -->
    <div class="p-4 pb-2">
      <div class="flex items-center justify-between">
        <div class="flex items-center gap-2">
          <div class="bg-primary/10 p-2 rounded-full">
            <LocateFixed class="h-5 w-5 text-primary" />
          </div>
          <div>
            <h1 class="text-xl font-semibold">LoRa GPS Tracker</h1>
            <p class="text-sm text-muted-foreground">Real-time location monitoring</p>
          </div>
        </div>
        
        <div>
          <button
            onclick={() => locationStore.toggleFullscreen()}
            class="h-9 w-9 rounded-md border border-input bg-background p-0 shadow-sm hover:bg-accent hover:text-accent-foreground"
            aria-label="Toggle fullscreen"
          >
            <Maximize2 class="h-4 w-4 m-auto" />
          </button>
        </div>
      </div>
    </div>

    <!-- Tabs -->
    <div class="flex-grow p-0 flex flex-col">
      <div class="border-b">
        <div class="flex px-4">
          <button
            class="px-4 py-2 border-b-2 text-sm font-medium {activeTab === 'map' ? 'border-primary text-primary' : 'border-transparent hover:text-primary'}"
            onclick={() => activeTab = 'map'}
          >
            Map View
          </button>
          <button
            class="px-4 py-2 border-b-2 text-sm font-medium {activeTab === 'data' ? 'border-primary text-primary' : 'border-transparent hover:text-primary'}"
            onclick={() => activeTab = 'data'}
          >
            Data Analysis
          </button>
        </div>
      </div>

      <!-- Tab Content -->
      <div class="flex-grow p-4">
        {#if activeTab === 'map'}
          <div class="block w-full">
            <MapView />
          </div>
        {:else if activeTab === 'data'}
          <div class="block w-full">
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4 mb-4">
              <TripSummary />
              <CurrentLocationDetails />
            </div>
            <CompletePositionLog />
          </div>
        {/if}
      </div>
    </div>

    <!-- Footer -->
    <div class="p-4">
      <Controls />
    </div>
  </div>
</div>