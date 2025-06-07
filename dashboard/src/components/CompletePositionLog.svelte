<!-- dashboard/src/components/CompletePositionLog.svelte -->
<script lang="ts">
  import { locationStore } from '$lib/stores/locationStore.svelte';
</script>

<div class="border rounded-lg shadow-sm h-full flex flex-col bg-card">
  <div class="p-3 border-b">
    <h3 class="text-base font-semibold">Complete Position Log</h3>
  </div>
  
  <div class="flex-grow overflow-y-auto overflow-x-hidden">
    <table class="w-full text-left table-fixed">
      <colgroup>
        <col style="width: 22%;" />
        <col style="width: 18%;" />
        <col style="width: 18%;" />
        <col style="width: 14%;" />
        <col style="width: 14%;" />
        <col style="width: 14%;" />
      </colgroup>
      <thead class="bg-muted/50 sticky top-0 z-10">
        <tr>
          <th class="px-2 py-2.5 text-sm font-medium text-muted-foreground truncate">Time</th>
          <th class="px-2 py-2.5 text-sm font-medium text-muted-foreground truncate">Latitude</th>
          <th class="px-2 py-2.5 text-sm font-medium text-muted-foreground truncate">Longitude</th>
          <th class="px-2 py-2.5 text-sm font-medium text-muted-foreground truncate">Altitude (m)</th>
          <th class="px-2 py-2.5 text-sm font-medium text-muted-foreground truncate">Accuracy (m)</th>
          <th class="px-2 py-2.5 text-sm font-medium text-muted-foreground truncate">Speed (m/s)</th>
        </tr>
      </thead>
      <tbody>
        {#if locationStore.positionHistory.length === 0}
          <tr>
            <td colspan="6" class="text-center py-10 text-base text-muted-foreground">
              No position data yet. Start simulation or connect device.
            </td>
          </tr>
        {:else}
          {#each [...locationStore.positionHistory].reverse() as pos, idx}
            {@const isLatest = idx === 0}
            <tr class="{isLatest ? 'bg-primary/10 border-l-4 border-l-primary' : 'border-t border-muted/50'} transition-colors duration-300">
              <td class="font-mono {isLatest ? 'text-base py-3.5' : 'text-sm py-3'} px-2 whitespace-nowrap">
                <div class="flex items-center gap-2">
                  {#if isLatest}
                    <span class="relative flex h-3 w-3" title="Latest Reading"> 
                      <span class="animate-ping absolute inline-flex h-full w-full rounded-full bg-primary opacity-75"></span>
                      <span class="relative inline-flex rounded-full h-3 w-3 bg-primary"></span>
                    </span>
                  {/if}
                  <span>{locationStore.formatTime(pos.timestamp)}</span>
                </div>
              </td>
              <td class="font-mono {isLatest ? 'text-base py-3.5' : 'text-sm py-3'} px-2 truncate" title={pos.lat.toString()}>{pos.lat.toFixed(6)}</td>
              <td class="font-mono {isLatest ? 'text-base py-3.5' : 'text-sm py-3'} px-2 truncate" title={pos.lng.toString()}>{pos.lng.toFixed(6)}</td>
              <td class="font-mono {isLatest ? 'text-base py-3.5' : 'text-sm py-3'} px-2 truncate">{pos.altitude?.toFixed(1) ?? 'N/A'}</td>
              <td class="font-mono {isLatest ? 'text-base py-3.5' : 'text-sm py-3'} px-2 truncate">{pos.accuracy?.toFixed(1) ?? 'N/A'}</td>
              <td class="font-mono {isLatest ? 'text-base py-3.5' : 'text-sm py-3'} px-2 truncate">{pos.speed?.toFixed(1) ?? 'N/A'}</td>
            </tr>
          {/each}
        {/if}
      </tbody>
    </table>
  </div>
</div>