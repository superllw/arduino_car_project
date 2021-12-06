#ifndef _Move_H_
#define _Move_H


class Car_move
{
public:
    void run(int v,int t);          //前进 速度v,时间t
    void brake(int t);              //停止
    void turnleft(int v,int t);     //原地向左
    void turnright(int v,int t);    //
    void left(int v,int t);         //
    void right(int v,int t);        //
    void goback(int v,int t);         //后退

private:
    int Leftmotor1 = 5;             //左电机前进
    int Leftmotor2 = 9;             //左电机后退
    int Rightmotor1 = 6;            //右电机前进
    int Rightmotor2 = 10;           //右电机后退

};

#endif