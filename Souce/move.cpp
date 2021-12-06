#include"move.h"
#define compensate 25    //速度补偿
#include<Arduino.h>

void Car_move::run(int v,int t){
    digitalWrite(Rightmotor1,HIGH);  // 右电机前进
    digitalWrite(Rightmotor2,LOW);     
    analogWrite(Rightmotor1,v);       
    analogWrite(Rightmotor2,0);
    digitalWrite(Leftmotor1,HIGH);  // 左电机前进
    digitalWrite(Leftmotor2,LOW);
    analogWrite(Leftmotor1,v + compensate);
    analogWrite(Leftmotor2,0);
    delay(t * 100);   
}
void Car_move::brake(int time)  //刹车，停车
{
    digitalWrite(Rightmotor1,LOW);
    digitalWrite(Rightmotor2,LOW);
    digitalWrite(Leftmotor1,LOW);
    digitalWrite(Leftmotor2,LOW);
    delay(time * 100);            
}

void Car_move::turnleft(int v,int t)         //左转(左轮不动，右轮前进)
{
    digitalWrite(Rightmotor1,HIGH);	// 右电机前进
    digitalWrite(Rightmotor2,LOW);
    analogWrite(Rightmotor1,v); 
    analogWrite(Rightmotor2,0);
    digitalWrite(Leftmotor1,LOW);   //左轮后退
    digitalWrite(Leftmotor2,LOW);
    analogWrite(Leftmotor1,0); 
    analogWrite(Leftmotor2,0);
    delay(t * 100);	
}

void Car_move::turnright(int v,int t)
{
    digitalWrite(Rightmotor1,LOW);	// 右电机前进
    digitalWrite(Rightmotor2,LOW);
    analogWrite(Rightmotor1,0); 
    analogWrite(Rightmotor2,0);
    digitalWrite(Leftmotor1,HIGH);   //左轮后退
    digitalWrite(Leftmotor2,LOW);
    analogWrite(Leftmotor1,v); 
    analogWrite(Leftmotor2,0);
    delay(t * 100);	
}

void Car_move::left(int v,int t)
{
    digitalWrite(Leftmotor1,LOW);
    digitalWrite(Leftmotor2,HIGH);
    analogWrite(Leftmotor1,0);
    analogWrite(Leftmotor2,v);
    digitalWrite(Rightmotor1,HIGH);
    digitalWrite(Rightmotor2,LOW);
    analogWrite(Rightmotor1,v);
    analogWrite(Rightmotor2,0);
    delay(t*100);

}
void Car_move::right(int v,int t)
{
    digitalWrite(Leftmotor1,HIGH);
    digitalWrite(Leftmotor2,LOW);
    analogWrite(Leftmotor1,v);
    analogWrite(Leftmotor2,0);
    digitalWrite(Rightmotor1,LOW);
    digitalWrite(Rightmotor2,HIGH);
    analogWrite(Leftmotor1,0);
    analogWrite(Leftmotor2,v);
    delay(t*100);
}

void Car_move::goback(int v,int t)
{
    digitalWrite(Leftmotor1,LOW);
    digitalWrite(Leftmotor2,HIGH);
    analogWrite(Leftmotor1,0);
    analogWrite(Leftmotor2,v);
    digitalWrite(Rightmotor1,LOW);
    digitalWrite(Rightmotor2,HIGH);
    analogWrite(Rightmotor1,0);
    analogWrite(Rightmotor2,v - compensate );
    delay(t*100);
}