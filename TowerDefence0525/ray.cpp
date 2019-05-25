#include "ray.h"
#include "enemy.h"
#include "mainwindow.h"

#include <QAnimationDriver>
#include <QPropertyAnimation>
#include <QList>

using namespace std;

Ray::Ray(QPoint position, int targetRadius, int damage, MainWindow *game)
    : m_pos(position)
    , m_game(game)
    , m_damage(damage)
    , m_targetRadius(targetRadius)
{
    m_currentRadius = 0;
}

void Ray::move()
{
    static const int duration = 800;

    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentRadius");
    animation->setDuration(duration);
    animation->setStartValue(0);
    animation->setEndValue(m_targetRadius);

    connect(animation, SIGNAL(finished()),this,SLOT(hitTarget()));

    animation->start();
}

void Ray::hitTarget()
{
    m_game->rayEffect(this);
}

void Ray::draw(QPainter *painter)
{
    painter->save();

    painter->setPen(QColor(255,0,0));
    QPen pen = painter->pen();
    pen.setWidth(40);
    painter->setPen(pen);
    painter->drawEllipse(m_pos, m_currentRadius,m_currentRadius);

    pen.setColor(QColor(255,255,0));
    pen.setWidth(60);
    painter->setPen(pen);
    painter->setOpacity(0.75);
    painter->drawEllipse(m_pos, m_currentRadius-20,m_currentRadius-20);

    painter->restore();
}
