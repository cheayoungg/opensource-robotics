// sketch_serial_comm.ino
// single character communication between Arduino and Raspberry Pi
int inByte = 0;         // incoming serial byte

void setup() 
{

    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect
      }
  // send a byte to establish contact until receiver responds
    establishContact();
}

void loop() {
   static unsigned char count = 0;
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    if (inByte == 'A')
    {
      count++;
      Serial.write(count % 256);
    }   
   
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}
