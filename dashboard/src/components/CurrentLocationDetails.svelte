<!-- CurrentLocationDetails.svelte -->
<script>
  import { locationStore } from '$lib/stores/locationStore.svelte';
</script>

<div class="border rounded-lg shadow-sm p-4">
  <h3 class="text-base font-semibold mb-3">Current Location Details</h3>
  
  <div class="space-y-2">
    <div>
      <div class="text-sm font-medium text-muted-foreground">Coordinates</div>
      <div class="font-mono text-sm">
        {locationStore.currentLocation.lat.toFixed(6)}, {locationStore.currentLocation.lng.toFixed(6)}
      </div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Altitude</div>
      <div class="font-mono text-sm">
        {locationStore.currentLocation.altitude 
          ? `${locationStore.currentLocation.altitude.toFixed(1)} meters` 
          : 'N/A'}
      </div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Accuracy</div>
      <div class="font-mono text-sm">
        {locationStore.currentLocation.accuracy 
          ? `±${locationStore.currentLocation.accuracy.toFixed(1)} meters` 
          : 'N/A'}
      </div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Last Updated</div>
      <div class="font-mono text-sm">{locationStore.formatTime(locationStore.currentLocation.timestamp)}</div>
    </div>
  </div>
</div>