#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "508_Wi-Fi_2.4";
const char* password = "123456789";
const char* mqtt_server = "broker.mqttdashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

char clientID[15];
char destinationTopic[]   = "iot-workshop";

void callback(char* topic, byte* payload, unsigned int length) {
  String payload_str = "";
  String topic_str = String(topic);
  for (int i = 0; i < length; i++) {
    payload_str += (char)payload[i];
  }
  Serial.print("Message arrived [");
  Serial.print(topic_str);
  Serial.print("] ");
  Serial.println(payload_str);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String("iot-" + String(random(1000000))).toCharArray(clientID, 15); //Random Client ID
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(destinationTopic, "hello, world");
      // Subscribing...
      client.subscribe(destinationTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}