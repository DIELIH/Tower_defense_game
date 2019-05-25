#include "ice.h"
#include "enemy.h"
#include "mainwindow.h"

#include <QAnimationDriver>
#include <QPropertyAnimation>
#include <QList>

using namespace std;

Ice::Ice(QPoint position, int targetRadius, double deceleration, MainWindow *game)
    : m_pos(position)
    , m_game(game)
    , m_deceleration(deceleration)
    , m_targetRadius(targetRadius)
{
    m_currentRadius = 0;
}

void Ice::move()
{
    static const int duration = 800;

    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentRadius");
    animation->setDuration(duration);
    animation->setStartValue(0);
    animation->setEndValue(m_targetRadius);

    connect(animation, SIGNAL(finished()),this,SLOT(hitTarget()));

    animation->start();
}

void Ice::hitTarget()
{
    m_game->iceEffect(this);
}

void Ice::draw(QPainter *painter)
{
    painter->save();

    painter->setPen(QColor(0,0,255));
    QPen pen = painter->pen();
    pen.setWidth(40);
    painter->setPen(pen);
    painter->drawEllipse(m_pos, m_currentRadius,m_currentRadius);

    pen.setColor(QColor(0,255,255));
    pen.setWidth(60);
    painter->setPen(pen);
    painter->setOpacity(0.75);
    painter->drawEllipse(m_pos, m_currentRadius-20,m_currentRadius-20);

    painter->restore();
}
