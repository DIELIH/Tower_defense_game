#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "QPoint"
#include "QPainter"

// 敌人移动的航线，用指针连接下一个的QPoint
class WayPoint
{
public:

    WayPoint();
    WayPoint(QPoint pos);

    //void copy(WayPoint *waypoint);

    void setNextWayPoint(WayPoint *nextPoint);
    void setPos(QPoint pos);

    WayPoint* nextWayPoint() const;
    const QPoint pos() const;

    void draw(QPainter *painter) const;

private:
    QPoint m_pos;
    WayPoint *m_nextWayPoint;
};

#endif // WAYPOINT_H
