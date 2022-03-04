/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>


SocketIOClient client;
ESP8266WebServer Server;
AutoConnect      Portal(Server);
const char* ssid     = "SSID";
const char* password = "WPA KEY";

char host[] = "192.168.1.122";
int port = 3000;
extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void setup() {
  Serial.begin(115200);
  delay(10);
  Server.on("/", rootPage);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
if (client.connected())
  {
    client.send("connection", "message", "Connected !!!!");
  }
}

void loop() {
  Portal.handleClient();
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    //client.heartbeat(0);
    client.send("atime", "message", "Time please?");
    lastsend = millis();
  }
  if (client.monitor())
  {
    lastreply = millis(); 
    Serial.println(RID);
    if (RID == "atime" && Rname == "time")
    {
      Serial.print("Il est ");
      Serial.println(Rcontent);
    }
  }
  

}
