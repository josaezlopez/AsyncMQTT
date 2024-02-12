#include <Arduino.h>
#include <AsyncMQTT.h>

#define MQTT_USER  "xxxxxxxx"
#define MQTT_PASS  "xxxxxxxx"
#define MQTT_HOST  "xxxxxxxx.xxxxxxxx.xxx"
#define MQTT_PORT  1883
#define SSID      "xxxxxxxx"
#define WIFI_PASS "xxxxxxxx"


#ifdef ARDUINO_ARCH_ESP32
      #include <soc/soc.h>
      #include "soc/rtc_cntl_reg.h"
#endif



AsyncMQTT mqttClient(MQTT_USER,MQTT_PASS,MQTT_HOST,MQTT_PORT);

void setup(){

      #if defined(ESP32)
            WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
      #endif

      Serial.begin(115200);

      mqttClient.begin(SSID,WIFI_PASS);
      
      mqttClient.subscribe("/Casa/powermeter");
      mqttClient.subscribe("/Casa/entrada");
      mqttClient.subscribe("/Casa/comedor");
      mqttClient.subscribe("/Casa/salon");
      mqttClient.subscribe("/Casa/cocina");


      // Alls topics.
      for (const auto& [topic, value] : mqttClient.getTopics()){
            Serial.printf("Topic: %s state = %s\r\n",topic.c_str(),value.ultimoEstado.c_str());
      }




}

void loop() {
      if(mqttClient.isPublishReceived()){
            Serial.printf("(Publish) Topic: %s Value= %s\r\n",mqttClient.topicReceived.c_str(),mqttClient.valueReceived.c_str());
      }
}

