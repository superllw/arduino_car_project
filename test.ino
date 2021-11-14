/*
    //BUILT AT 
    模式1：避障停止
    模式2.
    模式3.
    模式4.
    //行驶功能正常    11.13
    //舵机转向正常      11.13
    //按钮功能
    //显示屏
    //测距
    //整体调试
*/

#include<Servo.h>
#include<LiquidCrystal.h>
//定义运动状态
int STOP = 0;
int GO = 1;
int BACK = 2;
int LEFT = 3;
int RIGHT = 4;

//定义电机接口
int Leftmotor1 = 5;             //左电机后退
int Leftmotor2 = 9;             //左电机前进
int Rightmotor1 = 6;            //右电机前进
int Rightmotor2 = 10;           //右电机后退
//定义舵机
Servo Myservo;
// int Myservo = 2;
//定义按钮接口
int Button = A0;

//定义超声波接受，发送接口
int Trig = A4;
int Echo =A5;
//声明led屏
LiquidCrystal lcd(3,4,7,8,11,12,13);


//函数
//--------------------运动部分----------------------//
void run(int v,int t);          //前进 速度v,时间t
void brake(int t);              //停止
void turnleft(int v,int t);     //原地向左
void turnright(int v,int t);    //
void left(int v,int t);         //
void right(int v,int t);        //
void back(int v,int t);         //后退
//-------------------传感器部分---------------------//
void key_scan();                //按键状态
int GetDistance();              //超声波获取距离
void Car_main();                //小车主函数
void Lcd_display(int distance);

//--------------------------------------------------//
void setup(){
    pinMode(Leftmotor1,OUTPUT);
    pinMode(Leftmotor2,OUTPUT);
    pinMode(Rightmotor1,OUTPUT);
    pinMode(Rightmotor2,OUTPUT);
    pinMode(Button,INPUT);
    pinMode(Trig,OUTPUT);
    pinMode(Echo,INPUT);
    // pinMode(Myservo,OUTPUT);
    Myservo.attach(2);
    lcd.begin(16,2);
    //初始化串口
    Serial.begin(9600);

}

void loop(){
    Car_main();

}

void run(int v,int t)     // 前进
{
    digitalWrite(Rightmotor1,HIGH);  // 右电机前进
    digitalWrite(Rightmotor2,LOW);     
    analogWrite(Rightmotor1,v);       
    analogWrite(Rightmotor2,0);
    digitalWrite(Leftmotor1,HIGH);  // 左电机前进
    digitalWrite(Leftmotor2,LOW);
    analogWrite(Leftmotor1,v);
    analogWrite(Leftmotor2,0);
    delay(t * 100);   
}

void brake(int time)  //刹车，停车
{
    digitalWrite(Rightmotor1,LOW);
    digitalWrite(Rightmotor2,LOW);
    digitalWrite(Leftmotor1,LOW);
    digitalWrite(Leftmotor2,LOW);
    delay(time * 100);            
}

void turnleft(int v,int t)         //左转(左轮不动，右轮前进)
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

void turnright(int v,int t)
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

void left(int v,int t)
{
    digitalWrite(Leftmotor1,LOW);
    digitalWrite(Leftmotor2,HIGH);
    analogWrite(Leftmotor1,0);
    analogWrite(Leftmotor2,v);
    digitalWrite(Rightmotor1,HIGH);
    digitalWrite(Rightmotor2,LOW);
    analogWrite(Leftmotor1,v);
    analogWrite(Leftmotor2,0);
    delay(t*100);

}
void right(int v,int t)
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

void back(int v,int t)
{
    digitalWrite(Leftmotor1,LOW);
    digitalWrite(Leftmotor2,HIGH);
    analogWrite(Leftmotor1,0);
    analogWrite(Leftmotor2,v);
    digitalWrite(Rightmotor1,LOW);
    digitalWrite(Rightmotor2,HIGH);
    analogWrite(Leftmotor1,0);
    analogWrite(Leftmotor2,v);
    delay(t*100);

}

void key_scan()
{
    int State = 1;
    if(!digitalRead(Button))
    {
        //按下按钮
        delay(200);
        State++;
        delay(500);
        if(State>=3)
        {
            State = 1;
        }
    }
    switch (State)
    {
    case 1:
        Serial.println("模式1");
        break;
    case 2:
        Serial.println("模式2");
        break;
    case 3:
        Serial.println("模式3");
    default:
        break;
    }
}

int GetDistance()
{
    digitalWrite(Trig,LOW);
    delayMicroseconds(2);
    digitalWrite(Trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig,LOW);
    int distance = pulseIn(Echo,HIGH)/58;
    return distance;
    
    delay(1000);

}

void Lcd_display_test()
{
    lcd.home();
    lcd.print("Lixiang");
    lcd.setCursor(1,2);
    lcd.print("wo shi ni die");
    delay(5000);
    while(!digitalRead(Button)){}
    while (digitalRead(Button))
    {
        delay(500);
        lcd.clear();
    }
    

}

void Lcd_display_m()
{
    
}

void Car_main()
{
    //测试按钮性能
    /*
        Key_scan();
    */
   //测试测距模块
   /*
        int a = GetDistance();
        Serial.println(a);
   */
    //测试显示屏
    // Lcd_display_test();

    
}