# Marco Teórico del Proyecto LoRa Tracker

Este documento establece las bases teóricas sobre las que se construye el proyecto, abarcando los principios de comunicación por radiofrecuencia, las particularidades de la tecnología LoRa y los fundamentos del sistema de posicionamiento GPS.

## Fundamentos de Radiocomunicación

### Modulación: Chirp Spread Spectrum (CSS)
- **Principio Clave**: La tecnología LoRa se basa en la modulación patentada **Chirp Spread Spectrum (CSS)**. A diferencia de las modulaciones tradicionales, CSS codifica la información en "chirps", que son señales de banda ancha cuya frecuencia aumenta o disminuye a lo largo del tiempo.
- **Ventajas para el Proyecto**:
    - **Resistencia al Ruido**: La energía de la señal se distribuye en un ancho de banda amplio, lo que permite que el receptor la decodifique incluso si la señal está por debajo del nivel de ruido del entorno. Esto es fundamental para lograr comunicación en condiciones no ideales.
    - **Largo Alcance**: La robustez de la modulación permite mantener un enlace de comunicación estable a grandes distancias con una potencia de transmisión muy baja, optimizando la duración de la batería del tracker.
    - **Coexistencia**: Múltiples dispositivos pueden transmitir en el mismo canal con mínima interferencia mutua, gracias a la cuasi-ortogonalidad de los chirps con diferentes factores de dispersión.

### Banda de Frecuencia y Espectro
- **Banda de Operación**: El proyecto opera en la banda de **433MHz**, que pertenece a las bandas ISM (Industrial, Científica y Médica) de uso libre. Esto elimina la necesidad de licencias, siempre que se respeten las regulaciones locales de potencia.
- **Ventajas Estratégicas de 433MHz**:
    - **Balance Alcance-Penetración**: Ofrece una excelente propagación de la señal y una capacidad superior para penetrar obstáculos (edificios, vegetación) en comparación con bandas de mayor frecuencia como 2.4GHz.
    - **Tamaño de Antena Favorable**: La longitud de onda (~69 cm) permite el uso de antenas eficientes y de tamaño práctico (e.g., monopolo de 1/4 de onda de ~17 cm), ideales para un dispositivo tracker portátil.

## Propagación y Viabilidad del Enlace

### Cálculo de Presupuesto de Enlace (Link Budget)
- **Concepto**: Para verificar que el objetivo de 5 km es alcanzable, se realiza un **Cálculo de Presupuesto de Enlace**. Este balancea todas las ganancias y pérdidas de señal desde el transmisor hasta el receptor.
- **Fórmula Básica**: `Potencia Recibida (dBm) = Potencia de Tx (dBm) + Ganancias (dBi) - Pérdidas (dB)`
- **Elementos Críticos para el Proyecto**:
    - **Potencia de Transmisión (Tx Power)**: El módulo SX1276 permite transmitir hasta **+20 dBm** (100 mW).
    - **Ganancia de Antena**: Capacidad de la antena para concentrar la energía. Se estiman de +3 a +5 dBi para las antenas del proyecto.
    - **Pérdidas de Propagación (Path Loss)**: Atenuación natural de la señal con la distancia.
    - **Sensibilidad del Receptor**: La mínima potencia que el receptor puede decodificar. Para LoRa, este valor es excepcionalmente bajo (**hasta -148 dBm**), siendo la característica más importante para lograr largo alcance.
- **Conclusión**: Un margen de enlace positivo (Potencia Recibida > Sensibilidad) confirma la viabilidad teórica del sistema a la distancia objetivo.

## Fundamentos de Antenas

### Parámetros Clave
- **Impedancia**: Para una máxima transferencia de potencia, la impedancia de la antena debe coincidir con la del circuito del módulo LoRa, que es de **50 ohmios**. Un desajuste reduce el alcance efectivo.
- **Ganancia y Direccionalidad**:
    - **Omnidireccional**: Iradia la señal de manera uniforme en un plano (e.g., un dipolo). Es la elección ideal para el **tracker móvil**, ya que no necesita apuntar en una dirección específica.
    - **Direccional**: Concentra la potencia en una dirección, logrando mayor ganancia (e.g., una antena Yagi). Es una excelente opción para la **estación base** para maximizar el alcance y la calidad de la señal desde una ubicación fija.
- **Polarización**: Se refiere a la orientación del campo eléctrico. Para comunicaciones terrestres como esta, se utiliza la **polarización vertical**. Es crucial que tanto la antena del tracker como la de la estación base tengan la misma polarización.

## Sistema de Posicionamiento Global (GPS)

### Principio de Funcionamiento
- **Concepto**: El proyecto utiliza un módulo GPS (NEO-6M) para la geolocalización. Este sistema se basa en una constelación de satélites en órbita terrestre media (MEO) que transmiten señales de tiempo precisas.
- **Trilateración**: El receptor GPS calcula su posición mediante los siguientes pasos:
    1. Recibe señales de tiempo de múltiples satélites.
    2. Mide el desfase temporal de cada señal para calcular su distancia a cada satélite.
    3. Con la distancia a un mínimo de cuatro satélites, puede determinar su posición 3D (latitud, longitud y altitud).

### Integración en el Proyecto
- **Formato de Datos**: El módulo GPS entrega la información a través de su puerto serie en un formato estandarizado conocido como **sentencias NMEA**.
- **Librería de Software**: En el firmware del tracker, la librería **`TinyGPSPlus`** se encarga de interpretar estas sentencias NMEA.
- **Función Práctica**: Su rol es extraer los datos clave requeridos por el proyecto de las complejas cadenas de texto NMEA: **latitud**, **longitud**, **altitud** y **velocidad**.