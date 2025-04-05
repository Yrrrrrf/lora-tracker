// This is a fallback declaration file for Leaflet
// It's better to use @types/leaflet, but this can help if that's not working

declare module 'leaflet' {
    export interface MapOptions {
      center?: LatLngExpression;
      zoom?: number;
      // Add other options as needed
    }
  
    export interface LatLng {
      lat: number;
      lng: number;
      // Add other properties as needed
    }
  
    export type LatLngExpression = LatLng | [number, number] | [number, number, number];
  
    export interface LatLngBounds {
      // Add properties as needed
    }
  
    export interface PointExpression {
      x: number;
      y: number;
    }
  
    export interface IconOptions {
      iconUrl: string;
      iconRetinaUrl?: string;
      iconSize?: PointExpression;
      iconAnchor?: PointExpression;
      popupAnchor?: PointExpression;
      shadowUrl?: string;
      shadowSize?: PointExpression;
      shadowAnchor?: PointExpression;
      // Add other options as needed
    }
  
    export interface Icon {
      // Add properties and methods as needed
    }
  
    export interface Marker {
      setLatLng(latlng: LatLngExpression): this;
      getPopup(): Popup | undefined;
      // Add other methods as needed
    }
  
    export interface MarkerOptions {
      icon?: Icon;
      // Add other options as needed
    }
  
    export interface Popup {
      setContent(content: string): this;
      // Add other methods as needed
    }
  
    export interface PopupOptions {
      // Add options as needed
    }
  
    export interface Polyline {
      setLatLngs(latlngs: LatLngExpression[]): this;
      // Add other methods as needed
    }
  
    export interface PolylineOptions {
      color?: string;
      weight?: number;
      opacity?: number;
      dashArray?: string;
      // Add other options as needed
    }
  
    export interface TileLayer {
      // Add properties and methods as needed
    }
  
    export interface TileLayerOptions {
      attribution?: string;
      // Add other options as needed
    }
  
    export interface LayerGroup {
      // Add properties and methods as needed
    }
  
    export interface ControlOptions {
      position?: string;
      // Add other options as needed
    }
  
    export interface Map {
      setView(center: LatLngExpression, zoom?: number): this;
      eachLayer(fn: (layer: any) => void): this;
      removeLayer(layer: any): this;
      addLayer(layer: any): this;
      remove(): this;
      // Add other methods as needed
    }
  
    export class Map {
      constructor(element: HTMLElement | string, options?: MapOptions);
      // Add other constructor signatures as needed
    }
  
    export class Icon {
      constructor(options: IconOptions);
      // Add other constructor signatures as needed
    }
  
    export class Marker {
      constructor(latlng: LatLngExpression, options?: MarkerOptions);
      bindPopup(content: string, options?: PopupOptions): this;
      addTo(map: Map): this;
      // Add other methods as needed
    }
  
    export class Polyline {
      constructor(latlngs: LatLngExpression[], options?: PolylineOptions);
      addTo(map: Map): this;
      // Add other methods as needed
    }
  
    export class TileLayer {
      constructor(urlTemplate: string, options?: TileLayerOptions);
      addTo(map: Map): this;
      // Add other methods as needed
    }
  
    export const control: {
      zoom(options?: ControlOptions): any;
      // Add other factory methods as needed
    };
  
    export default {
      map: (element: HTMLElement | string, options?: MapOptions) => new Map(element, options),
      marker: (latlng: LatLngExpression, options?: MarkerOptions) => new Marker(latlng, options),
      tileLayer: (urlTemplate: string, options?: TileLayerOptions) => new TileLayer(urlTemplate, options),
      polyline: (latlngs: LatLngExpression[], options?: PolylineOptions) => new Polyline(latlngs, options),
      control: control,
      Icon: Icon,
      // Add other exports as needed
    };
  }