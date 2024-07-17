#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

const char* ssid = "missid";
const char* password = "mipass";

const char* server = "api.thingspeak.com";
const char* apiKey = "mikey";
const int sensorPin = 4; // Cambiar el pin a GPIO4 (D2) para la interrupción
volatile int pulseCount = 0;
unsigned long lastTime = 0;
const float radioAspas = 0.12; // Radio en metros (12 cm convertido a metros)
const float pulsosPorVuelta = 2.0;
const float segundosPorMinuto = 60.0;

boolean sendData = false; // Variable para controlar el envío de datos a ThingSpeak

float revolucionesPorMinuto = 0.0; // Variable global para almacenar las RPM
float velocidadLineal = 0.0; // Variable global para almacenar la velocidad lineal en m/s

void ICACHE_RAM_ATTR countPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, FALLING);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  if (millis() - lastTime >= 15000) { // Envía datos cada 10 segundos
    detachInterrupt(digitalPinToInterrupt(sensorPin));
    revolucionesPorMinuto = (float)pulseCount * segundosPorMinuto / pulsosPorVuelta;
    velocidadLineal = (2 * PI * radioAspas) * revolucionesPorMinuto / 60.0; // En metros por segundo
    Serial.print("RPM: ");
    Serial.print(revolucionesPorMinuto);
    Serial.print(", Velocidad lineal: ");
    Serial.print(velocidadLineal);
    Serial.println(" m/s");
    pulseCount = 0;
    attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, FALLING);
    lastTime = millis();
    sendData = true; // Habilitar el envío de datos a ThingSpeak
  }

  if (sendData) {
    sendToThingSpeak(); // Enviar datos a ThingSpeak
    sendData = false; // Deshabilitar el envío de datos hasta el siguiente intervalo
  }
}

void sendToThingSpeak() {
  WiFiClient client;
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field7=";
    postStr += String(revolucionesPorMinuto);
    postStr += "&field8=";
    postStr += String(velocidadLineal);
    postStr += "\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + String(apiKey) + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("Datos enviados a ThingSpeak");
  }
  client.stop();
}
