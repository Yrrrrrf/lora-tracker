<!-- LocationInfo.svelte -->
<script>
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { Navigation, Clock, LocateFixed } from 'lucide-svelte';
</script>

<div class="grid grid-cols-2 sm:grid-cols-4 gap-2 mt-2">
  <!-- Coordinates -->
  <div class="bg-muted/40 p-3 rounded-md border shadow-sm">
    <div class="flex items-center gap-2">
      <Navigation class="h-4 w-4 text-primary" />
      <div>
        <div class="text-xs text-muted-foreground">Coordinates</div>
        <div class="font-mono text-xs truncate">
          {locationStore.currentLocation.lat.toFixed(6)}, {locationStore.currentLocation.lng.toFixed(6)}
        </div>
      </div>
    </div>
  </div>

  <!-- Last Update -->
  <div class="bg-muted/40 p-3 rounded-md border shadow-sm">
    <div class="flex items-center gap-2">
      <Clock class="h-4 w-4 text-primary" />
      <div>
        <div class="text-xs text-muted-foreground">Last Update</div>
        <div class="font-mono text-xs">
          {locationStore.formatTime(locationStore.currentLocation.timestamp)}
        </div>
      </div>
    </div>
  </div>

  <!-- Altitude -->
  <div class="bg-muted/40 p-3 rounded-md border shadow-sm">
    <div class="flex items-center gap-2">
      <LocateFixed class="h-4 w-4 text-primary" />
      <div>
        <div class="text-xs text-muted-foreground">Altitude</div>
        <div class="font-mono text-xs">
          {locationStore.currentLocation.altitude 
            ? `${locationStore.currentLocation.altitude.toFixed(1)} m` 
            : 'N/A'}
        </div>
      </div>
    </div>
  </div>

  <!-- Speed -->
  <div class="bg-muted/40 p-3 rounded-md border shadow-sm">
    <div class="flex items-center gap-2">
      <Navigation class="h-4 w-4 text-primary" />
      <div>
        <div class="text-xs text-muted-foreground">Speed</div>
        <div class="font-mono text-xs">
          {locationStore.currentLocation.speed !== undefined 
            ? `${locationStore.currentLocation.speed.toFixed(1)} m/s` 
            : 'N/A'}
        </div>
      </div>
    </div>
  </div>
</div>