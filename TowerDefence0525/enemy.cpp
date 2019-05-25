#include "enemy.h"
#include <QtMath>
#include <QTimer>

#include "mainwindow.h"
#include "tower.h"

//碰撞函数，非成员，用在move()中判断敌人是否到达航点
//简单数学而已
bool collisionWithCircle(QPoint point1, int radius1, QPoint point2, int radius2)
{
    const int xdif = point1.x() - point2.x();
    const int ydif = point1.y() - point2.y();
    const int distance = qSqrt(xdif * xdif + ydif * ydif);
    if (distance <= radius1 + radius2)
        return true;
    return false;
}

//该构造函数使用在MainWindow的loadWave()中，因为enemy出现是被人制定好的
Enemy::Enemy(WayPoint *startWayPoint, MainWindow *game, const QPixmap &sprite)
    //*startWayPoint传递的是MainWindow管理的航点列表（m_wayPointsList）中最后一个元素（即第一个航点）
    : QObject(0)  //必须要加，我也不知为啥
    , m_pos(startWayPoint->pos())
    , m_sprite(sprite)
{
    m_maxHp = 40;
    m_currentHp = 40;
    m_active = false;
    m_walkingSpeed = 2.0;
    m_currentSpeed = 2.0;
    m_destinationWayPoint = startWayPoint->nextWayPoint() ;
    m_rotationSprite = 0.0;
    m_game = game;
    m_ifDecelerated = false;
}

void Enemy::draw(QPainter *painter)
{
    //未激活，不画出
    if (!m_active){ return; }

    static const int Health_Bar_Width = 60;   //血条长度
    static const int Health_Bar_Height = 5;   //血条高度
      //敌人图像大小是60

    painter->save();  //save()与restore()对应使用

    painter->setPen(Qt::NoPen);  //画笔风格

    // 绘制血条
    //血条左上角坐标
    QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 , -30 - 2 * Health_Bar_Height);
    // 绘制红色方框，表示最大生命,固定大小不变
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, Health_Bar_Height));
    painter->drawRect(healthBarBackRect);
    // 绘制绿色方框，表示当前生命,受m_currentHp / m_maxHp的变化影响
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, Health_Bar_Height));
    painter->drawRect(healthBarRect);

    // 偏移坐标,由中心点到左上角
    static const QPoint offsetPoint(-60 / 2,-60 / 2);
    //painter->rotate(m_rotationSprite); //旋转，暂且不用

    // 绘制敌人  
    painter->drawPixmap(m_pos + offsetPoint, m_sprite);
    if(ifDecelerated())
    {
        painter->setBrush(QColor(0,0,255));
        painter->setOpacity(0.5);
        painter->drawRect(m_pos.x() + offsetPoint.x(), m_pos.y() + offsetPoint.y(), 60, 60);
    }
    painter->restore();
}

void Enemy::move(){
    //未激活，不移动
    if (!m_active) {return;}

    //如果抵达航点
    if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))  //碰撞函数，见本页最上
        //判断距离用1不用0，因为时差因素，不可能恰好坐标重合
    {
        // 如果还有下一个航点，继续走
        if (m_destinationWayPoint->nextWayPoint())
        {
            m_pos = m_destinationWayPoint->pos();
            m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
        }
        // 如果进入基地，城堡受到伤害，塔停止攻击，移除敌人
        else
        {
            m_game->getHpDamage();
            foreach (Tower *tower, m_attackedTowerList)
            {
                tower->clearTargetEnemy();
                tower->stopFire();
            }
            m_game->removedEnemy(this);
            return;
        }
    }

    //如果还在前往航点
    QPoint targetPoint = m_destinationWayPoint->pos();  //  目标航点的坐标
       //（未来：修改m_walkingSpeed可以添加移动状态,加快,减慢,另设m_standardWalkingSpeed为基准值）
    QVector2D normalized(targetPoint - m_pos);  //  这是由当前坐标指向目标航点的2D向量
    normalized.normalize();  //进行向量标准化：normalized被修改为该2D向量方向上的单位向量
    m_pos = m_pos + normalized.toPoint() * m_currentSpeed;  //修改坐标，实现移动
    //m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) ; //旋转，暂且不用

}

void Enemy::doActivate()
{
    m_active = true;
}

void Enemy::getRemoved()
{
    //if (m_attackedTowerList.empty())  //？没有攻击塔，敌人为什么会阵亡？
        //return;

    //通知攻击塔，此敌人已阵亡
    foreach (Tower *attacker, m_attackedTowerList)
        attacker->targetKilled();

    // 通知game,此敌人已经阵亡
    m_game->removedEnemy(this);
}


void Enemy::getDamaged(int damage)
{
    //m_game->audioPlayer()->playSound(LaserShootSound);  //音效，先不做
    m_currentHp -= damage;  //减血

    // 若阵亡,则需要移除
    if (m_currentHp <= 0){
        //m_game->audioPlayer()->playSound(EnemyDestorySound);  //音效，先不做
        m_game->awardGold(200);  //获得金钱（修改：由敌人属性决定）
        foreach (Tower *tower, m_attackedTowerList)
            tower->loseEnemy();
        getRemoved();  //移除敌人
    }
}

void Enemy::getAttacked(Tower *tower){
    m_attackedTowerList.push_back(tower);
}

void Enemy::getDecelerated(double deceleration)
{
    //m_game->audioPlayer()->playSound(LaserShootSound);  //音效，先不做
    m_currentSpeed -= deceleration;  //减速
    m_ifDecelerated = true;
    QTimer::singleShot( 3000, this, SLOT(recoverSpeed()));
}

void Enemy::recoverSpeed()
{
    m_currentSpeed = m_walkingSpeed;
    m_ifDecelerated = false;
}
