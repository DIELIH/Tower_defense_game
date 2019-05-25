#ifndef RAY_H
#define RAY_H

#include <QPoint>
#include <QPixmap>
#include <QObject>

#include "enemy.h"

class MainWindow;

class Ray :public QObject
{
    Q_OBJECT
    Q_PROPERTY(int m_currentRadius
               READ currentRadius
               WRITE setCurrentRadius)
public:

    Ray();
    Ray(QPoint position, int targetRadius, int damage, MainWindow *game);

    int currentRadius() { return m_currentRadius; }
    void setCurrentRadius( int radius ) { m_currentRadius = radius; }
    void move();
    void draw( QPainter *painter );

    QPoint getPos(){return m_pos;}
    int getTargetRadius(){return m_targetRadius;}
    int getDamage(){return m_damage;}

public slots:

    void hitTarget();

private:
    MainWindow *m_game;
    int m_damage;
    const QPoint m_pos;
    const int m_targetRadius;
    int m_currentRadius;
};

#endif // RAY_H
