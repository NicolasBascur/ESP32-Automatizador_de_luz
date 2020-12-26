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

void loop()
{
  WiFiClient client = server.available();             // Habilita el servidor para escuchar a cualquier cliente entrante
  s1=touchRead(T0);                                   
  if(s1<30)                                           // luego verifica el estado del LED para alternar este
    {
      state=!state;                                   // Alternar el estado del LED
      Serial.print("Estado: ");
      Serial.println(state);
      delay(500);
      digitalWrite(output5,state);                    // Escribe el estado en el LED
    }
  if (client) 
  {                                                   // Si un nuevo cliente se conecta
    Serial.println("Nuevo cliente!");                 // imprime el mensaje en el puerto serie
    String lineactual = "";                           // string para contener estos datos entrantes 
    while (client.connected())                        // entra en un loop mientras el cliente está conectado
    {            
      if (client.available())                         // Si hay bytes para leer 
      {             
        char c = client.read();                       // lee un byte para luego
        Serial.write(c);                              // imprimirlo en el monitor serial
        encabezado += c;
        if (c == '\n')                                // si el byte es un carácter de nueva línea
        {                                                                                  
          if (lineactual.length() == 0)              // si la línea actual está en blanco, tenemos dos caracteres de nueva línea seguidos.
          {                                           // ese es el final de la solicitud HTTP del cliente así que se envía una respuesta:
            client.println("HTTP/1.1 200 OK");        // Puesto que los encabezados HTTP siempre comienzan con un código de respuesta (como ejemplo: HTTP / 1.1 200 OK)
            client.println("Content-type:text/html"); // y un tipo contenido para que el usuario sepa lo que viene, luego una línea en blanco:
            client.println("Connection: close");
            client.println(); 
            if (encabezado.indexOf("GET /H") >= 0)        // Enciende y apaga las GPIO
            {
              state=!state;
              digitalWrite(output5, state);
              if(state==true)
              {
                Serial.println("GPIO 5 en estado on");
                output5State = "on";
              }
              else if(state==false)
              {
                Serial.println("GPIO 5 en estado off");
                output5State = "off";
              }
            }
            client.println("<!DOCTYPE html><html>");                                                                                   // Mostrar la página web HTML
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");     // CSS para diseñar los botones de encendido / apagado
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>ESP32 test</h1>");                                                       // Encabezado de la página web 
            client.println("<p>Luz esta " + output5State + "</p>");                                                  // Muestra el estado actual y los botones ON / OFF para GPIO 5      
            if (output5State=="off")                                                                           // Si 'output5State' está apagado, muestra el botón ON
            {  
              client.println("<p><a href=\"/H\"><button class=\"button\">ON</button></a></p>");
            } 
            else 
            {
              client.println("<p><a href=\"/H\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            client.println();                                                  // La respuesta HTTP termina con otra línea en blanco
            break;                                                             // Salir del bucle while
          } 
          else 
          { 
            lineactual = "";                                                  // si se tiene una nueva línea, borra la linea actual
          }
        } 
        else if (c != '\r')                                                    // si tiene algo más que un carácter de retorno 
        {  
          lineactual += c;                                                    // Se agrega al final de la línea actual
        }
      }
    }
    encabezado = "";                                                               // Borramos la variable de encabezado
    client.stop();                                                             // Cerramos la conexion
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}
