#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// For a real ESP32, put your WiFi name and password here.
// In Breadboard, these are automatically replaced with the simulator WiFi.
const char* ssid = "[REDACTED]";
const char* password = "your wifi password";

WebServer server(80);

String getJoke() {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, "https://icanhazdadjoke.com/");
  http.addHeader("Accept", "text/plain");

  String joke = "Could not get a joke right now.";

  if (http.GET() == 200) {
    joke = http.getString();
  }

  http.end();
  return joke;
}

void showJoke() {
  String joke = getJoke();

  String page =
    "<!doctype html>"
    "<html><body style='font-family:Arial;text-align:center;padding:50px'>"
    "<h1>ESP32 joke machine</h1>"
    "<p style='font-size:20px'>" + joke + "</p>"
    "<p><a href='./'>Get another joke</a></p>"
    "</body></html>";

  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.on("/", showJoke);
  server.begin();

  Serial.println("\nOpen this link:");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}