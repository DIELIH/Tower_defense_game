#include "towerfreeze.h"
#include "QTimer"
#include "mainwindow.h"
#include "ice.h"

TowerFreeze::TowerFreeze(QPoint pos, MainWindow *game)
    :Tower(pos,game)
{
    setDamage(0);
    m_deceleration = 1.0;
    setSprite(QPixmap(":/pics/towerfreeze.png"));
}

void TowerFreeze::shootWeapon()
{
    Ice *ice = new Ice(Tower::getPos(), Tower::getAttackRange(), m_deceleration, Tower::getGame());
    ice->move();
    Tower::getGame()->addIce(ice);
}
