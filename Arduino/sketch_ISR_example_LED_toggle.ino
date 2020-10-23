#include <TimerOne.h>

int a; // LED toggle
int n; // iteration number
void setup() {
  pinMode(13,OUTPUT);
  Timer1.initialize(1000000); // Period = 0.1
  Timer1.attachInterrupt(timerISR);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void timerISR()
{
 a = a^1; 
 digitalWrite(13, a);
 n++; 
}
