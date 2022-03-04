#include <ESP8266HTTPClient.h>
#include <SocketIoClient.h>


//192.168.1.37 --My IP Address

SocketIoClient webSocket;

const char* ssid     = "ssid";      // SSID
const char* password = "pass";        // Password
const char* host = "192.168.1.37";        // Server IP (localhost)
const int   port = 8080;                  // Server Port
const char* url = "http://localhost:8080/test";

void event(const char * payload, size_t length) {
  Serial.println("Message");
}

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  Serial.print("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting To Socket");
  webSocket.on("event", event);
  webSocket.begin("192.168.1.37", 8080);

}

void loop() {

    webSocket.loop();
 
}
