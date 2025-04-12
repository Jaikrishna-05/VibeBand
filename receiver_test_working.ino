#include <RH_ASK.h>
#include <SPI.h> 

RH_ASK driver(2000, 8, 9);

const int vibrationMotor = 13; 

void setup() {
  Serial.begin(9600);
  pinMode(vibrationMotor, OUTPUT);

  
  if (!driver.init()) {
    Serial.println("Receiver init failed");
  } else {
    Serial.println("Receiver ready");
  }
}

void loop() {
  uint8_t buf[12];              
  uint8_t buflen = sizeof(buf); 

  
  if (driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; 
    Serial.print("Received: ");
    Serial.println((char *)buf);

  
    if (strcmp((char *)buf, "ALERT") == 0) {
      digitalWrite(vibrationMotor, HIGH);
      delay(3000); 
      digitalWrite(vibrationMotor, LOW);
    }
  }
}
