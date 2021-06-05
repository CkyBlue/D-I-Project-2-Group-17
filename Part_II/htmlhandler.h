#pragma once
#include <WiFi.h>
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
}
