<!-- CompletePositionLog.svelte -->
<script>
  import { locationStore } from '$lib/stores/locationStore.svelte';
</script>

<div class="border rounded-lg shadow-sm">
  <div class="p-3 border-b">
    <h3 class="text-base font-semibold">Complete Position Log</h3>
  </div>
  
  <div class="overflow-auto max-h-[300px]">
    <table class="w-full">
      <thead class="bg-muted/50 sticky top-0">
        <tr>
          <th class="px-4 py-2 text-left text-xs font-medium">Time</th>
          <th class="px-4 py-2 text-left text-xs font-medium">Latitude</th>
          <th class="px-4 py-2 text-left text-xs font-medium">Longitude</th>
          <th class="px-4 py-2 text-left text-xs font-medium">Altitude</th>
          <th class="px-4 py-2 text-left text-xs font-medium">Speed</th>
          <th class="px-4 py-2 text-left text-xs font-medium">Accuracy</th>
        </tr>
      </thead>
      <tbody>
        {#each [...locationStore.positionHistory].reverse() as pos, idx}
          <tr class={idx === 0 ? 'bg-primary/5' : 'border-t border-muted'}>
            <td class="font-mono text-xs py-2 px-4">{locationStore.formatTime(pos.timestamp)}</td>
            <td class="font-mono text-xs py-2 px-4">{pos.lat.toFixed(6)}</td>
            <td class="font-mono text-xs py-2 px-4">{pos.lng.toFixed(6)}</td>
            <td class="font-mono text-xs py-2 px-4">{pos.altitude?.toFixed(1) || 'N/A'}</td>
            <td class="font-mono text-xs py-2 px-4">{pos.speed?.toFixed(1) || 'N/A'}</td>
            <td class="font-mono text-xs py-2 px-4">{pos.accuracy?.toFixed(1) || 'N/A'}</td>
          </tr>
        {/each}
      </tbody>
    </table>
  </div>
</div>