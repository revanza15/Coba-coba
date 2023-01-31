#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ThingsBoard.h>

#define dht_dpin 15 //D15
#define DHTTYPE DHT22
DHT dht(dht_dpin, DHTTYPE);

WiFiClient client;
//Konfigurasi WiFi
const char *ssid ="Name SSID";
const char *password = "Password WIFI";

#define TOKEN "Name TOKEN "
#define THINGSBOARD_SERVER "thingsboard.cloud"

WiFiClient espClient;
ThingsBoard tb(espClient);


void setup(void) 
{
  dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and Temperature");
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.println("");

  Serial.print("Connecting");
  while (WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  //jika koneksi berhasil maka akan muncul address di serial monitor
  Serial.println(".");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 Serial.print("Current humidity = ");
 Serial.print(h);
 Serial.print("% ");
 Serial.print("Temperature = ");
 Serial.print(t);
 Serial.println("C");
 delay (1000);

 if(!tb.connected()) { 
  //connect to the ThingsBoard
  Serial.print("Connecting to : ");
  Serial.print(THINGSBOARD_SERVER);
  Serial.print("With Token : ");
  Serial.println(TOKEN);
  if (!tb.connect(THINGSBOARD_SERVER, TOKEN))
  {
    Serial.println("Failed to Connecting");
    return;
 }
}
delay(100);
Serial.println("Sending data...");
tb.sendTelemetryFloat("Temperature", t);
tb.sendTelemetryFloat("Humidity", h);

}

