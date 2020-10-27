// sketch_serial_comm_3Bytes.ino
// serial_comm_binary_3Bytes.py
// 3 bytes(character) data transfer between Raspberry pi and Arduino

#define  NUM_OF_RECEIVED_BYTES    (3)  
 bool sendReady = 0;
 char receivedData[NUM_OF_RECEIVED_BYTES];

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
   
  // if we get a valid byte, read analog ins:
  if (Serial.available() == NUM_OF_RECEIVED_BYTES) {
    // get incoming byte:
    for (int i=0; i < NUM_OF_RECEIVED_BYTES; i++){
        receivedData[i] = Serial.read();
    }
      sendReady = 1;
  }
    
    if(sendReady) {
       // send back received values values:
    Serial.write(receivedData[0]);
    Serial.write(receivedData[1]);
    Serial.write(receivedData[2]);
    //delay(1000);
    }
    sendReady = 0;
}
