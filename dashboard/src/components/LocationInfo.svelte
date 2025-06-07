<!-- dashboard/src/components/LocationInfo.svelte -->
<script lang="ts">
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { Navigation, MapPin, TrendingUp, ShieldCheck } from 'lucide-svelte'; // Changed icons
</script>

<div class="grid grid-cols-2 sm:grid-cols-4 gap-3 mt-3">
  <!-- Coordinates -->
  <div class="bg-muted/40 p-3 rounded-md border shadow-sm">
    <div class="flex items-center gap-2">
      <MapPin class="h-4 w-4 text-primary" />
      <div>
        <div class="text-xs text-muted-foreground">Coordinates</div>
        <div class="font-mono text-sm truncate">
          {locationStore.currentLocation.lat.toFixed(6)}, {locationStore.currentLocation.lng.toFixed(6)}
        </div>
      </div>
    </div>
  </div>

  <!-- Altitude -->
  <div class="bg-muted/40 p-3 rounded-md border shadow-sm">
    <div class="flex items-center gap-2">
      <TrendingUp class="h-4 w-4 text-primary" />
      <div>
        <div class="text-xs text-muted-foreground">Altitude</div>
        <div class="font-mono text-sm">
          {locationStore.currentLocation.altitude 
            ? `${locationStore.currentLocation.altitude.toFixed(1)} m` 
            : 'N/A'}
        </div>
      </div>
    </div>
  </div>
  
  <!-- Accuracy -->
  <div class="bg-muted/40 p-3 rounded-md border shadow-sm">
    <div class="flex items-center gap-2">
      <ShieldCheck class="h-4 w-4 text-primary" />
      <div>
        <div class="text-xs text-muted-foreground">Accuracy</div>
        <div class="font-mono text-sm">
          {locationStore.currentLocation.accuracy 
            ? `±${locationStore.currentLocation.accuracy.toFixed(1)} m` 
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
        <div class="font-mono text-sm">
          {locationStore.currentLocation.speed !== undefined 
            ? `${locationStore.currentLocation.speed.toFixed(1)} m/s` 
            : 'N/A'}
        </div>
      </div>
    </div>
  </div>
</div>