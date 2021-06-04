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

            client.print("<!DOCTYPE html><html><body>");

            client.print("<a style=\"color:red;\" href=\"/Red\">Red</a><br>");
            client.print("<a style=\"color:green;\"href=\"/Green\">Green</a><br>");
            client.print("<a style=\"color:blue;\" href=\"/Blue\">Blue</a><br>");

            client.print("</body></html>");

/*             client.print("<!DOCTYPE html> <html> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> ");
            client.print("<title>Document</title> <style> * { box-sizing: border-box } .tab { float: left; ");
            client.print("border: 1px solid #ccc; background-color: #f1f1f1; width: 30%; height: 300px; } .tab button { ");
            client.print("display: block; background-color: inherit; color: black; padding: 22px 16px; width: 100%; ");
            client.print("border: none; outline: none; text-align: left; cursor: pointer; transition: 0.3s; ");
            client.print("} .tab button:hover { background-color: #ddd; } .tab button.active { background-color: #ccc; ");
            client.print("} .tabcontent { float: left; padding: 0px 12px; border: 1px solid #ccc; width: 70%; ");
            client.print("border-left: none; height: 300px; } </style> </head> <body> <div class=\"tab\"> <button class=\"tablinks\" id=\"defaultOpen\" onclick=\"openColor(event, 'Red')\">Red</button> ");
            client.print("<button class=\"tablinks\" onclick=\"openColor(event, 'Green')\">Green</button> <button class=\"tablinks\" onclick=\"openColor(event, 'Blue')\">Blue</button> ");
            client.print("</div> <div id=\"Red\" class=\"tabcontent\"> <h3>Red</h3> <p>LED Lights up to color Red.</p> ");
            client.print("</div> <div id=\"Green\" class=\"tabcontent\"> <h3>Green</h3> <p>LED Lights up to color Green.</p> ");
            client.print("</div> <div id=\"Blue\" class=\"tabcontent\"> <h3>Blue</h3> <p>LED Lights up to color Blue.</p> ");
            client.print("</div> <a style=\"color:red;\" href=\"/Red\">Red</a><br> <a style=\"color:green;\"href=\"/Green\">Green</a><br> ");
            client.print("<a style=\"color:blue;\" href=\"/Blue\">Blue</a><br> </body> <script> function openColor(evt, colorName) { ");
            client.print("var i, tabcontent, tablinks; tabcontent = document.getElementsByClassName(\"tabcontent\"); ");
            client.print("for (i = 0; i < tabcontent.length; i++) { tabcontent[i].style.display = \"none\"; ");
            client.print("} tablinks = document.getElementsByClassName(\"tablinks\"); for (i = 0; i < tablinks.length; i++) { ");
            client.print("tablinks[i].className = tablinks[i].className.replace(\" active\", \"\"); } document.getElementById(colorName).style.display = \"block\"; ");
            client.print("evt.currentTarget.className += \" active\"; fetch('/' + colorName).then(function (response) { ");
            client.print("console.log('Success!', response); }).catch(function (err) { console.warn('Something went wrong.', err); ");
            client.print("}); } window.onload = function (e) { document.getElementById(\"defaultOpen\").click(); ");
            client.print("} </script> </html> "); */

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;  
        }

        // Serial.print("Current Line: " + currentLine + "\n");

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
