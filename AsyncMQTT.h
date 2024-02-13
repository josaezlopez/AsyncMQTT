#pragma once


#if defined(ESP32)
    #include <WiFi.h>
    #include <freertos/FreeRTOS.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <Ticker.h>
#endif

#include <PubSubClient.h>
#include <string.h>
#include <string>
#include <map>

struct tema{
    //std::string tema;
    std::string ultimoEstado;

};

class AsyncMQTT{
    public:
        AsyncMQTT(const char* _usuario, const char* _password, const char* _host,int _port);
        bool WiFiConnect(const char* _ssid, const char* _wifiPassword);
        bool BrokerConnect();
        bool loop();
        void subscribe(std::string topic);
        void subscribeAll();
        void begin(const char* _ssid,const char* _passwitfi);
        bool isPublishReceived();
        bool publish(const char* topic, const char* payload);
        bool publish(const char* topic, const char* payload, boolean retained);
        bool publish(const char* topic, const uint8_t* payload, unsigned int plength);
        bool publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained);
        std::map<std::string, tema> getTopics() { return temas; }
        std::string valueReceived;
        std::string topicReceived;

    private:
        bool publishReceived = false;
        void setCallback();
        const char* usuario;
        const char* password;
        const char* host;
        const char* ssid;
        const char* wifiPassword;
        int port;
        std::map<std::string, tema> temas;
        WiFiClient  WFClient;
        PubSubClient* client = nullptr;
        #if defined(ESP32)
            TaskHandle_t handleTask;
        #elif defined(ESP8266)
            Ticker tLoop;
        #endif
};