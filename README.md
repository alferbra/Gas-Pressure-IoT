# Gas-Pressure-IoT
Check pressure of a gas tank with dev board TTGO T-Call V1.3 ESP32. Project made with PlatformIO in Visual Studio Code

Components:

- TTGO T-Call V1.3 ESP32. It is a dev board with the ESP32 microcontroller and a SIM800L GSM module.
- I used a 18650 battery shield for power. It has voltage retgulator to give 5 and 3.3 V. It is also already prepared to charge the battery via micro-USB.
- To measure the pressure I used a sensor which is power with 5 V and gives a signal between 0 and 4.5 V, which corresponds from 0 to 12 bar.


Diagram:

![alt text](https://github.com/alferbra/Gas-Pressure-IoT/blob/main/img/Diagram.png?raw=true)


You will see your data in Thingspeak:

![alt text](https://github.com/alferbra/Gas-Pressure-IoT/blob/main/img/Thingspeak%20chart.png?raw=true)


Finally, I used an old sensor case to protect the project:

![alt text](https://github.com/alferbra/Gas-Pressure-IoT/blob/main/img/sensor%20case.jpg?raw=true)
