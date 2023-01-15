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
3. Blasts out raw uint16_t measurement samples and their NTP synchronized timestamps as soon you connect to the TCP port 1234

#### Limitations 

The I2C clock stretching capability of the ESP32 is limited.

The highest resolution you can choose is 13 Bit. Everything higher will lead to timeouts.

#### Hardware setup

Pin 21: SDA 

Pin 22: SCL

#### How to get this thing running?

1. Modifiy the wifi.c to connect it to your local accesspoint

2. Compile it using VisualStudioCode and the ESP-IDF plugin

3. Use record-samples.py to save the measured samples to a file (e.q. examples.txt)

4. Use the show_* scripts to visualize them
