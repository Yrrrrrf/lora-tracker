## Bases Teóricas de la Comunicación de Datos

  ### Conceptos Analógico vs Digital
- **Señales analógicas**: Son continuas y representan datos como ondas que varían continuamente.
- **Señales digitales**: Son discretas y representan datos como pulsos distintos (0s y 1s).
- **Importancia**: LoRa utiliza técnicas digitales sobre un medio analógico, por lo que entender ambos conceptos es fundamental. 

### Análisis de Fourier y Espectro
- **Principio clave**: Cualquier señal puede descomponerse en sumas de ondas senoidales de diferentes frecuencias.
- **Aplicación práctica**: Te permite entender cómo se comportará tu señal LoRa en el espectro de 433MHz y qué ancho de banda ocupará.
- **Impacto en el diseño**: Determina la separación necesaria entre canales y ayuda a evitar interferencias. 

### Ancho de Banda y Capacidad del Canal
- **Ancho de banda**: Rango de frecuencias que ocupa una señal (medido en Hz).
- **Fórmula de Shannon**: C = W × log₂(1 + S/N)
	- Donde C es capacidad en bits por segundo, W es ancho de banda, S/N es relación señal-ruido
- **Para tu proyecto**: Un mayor ancho de banda permite más datos pero consume más energía; LoRa optimiza esta relación.

### Modulación: Clave para [[LoRa]]
- **Concepto**: Proceso de codificar información en una onda portadora.
- **Tipos principales**:
	- ASK (Amplitude Shift Keying): Varía la amplitud
	- FSK (Frequency Shift Keying): Varía la frecuencia
	- PSK (Phase Shift Keying): Varía la fase
- **CSS en LoRa**: LoRa usa Chirp Spread Spectrum, una técnica especial que "dispersa" la señal para hacerla más resistente a interferencias y permitir mayor alcance.

## Espectro Electromagnético y Bandas de Frecuencia
### Organización del Espectro

- **Bandas principales**: ELF, VLF, LF, MF, HF, VHF, UHF, SHF, EHF

- **Características de la banda de 433MHz** (UHF):

  - Pertenece a la banda UHF (300-3000 MHz)

  - Buena penetración en edificios

  - Longitud de onda aproximada: 69 cm

  - Banda ISM de uso libre en muchas regiones

  

### Bandas ISM de Uso Libre

- **Concepto**: Bandas Industrial, Scientific and Medical, disponibles sin licencia

- **Principales bandas**: 433 MHz (Europa), 915 MHz (América), 2.4 GHz (mundial)

- **Regulaciones**: Potencia máxima permitida y ciclo de trabajo (importante para LoRa)

- **Ventaja para tu proyecto**: La banda 433 MHz ofrece buen equilibrio entre alcance y tamaño de antena

  

## Propagación de Señales Inalámbricas

  

### Tipos de Propagación

- **Línea de vista**: Transmisión directa entre antenas

  - *Alcance teórico* = 3.57 × √(h₁ + √h₂) [km] donde h₁,h₂ son alturas en metros

- **Propagación por reflexión**: Rebotes en superficies

- **Difracción**: Capacidad de "doblar" alrededor de obstáculos

- **Dispersión**: Reflejos en múltiples superficies pequeñas

  

### Factores que Afectan la Propagación

- **Atenuación**: Pérdida de intensidad con la distancia

- **Obstáculos**: Edificios, árboles, montañas

- **Condiciones atmosféricas**: Lluvia, humedad

- **Interferencias**: Otras señales en la misma banda

  

### Cálculo de Link Budget

- **Concepto**: Balance energético entre transmisor y receptor

- **Fórmula básica**: Potencia Recibida = Potencia Transmitida + Ganancias - Pérdidas

- **Elementos críticos**:

  - Potencia de transmisión (dBm)

  - Ganancia de antenas (dBi)

  - Pérdidas de propagación (dB)

  - Sensibilidad del receptor (dBm)

  

## Fundamentos de Antenas

  

### Parámetros Clave

- **Direccionalidad**: Capacidad de concentrar energía en dirección específica

  - Omnidireccional: Igual en todas direcciones (horizontal)

  - Direccional: Concentra energía en una dirección

- **Ganancia**: Capacidad de amplificar la señal (medida en dBi)

- **Polarización**: Orientación del campo eléctrico

  - Vertical: Mejor para comunicaciones móviles

  - Horizontal: Menor interferencia en ciertas aplicaciones

  - Circular: Usada en satélites, menos afectada por rotación

- **Impedancia**: Resistencia al flujo de corriente eléctrica (idealmente 50 ohm)

  - Crítica para máxima transferencia de energía

  

### Tipos de Antenas para Aplicaciones LoRa

- **Dipolo**: Simple, omnidireccional, ~2.15 dBi

- **Monopolo/Whip**: Común en dispositivos portátiles

- **Helicoidal**: Compacta, buena para dispositivos pequeños

- **Yagi**: Direccional, alta ganancia, ideal para estaciones base

- **Patch**: Plana, direccional, útil en gateways

  

## Comunicación Satelital

  

- **[[Geostationary Orbit|GEO (Geoestacionaria)]]**: 35,786 km, fija sobre un punto de la Tierra
- [[Medium Earth Orbit|MEO (Media)]]: 2,000-35,000 km, utilizada para navegación (GPS)
- [[Low Earth Orbit|LEO (Baja)]]: 160-2,000 km, baja latencia, utilizada para IoT y comunicaciones
