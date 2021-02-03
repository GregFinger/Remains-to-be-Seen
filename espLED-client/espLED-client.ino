#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <NeoPixelBus.h>

#define LED_COUNT 256
#define UDP_TX_PACKET_MAX_SIZE 1500

//connect LED data to rx pin
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT);

// wifi connection variables
const char* ssid = "network";
const char* password = "password";
boolean wifiConnected = false;

// UDP variables
unsigned int localPort = 8888;
IPAddress ip(192, 168, 1, 211);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);

WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void setup() {

  Serial.begin(115200);

  strip.Begin();
  strip.Show();
}

void loop() {

  ESP.wdtEnable(WDTO_8S);
  ESP.wdtFeed();
  
  if (wifiConnected == 1) {
    if (udpConnected) {
      int packetSize = UDP.parsePacket();
      RgbColor pixel;
      if (packetSize) {
        UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        for (uint16_t i = 0; i < LED_COUNT; i++) {
          pixel = RgbColor(packetBuffer[(i * 3)], packetBuffer[(i * 3) + 1], packetBuffer[(i * 3) + 2]);
          strip.SetPixelColor(i, pixel);
        }
        strip.Show();
      }
    }
  }
  
  if (wifiConnected == 0) {

    wifiConnected = connectWifi();
    if (wifiConnected) {
      udpConnected = connectUDP();
    }
  }
  yield();
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}

// connect to UDP – returns true if successful or false if not
boolean connectUDP() {
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }

  return state;
}
