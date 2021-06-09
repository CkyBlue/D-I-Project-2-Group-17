#include <WiFi.h>
#include <WiFiAP.h>

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "M5Atom.h"
#include "temperature_record.h"

const char* ssid = "CkyBlue";
const char* password = "Electrolysis";

AsyncWebServer server(80);

float round_to_2dp(float num)
{
    int t = num * 100;
    return t / 100.0f;
}

String getTemperature() { return String(round_to_2dp(currentTemp)); }
String getAvgTemperature() { return String(round_to_2dp(getAverageTemperature())); }

void setup(){  
    if(!SPIFFS.begin()){ Serial.println("An Error has occurred while mounting SPIFFS"); return; }

    M5.begin(true, true, true);
    M5.IMU.Init();

    updateTemperatureData();
    enqueueTemperatureData();

    M5.dis.clear();
    delay(10);

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
    server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/highcharts.js", "text/javascript");
    });

    server.begin();
}

void loop(){
    updateTemperatureData();
    enqueueTemperatureData();

    delay(samplingDelay * 1000);
    M5.update();
}
