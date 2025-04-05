<!-- Controls.svelte -->
<script lang="ts">
  import { onDestroy } from 'svelte';
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { RefreshCw } from 'lucide-svelte';
  
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

<div class="flex justify-between items-center">
  <div class="text-xs text-muted-foreground">
    Last updated: {locationStore.formatTime(locationStore.currentLocation.timestamp)}
  </div>
  
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
</div>