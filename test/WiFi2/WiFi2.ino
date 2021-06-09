#include <WiFi.h>
#include <WiFiAP.h>

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "M5Atom.h"

const char* ssid = "CkyBlue";
const char* password = "Electrolysis";

AsyncWebServer server(80);

float currentTemperature;
float avgTemperature;

float round_to_2dp(float num)
{
    int t = num * 100;
    return t / 100.0f;
}

String getTemperature() { return String(round_to_2dp(currentTemperature)); }
String getAvgTemperature() { return String(round_to_2dp(avgTemperature)); }

void setup(){  
    currentTemperature = 45;
    avgTemperature = 20;


    if(!SPIFFS.begin()){ Serial.println("An Error has occurred while mounting SPIFFS"); return; }

    WiFi.softAP(ssid);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html");
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getTemperature().c_str());
    });
    server.on("/avg_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getAvgTemperature().c_str());
    });
    server.on(“/highcharts.js”, HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, “/highcharts.js”, “text/javascript”);
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
    else currentTemperature -= (16 - counter);

    avgTemperature = 75 - currentTemperature;

    delay(500);
}
