#include <AsyncMQTT.h>


// Constructor
AsyncMQTT::AsyncMQTT(const char* _usuario, const char* _password, const char* _host,int _port){
    usuario = _usuario;
    password = _password;
    host = _host;
    port =_port;
}


// Conecta la wifi
bool AsyncMQTT::WiFiConnect(const char* _ssid, const char* _wifiPassword){
    ssid = _ssid;
    wifiPassword = _wifiPassword;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,wifiPassword);
    Serial.println("Conectando a la wifi");
    while(WiFi.status()!=WL_CONNECTED){
        Serial.print(".");
        delay(1000);

        }
    client = new PubSubClient(WFClient);
    return true;
}

// Pone el Callback
void AsyncMQTT::setCallback(){
    client->setCallback(
        [this](char* topic, byte* payload, unsigned int length) {
            valueReceived.assign((char*)payload,length);
            topicReceived.assign(topic);
            temas[topic].ultimoEstado = valueReceived;
            publishReceived = true;
        }
    );
}

void AsyncMQTT::begin(const char* _ssid,const char* _passwifi){

    WiFiConnect(_ssid,_passwifi);
    BrokerConnect();
    
    #if defined(ESP8266)
        tLoop.attach(0.1,
            [this](){
                client->loop();
            }
        );
    #elif defined(ESP32)
        xTaskCreate(
            [](void* param){
                AsyncMQTT* p = static_cast<AsyncMQTT*>(param);
                while(true){
                    p->client->loop();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                    }
                },
            "AsyncMQTT",2048,this,3,NULL);
    #endif

}


// Conecta con el broker
bool AsyncMQTT::BrokerConnect(){

    while(WiFi.status()!=WL_CONNECTED){
        WiFiConnect(ssid,wifiPassword);
    }

    Serial.println("Conectando al broker.");
    client->setServer(host,port);
    for(int n=0; n <30 ; n++){
        if(client->connect("esp8266-123456",usuario,password)){
            Serial.println("Conectado");
            setCallback();
            subscribeAll();
            return true;
            }
        else{
            Serial.print(".");
            delay(1000);
            }
    }
    return false;
}

void AsyncMQTT::subscribeAll(){

    for (const auto& [topic, ultimoEstado] : temas){
        client->subscribe(topic.c_str());
    }
}

// Se suscribe a un topic
void AsyncMQTT::subscribe(std::string topic){

    if (auto search = temas.find(topic); search != temas.end()){
       return;
    }
    else{
        tema nuevoTema;
        nuevoTema.ultimoEstado = "";
        temas.insert ( std::pair<std::string,tema>(topic,nuevoTema) );
        client->subscribe(topic.c_str());
    }
}


bool AsyncMQTT::loop(){
if(client != nullptr){
    if(client->connected())
        client->loop();
        return true;
    }
    return false;
return false;
}


bool AsyncMQTT::isPublishReceived(){
    bool status = publishReceived;
    publishReceived =false;
    return status;
}    

bool AsyncMQTT::publish(const char* topic, const char* payload){
    return client->publish(topic,payload);
    }

bool AsyncMQTT::publish(const char* topic, const char* payload, boolean retained) {
    return client->publish(topic,(const uint8_t*) payload, retained);
    }

bool AsyncMQTT::publish(const char* topic, const uint8_t* payload, unsigned int plength) {
    return client->publish(topic, payload, plength, false);
    }

bool AsyncMQTT::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    return client->publish(topic, payload, plength, retained);
    }

