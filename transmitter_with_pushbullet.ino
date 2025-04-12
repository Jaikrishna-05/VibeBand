#include <ESP8266WiFi.h>
#include <RH_ASK.h>
#include <SPI.h> 

const char* ssid     = "VITC-EVENT";
const char* password = "Eve@11^12#$";
const char* host     = "api.pushingbox.com";
const char* devid    = "v8F537FA4F94F30E";  

const int buttonPin  = D5;           
RH_ASK driver(2000, D7, D6);         
bool messageSent = false;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);

  
  if (!driver.init()) {
    Serial.println("Transmitter init failed");
  } else {
    Serial.println("Transmitter ready");
  }

  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendPushingBoxNotification() {
  WiFiClient client;
  const int httpPort = 80; 
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection to PushingBox failed");
    return;
  }

  String url = "/pushingbox?devid=" + String(devid);
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println("\nNotification sent. Closing connection.");
}

void loop() {
  static unsigned long lastDebounceTime = 0; 
  static int lastButtonState = HIGH; 
  int buttonState = digitalRead(buttonPin);

  
  if (buttonState != lastButtonState) {
    lastDebounceTime = millis(); 
  }

  
  if ((millis() - lastDebounceTime) > 50) {
    
    if (buttonState == LOW && !messageSent) {
      messageSent = true;

      
      const char *msg = "ALERT";
      driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
      Serial.println("Sent RF: ALERT");

      
      sendPushingBoxNotification();
    }
  }

  if (buttonState == HIGH) {
    messageSent = false;
  }

  lastButtonState = buttonState; 
}