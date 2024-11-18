#include <WiFi.h>

// Configuración del Access Point
const char* ssid = "AP_thais";
const char* password = "12345678";
const int channel = 6; 
const int maxClients = 2; 
IPAddress localIP(192, 168, 1, 2); 
IPAddress gateway(192, 168, 1, 1); 
IPAddress subnet(255, 255, 255, 0); 
IPAddress dhcpStart(192, 168, 1, 5); 

void setup() {
  Serial.begin(9600);

  if (!WiFi.softAPConfig(localIP, gateway, subnet, dhcpStart)) {
    Serial.println("No se pudo configurar el Access Point.");
    return;
  }

  if (!WiFi.softAP(ssid, password, channel, WIFI_AUTH_WPA2_PSK, maxClients)) {
    Serial.println("No se pudo iniciar el Access Point.");
    return;
  }

  // Mostrar la información del Access Point
  Serial.println("Modo Access Point iniciado...");
  Serial.print("SSID: ");
  Serial.println(String(WiFi.softAPSSID()));
  Serial.print("Contraseña: ");
  Serial.println(password);
  Serial.print("Canal de frecuencia: ");
  Serial.println(channel);
  Serial.print("Modo de encriptación: ");
  Serial.println("WPA2_PSK");
  Serial.print("Rango de direcciones IP: ");
  Serial.print(dhcpStart);
  Serial.print(" - ");
  Serial.println(String(dhcpStart[0]) + "." + String(dhcpStart[1]) + "." + String(dhcpStart[2]) + "." + String(dhcpStart[3] + maxClients - 1));
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Máscara de subred: ");
  Serial.println(subnet);
  Serial.print("Puerta de enlace: ");
  Serial.println(gateway);
  Serial.print("Número máximo de clientes: ");
  Serial.println(maxClients);
  Serial.print("Número de clientes conectados: ");
  Serial.println(WiFi.softAPgetStationNum());
}

void loop() {
  delay(20000);
  Serial.print("Número de clientes conectados: ");
  Serial.println(WiFi.softAPgetStationNum());
}
