// 1-2상 여자방식
//출처: https://juahnpop.tistory.com/127 [Example & Tutorial]

#define motorPin1 11 // IN1
#define motorPin2 10 // IN2
#define motorPin3 9 // IN3
#define motorPin4 8 // IN4
#define step 4096 // 1바퀴 스텝수

int pinArray[4] = { motorPin1, motorPin2, motorPin3, motorPin4 };
int CW[8] = {
                0b1000,
                0b1100,
                0b0100,
                0b0110,
                0b0010,
                0b0011,
                0b0001,
                0b1001
            };
 
int CCW[8] = {
                0b1000,
                0b1001,
                0b0001,
                0b0011,
                0b0010,
                0b0110,
                0b0100,
                0b1100
            };
void setup(){
    // 스텝모터 드라이브 보드의 IN 연결핀 출력으로 설정
    for(int i = 0 ; i < 4 ; i++ ) pinMode(pinArray[i], OUTPUT);
}
 
void loop(){
    // CW방향 1바퀴 후, CCW 1바퀴 
    int temp;
    for(int i = 0 ; i < step ; i++)
    {
        temp = i % 8;
        digitalWrite(pinArray[0], bitRead(CW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CW[temp], 3));
        delay(1);
    }
    delay(1000);
 
    for(int i = 0 ; i < step ; i++)
    {
        temp = i % 8;
        digitalWrite(pinArray[0], bitRead(CCW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CCW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CCW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CCW[temp], 3));
        delay(1);
    }
    delay(1000);
}
