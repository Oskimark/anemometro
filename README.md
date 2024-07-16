# Proyecto de Medición de RPM y Velocidad Lineal

Este proyecto utiliza un sensor conectado a un ESP8266 para medir las revoluciones por minuto (RPM) y la velocidad lineal de un objeto giratorio, y enviar estos datos a ThingSpeak.

## Requisitos

- ESP8266
- Sensor de pulso (por ejemplo, un sensor de efecto Hall)
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


Estas líneas incluyen las bibliotecas necesarias para conectar el ESP8266 a WiFi y enviar datos a ThingSpeak.

Constantes
const char* ssid = "anita";
const char* password = "cinemanteca";
const char* server = "api.thingspeak.com";
const char* apiKey = "7H2ZRKBIHE55LPRF";
const int sensorPin = 4; // Cambiar el pin a GPIO4 (D2) para la interrupción
const float radioAspas = 0.12; // Radio en metros (12 cm convertido a metros)
const float pulsosPorVuelta = 2.0;
const float segundosPorMinuto = 60.0;
Estas constantes configuran la conexión WiFi, el servidor de ThingSpeak, el pin del sensor y los parámetros para calcular la velocidad.

Variables Globales

Estas constantes configuran la conexión WiFi, el servidor de ThingSpeak, el pin del sensor y los parámetros para calcular la velocidad.

Variables Globales
