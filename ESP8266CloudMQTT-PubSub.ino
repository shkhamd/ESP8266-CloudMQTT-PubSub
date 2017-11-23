#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid          = "0xd52211";
const char* password      = "c0d3men#";
const char* mqttServer    = "m14.cloudmqtt.com";
const int   mqttPort      = 19032;
const char* mqttUser      = "xvatcjpq";
const char* mqttPassword  = "_VhOM2tvhFnc";
const char* mqttClientID  = "ESP8266-ClientID-shaikh";
const char* mqttOutTopic  = "ESP/Pub/PubHello";
const char* mqttInTopic   = "ESP/Sub/SubHello";


char  msg[50];
int   value     = 0;
long  lastMsg   = 0;


WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi.."); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println(".");
  }

  Serial.println("CONNECTED!");
  Serial.println("IP: ");
  Serial.print(WiFi.localIP());
  pinMode(BUILTIN_LED, OUTPUT); 
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}
 
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic); 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  } 
  Serial.println("\n-----------------------");

  if ((char)payload[0] == '1') 
  {
    Serial.println("\n\nLIGHT TURNED ON\n\n\n");
    digitalWrite(BUILTIN_LED, LOW);
  } 
  else
  {
    Serial.println("\n\nLIGHT TURNED OFF\n\n\n");
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("\n\nConnecting to CloudMQTT...");
    if(client.connect(mqttClientID, mqttUser, mqttPassword))
    {
      delay(10);
      Serial.println("CONNECTED");
    } 
    else 
    {
      Serial.print("Failed! RC = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "Test message #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(mqttOutTopic, msg);
    client.subscribe(mqttInTopic);
  }
}
