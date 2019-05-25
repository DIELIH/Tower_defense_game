#include "bullet.h"
#include "enemy.h"
#include "mainwindow.h"

#include <QAnimationDriver>
#include <QPropertyAnimation>
#include <QList>

using namespace std;

//构造函数，各项含义见.h
Bullet::Bullet(QPoint startPos, QPoint targetPos, int damage, Enemy *targetEnemy, MainWindow *game)
    : m_sprite(QPixmap(":/pics/bullet.png"))
    , m_game(game)
    , m_damage(damage)
    , m_targetEnemy(targetEnemy)
    , m_startPos(startPos)
    , m_targetPos(targetPos)
    , m_currentPos(startPos)
{}

void Bullet::move()  //子弹移动
{
    static const int duration = 100;  // 存放动画持续时间（在100毫秒内击中敌人）

    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentPos");
    animation->setDuration(duration);      //设置持续时间
    animation->setStartValue(m_startPos);  //设置起点坐标
    animation->setEndValue(m_targetPos);   //设置终点坐标

    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));
        //关联animation->finished()与Bullet的hitTarget()：动画结束,触发 击中敌人

    animation->start();  //动画开始
        //animation->start()后，动画自动进行，不用再管，无须每帧设置中途各点坐标
}

void Bullet::hitTarget()  //击中敌人时，是与animation->finished()关联的
{
    // 可能多个炮弹击中敌人,而其中一个已将其消灭,导致敌人已被delete
    // 因此先判要断敌人是否还在
    if ( m_game->findEnemy(m_targetEnemy) )
        //Returns -1 if no item matched.
        { m_targetEnemy->getDamaged(m_damage); }  //目标敌人受到伤害

    m_game->removedBullet(this);  //令mainwindow删除子弹
}

void Bullet::draw(QPainter *painter)
{
    painter->drawPixmap(m_currentPos - QPoint(), m_sprite);
}
