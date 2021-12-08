/*
    IDE选择的是vscode + arduino
    舵机如果接口是9，10的话，可以导入Servo.h库，声明舵机，可以利用其中的write函数进行舵机角度的转换


*/
/*
    BUILT AT                       11.09
    BUILT BY                       LLW

    模式1:巡线      待测试
    模式2.避障      待测试
    模式3.跟随      正常
    //行驶功能      正常            11.13
    //舵机转向      正常            11.13
    //按钮功能      正常            11.16
    //显示屏        正常            11.14
    //测距          正常            11.15
    //整体调试
*/

#include<Servo.h>                   //导入舵机库
#include<LiquidCrystal.h>           //导入屏幕库
#include"move.h"                   //导入移动库

//定义运动状态
int STOP = 0;
int GO = 1;
int BACK = 2;
int LEFT = 3;
int RIGHT = 4;

//定义电机接口
int Leftmotor1 = 5;                 //左电机后退
int Leftmotor2 = 9;                 //左电机前进
int Rightmotor1 = 6;                //右电机前进
int Rightmotor2 = 10;               //右电机后退
//定义舵机
Servo Myservo;
//声明led屏
LiquidCrystal lcd(3,4,7,8,11,12,13);
//声明小车移动
Car_move mycar;

//定义按钮接口
int Button_1 = A0;                  //翻页按钮
int Button_2 = A1;                  //确认按钮
//定义超声波接受，发送接口
int Trig = A4;
int Echo =A5;
//定义红外传感器
const int SensorRight = A2;   	    //右循迹红外传感器(P3.2 OUT1)
const int SensorLeft = A3;     	    //左循迹红外传感器(P3.3 OUT2)

int Hongwai_Left;                   //左循迹红外传感器状态
int Hongwai_Right;                  //右循迹红外传感器状态

int servopin = 2;                   //设置舵机驱动脚到数字口2
int angle;                          //定义角度变量
int pulsewidth;                     //定义脉宽变量


int menu();                         //菜单
int GetDistance();                  //超声波获取距离
void Car_main();                    //小车主函数    
void car_tracking();                //小车循迹
void car_avoid();                   //小车避障
void car_follow();                  //小车跟随

//-------------------------------------------------------------------//
void setup(){
    //小车电机端口设置为输出
    pinMode(Leftmotor1,OUTPUT);     
    pinMode(Leftmotor2,OUTPUT);
    pinMode(Rightmotor1,OUTPUT);
    pinMode(Rightmotor2,OUTPUT);
    //按钮1，2端口设置为输入
    pinMode(Button_1,INPUT);        
    pinMode(Button_2,INPUT);

    pinMode(Trig,OUTPUT);           //超声波发送端口设置为输出
    pinMode(Echo,INPUT);            //超声波接受端口设置为输入
    //红外线端口设置为输入
    pinMode(SensorRight, INPUT);    
    pinMode(SensorLeft, INPUT); 
    //设定舵机接口为输出接口
    //如果采用的舵机接口为9跟10可以采用 Myservo.attach()设置舵机端口
    pinMode(servopin,OUTPUT);       
    
    //初始化串口
    Serial.begin(9600);
  
}

void loop(){
    Car_main();
    // test();

}

//-----------------------------------------------------------------//

int menu()
{
    int state = 0;              //存放返回的值
    while(1)                    //确认按钮没被按下时
    {
        lcd.clear();            //清屏
        lcd.setCursor(4,1);     //从第一行第四列开始写
        lcd.print("1.Tracking");//输出文本
        state++;                //返回值加一
        delay(200);             //延时

        while(1)
        {
            if(digitalRead(Button_1)||digitalRead(Button_2))        //按钮一或者按钮二按下，退出死循环
            {
                break;
            }
        }
        if(digitalRead(Button_2)==1)                   //如果是按钮二按下，表示确认1功能
        {
           goto zhao;                                  //退出外层循环
        }else{                                         //表示按下的是按钮一，进行翻页操作
            lcd.clear();                               //同上
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
               goto zhao;                               //退出外层循环
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
    return state;                                           //返回值，表示选择的模式
}
void car_tracking(){
    
    while(1)
    {
        H = digitalRead(SensorRight);                      //读取右红外线传感器电平
    Hhongwai_Left = digitalRead(SensorLeft);                       //读取左红外线传感器电平
        if Hhongwai_Left == LOW&&H==LOW){                            //左右红外线都为低电平，表示都未检测到黑线
            mycar.run(100,0);                               //小车直走 
            delay(10);
        } 
        else if Hhongwai_Left == HIGH & H == LOW)                    //左红外线传感器为高电平，右红外线传感器为低电平，表示向右偏离
        {
            mycar.turnleft(80,0);                           //小车向左转
            delay(10);
        }
        else if (H == HIGH &Hhongwai_Left == LOW)                    //右红外线传感器为高电平，左红外线传感器为低电平，表示向左偏离
        {
            mycar.turnright(80,0);                          //小车右转
            delay(10);
        }
        else{                                               //都检测到黑线，小车停车
            mycar.brake(10);
        }
        delay(2);
    }
}

void car_avoid(){
    int Distance[2];                                       //声明数组，存放距离
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



void car_follow()
{
    for(int i;i<15;i++){
            servopulse(80);                     //舵机摆正
    }
    while(1)
    {
        int distance = GetDistance();           //读取距离
        if(distance < 10){
            mycar.goback(100,1);                //小于10，后退
            delay(10);
        }else if(distance == 10){               
            mycar.brake(1);                     //等于10，停止
            delay(10);
        }else if((distance > 10) && (distance < 30)){
            mycar.run(100,1);                   //在10与30之间，前进
            delay(10);
        }else{
            mycar.brake(1);                     //大于30，停止
            delay(10);
        }
        // Serial.println(distance);            //串口通讯，返回距离值
    }
}

int GetDistance(){
    /*超声波测距函数，原理见博客：
    https://blog.csdn.net/u012308586/article/details/105747760?ops_request_misc=%25
    7B%2522request%255Fid%2522%253A%2522163897671216780261995044%2522%252C%2522scm%
    2522%253A%252220140713.130102334..%2522%257D&request_id=16389767121678026199504
    4&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_posi
    tive~default-2-105747760.first_rank_v2_pc_rank_v29&utm_term=arduino%E8%B6%85%E5
    %A3%B0%E6%B3%A2%E6%B5%8B%E8%B7%9D&spm=1018.2226.3001.4187*/

    digitalWrite(Trig,LOW);
    delayMicroseconds(2);
    digitalWrite(Trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig,LOW);
    int distance = pulseIn(Echo,HIGH);
    distance /= 58;
    return distance;
}

void servopulse(int angle)                        //舵机转动到angle角度
{
    //原理就是模拟输出PWM值
    pulsewidth=(angle*11)+500;                    //将角度转化为脉宽值
    digitalWrite(servopin,HIGH);
    delayMicroseconds(pulsewidth);                //延时脉宽值的微秒数
    digitalWrite(servopin,LOW);
    delay(20-pulsewidth/1000);                    ////延时周期内剩余时间
}

void Car_main()
{
    int choose = menu();                        //进入主菜单，获取模式返回值
    switch (choose)                             //按返回值进入不同的模式
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
    default:                                        //返回值有误报错
        lcd.clear();
        lcd.setCursor(4,1);
        lcd.print("error!");
        while(1);
        break;
    }
    
}



void test()
{
    //测试函数，用于调试小车部分功能
}

