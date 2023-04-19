#include <Adafruit_NeoPixel.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>     // use this for MKR1010 or Nano 33 IoT
#include <Arduino_JSON.h>
#include "arduino_secrets.h"

#define LED_PIN     6
#define LED_COUNT  24
#define BRIGHTNESS 10 // Set BRIGHTNESS to about 1/5 (max = 255)

const String OP_REF = "MTA";
const String MON_REF = "305103";

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

const char serverAddress[] = "http://bustime.mta.info/api/siri/stop-monitoring.json";

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress);

long lastRequest = 0;
int interval = 5000;

void setup() {
  Serial.begin(9600);

  if (!Serial) delay(3000);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);     // print the network name (SSID);

    blink_led();
    // Connect to WPA/WPA2 network:
    WiFi.begin(SECRET_SSID, SECRET_PASS);
  }

  wifi_success();

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  if (millis() - lastRequest > interval ) {
    lastRequest = millis();
    sendRequest();
  }
}

void sendRequest(){
  String path = "?key=" + KEY + "&OperatorRef=" + OP_REF + "&MonitoringRef=" + MON_REF;

  // send the GET request
    Serial.println("making GET request");
    client.get(path);

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
}

void blink_led() {
  strip.show();
  strip.setPixelColor(1, strip.Color(0, 0, 255, 0));
  strip.show();
  delay(250);
  strip.show();
}

void wifi_success() {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, strip.Color(0, 0, 255, 0));         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(50);                           //  Pause for a moment
  }

  pulseBlue();
}

void pulseBlue() {
  for (int j = 255; j >= 0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, strip.gamma8(j), 0));
    strip.show();
    delay(1);
  }

  for (int j = 0; j < 256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, strip.gamma8(j), 0));
    strip.show();
    delay(1);
  }

  for (int j = 255; j >= 0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, strip.gamma8(j), 0));
    strip.show();
    delay(1);
  }
}
