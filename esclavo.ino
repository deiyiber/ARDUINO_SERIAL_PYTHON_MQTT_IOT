//ESCLAVO

//LIBRERIAS A UTILIZAR
#include "ESP8266WiFi.h"
#include <aREST.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//DEFINIMOS
#define DHTPIN D2
#define DHTTYPE DHT22
#define MQ A0

//INICIAMOS
DHT dht(DHTPIN, DHTTYPE);

//aREST
aREST rest = aREST();

//WiFi DATOS DE LA RED CON LA CLAVE
const char* ssid = "Tu Wifi";
const char* password = "Tu contrasena";

//PUETO A UTILIZAR 
#define LISTEN_PORT 80

//CREAMOS LA INSTANCIA EN EL SERVIDOR
WiFiServer server(LISTEN_PORT);

//CREAMOS LAS VARIABLES QUE VAMOS A TRANSMITIR 
int mq, humidity, temperature;

void setup(){
  Serial.begin(9600);
  dht.begin();
  
//INICIAR LAS VARIABLES
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.variable("contaminacion",&mq);

//NOMBRE ID
  rest.set_id("0");
  rest.set_name("sensor_node");

//CONEXION CON EL WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected!");

//COMENZAR O INICIAL EL SERVIDOR
  server.begin();
  Serial.println("Server started!");

//IP
  Serial.println(WiFi.localIP());
}

void loop() {
  //Wait 1s
  delay(1000);
  mq = analogRead(MQ); //MQ135
  humidity = dht.readHumidity(); //RH %0 - 100 (Punto rocio)
  temperature = dht.readTemperature(); //0 - 100 *C

  //REST Calls
  WiFiClient client = server.available();
  if (!client) {
    return;
    }
  while(!client.available()){
    delay(1);
    }
  rest.handle(client);  
}
