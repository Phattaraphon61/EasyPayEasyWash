
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AutoConnect.h>
#include <ESP.h>

ESP8266WebServer Server;
AutoConnect      Portal(Server);
SocketIOclient socketIO;

#define USE_SERIAL Serial

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}
void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      USE_SERIAL.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;
    case sIOtype_EVENT:
      USE_SERIAL.printf("[IOc] get event: %s\n", payload);
      break;
    case sIOtype_ACK:
      USE_SERIAL.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      USE_SERIAL.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      USE_SERIAL.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

void setup() {
  delay(1000);
  USE_SERIAL.begin(115200);
  USE_SERIAL.print("Chip ID: ");
  USE_SERIAL.println(ESP.getChipId());
  USE_SERIAL.println(Portal.begin());
  Server.on("/", rootPage);
  if (Portal.begin()) {
    USE_SERIAL.println("WiFi connected: " + WiFi.localIP().toString());
  }

  socketIO.begin("192.168.1.122", 5000);

  // event handler
  socketIO.onEvent(socketIOEvent);
}
unsigned long messageTimestamp = 0;
void loop() {
  Portal.handleClient();
  socketIO.loop();

  uint64_t now = millis();

  if (now - messageTimestamp > 2000) {
    messageTimestamp = now;

    // creat JSON message for Socket.IO (event)
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();

    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("event_name");

    // add payload (parameters) for the event
    JsonObject param1 = array.createNestedObject();
    param1["now"] = (uint32_t) now;

    // JSON to String (serializion)
    String output;
    serializeJson(doc, output);

    // Send event
    socketIO.sendEVENT(output);

    // Print JSON for debugging
    USE_SERIAL.println(output);
  }


  //  if (Serial.available() > 0) //Checks is there any data in buffer
  //  {
  //    if (char(Serial.read()) == 'Y') {
  //      WiFi.disconnect();
  //      Serial.println("YES");//Read serial data byte and send back to serial monitor
  //    };
  //    if (char(Serial.read()) == 'N') {
  //      Serial.println("NO");//Read serial data byte and send back to serial monitor
  //    };
  //  }
  //  delay(1000);
}
