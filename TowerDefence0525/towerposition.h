#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPainter>

class TowerPosition
{
public:
    TowerPosition(QPoint pos, const QPixmap &sprite = QPixmap(":/pics/towerposition.png"));

    void setHasTower(bool hasTower = true);
    bool containPoint(const QPoint &pos) const;
    bool hasTower() const;
    const QPoint centerPos() const;

    void draw(QPainter *painter) const;

private:
    QPoint		m_pos;//塔位坐标
    bool		m_hasTower;//该处是否有塔
    QPixmap		m_sprite;//保存绘制图片

    static const QSize ms_fixedSize;
};

#endif // TOWERPOSITION_H
