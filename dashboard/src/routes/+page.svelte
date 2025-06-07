<!-- dashboard/src/routes/+page.svelte -->
<script lang="ts">
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { LocateFixed } from 'lucide-svelte';
  import MapView from '../components/MapView.svelte';
  import LocationInfo from '../components/LocationInfo.svelte';
  import TripSummary from '../components/TripSummary.svelte';
  import CompletePositionLog from '../components/CompletePositionLog.svelte';
</script>

<div class="w-full h-screen bg-background flex flex-col p-2 sm:p-4 overflow-hidden">
  <div class="w-full bg-card rounded-xl shadow-lg flex flex-col flex-grow overflow-hidden">
    <div class="p-4 pb-3 border-b">
      <div class="flex items-center justify-between">
        <div class="flex items-center gap-3">
          <div class="bg-primary/10 p-2.5 rounded-full">
            <LocateFixed class="h-5 w-5 sm:h-6 sm:w-6 text-primary" />
          </div>
          <div>
            <h1 class="text-lg sm:text-xl font-semibold">LoRa GPS Tracker</h1>
            <p class="text-xs sm:text-sm text-muted-foreground">Real-time location monitoring</p>
          </div>
        </div>
      </div>
    </div>

    <div class="flex-grow p-4 grid grid-cols-1 lg:grid-cols-3 gap-4 overflow-hidden">
      <div class="lg:col-span-2 flex flex-col min-h-0">
        <div class="flex-grow relative min-h-0">
          <MapView />
        </div>
        <LocationInfo />
      </div>

      <div class="lg:col-span-1 flex flex-col gap-4 min-h-0">
        <TripSummary />
        <div class="flex-grow min-h-0 overflow-hidden">
          <CompletePositionLog />
        </div>
      </div>
    </div>
  </div>
</div>