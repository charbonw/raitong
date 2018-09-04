# raitong

Raitong Organics sensor array code repository

Receiver:

thingspeak.ino -- code for the Arduino/LoRa receiver
thingspeak channel: https://thingspeak.com/channels/568023
Note: make sure the TX/RX pin numbers are correct for softwareSerial

Transmitter:

In order to program the Raspberry Pi, it must be connected to WiFi:
  + It will automatically connect to a hotspot with the following settings:
    + SSID: RPi
    + Password: raitong498
    + Security: WPA2 PSK
    + Broadcast channel: 2.4 GHz
  + Alternatively, bring it to the mill and it will connect to the office WiFi
  
Once the Pi is connected, you can easily program it in our browser using dataplicity.com:
  + dataplicity.com
  + email: charbow@oregonstate.edu
  + password: raitong498
  + if you need to access sudo through dataplicity:
    + $ su pi
    + $ username: pi
    + $ password: raitong
    
Dataplicity can be laggy. To use an SSH terminal like PuTTy, use the following info:
  + Hotspot:
    + IP: 192.168.43.48
    + port: 22
  + Raitong WiFi:
    + IP: 192.168.1.2
    + port: 22
  + username: pi
  + password: raitong
  + The Pi does not have a static IP, so the IP addresses can change
    + in this case, connect to the Pi via dataplicity and check the IP address with "Hostname -I"
    
The main files are located within /home/pi
on_start.sh runs on startup, and calls read_sensors.py in a loop
read_sensors.py grabs data from each of the sensors and writes it to the local file test-data.csv
next, the data is transmitted via the LoRa module
finally, back in on_start.sh, test-data.csv is copied to a USB drive if one exists



