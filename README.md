# Proyecto de Medición de RPM y Velocidad Lineal

Este proyecto utiliza un sensor conectado a un ESP8266 para medir las revoluciones por minuto (RPM) y la velocidad lineal de un objeto giratorio, y enviar estos datos a ThingSpeak.

## Requisitos

- ESP8266
- Sensor de pulso (por ejemplo, un sensor de efecto Hall, en este caso es un sensor optico ir)
- Cuenta en ThingSpeak

## Instalación

1. **Configurar el ESP8266:**
   - Conecte su ESP8266 a su ordenador y cargue este código usando el Arduino IDE.
   - Asegúrese de tener las bibliotecas `ThingSpeak` y `ESP8266WiFi` instaladas en el Arduino IDE.

2. **Configurar ThingSpeak:**
   - Cree una cuenta en ThingSpeak.
   - Cree un nuevo canal y obtenga su API Key.

## Código

### Bibliotecas

```cpp
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
```
Estas líneas incluyen las bibliotecas necesarias para conectar el ESP8266 a WiFi y enviar datos a ThingSpeak.

## Constantes
```cpp

const char* ssid = "miwifi";
const char* password = "mipass";
const char* server = "api.thingspeak.com";
const char* apiKey = "miapikey";
const int sensorPin = 4; // Cambiar el pin a GPIO4 (D2) para la interrupción
const float radioAspas = 0.12; // Radio en metros (12 cm convertido a metros)
const float pulsosPorVuelta = 2.0;
const float segundosPorMinuto = 60.0;
Estas constantes configuran la conexión WiFi, el servidor de ThingSpeak, el pin del sensor y los parámetros para calcular la velocidad.
```
## Variables Globales
```cpp
volatile int pulseCount = 0;
unsigned long lastTime = 0;
boolean sendData = false; // Variable para controlar el envío de datos a ThingSpeak
float revolucionesPorMinuto = 0.0; // Variable global para almacenar las RPM
float velocidadLineal = 0.0; // Variable global para almacenar la velocidad lineal en m/s
Estas variables globales almacenan el número de pulsos del sensor, el tiempo del último envío de datos, y las RPM y velocidad lineal calculadas.
```
## Función de Interrupción
```cpp
void ICACHE_RAM_ATTR countPulse() {
  pulseCount++;
}
Esta función se llama cada vez que el sensor detecta un pulso (una vuelta).
```
## Configuración (Setup)
```cpp
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
En esta función, se configura la comunicación serie, el pin del sensor, y la conexión WiFi.
```
## Bucle Principal (Loop)
```cpp
void loop() {
  if (millis() - lastTime >= 15000) { // Envía datos cada 15 segundos
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
En este bucle, el código calcula las RPM y la velocidad lineal cada 15 segundos y luego envía los datos a ThingSpeak.
```
## Enviar Datos a ThingSpeak

```cpp

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
Esta función crea una conexión HTTP con el servidor de ThingSpeak y envía los datos calculados.
```
## Contribución
Si desea contribuir a este proyecto, por favor, haga un fork del repositorio y envíe un pull request con sus mejoras o correcciones.

## Licencia
Este proyecto está bajo la licencia MIT. Consulte el archivo LICENSE para más detalles.

r

Este README proporciona una descripción detallada del código, explicando cada parte y cómo conf

![Texto alternativo](imagen 1 .jpg)
