// 360도 회전하려면 2048 스텝이 필요.
// 아두이노 디지털 8,9,10,11번 핀을 
// 스테퍼모터 IN1,IN3,IN2,IN4에 연결
// 인스턴스 생성시 1-3-2-4 순서로 대응되는 핀 번호 입력
// 구동방식 : 2상 여자방식
// 시리얼통신으로 모터 제어

#include <Stepper.h>
const int stepsPerRevolution = 2048;  
const int IN1=8, IN2=9, IN3=10, IN4=11;
int steps = 2048; // step to rotate ( full circle)
Stepper motor(stepsPerRevolution,IN1,IN3,IN2,IN4); 

void setup() {
  // put your setup code here, to run once:
  while(!Serial); // wait until serial is ready
  Serial.begin(9600);
  motor.setSpeed(10); // 스텝모터 속도를 10RPM으로 설정
  Serial.println("회전각도 입력(DEG):");
}
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    int val = Serial.parseInt(); // 회전각 (-360~360) int형으로 읽기
    Serial.println(val);
    val = map(val,-360,360,-stepsPerRevolution,stepsPerRevolution);
    motor.step(val);
    
    delay(50);
  }
}

