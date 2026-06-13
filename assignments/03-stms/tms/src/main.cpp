#include <Arduino.h>
#include <PubSubClient.h>

#include "secrets.h" // ignored, should define WIFI_SSID and WIFI_PASSWD
#include "devices/wifi.hpp"

WifiManager wifiManager(WIFI_SSID, WIFI_PASSWD);

void setup() {
  Serial.begin(115200);
  delay(2000); 

  Serial.print("setup() is running on core ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  Serial.print("loop() is running on core ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}