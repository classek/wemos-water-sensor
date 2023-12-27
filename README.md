
This is a simple proof of concept using an ESP8266 WiFi module and a water sensor to send SMS alerts when the water level drops below a certain threshold

This project is designed to help you keep track of the water level in a container and alert you when it falls below a certain threshold. It uses an ESP8266 WiFi module, an analog water level sensor, and the 46elks SMS API (https://46elks.com/) to send alerts to your phone when the water level drops too low.

The code reads the analog signal from the water level sensor and compares it to a pre-defined threshold value. If the water level is below the threshold, the code uses the 46elks API to send an SMS message to your phone alerting you to refill the container.

The program also includes an NTP client that updates the time so that the SMS message includes the current time stamp. The WiFi module connects to your home or office WiFi network, allowing you to receive SMS alerts no matter where you are.

IT's a simple and effective way to keep track of the water level in a container and avoid running out of water. With a few simple modifications, you can adapt this code to work with different types of sensors or APIs and create your own custom alerts for other types of devices or sensors.







