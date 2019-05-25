#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QPixmap>
#include <QObject>

#include "enemy.h"

class MainWindow;  //与MainWindow相互包含头文件会报错。因此先要在这里声明一下，再在cpp中包含

class Bullet : public QObject //只有继承自QObject才可用槽函数(slots)和属性(property)
{
    Q_OBJECT  //还需添加这个
    //属性系统（The Property System），将m_currentPos注册为属性(property)，可用动画（QPropertyAnimation）类及其功能
    //注册属性如下：系统会用READ函数读取变量，用WRITE函数修改变量
    Q_PROPERTY(QPoint m_currentPos   //需要被注册为属性的变量
               READ currentPos       //读取的函数
               WRITE setCurrentPos)  //修改的函数
public:

    Bullet();
    Bullet(QPoint startPos, QPoint targetPos, int damage, Enemy *targetEnemy, MainWindow *game);
        //起点、终点、伤害、指针目标敌人、指针mainwindow

    QPoint currentPos() { return m_currentPos; }              //返回当前坐标
    void setCurrentPos( QPoint pos ) { m_currentPos = pos; }  //修改现在坐标
    void move();                                              //移动动画，QPropertyAnimation在其中
    void draw( QPainter *painter );

public slots:  //槽函数，关联与QPropertyAnimation的end()

    void hitTarget();  //击中敌人，触发效果

private:

    QPixmap m_sprite;    //图像
    MainWindow *m_game;  //指向mainwindow，屏幕上各种东西的管理列表都在其中

    int m_damage;     //伤害，由tower决定，见本构造函数
    Enemy *m_targetEnemy;  //指向目标敌人

    const QPoint m_startPos;   //起始坐标，常量
    const QPoint m_targetPos;  //目标坐标，常量
    QPoint m_currentPos;       //当前坐标，变量

};

#endif // BULLET_H
