#include <base64.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "<Enter Wifi>";
const char* wifiPassword = "<Enter wifi password>";
const char* apiKey = "<enter api key>";
const char* apiSecret = "<enter api password>";
const char* fromNumber = "H2OSensor"; //Change the namne maximun 11 charakter
const char* toNumber = "+4670123456"; ///Change phone numer to with country code
const char* message = "You need to fill your water";

WiFiClientSecure client;
int sensorPin = A0; // Connect sensor to Analog Pin 0
int threshold = 250; // Threshold please measure before to avoid falsepositive

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");// npt server Europe

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WiFi (Status: ");
    Serial.print(WiFi.status());
    Serial.println(")...");
  }
  Serial.println("Connected to WiFi");

  client.setInsecure();

  timeClient.begin(); // Start NTP-client
}

void loop() {
  // Read value from sensor
  int sensorValue = analogRead(sensorPin);

  // Compare values
  if (sensorValue < threshold) {
    if (client.connect("api.46elks.com", 443)) {
      Serial.println("Connected to API");

      timeClient.update(); // Uppdatera NTP-klienten
      String formattedTime = timeClient.getFormattedTime();

      String auth = String(apiKey) + ":" + String(apiSecret);
      String encodedAuth = base64::encode(auth);

      String data = "to=" + String(toNumber) + "&from=" + String(fromNumber) + "&message=" + String(message) + " - Sent at " + formattedTime + "&dryrun=yes";
      //Change dryrun to no when you are willing to go live
      Serial.println("Sending HTTPS request to API:");
      client.print("POST /a1/sms HTTP/1.1\r\n");
      client.print("Host: api.46elks.com\r\n");
      client.print("Authorization: Basic " + encodedAuth + "\r\n");
      client.print("Content-Type: application/x-www-form-urlencoded\r\n");
      client.print("Content-Length: " + String(data.length()) + "\r\n");
      client.print("\r\n");
      client.print(data);
      Serial.println();
      String response = client.readString();
      Serial.println("API response: " + response);

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      int statusCode = client.parseInt();
      Serial.println("HTTP status code: " + String(statusCode));

      if (statusCode == 200) {
        Serial.println("SMS sent successfully.");
      } else {
        Serial.println("Error sending SMS.");
      }
    } else {
      Serial.println("Failed to connect to API");
    }
  }

  delay(1000 * 60 * 60); //Check once an hour 
}
