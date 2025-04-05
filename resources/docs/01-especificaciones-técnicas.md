# Especificaciones Técnicas del Proyecto

## Tecnología [[000-Knowledge/CS/network/LoRa]]

  

### ¿Qué es LoRa?

  

LoRa (Long Range) es una tecnología de comunicación inalámbrica de área amplia y bajo consumo energético desarrollada por Semtech. Sus características fundamentales incluyen:

  

- **Largo alcance**: De 2 a 5 km en entornos urbanos y hasta 15 km en línea de vista directa

- **Bajo consumo**: Diseñada para dispositivos alimentados por baterías con años de autonomía

- **Resistencia a interferencias**: Alta inmunidad al ruido y a las interferencias gracias a la modulación CSS

- **Capacidad de penetración**: Buena penetración en edificios y obstáculos, especialmente en frecuencias más bajas como 433MHz

  

### Modulación CSS (Chirp Spread Spectrum)

  

LoRa utiliza una modulación patentada conocida como Chirp Spread Spectrum (CSS) que:

  

- Esparce la señal a lo largo de un ancho de banda más amplio

- Transmite "chirps" (señales que varían en frecuencia con el tiempo)

- Ofrece mayor resistencia a interferencias y desvanecimiento

- Permite decodificación de señales incluso por debajo del nivel de ruido (-20dB)

  

### Diferencia entre LoRa y LoRaWAN

  

Es importante distinguir entre ambos conceptos:

  

- **LoRa**: Es la capa física (PHY) que define el tipo de modulación utilizada

- **LoRaWAN**: Es un protocolo de comunicación y arquitectura de red construido sobre LoRa

  

Para este proyecto inicial, utilizaremos comunicación LoRa punto a punto, dejando la implementación completa de LoRaWAN para futuras fases.

  

## Especificaciones de Hardware

  

### Módulo LoRa

  

Trabajaremos con módulos basados en el chip **SX1276** de Semtech, con las siguientes características:

  

- **Frecuencia**: 433MHz (banda ISM libre en nuestra región)

- **Potencia de transmisión**: Configurable hasta +20dBm (100mW)

- **Sensibilidad del receptor**: Hasta -148dBm (dependiendo de la configuración)

- **Factor de dispersión (SF)**: Configurable entre SF7 y SF12

- **Ancho de banda**: Seleccionable (125kHz, 250kHz, 500kHz)

- **Tasa de datos**: Desde 0.3 kbps hasta 37.5 kbps

  

### Módulo GPS

  

Se utilizará un módulo GPS de bajo consumo con las siguientes características:

  

- **Chip recomendado**: u-blox NEO-6M, NEO-7M o similar

- **Sensibilidad**: -160dBm o mejor para tracking

- **Precisión**: CEP (50%) de 2.5m o mejor

- **Tiempo de adquisición**: TTFF (tiempo para primera fijación) < 30s

- **Interfaces**: UART/I2C para integración con microcontrolador

- **Consumo**: Modo activo < 50mA, modo standby < 10mA

  

### Plataforma de Desarrollo

  

El prototipo se implementará utilizando:

  

- **Microcontrolador**: Arduino/ESP32 para facilitar el desarrollo inicial

- **Alimentación**: Sistema de baterías LiPo 3.7V con circuito de carga

- **Interfaz**: Conexiones UART/SPI para módulos GPS y LoRa

- **Indicadores**: LEDs para estado de operación y conexión

  

### Consideraciones para la Antena

  

Para alcanzar los 5km requeridos, la antena es un componente crítico:

  

- **Tipo**: Inicialmente se evaluarán antenas monopolo 1/4λ y dipolo 1/2λ

- **Ganancia objetivo**: Mínimo 3dBi, preferentemente 5-6dBi

- **Impedancia**: 50 ohm (estándar para estos sistemas)

- **Polarización**: Vertical para este caso de uso

- **Longitud física** (para dipolo a 433MHz): Aproximadamente 34.6cm (λ/2)

- **Material**: Cobre o aluminio para el elemento radiante

- **Conector**: SMA o similar para conexión al módulo LoRa

  

## Cálculos de Link Budget

  

Para verificar la viabilidad de la comunicación a 5km, se considera:

  

1. **Potencia de transmisión**: +20dBm (máxima del SX1276)

2. **Ganancia de antena transmisora**: +5dBi (estimada)

3. **Pérdidas de propagación** (modelo simplificado para 433MHz a 5km): -125dB

4. **Ganancia de antena receptora**: +5dBi (estimada)

5. **Sensibilidad del receptor**: -148dBm (con SF12, BW 125kHz)

  

**Cálculo simplificado**:

```

Potencia Recibida = Potencia TX + Ganancia TX - Pérdidas + Ganancia RX

Potencia Recibida = 20dBm + 5dBi - 125dB + 5dBi = -95dBm

```

  

Con un margen de enlace de 53dB (-95dBm vs -148dBm de sensibilidad), el sistema debería ser capaz de alcanzar los 5km requeridos, incluso con factores adicionales de pérdida como obstáculos o condiciones atmosféricas adversas.

  

## Software y Protocolos

  

### Estructura de Datos

  

El paquete básico de datos a transmitir incluirá:

  

- **Identificador del dispositivo**: 1-4 bytes

- **Coordenadas GPS**: Latitud y longitud (8 bytes)

- **Timestamp**: Marca de tiempo UTC (4 bytes)

- **Estado de batería**: Nivel en porcentaje (1 byte)

- **Flags de estado**: Para indicar condiciones especiales (1 byte)

- **Checksum**: Verificación de integridad (2 bytes)

  

### Protocolo de Comunicación

  

Se implementará un protocolo simple con las siguientes características:

  

- **Modo de comunicación**: Semi-dúplex, principalmente unidireccional (tracker hacia base)

- **Intervalos de transmisión**: Configurable entre 10 segundos y 10 minutos

- **Confirmación**: Inicialmente sin confirmación (para máxima autonomía)

- **Encriptación**: Básica para primeras fases (XOR o similar)

  

### Consideraciones de Firmware

  

El firmware del tracker implementará:

  

- **Ciclos de sueño**: Modo de bajo consumo entre transmisiones

- **Adaptación dinámica**: Ajuste automático de parámetros según condiciones

- **Almacenamiento local**: Buffer para puntos GPS en caso de pérdida de conexión

- **Detección de movimiento**: Posible uso de acelerómetro para transmisión basada en eventos