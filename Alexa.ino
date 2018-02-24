#include <dht.h>

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "alexaarduino-4ee6a.firebaseio.com"
#define FIREBASE_AUTH "KXrRUE6yq5BEXIC6S25Bx8UAwRVOm0uGMyBaWQST"
#define WIFI_SSID "bayuabi"
#define WIFI_PASSWORD "12345678"

#define dhtPin D8
dht DHT;
int lastPowerState = 0;

#define relayPin D5

unsigned long prev=0;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin,OUTPUT);
  digitalWrite(relayPin,LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  DHT.read11(dhtPin);
  int temperature = DHT.temperature;

  unsigned long current = millis();
  if(current - prev > 60000){
    Firebase.setInt("temperature",temperature);
    prev = current;
  }
  
  int power = Firebase.getInt("power");
  Serial.println(power);

  if(power != lastPowerState){
    if(power ==1){
      for(int i=0; i<3;i++){
      digitalWrite(relayPin,HIGH);
      delay(100);
      digitalWrite(relayPin,LOW);
      delay(100);
      }
    }
    if(power == 0){
      for(int i=0; i<2;i++){
      digitalWrite(relayPin,HIGH);
      delay(100);
      digitalWrite(relayPin,LOW);
      delay(100);
      }
    }
    lastPowerState = power;
  }
  else{
    digitalWrite(relayPin,LOW);
  }
}
