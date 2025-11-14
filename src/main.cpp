#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <map>
#include "esp_wpa2.h"

// Enter your information
const char* ssid = "";
const char* eap_user = "";
const char* eap_pass = "";

std::map<int, int> pinStates;

WebServer server(80);

void handleRoot();
void handleSetPin();
bool isNumeric(String str);
bool isValidPin(int pin);

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)eap_user, strlen(eap_user));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)eap_user, strlen(eap_user));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)eap_pass, strlen(eap_pass));
  
  esp_wifi_sta_wpa2_ent_enable();

  WiFi.begin(ssid);

  Serial.println("[INFO] Serial started");

  Serial.println("[INFO] Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("[INFO] Connected. My local IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("[INFO] Starting web server...");
  server.on("/", handleRoot);
  server.on("/setpin", handleSetPin);
  Serial.println("[INFO] Web server is ready");

  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html><body>\
  <h1>Pinner</h1>\
  <form action='/setpin' method='GET'>\
  <input type='text' name='pin'>\
  <button type='submit' name='action' value='on'>Turn On</button>\
  <button type='submit' name='action' value='off'>Turn Off</button>\
  </form>";

  html += "<h2>Pin States</h2>";

  if (pinStates.empty()) {
    html += "<p>No pins configured yet</p>";
  } else {
    html += "<table> <tr> <th><b>Pin</b></th> <th><b>State</b></th> </tr>";
    for (auto const& pair : pinStates) {
      int pin = pair.first;
      int state = pair.second;

      html += "<tr><td>";
      html += pin;
      html += "</td>";
      html += "<td>";
      html += (state == HIGH) ? "<span style'color: green;'>ON</span>" : "<span>OFF</span>";
      html += "</td></tr>";
    }
    html += "</table>";
  }
  html += "</body></html>";

  server.send(200, "text/html", html);
  Serial.print("["); 
  if (server.method() == HTTP_GET) {
    Serial.print("GET");
  } else if (server.method() == HTTP_POST) {
    Serial.print("POST");
  }
  Serial.print("] 200: "); Serial.println(server.client().remoteIP());
}

void handleSetPin() {
  String pin_str = server.arg("pin");
  String action_str = server.arg("action");

  if (!isNumeric(pin_str)) {
    Serial.print("[ERROR] Invalid pin value recived: "); Serial.println(pin_str);
    server.send(400, "text/plain", "Error: Invalid pin value received. It must be a number");
    return;
  }

  unsigned short pin_number = pin_str.toInt();

  if (!isValidPin(pin_number)) {
    Serial.print("[ERROR] Invalid pin selected "); Serial.print(pin_number); Serial.println(" (not usable as output)");
    server.send(400, "text/plain", "Error: Invalid or unsafe pin selected. Use GPIOs like 13, 14, 16-33");
    return;
  }

  Serial.print("[");
  if (server.method() == HTTP_GET) {
    Serial.print("GET");
  } else if (server.method() == HTTP_POST) {
    Serial.print("POST");
  }
  Serial.print("] 200: "); Serial.println(server.client().remoteIP());
  Serial.print(" -> Pin: "); Serial.print(pin_number); Serial.print(", Action: ");
  Serial.println(action_str);

  pinMode(pin_number, OUTPUT);

  if (action_str == "on") {
    digitalWrite(pin_number, HIGH);
    pinStates[pin_number] = HIGH;

  } else if (action_str == "off") {
    digitalWrite(pin_number, LOW);
    pinStates[pin_number] = LOW;
  }
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Redirecting...");
}

bool isNumeric(String str) {
  if (str.length() == 0) {
    return false;
  }
  for (int i = 0; i < str.length(); i++) {
    if (!isDigit(str.charAt(i))) {
      return false;
    }
  }
  return true;
}

bool isValidPin(int pin) {
  if (pin >= 34 && pin <= 39) {
    return false;
  } else if (pin >= 6 && pin <= 11) {
    return false;
  }

  switch (pin) {
    case 0:
    case 1:
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
      return false;
    default:
    if (pin < 0 || pin > 39) {
      return false;
    }
    return true;
  }
}