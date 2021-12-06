/*
    //BUILT AT 
    模式1:巡线
    模式2.避障
    模式3.跟随
    //行驶功能      正常            11.13
    //舵机转向      正常            11.13
    //按钮功能      正常            11.16
    //显示屏        正常            11.14
    //测距          正常            11.15
    //整体调试
*/

#include<Servo.h>
#include<LiquidCrystal.h>
#include<move.h>

Car_move mycar;

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



//定义按钮接口
int Button_1 = A0;              //翻页按钮
int Button_2 = A1;              //确认按钮
//定义超声波接受，发送接口
int Trig = A4;
int Echo =A5;
//声明led屏
LiquidCrystal lcd(3,4,7,8,11,12,13);
const int SensorRight = A2;   	//右循迹红外传感器(P3.2 OUT1)
const int SensorLeft = A3;     	//左循迹红外传感器(P3.3 OUT2)

int SL;    //左循迹红外传感器状态
int SR;    //右循迹红外传感器状态

int servopin=2;//设置舵机驱动脚到数字口2
int myangle;//定义角度变量
int pulsewidth;//定义脉宽变量
int val;


int menu();                     //菜单
int GetDistance();              //超声波获取距离
void Car_main();                //小车主函数
void Lcd_display(int distance);
void car_tracking();
void car_avoid();
void car_follow();
void goback(int v,int t);
//--------------------------------------------------//
void setup(){
    pinMode(Leftmotor1,OUTPUT);
    pinMode(Leftmotor2,OUTPUT);
    pinMode(Rightmotor1,OUTPUT);
    pinMode(Rightmotor2,OUTPUT);
    pinMode(Button_1,INPUT);
    pinMode(Button_2,INPUT);
    pinMode(Trig,OUTPUT);
    pinMode(Echo,INPUT);
    pinMode(SensorRight, INPUT);    
    pinMode(SensorLeft, INPUT); 
    pinMode(servopin,OUTPUT);//设定舵机接口为输出接口
    // lcd.begin(16,2);
    //初始化串口
    Serial.begin(9600);
  
}

void loop(){
    Car_main();
    // test();

}

//-------------------------------------------------//

int menu()
{
    int state = 0;
    while(1)                   //确认按钮没被按下时
    {
        lcd.clear();
        lcd.setCursor(4,1);
        lcd.print("1.Tracking");
        state++;
        delay(200);
        while(1)
        {
            if(digitalRead(Button_1)||digitalRead(Button_2))
            {
                break;
            }
        }
        if(digitalRead(Button_2)==1)                   //确认1功能
        {
           goto zhao;
        }else{
            lcd.clear();
            lcd.setCursor(4,1);
            lcd.print("2.Avoid");
            state++;
            delay(200);
            while(1)
            {
                if(digitalRead(Button_1)==1||digitalRead(Button_2)==1)
                {
                    break;
                }
            }
            if(digitalRead(Button_2)==1)                  //确认2功能
            {
               goto zhao;
            }else{
                lcd.clear();
                lcd.setCursor(4,1);
                lcd.print("3.follow");
                state++;
                delay(200);
                while(1)
                {
                    if(digitalRead(Button_1)==1||digitalRead(Button_2)==1)
                    {
                        break;
                    }
                }
                    if(digitalRead(Button_2) == 1){
                      goto zhao;
                    }else{
                      state = 0;
                    
                    }
                
            }
        }
    }
    zhao:
    return state;
}
void car_tracking(){
    digitalWrite(2,LOW);
    while(1)
    {
        SR = digitalRead(SensorRight);
        SL = digitalRead(SensorLeft);
        if (SL == LOW&&SR==LOW)
            mycar.run(150,0);  
        else if (SL == HIGH & SR == LOW)// 左循迹红外传感器,检测到信号
        
            mycar.turnleft(150,0);
        else if (SR == HIGH & SL == LOW) // 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右转  
            mycar.turnright(150,0);
        else // 都是白色, 停止
            mycar.brake(10);
    }
}

void car_avoid(){
    int Distance[2];
    //Distance[0]:  左边的距离，Distance[1]:中间的距离，Distance[2]：右边的距离
    while(1)
    {
        Distance[1] =  GetDistance();           //中间的距离
        if(Distance[1] < 30)
        {
            mycar.goback(150,2);
            mycar.brake(2);
            for(int i = 0;i<=50;i++){
                servopulse(0);
            }
            Distance[2] = GetDistance();        //右边的距离
            for(int i = 0;i<=50;i++){
                servopulse(170);
            }
            Distance[0] = GetDistance();        //左边的距离
            for(int i = 0;i<=50;i++){
                servopulse(80);
            }
            if((Distance[0] < 30) && (Distance[2] < 35))
                mycar.left(150,0.7);
            else if(Distance[0] > Distance[2])
            {
                mycar.turnleft(200,3);
                mycar.brake(1);
            }
            else
            {
                mycar.turnright(200,3);
                mycar.brake(1);
            }
        }
        else
        {
            mycar.run(150,0);
        }
    }
}

int GetDistance(){
    digitalWrite(Trig,LOW);
    delayMicroseconds(2);
    digitalWrite(Trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig,LOW);
    int distance = pulseIn(Echo,HIGH);
    distance /= 58;
    return distance;
}

void car_follow()
{
    for(int i;i<15;i++){
            servopulse(80);
    }
    while(1)
    {
        int distance = GetDistance();
        if(distance < 10){
            mycar.goback(100,1);
            delay(10);
        }else if(distance == 10){
            mycar.brake(1);
            delay(10);
        }else if((distance > 10) && (distance < 30)){
            mycar.run(100,1);
            delay(10);
        }else{
            mycar.brake(1);
            delay(10);
        }
        // Serial.println(distance);
    }
}

void Car_main()
{
    int choose = menu();
    switch (choose)
    {
    case 1:
    //循迹功能：
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("Now is Tracking");
        car_tracking();
        // while(1);
        break;
    case 2:
    //避障
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("Now is Avoid");
        car_avoid();
        break;
    case 3:
    //跟随功能：
        lcd.clear();
        lcd.setCursor(1,1);
        lcd.print("Now is follow");
        car_follow();
        break;
    default:
        lcd.clear();
        lcd.setCursor(4,1);
        lcd.print("error!");
        break;
    }
    
}

void test()
{
    mycar.turnleft(200,3);          //90度左转
    // mycar.turnright(200,3);         //90度右转
    while(1);
}

void servopulse(int myangle)
{
    pulsewidth=(myangle*11)+500;
    digitalWrite(servopin,HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servopin,LOW);
    delay(20-pulsewidth/1000);
}