---
tags:
  - protocol
  - network
  - project
subject: Protocolos de transmisión de datos
---
# [[LoRa]] Tracker

The main goal of this project is to develop a **GPS tracker** using the [[000-Zettelkasten/LoRa|LoRa]] protocol.

>This protocol is known for its ability to transmit data over long distances with minimal power consumption.

## Members
- Fernando Bran Reza Campos
- Karen Navarro Hurtado
- Antonio Brayan Peña Bernabe

## Requirements
- Reach a minimum transmission of $5$ **kilometers**
- Consider key aspects related to the antenna, such as:
	- Transmission power `watts or dBm`
	- Gain `directly associated with the transmission power`
	- Receiver sensitivity `depends on the quality of the materials`
	- Impedance `antenna resistance` *search for more information related to this...*
	- Polarization `vertical, horizontal, circular` (depends on the antenna shape)
	- Bandwidth `transmission frequency`
	- Directivity `towards where the signal is directed`
	- Radiation angle `signal aperture angle`Transmission power `watts or dBm`
- Integrate a **GPS module** to acquire the geographical position
- Configure the **[[000-Zettelkasten/LoRa|LoRaWAN]]** network to manage communication
- Design an **antenna** that optimizes signal quality

 - [[300-Yrrrrrf/lora-tracker/docs/lora-tracker.canvas|LoRa Tracker Canvas]]
- [[arduino|Arduino]] UNO R3

```rust
define_layer_struct! {
    // * Transport Layer (+any-byte-stream)
    Segment { header: PortAddress, payload: Bytes },
    // * Network Layer (+ip)
    Packet { header: Ipv4Address, pdu: Vec<Segment> },
    // * Data Link Layer
    Frame { header: MacAddress, network_pdu: Vec<Packet> }
}
```

## Links
- [[300-Yrrrrrf/lora-tracker/docs/00-marco-teorico|00-marco-teorico]]
- [[300-Yrrrrrf/lora-tracker/docs/01-especificaciones-técnicas|01-especificaciones-técnicas]]

## References
- [LoRaWAN® | The Things Network](https://www.thethingsnetwork.org/docs/lorawan/)
- [lora-tracker repository](https://github.com/Yrrrrrf/lora-tracker)
-  **Electronic Clinic**
	- [SX1278 Overview](https://www.youtube.com/watch?v=r-jwNlMQPIk)
	    - [source](https://www.electroniclinic.com/lora-sx1278-arduino-hello-world-sensor-monitoring-projects/)
	- [LoRa Network](https://www.youtube.com/watch?v=BeWbStpLirU)
	    - [source](https://www.electroniclinic.com/lora-network-master-arduino-lora-to-multiple-arduino-lora-nodes-lora-end-nodes/)
	    - 01 [Hello World](https://www.youtube.com/watch?v=r-jwNlMQPIk)
	    - 02 [Two Way Communication](https://www.youtube.com/watch?v=feWnciEnaBA)
	    - 03 [SX Lora Modules](https://www.youtube.com/watch?v=wUF9ysBEHfo)
	    - 04 [Nodes Communication w/ Master Lora Node(`Lora Gateway`)](https://www.youtube.com/watch?v=tXoAa_II2OY)
	- [LoRa GPS](https://www.youtube.com/watch?v=d5zfD6ekRYA)
- **IOT Project Ideas**
	- [Lora Two Way Communication](https://iotprojectsideas.com/lora-based-two-way-wireless-communication-system-with-arduino/)
	- [Working LoRa!](https://www.youtube.com/watch?v=08WNTuHj_QU)
