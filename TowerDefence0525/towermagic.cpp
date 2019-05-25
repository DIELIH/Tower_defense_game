#include "towermagic.h"
#include "QTimer"
#include "mainwindow.h"
#include "ray.h"

TowerMagic::TowerMagic(QPoint pos, MainWindow *game)
    :Tower(pos,game)
{
    setDamage(5);
    setSprite(QPixmap(":/pics/towermagic.png"));
}

void TowerMagic::shootWeapon()
{
    Ray *ray = new Ray(Tower::getPos(), Tower::getAttackRange(),Tower::getDamage(),Tower::getGame());
    ray->move();
    Tower::getGame()->addRay(ray);
}
