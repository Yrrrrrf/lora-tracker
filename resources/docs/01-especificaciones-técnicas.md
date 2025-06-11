# Especificaciones Técnicas del Proyecto

## Arquitectura General del Sistema

El sistema se estructura en una arquitectura de tres capas claramente definidas que abarcan desde la captura de datos en campo hasta su visualización en una interfaz web.

*   **Dispositivo Tracker (Capa de Adquisición)**: Es un nodo móvil basado en Arduino, encargado de obtener coordenadas GPS y datos de sensores ambientales. Su única función es empaquetar y transmitir esta información de forma inalámbrica.
*   **Estación Base (Capa de Comunicación y Puente)**: Consiste en un nodo receptor LoRa (Arduino) conectado vía USB a un ordenador. Un script en Python actúa como puente, recibiendo los datos del puerto serie y reenviándolos a la aplicación web mediante una petición HTTP.
*   **Dashboard (Capa de Presentación)**: Una aplicación web moderna construida con SvelteKit. Provee una API para recibir los datos y una interfaz de usuario rica para la visualización en tiempo real del tracker sobre un mapa, junto con métricas y logs históricos.

## Flujo de Datos Completo

El recorrido de la información desde su origen hasta el usuario final sigue el siguiente flujo:

1.  **Captura (Tracker)**: El microcontrolador del tracker lee los datos del módulo GPS (`TinyGPSPlus`) y sensores (`DHT`).
2.  **Serialización (Tracker)**: Los datos se formatean en una cadena de texto con formato **JSON**. Ejemplo: `{"lat":19.4326,"lng":-99.1332,"alt":2250.0}`.
3.  **Transmisión LoRa (Tracker -> Base)**: La cadena JSON se envía como carga útil (payload) de un paquete LoRa utilizando el módulo `LoRaRadio`. La comunicación es punto a punto y unidireccional.
4.  **Recepción LoRa (Base)**: La estación base, en modo de escucha continua, recibe el paquete LoRa.
5.  **Puente a Serie (Base)**: El firmware del receptor extrae la carga útil (la cadena JSON) y la imprime directamente en su puerto serie USB.
6.  **Puente a Web (Python Script)**: El script `dashboard/python/main.py` lee la línea del puerto serie, la decodifica y realiza una petición `HTTP POST` al endpoint de la API del dashboard.
7.  **Ingesta de Datos (API SvelteKit)**: El endpoint `/api/location` recibe la petición POST, valida el JSON y almacena la nueva posición en una variable en memoria del servidor.
8.  **Visualización (Frontend Svelte)**: La interfaz de usuario del dashboard realiza peticiones `HTTP GET` periódicas al mismo endpoint para obtener la última posición y el historial. Los componentes de Svelte 5 (Runes) se actualizan reactivamente para mostrar la nueva posición en el mapa y en las tablas de datos.

## Especificaciones del Hardware y Firmware

### Módulo Tracker (Transmisor)

*   **Microcontrolador**: Arduino UNO R3 o compatible.
*   **Módulo LoRa**: Basado en el chip SX1276, operando en la frecuencia de **433MHz**. Gestionado a través del módulo de software `LoRaRadio`.
*   **Módulo GPS**: NEO-6M, comunicándose vía `SoftwareSerial` y procesado con la librería `TinyGPSPlus`.
*   **Librerías Clave**:
    *   `sandeepmistry/LoRa`: Para la comunicación LoRa.
    *   `mikalhart/TinyGPSPlus`: Para el parseo de sentencias NMEA del GPS.
    *   `adafruit/DHT sensor library`: Para el sensor de temperatura y humedad.
    *   `fmalpartida/LiquidCrystal`: Para la pantalla LCD opcional.
*   **Firmware**: El código se encuentra en `lora-cpp/src/main.cpp` y utiliza una arquitectura modular con las clases `Gps`, `LcdDisplay`, `EnvSensor` y `LoRaRadio`.

### Módulo Estación Base (Receptor)

*   **Microcontrolador**: Arduino UNO R3 o compatible.
*   **Módulo LoRa**: Configurado con los mismos parámetros de radio (frecuencia, etc.) que el tracker para asegurar la comunicación.
*   **Firmware**: El código se encuentra en `lora-cpp/src/main-receiver.cpp`. Su lógica es simple: escuchar paquetes LoRa y reenviar la carga útil recibida al puerto `Serial`.

## Especificaciones del Software

### Puente Serie-Web (Python)

*   **Ubicación**: `dashboard/python/main.py`
*   **Dependencias**: `pyserial` para la comunicación con el puerto serie y `requests` para las peticiones HTTP.
*   **Función**: Actúa como un servicio desacoplado que traduce los datos del dominio del hardware (serie) al dominio web (HTTP), permitiendo que el dashboard sea agnóstico al hardware.

### Dashboard (SvelteKit)

*   **Framework**: **Svelte 5** (con Runes) y **SvelteKit**.
*   **Lenguaje**: **TypeScript**, asegurando tipado estricto.
*   **Estilos**: **TailwindCSS** para un diseño basado en utilidades.
*   **Componentes Clave**:
    *   `MapView.svelte`: Renderiza el mapa interactivo con **Leaflet.js**.
    *   `CompletePositionLog.svelte`: Muestra el historial completo de posiciones.
    *   `TripSummary.svelte`: Presenta métricas clave del viaje.
*   **Gestión de Estado**: Se centraliza en `dashboard/src/lib/stores/locationStore.svelte.ts`, utilizando el sistema de reactividad de Svelte 5 para una gestión eficiente y declarativa del estado de la aplicación.
*   **API Backend**:
    *   `POST /api/location`: Recibe datos del puente Python.
    *   `GET /api/location`: Sirve la última posición y el historial al frontend.

## Protocolo de Comunicación y Estructura de Datos

### Paquete LoRa

Se utiliza un formato de paquete simple definido en el módulo `LoRaRadio` para el direccionamiento:

| Campo             | Tamaño (bytes) | Descripción                                  |
| ----------------- | -------------- | -------------------------------------------- |
| Dirección Destino | 1              | Dirección del nodo receptor (e.g., `0xAA`).    |
| Dirección Origen  | 1              | Dirección del nodo emisor (e.g., `0xBB`).      |
| ID del Mensaje    | 1              | Contador de mensajes, se incrementa en cada envío. |
| Longitud Payload  | 1              | Tamaño en bytes de la carga útil.            |
| Payload (Carga Útil) | Variable       | La cadena de datos en formato JSON.          |

### Estructura de Datos (Payload)

La carga útil del paquete LoRa es una cadena de texto en formato **JSON**, que ha demostrado ser flexible y fácilmente parseable tanto en el backend como en el firmware de prueba.

*   **Campos Obligatorios**:
    *   `lat`: Latitud (número de punto flotante).
    *   `lng`: Longitud (número de punto flotante).
*   **Campos Opcionales**:
    *   `alt`: Altitud en metros.
    *   `spd`: Velocidad en m/s.
    *   `acc_hdop`: Precisión horizontal (HDOP).
