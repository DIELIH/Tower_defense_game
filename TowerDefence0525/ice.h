#ifndef ICE_H
#define ICE_H

#include <QPoint>
#include <QPixmap>
#include <QObject>

#include "enemy.h"

class MainWindow;

class Ice :public QObject
{
    Q_OBJECT
    Q_PROPERTY(int m_currentRadius
               READ currentRadius
               WRITE setCurrentRadius)
public:

    Ice();
    Ice(QPoint position, int targetRadius, double deceleration, MainWindow *game);

    int currentRadius() { return m_currentRadius; }
    void setCurrentRadius( int radius ) { m_currentRadius = radius; }
    void move();
    void draw( QPainter *painter );

    QPoint getPos(){return m_pos;}
    int getTargetRadius(){return m_targetRadius;}
    int getDeceleration(){return m_deceleration;}

public slots:

    void hitTarget();

private:
    MainWindow *m_game;
    double m_deceleration;
    const QPoint m_pos;
    const int m_targetRadius;
    int m_currentRadius;
};

#endif // ICE_H
