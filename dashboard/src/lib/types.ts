// src/lib/types.ts
import type { Map, Marker, Polyline, Icon, LatLngExpression, TileLayer } from 'leaflet';

export type MapStyle = 'streets' | 'satellite' | 'terrain';

export interface Position {
  lat: number;
  lng: number;
  timestamp: string;
  accuracy?: number;
  altitude?: number;
  speed?: number;
}

// Leaflet type exports
export type {
  Map as LeafletMap,
  Marker as LeafletMarker,
  Polyline as LeafletPolyline,
  Icon as LeafletIcon,
  LatLngExpression,
  TileLayer as LeafletTileLayer
};
