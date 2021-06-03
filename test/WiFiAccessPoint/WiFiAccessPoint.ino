#include "M5Atom.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

void fillPix(unsigned int color){
    for (int i = 0; i < 25; i++) M5.dis.drawpix(i, color);
}

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);

void setup() {
  Serial.println("Configuring access point...");

  //WiFi.softAP(ssid, password);
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  M5.begin(true, true, true);

  M5.IMU.Init();
  M5.dis.clear();
  
  delay(10);
}

void loop() {
  WiFiClient client = server.available(); 

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";               
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        if (c == '\n') {                    

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            //client.print("<!DOCTYPE html><html><body>");

            client.print("<a style=\"color:red;\" href=\"/Red\">Red</a><br>");
            client.print("<a style=\"color:green;\"href=\"/Green\">Green</a><br>");
            client.print("<a style=\"color:blue;\" href=\"/Blue\">Blue</a><br>");

            //client.print("<form method=\"GET\" action=\"/Red\"><button type=\"submit\">Red</button></form>")

            //client.print("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;  
        }

        if (currentLine.endsWith("GET /Red")) {
          fillPix(0x00ff00);              
        }
        else if (currentLine.endsWith("GET /Green")) {
          fillPix(0xff0000);             
        }
        else if (currentLine.endsWith("GET /Blue")) {
          fillPix(0x0000ff);             
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }
}
