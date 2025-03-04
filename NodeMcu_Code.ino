#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "Cg 1.67";
const char* password = "ek theke aat";


const char* serverName = "http://192.168.1.100:5000/upload_distance";


const int TRIG_PIN = 14;   // D5 corresponds to GPIO14
const int ECHO_PIN = 12;   // D6 corresponds to GPIO12

WiFiClient wifiClient;
HTTPClient httpClient;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  connectToWiFi();
}

void connectToWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    Serial.print(".");
    attempts++;
  }


  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed");
  }
}

long measureDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);


  long duration = pulseIn(ECHO_PIN, HIGH, 26000);
  

  long distance = (duration / 2) / 29.1;


  if (distance < 0 || distance > 400) {
    return -1;
  }

  return distance;
}

void sendDistanceToServer(long distance) {

  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
    return;
  }


  String serverPath = String(serverName) + "?distance=" + String(distance);
  

  httpClient.begin(wifiClient, serverPath);
  

  int httpResponseCode = httpClient.GET();
  

  if (httpResponseCode > 0) {
    String response = httpClient.getString();
    Serial.print("Server Response: ");
    Serial.println(response);
  } else {
    Serial.print("Error in HTTP request: ");
    Serial.println(httpResponseCode);
  }
  

  httpClient.end();
}

void loop() {
  long distance = measureDistance();


  Serial.print("Distance: ");
  

  if (distance > 0) {
    Serial.print(distance);
    Serial.println(" cm");
    

    sendDistanceToServer(distance);
  } else {
    Serial.println("Measurement Error");
  }

  delay(1000);
}