#include <WiFi.h>
#include <ESP32Ping.h>
#include <ESPmDNS.h>

const char* ssid = "DAYSI";
const char* password = "1106042474001";

IPAddress localIP(192, 168, 100, 50); 
IPAddress gateway(192, 168, 100, 1);  
IPAddress subnet(255, 255, 255, 0);   
IPAddress dns(8, 8, 8, 8);   

void setup() {
  Serial.begin(9600);
  delay(1000);
  if (!WiFi.config(localIP, gateway, subnet, dns)) {
    Serial.println("No se ha podido configurar");
    return;
  }

  WiFi.mode(WIFI_STA);

  Serial.println("Conectando al punto de acceso...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n Conexión exitosa");
  if (!MDNS.begin("esp32")) {
    Serial.println("No se puede configuar el mDNS!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("Se ha configurado el mDNS");

  detallesConexion();
  estadisticasConexion();
  puntoAcceso();
  modoOperacionInfo();
}

void loop() {
}

void detallesConexion(){
  Serial.println("\n---Detalles de la conexion---");
  Serial.printf("Tipo de autenticación: %s\n", tipoAutenticacion());
  Serial.printf("Dirección IP: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("Mascara de subred: %s\n", WiFi.subnetMask().toString().c_str());
  Serial.printf("Puerta de enlace: %s\n", WiFi.gatewayIP().toString().c_str());
  Serial.println("--------------------------------");
}

void estadisticasConexion() {
  Serial.println("\n--- Estadísticas de la conexión ---");

  IPAddress ip_objetivo(192, 168, 100, 12);
  int tamanio = 32;
  static int enviados = 0;
  static int recibidos = 0;
  static int perdidos = 0;
  static int errTransmision = 0;
  static int errRecepcion = 0;
  static unsigned long tiempo_conexion = 0;
  unsigned long tiempo_inicial = millis();

  // Realizar ping al objetivo
  if (Ping.ping(ip_objetivo, tamanio)) {
    recibidos++;
  } else {
    perdidos++;
    errRecepcion++;
    errTransmision++;
  }

  enviados = recibidos + perdidos;
  unsigned long tiempo_transcurrido = millis() - tiempo_inicial;
  tiempo_conexion += tiempo_transcurrido;

  // Mostrar estadísticas
  Serial.println("Paquetes enviados: " + String(enviados));
  Serial.println("Paquetes recibidos: " + String(recibidos));
  Serial.println("Paquetes perdidos: " + String(perdidos));
  Serial.println("Errores en la transmisión: " + String(errTransmision));
  Serial.println("Errores en la recepción: " + String(errRecepcion));
  Serial.println("Bytes transferidos: " + String(recibidos * tamanio));
  Serial.println("Tiempo de conexión activa: " + String(tiempo_conexion / 1000) + " segundos");
  Serial.println("--------------------------------------");
}


void puntoAcceso() {
  Serial.println("\n--- Información del Punto de Acceso ---");
  Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
  Serial.printf("MAC del AP: %s\n", WiFi.BSSIDstr().c_str());
  Serial.printf("Canal de frecuencia: %d\n", WiFi.channel());
  Serial.printf("RSSI (nivel de señal): %d dBm\n", WiFi.RSSI());
  Serial.println("----------------------------------------");
}

void modoOperacionInfo() {
  Serial.println("\n--- Modo de Operación WiFi ---");
  Serial.println("Modo actual: Station (STA)");
  Serial.println("--------------------------------");
}

const char* tipoAutenticacion() {
  // Escanea las redes WiFi disponibles
  int numRedes = WiFi.scanNetworks();
  for (int i = 0; i < numRedes; i++) {
    // Compara el SSID de la red escaneada con el SSID al que estamos conectados
    if (WiFi.SSID(i) == WiFi.SSID()) {
      // Obtiene el tipo de autenticación de la red escaneada
      wifi_auth_mode_t encryptionType = WiFi.encryptionType(i);
      switch (encryptionType) {
        case WIFI_AUTH_OPEN: return "Abierta";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA-PSK";
        case WIFI_AUTH_WPA2_PSK: return "WPA2-PSK";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2-PSK";
        case WIFI_AUTH_WPA3_PSK: return "WPA3-PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
        default: return "Desconocido";
      }
    }
  }
  return "Red no encontrada"; // Si no encuentra el SSID
}

String modoOperacion(){
  switch (WiFi.getMode()) {
    case WIFI_STA: return "WIFI_STA";
    case WIFI_AP: return "WIFI_AP";
    case WIFI_AP_STA: return "WIFI_AP_STA";
    default: return "Desconocido";
  }
}
