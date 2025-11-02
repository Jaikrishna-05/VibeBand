// ESP32 Transmitter Code for 433MHz RF Module
int TX_pin = 5;        // 433MHz transmitter data pin (you can change this)
int front = 18;    // Button pins - adjust these to your wiring
int back = 19;
int left = 21;
int rght = 22;

void setup() {
  pinMode(TX_pin, OUTPUT);
  pinMode(front, INPUT_PULLUP);  // Using internal pullup resistors
  pinMode(back, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(rght, INPUT_PULLUP);
}

void loop() {
  // Note: INPUT_PULLUP means buttons are active LOW (pressed = LOW)
  if(!digitalRead(front))
  {
    write_freq(1000);
  }
  else if(!digitalRead(back))
  {
    write_freq(1500);
  }
  else if(!digitalRead(left))
  {
    write_freq(2000);
  }
  else if(!digitalRead(rght))
  {
    write_freq(2500);
  }  
  else
  {
    write_freq(3000);  // No button pressed
  }
}

void write_freq(int DELAY)
{
  digitalWrite(TX_pin, LOW);
  delayMicroseconds(DELAY);
  digitalWrite(TX_pin, HIGH);
  delayMicroseconds(DELAY);
}
