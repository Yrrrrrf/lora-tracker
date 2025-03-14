# Requisitos del Proyecto

El objetivo principal de este proyecto es desarrollar un **tracker GPS** utilizando el protocolo [**LoRa**](https://es.wikipedia.org/wiki/LoRaWAN). Este protocolo es conocido por su capacidad para transmitir datos a largas distancias con un consumo de energía mínimo.

## Requisitos del Proyecto
- Alcanzar una transmisión mínima de **5 kilómetros**
> Recuerdo que el profesor mencionó algo de que la antena debia ser de **433MHz**, pero no estoy seguro si es un requisito o una recomendación (por lo de la frecuencia de transmisión).
- Considerar aspectos clave relacionados con la antena, como:
  - Potencia de transmisión `watts o dBm`
  - Ganancia `directamente asociala a la potencia de transmisión`
  - Sensibilidad del receptor `depende de la caldiad de los materiales`
  - Impedancia `resistencia de la antena.` *buscar mas información relacionado a esto...*
  - Polarización `vertical, horizontal, circular` (depende de la forma de la antena)
  - Ancho de banda `frecuencia de transmisión`
  - Direccionalidad `hacia donde se dirige la señal`
  - Ángulo de radiación `ángulo de apertura de la señal`
- Integrar un **módulo GPS** para adquirir la posición geográfica
- Configurar la red **LoRaWAN** para gestionar la comunicación
- Diseñar una **antena** que optimice la calidad de la señal

## [Marco Teórico](00-marco-teorico.md)
Para comprender los fundamentos de la comunicación inalámbrica y la tecnología LoRa, se presenta un marco teórico que aborda los siguientes temas:
- Introducción a LoRa y LoRaWAN
- Fundamentos de la Comunicación Inalámbrica
- Aplicación en el Desarrollo de un Tracker GPS
- Recursos de Semtech, LoRa Alliance y Artículos Académicos