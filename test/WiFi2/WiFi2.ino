#include <WiFi.h>
#include <WiFiAP.h>

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "M5Atom.h"

const char* ssid = "CkyBlue";
const char* password = "Electrolysis";

AsyncWebServer server(80);

float currentTemperature;

float round_to_2dp(float num)
{
    int t = num * 100;
    return t / 100.0f;
}

String getTemperature() { return String(round_to_2dp(num)); }

void setup(){  
    currentTemperature = 45;

    if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFi.softAP(ssid);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    //server.begin();

    /*   WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP()); */

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html");
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getTemperature().c_str());
    });

    server.begin();
}
 
int counter = 0;
bool flag = true;

void loop(){
    if (++counter >= 17) {
        flag = !flag;
        counter = 0;
    }

    if (flag) currentTemperature += counter;
    else currentTemperature -= (16 - counter)

    delay(500);
}