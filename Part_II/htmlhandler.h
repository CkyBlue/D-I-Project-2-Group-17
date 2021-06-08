#pragma once
/*#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <temperature_record.h>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

char temp[800];
WebServer server(80);
const char *ssid = "Weather Station";
const char *password = "12345";
WiFiClient client = server.available();
void initializeServer()
{
    //WiFi.mode(WIFI_STA);
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    server.on("/", handle_on_connect);
    server.onNotFound(handlenotfound);
    server.begin();

    string line;
    ifstream in("htmlfile.txt");
    if (in.fail())
    {
        cerr << "Error opening file. Exiting.\n";
        exit(-1);
    }

    while (getline(in, line))
    {
        temp << line << '\n';
    }
    in.close();
}
void handlenotfound()
{
    server.send(404, "text/plain", "ERROR 404 FILE NOT FOUND");
}

void handle_on_connect()
{
    //fetch active temp
    server.send(200, "text/html", SendHTML(currentTemp, getAverageTemperature()));
}
String SendHTML(float ActiveTemp, float temp24)
{
    snprintf(temp, 800,temp, ActiveTemp,temp24);
    return temp;
}*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer
#include <SPIFFS.h>
#include <temperature_record.h>

const char *ssid = "Weather Station";
const char *password = "12345";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void initialize()
{

    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);
    // Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html"); });
    server.on("/ActiveTemp", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", currentTemp); });
    server.on("/Temp24", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", getAverageTemperature()); });

    // Start server
    server.begin();
}
