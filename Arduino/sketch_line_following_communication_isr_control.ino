/******************************************************************************************/
//        MOTOR RIGHT     ==>  MOTOR DRIVER A
//        MOTOR LEFT      ==>  MOTOR DRIVER B
//        IR SENSOR RIGHT ==>  A0 (SENSOR0)
//        IR SENSOR LEFT  ==>  A1 (SENSOR1)
//        ECHO            ==>  8
//        TRIG            ==>  9
//        TXD             ==>  4
//        RXD             ==>  5
//
/******************************************************************************************/
#include <SoftwareSerial.h>
#include <Ultrasonic.h>
#include <TimerOne.h>

// ISR
// motion 설정
const char F = 1;
const char B = 2;
const char R = 3;
const char L = 4;
const char S = 5;
int powerLeft = 100, powerRight=100;
int command;

//ultrasonic sensor
char trig = 9;
char echo = 8;
int distance;
int cutDistance = 5;
Ultrasonic ultrasonic(trig,echo); // ultrasonic sensor object

// bluetooth communication
int blueRx = 4;
int blueTx = 5;
unsigned char commandReadyFlag = 0;
unsigned char commandBlue, power;
SoftwareSerial mySerial(blueRx,blueTx); // 소프트웨어 시리얼통신 포트 생성

// sensor
#define BLACK 0
#define WHITE 1
// 회전 방향  
#define CW  1   // clockwise
#define CCW 0   // counter-clockwise

// 왼쪽, 오른쪽 바퀴  
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

// motor settings
// 아두이노 PIN 설정
const byte PWMA = 3;      // PWM control (speed) for motor A
const byte PWMB = 11;     // PWM control (speed) for motor B
const byte DIRA = 12;     // Direction control for motor A
const byte DIRB = 13;     // Direction control for motor B

int sensor0, sensor1; // sensor reading 0 ~ 1023
int sensorLeft, sensorRight;  // BLACK(0), WHITE(1)


void setup() {
  // put your setup code here, to run once:

  // Initialize ISR
  Timer1.initialize(50000);  //  20 HZ 
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  
  Serial.begin(9600);
  mySerial.begin(9600);
  setupArdumoto();      // Set all pins as outputs
}

void loop() {
 
  communication();
    
  if (commandBlue == 1)
  {
     motionControl(command);
  }
  else
  robotStop();

 
}

// ultrasonic
void ultra(){
   distance = ultrasonic.read();
   /*
    Serial.print("distance(CM): ");
    Serial.println(distance); // distance in cm
    delay(500);
    */
}

 // setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

void driveArdumoto(byte motor, byte dir, byte spd)
{
  if(motor == MOTOR_RIGHT)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if(motor == MOTOR_LEFT)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

void robotForward(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CW,powerRight);
  driveArdumoto(MOTOR_LEFT,CCW,powerRight);
  
}

void robotBackward(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CCW,powerRight);
  driveArdumoto(MOTOR_LEFT,CW,powerRight);
  
}

void robotRight(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CCW,powerRight);
  driveArdumoto(MOTOR_LEFT,CCW,powerRight);
}

void robotLeft(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CW,powerRight);
  driveArdumoto(MOTOR_LEFT,CW,powerRight);
}

void robotStop()
{
  stopArdumoto(MOTOR_LEFT);
  stopArdumoto(MOTOR_RIGHT);
}
 // stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}


// Read sensor measurements
void readSensors(){
  sensor0 = 1023 - analogRead(A0);  // sensor Right
  sensor1 = 1023 - analogRead(A1);  // sensor Left
  /*sensorRight = sensor0;
    sensorLeft = sensor1;
    Serial.print("sensor R:");
    Serial.println(sensorRight);
    Serial.print("sensor L:");
   Serial.println(sensorLeft); */
}

// color finder
char colorFinder(int sensorValue) {
   char color;
   const int THRESHOLD = 800;
  if(sensorValue > THRESHOLD)
    color = WHITE;
   else
     color = BLACK;
   return color;
}


////////////////////////////////////////////////////////////////
// 로봇제어 함수
/////////////////////////////////////////////////////////////////

void robotControl(int sensorLeft, int sensorRight){
  if (sensorLeft == BLACK && sensorRight == BLACK){
      robotForward(100,100);
   
  }
            
   else if (sensorLeft == BLACK && sensorRight == WHITE) {
              robotLeft(100,100);
              delay(30);
    }
   else if (sensorLeft == WHITE && sensorRight == BLACK) {
              robotRight(100,100);
              delay(30);
    }
   else
              robotStop();
 }


// Bluetooth communication

void communication(){
  if (mySerial.available() >= 4){
    //Serial.println("data arrived ");
    unsigned char buffer[4];

    // read data from buffer and save to array
    for (char i=0; i<4; i++){
      buffer[i] = mySerial.read();
    }

    // verify data
    if ( buffer[0]== 255 && buffer[3] == 100) {
         commandBlue = buffer[1];
         power = buffer[2];   
        /*
         Serial.print("command:");
         Serial.println(command);
          Serial.print("power:");
         Serial.println(power);
         */
    }    
  }
}


/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
    readSensors();
    sendCommand();  
}

/////////////////////////////////////////
 // SUB-ROUTINES
////////////////////////////////////////

void sendCommand(){
    int leftSensorColor, rightSensorColor;
    rightSensorColor = colorFinder(sensor0); // A0
    leftSensorColor = colorFinder(sensor1);  // A1   
    
    if (leftSensorColor == BLACK && rightSensorColor == BLACK){
      command = F;
    }
    else if (leftSensorColor == WHITE && rightSensorColor == WHITE ) {
      command = S;
    }
    else if (leftSensorColor == WHITE && rightSensorColor == BLACK )  {
      command = R;
    }
     else if (leftSensorColor == BLACK && rightSensorColor == WHITE) { 
      command = L;
    }
    else {
      //
    }
}


void motionControl(int command){
  switch(command)
    {
      case F:
      robotForward(powerLeft, powerRight);     
      break;
      
      case B:
      robotBackward(powerLeft, powerRight);
      break;
      
      case R:
      robotRight(powerLeft, powerRight);
      delay(30);
      break;
      
      case L:
      robotLeft(powerLeft, powerRight);
      delay(30);
      break;
      
      case S:
      robotStop();
      break;

      default :
      robotStop();
      }
}
