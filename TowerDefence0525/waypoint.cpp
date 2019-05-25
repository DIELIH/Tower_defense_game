#include "waypoint.h"
#include <QPen>

WayPoint::WayPoint(QPoint pos)
    : m_pos(pos)
    , m_nextWayPoint(NULL)
{
}

void WayPoint::setNextWayPoint(WayPoint *nextPoint)
{
    m_nextWayPoint = nextPoint;
}

void WayPoint::setPos(QPoint pos){
    m_pos = pos;
}

WayPoint* WayPoint::nextWayPoint() const
{
    return m_nextWayPoint;
}

const QPoint WayPoint::pos() const
{
    return m_pos;
}

void WayPoint::draw(QPainter *painter) const
{
    painter->save();
    QPen pen = painter->pen();
    pen.setWidth(10);//改变线宽
    painter->setPen(pen);
    painter->setPen(QColor(0, 255, 0));
    painter->drawEllipse(m_pos, 10, 10);
    painter->drawEllipse(m_pos, 5, 5);

    if (m_nextWayPoint)
        painter->drawLine(m_pos, m_nextWayPoint->m_pos);
    painter->restore();
}
