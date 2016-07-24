/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

WiFiClient client;

const char* host = "your.cloud.host"
const int httpPort = 12345; //your port
String readstr;

void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFiManager wifiManager;
    wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
    wifiManager.autoConnect("AutoConnectAP");

    Serial.println("connected...yeey :)");
    Serial.println(WiFi.localIP());
}

void loop() {

    while (Serial.available()) {
        delay(3);  //delay to allow buffer to fill
        //cnt = 0;
        if (Serial.available() > 0) {
            char c = Serial.read();  //gets one byte from serial buffer
            readstr += c; //makes the string readString
            //cnt++;
        }
    }
    if (readstr.length() > 0) {
        if (!client.connected()) {
            Serial.print("connecting to ");
            Serial.println(host);    
            if (!client.connect(host, httpPort)) {
                Serial.println("connection failed");
                return;
            }
        }
        client.print(readstr);
        readstr = "";
        unsigned long timeout = millis();    
        while (client.available() == 0) {
            if (millis() - timeout > 5000) {
                Serial.println(">>> Client Timeout !");
                client.stop();
                return;
            }
        }  
        // Read all the lines of the reply from server and print them to Serial
        while(client.available()){
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }
        delay(100);    
    }
}
