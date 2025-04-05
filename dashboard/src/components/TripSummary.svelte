<!-- TripSummary.svelte -->
<script>
  import { locationStore } from '$lib/stores/locationStore.svelte';
</script>

<div class="border rounded-lg shadow-sm p-4">
  <h3 class="text-base font-semibold mb-3">Trip Summary</h3>
  
  <div class="grid grid-cols-2 gap-4">
    <div>
      <div class="text-sm font-medium text-muted-foreground">Total Distance</div>
      <div class="text-2xl font-bold">{(locationStore.totalDistance / 1000).toFixed(2)} km</div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Data Points</div>
      <div class="text-2xl font-bold">{locationStore.positionHistory.length}</div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Start Time</div>
      <div class="text-sm">
        {locationStore.positionHistory.length > 0 
          ? locationStore.formatTime(locationStore.positionHistory[0].timestamp) 
          : 'N/A'}
      </div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Latest Time</div>
      <div class="text-sm">{locationStore.formatTime(locationStore.currentLocation.timestamp)}</div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Date</div>
      <div class="text-sm">{locationStore.formatDate(locationStore.currentLocation.timestamp)}</div>
    </div>
    
    <div>
      <div class="text-sm font-medium text-muted-foreground">Current Speed</div>
      <div class="text-sm">
        {locationStore.currentLocation.speed !== undefined 
          ? `${locationStore.currentLocation.speed.toFixed(1)} m/s` 
          : 'N/A'}
      </div>
    </div>
  </div>
</div>