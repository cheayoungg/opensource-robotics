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

// ISR motion 설정
const char F = 1; // forward
const char B = 2; // backward
const char R = 3; // right
const char L = 4; // left
const char S = 5; // stop
const char A = 6; // obstacle avoid
int powerLeft = 100, powerRight=100; // motor power setting for left and right motors 0-255
int command; // motion control variable

//ultrasonic sensor 
char trig = 9; // trig pin : 9
char echo = 8; // echo pin : 8
int distance;  // distance
int cutDistance = 5; // 장애물 감지 최소 거리
Ultrasonic ultrasonic(trig,echo); // ultrasonic sensor object

// bluetooth communication
int blueRx = 4; // RXD
int blueTx = 5; // TXD
unsigned char commandReadyFlag = 0;
unsigned char commandBlue, power;   // bluetooth communication command, motor power configuration
SoftwareSerial mySerial(blueRx,blueTx); // 소프트웨어 시리얼통신 포트 생성

// IR sensor
#define BLACK 0
#define WHITE 1
// Motor 회전 방향  
#define CW  1   // clockwise
#define CCW 0   // counter-clockwise
// 왼쪽, 오른쪽 바퀴  
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

// motor shield settings
// 아두이노 PIN 설정
const byte PWMA = 3;      // PWM control (speed) for motor A
const byte PWMB = 11;     // PWM control (speed) for motor B
const byte DIRA = 12;     // Direction control for motor A
const byte DIRB = 13;     // Direction control for motor B

int sensor0, sensor1; // IR sensor reading 0 ~ 1023
int sensorLeft, sensorRight;  // IR sensor : BLACK(0), WHITE(1)


void setup() {
 
  // Initialize ISR
  Timer1.initialize(50000);  //  인터럽트 발생 주파 , FREQ=20 HZ,    COUNT =  1000000 / FREQ
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  
  Serial.begin(9600);  // serial
  mySerial.begin(9600); // software serial bluetooth communication
  setupArdumoto();      // Set all pins as outputs for motor control
}

void loop() {
 
  communication(); // 스마트폰앱과 블루투스 통신
    
  if (commandBlue == 1)  // 스마트폰앱에서 F 버튼 누르면 로봇제어 시작 !!
  {
     motionControl(command);  // 로봇제어
  }
  else
  robotStop();  // 스마트폰앱에서 F 제외한 다른 버튼 누르면 정지 !

 
}

/***********************************************************************************************/
/*************** FUNCTIONS *********************************************************************/
/***********************************************************************************************/


/*******************************  CONTROL ******************************************************/
//  CONTROL : interrupt service routine
void timerIsr()
{
    readSensors();  // read all sensors
    sendCommand();  // send motion control command to application context
}

// CONTROL : control algorithm  
void sendCommand(){
    int leftSensorColor, rightSensorColor; // 왼쪽, 오른쪽 센서 측정 컬러, WHITE : 1, BLACK : 0
    rightSensorColor = colorFinder(sensor0); // A0 오른쪽 IR 센서 측정 컬러
    leftSensorColor = colorFinder(sensor1);  // A1 왼쪽 IR 센서 측정 컬러   
    
    //초음파 센서 측정 거리를 기준으로 해서 제어하는 알고리즘 추가하고 싶다면 ....

    if (distance < 5){
      command = A;
    }
    else if (leftSensorColor == BLACK && rightSensorColor == BLACK){
      command = F; // 전진
    }
    else if (leftSensorColor == WHITE && rightSensorColor == BLACK )  {
      command = R;  // 우회전
    }
     else if (leftSensorColor == BLACK && rightSensorColor == WHITE) { 
      command = L;   // 좌회전
    }
    else if (leftSensorColor == WHITE && rightSensorColor == WHITE ) {
      command = S; // 정지
    }
    else {
      //  미지정
    }
}

// CONTROL : motion control ( 로봇 모션 제어)
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

      case A:   // 장애물 회피 모션 정의
      robotAvoidance();

      default :
      robotStop();
      }
}

/*****************************  END OF CONTROL ******************************/

/***********  SENSING ******************************************************/
// SENSING : Read all sensors
void readSensors(){
  readIrSensor();
  readUltraSensor();
  
}
// SENSING: Read IR sensor measurements
void readIrSensor(){
  sensor0 = 1023 - analogRead(A0);  // sensor Right
  sensor1 = 1023 - analogRead(A1);  // sensor Left
  /*sensorRight = sensor0;
    sensorLeft = sensor1;
    Serial.print("sensor R:");
    Serial.println(sensorRight);
    Serial.print("sensor L:");
   Serial.println(sensorLeft); */
}

// SENSING :  color finder
char colorFinder(int sensorValue) {
   char color;
   const int THRESHOLD = 800;  // 바닥 컬러 정하는 기준값 (threshold value for determining the color of the surface)
  if(sensorValue > THRESHOLD)   // 입력값이 기준값 보다 크면 white, otherwise black
    color = WHITE;
   else
     color = BLACK;
   return color;
}

// SENSING : read ultrasonic sensor
void readUltraSensor(){
   distance = ultrasonic.read();  // read distance in cm
   /*
    Serial.print("distance(CM): ");
    Serial.println(distance); // distance in cm
    delay(500);
    */
}
/************************** END OF SENSING ***********************************/

/*********** COMMUNICATION ***************************************************/
// COMMUNICATION : Bluetooth communication
// 4 BYTES : [255][COMMAND][POWER][100]
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
/****************** END OF COMMUNICATION **********************************/


/******************** MOTOR **********************************************************/
 // SETUP : initialize all pins
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

// MOTOR-CONTROL : direction & speed control
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

// MOTOR-CONTROL : motion-forward
void robotForward(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CW,powerRight);
  driveArdumoto(MOTOR_LEFT,CCW,powerRight);
  
}
// MOTOR-CONTROL : motion-backward
void robotBackward(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CCW,powerRight);
  driveArdumoto(MOTOR_LEFT,CW,powerRight);
  
}
// MOTOR-CONTROL : motion-right
void robotRight(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CCW,powerRight);
  driveArdumoto(MOTOR_LEFT,CCW,powerRight);
}

// MOTOR-CONTROL : motion-left
void robotLeft(int powerLeft, int powerRight)
{
  driveArdumoto(MOTOR_RIGHT,CW,powerRight);
  driveArdumoto(MOTOR_LEFT,CW,powerRight);
}

// MOTOR-CONTROL : motion-stop
void robotStop()
{
  stopArdumoto(MOTOR_LEFT);
  stopArdumoto(MOTOR_RIGHT);
}
 // MOTOR-CONTROL :  stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

/// MOTOR-CONTROL : avoid obstacle
void robotAvoidance()
{
  robotStop();
  robotRight(100,100); // 45도 
  delay(50);
  robotForward();  // 20 cm 
  delay(50);
  robotLeft(100,100);  // 75 deg
  delay(75); 
  robotForward();
}
/*********************** END OF MOTOR ***************************************/


/////////// DEPRECATED /////////////////////////////////////////
// 로봇제어 함수
/*
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
 */
