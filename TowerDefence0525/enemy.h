#ifndef ENEMY_H
#define ENEMY_H

#include <QPoint>
#include <QPixmap>
#include <QtMath>
#include <QObject>
#include <QTimer>

#include "waypoint.h"
#include "towerposition.h"

class Tower;

class MainWindow;  //与MainWindow相互包含头文件会报错。因此先要在这里声明一下，再在cpp中包含

class Enemy : public QObject  //只有继承QObject才可用槽函数
{
    Q_OBJECT  //还需加这个才可用槽函数

public:
    Enemy();
    Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite);

    void draw(QPainter *painter);
    void move();  //移动

    void getAttacked( Tower *tower );  //列入该tower攻击列表
    void getDamaged( int damage );     //受到Bullet伤害
    void getDecelerated(double deceleration);
    void getRemoved();

    QPoint getPos() { return m_pos; }
    int getPosX(){ return m_pos.x(); }
    int getPosY(){ return m_pos.y(); }

    bool ifDecelerated(){return m_ifDecelerated;}

public slots:  //槽函数

    void doActivate();  //激活敌人，使之出现
    void recoverSpeed();

private:

    bool m_active = false;  //是否激活
    MainWindow *m_game;     //指向mainwindow，管理列表在其中

    const QPixmap m_sprite;  //图像

    int m_maxHp;                         //最大生命值
    int m_currentHp;                     //当前生命值
    QList<Tower *> m_attackedTowerList;  //所有攻击来源的tower的列表，用来enemy消失时反馈

    QPoint m_pos;                     //当前位置
    double m_walkingSpeed;            //移动速度
    double m_currentSpeed;
    WayPoint *m_destinationWayPoint;  //当前移动终点

    double m_rotationSprite;  //图像旋转角度（暂未使用）
    bool m_ifDecelerated;
};

#endif // ENEMY_H
