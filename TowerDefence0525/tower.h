#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QPainter>
#include <QObject>
#include <QTimer>

#include "bullet.h"
#include "enemy.h"

class MainWindow;  //与MainWindow相互包含头文件会报错。因此先在这里前置声明，再在cpp中包含

class Tower : public QObject  //只有继承QObject才可用槽函数
{
    Q_OBJECT  //还需加这个才可用槽函数

public:
    Tower(){}
    Tower(QPoint pos, MainWindow *game);
    void draw(QPainter *painter);

    void targetKilled();  //敌人已被消灭

    void beginAttackEnemy(Enemy *enemy);  //开始攻击敌人
    void loseEnemy();

    void clearTargetEnemy(){m_targetEnemy = NULL;}
    void stopFire(){m_fireRateTimer->stop();}

    int getPosX(){return m_pos.x();}
    int getPosY(){return m_pos.y();}
    QPoint getPos(){return m_pos;}
    int getAttackRange(){return m_attackRange;}
    int getDamage(){return m_damage;}
    Enemy *getTargetEnemy(){return m_targetEnemy;}
    MainWindow *getGame(){return m_game;}

    void setSprite(QPixmap pixmap){m_sprite = pixmap;}
    QTimer *getFireRateTimer(){return m_fireRateTimer;}
    void setDamage(int damage){m_damage = damage;}

public slots:

    virtual void shootWeapon();  //发射子弹 //改动

private:

    QPoint m_pos;
    int m_attackRange;
    int m_damage;
    int m_fireRate;
    QPixmap m_sprite;

    //double m_rotationSpreite;  //旋转炮台，暂且不用
    Enemy *m_targetEnemy;
    QTimer *m_fireRateTimer;
    MainWindow *m_game;
};

#endif // TOWER_H
