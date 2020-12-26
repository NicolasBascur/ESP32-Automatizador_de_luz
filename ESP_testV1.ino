#include <WiFi.h>
const char* ssid     = "";               // Aca reemplazamos acorde con las credenciales de nuestra red WiFi
const char* password = "";            
WiFiServer server(80);                       // El puerto del servidor web configurado en 80
 
String encabezado;                               // Esta variable es para almacenar la solicitud HTTP
bool state=true;
String output5State = "off";                 // Variables auxiliares para almacenar el estado de salida actual
const int output5 = 5;                       // Asignar una variable de salida para declarar pines GPIO
int s1=0;
void setup() 
{
  Serial.begin(115200);                       // 'Baud Rate' puesta a 115200 la cual es la velocidad por defecto
  pinMode(output5, OUTPUT);                   // Inicializamos las variables de salida como salida
  digitalWrite(output5, LOW);                 // Establemos el pin de salida en LOW
  Serial.print("Conectando a: ");             // Nos conéctamos a la red Wi-Fi con el SSID y contraseña
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");                          // Imprime la dirección IP local e inicia el servidor web en Serial Monitor
  Serial.println("WiFi conectado.");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
