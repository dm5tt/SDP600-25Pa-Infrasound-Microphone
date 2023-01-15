#### Description

SDP600-25Pa Infrasound Microphone using an ESP32 and Python

#### Author

Holger Adams

#### License

GPLv3

#### Description 

This is a small implementation that

1. Connects to a Wifi accesspoint
2. Initializes the SDP600-25Pa differential pressure sensor
3. Blasts out raw uint16_t measurement samples as soon you connect to the TCP port 1234

#### Limitations 

The I2C clock stretching capability of the ESP32 is limited.

The highest resolution you can choose is 13 Bit. Everything higher will lead to timeouts.

#### Hardware setup

Pin 21: SDA 

Pin 22: SCL

#### Todo

1. Convert raw samples to Pascal ( 1.0 / (scaleFactor*data_sample) )

2. Wifi reconnects sometimes fails
