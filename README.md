# NodeMCU-WiFi-Pineapple
A WiFi Pineapple is is hacking device that appears as a regular wifi acsess point. When targets connect to this network the Captive Portal will steal their login info and you can accsess it via serial or an admin pannel. This code was written for a NodeMCU (ESP8266)
## DISCLAIMER
I AM NOT resposible for what you do with this code! It was created for educational perposes only and you may only use it when you have permission too.

## Installation
1: Download all of the files in a ZIP folder
2: Extract the files
3: Open /Pineapple.ino in Arduino IDE
4: Install required libraries (ESP8266WIFI, DNSServer)
5: Upload the code to your NodeMCU
6: Connect to the wifi network and let the magic happen!

## Accsess the Admin Panel
To accsess the admin panel, connect to the network (You may have to click the 3 dots in the corner of the captive portal and press "use network as is") and go to this URL http://192.168.4.1/admin. When a client submits their credentials you will see them and be able to download them as a .txt file.

## Modifying Captive Portal Page
In Arduino IDE find the function "handleRoot()" and edit the HTML below it. To add lines you MUST format it like so: page += "HTML CODE HERE";

## Changing SSID and IP
At the top of the code modify the const "ssid" EX: const char *ssid = "Free WiFi Hotspot";
Below where you modify the ssid, change any of the four numbers. (If your not sure what your doing leave this as the defualt) EX: IPAddress apIP(192, 168, 4, 1);
