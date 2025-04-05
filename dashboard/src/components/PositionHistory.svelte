<!-- PositionHistory.svelte -->
<script>
  import { locationStore } from '$lib/stores/locationStore.svelte';
  import { History } from 'lucide-svelte';
</script>

<div class="h-full flex flex-col border rounded-lg shadow-sm">
  <div class="p-3 border-b flex items-center justify-between">
    <div class="flex items-center gap-2">
      <History class="h-4 w-4 text-primary" />
      <h3 class="text-base font-semibold">Position History</h3>
    </div>
    <div class="text-xs py-0.5 px-2 rounded-full bg-muted font-normal">
      {locationStore.positionHistory.length} points
    </div>
  </div>

  <div class="overflow-auto max-h-[400px] h-full">
    <table class="w-full">
      <thead class="bg-muted/50 sticky top-0">
        <tr>
          <th class="w-[100px] px-4 py-2 text-left text-xs font-medium">Time</th>
          <th class="px-4 py-2 text-left text-xs font-medium">Coordinates</th>
        </tr>
      </thead>
      <tbody>
        {#each [...locationStore.positionHistory].reverse() as pos, idx}
          <tr class={idx === 0 ? 'bg-primary/5' : 'border-t border-muted'}>
            <td class="font-mono text-xs py-2 px-4">{locationStore.formatTime(pos.timestamp)}</td>
            <td class="font-mono text-xs py-2 px-4">{pos.lat.toFixed(6)}, {pos.lng.toFixed(6)}</td>
          </tr>
        {/each}
      </tbody>
    </table>
  </div>
</div>