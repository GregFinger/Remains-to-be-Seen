#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <NeoPixelBus.h>

/* All commented code is for FastLED */
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#include <FastLED.h>
//#define PIN 6

#define LED_COUNT 256
#define UDP_TX_PACKET_MAX_SIZE 1500

//connect LED data to rx pin
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT);

//CRGB leds[LED_COUNT];

const char *ssid = "Remains to be Seen";
const char *password = "plgrmdoesitbetter";
int wifichan = 3;
unsigned int localPort = 8888;

WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);
  delay(1000);

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password, wifichan) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  Serial.print("Connecting to UDP ... ");
  Serial.println(UDP.begin(localPort) ? "Ready" : "Failed!");

  //  pinMode(0, INPUT_PULLUP);
  //  FastLED.addLeds<WS2812B, PIN, GRB>(leds, LED_COUNT);

  strip.Begin();
  strip.Show();

}

void loop() {

  ESP.wdtEnable(WDTO_8S);
  ESP.wdtFeed();

  int packetSize = UDP.parsePacket();
  RgbColor pixel;
  if (packetSize) {

    //    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, UDP.remoteIP().toString().c_str(), UDP.remotePort());

    UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    for (uint16_t i = 0; i < LED_COUNT; i++) {
      pixel = RgbColor(packetBuffer[(i * 3)], packetBuffer[(i * 3) + 1], packetBuffer[(i * 3) + 2]);
      strip.SetPixelColor(i, pixel);
      //      leds[i].setRGB (packetBuffer[(i * 3)],
      //                      packetBuffer[(i * 3) + 1],
      //                      packetBuffer[(i * 3) + 2]);
    }
    //    FastLED.show();
    strip.Show();
  }

  yield();
}
