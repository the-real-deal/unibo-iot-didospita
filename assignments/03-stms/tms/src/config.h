#pragma once

#include "secrets.h" // ignored, should define WIFI_SSID and WIFI_PASSWD

#define SONAR_ECHO_PIN 18
#define SONAR_TRIGGER_PIN 5
#define ONLINE_LED_PIN 26
#define OFFLINE_LED_PIN 27

#define MQTT_BROKER_HOST "broker.mqtt-dashboard.com"
#define MQTT_BROKER_PORT 1883
#define MQTT_BASE_TOPIC "unibo-iot-didospita/assignment03"

#define LOOP_PERIOD_MS 100
#define SONAR_EVENTS_PERIOD_MS 150 // F
#define MQTT_EVENTS_PERIOD_MS LOOP_PERIOD_MS
