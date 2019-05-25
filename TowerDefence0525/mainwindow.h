#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QList>

#include "towerposition.h"
#include "tower.h"
#include "towermagic.h"
#include "towerfreeze.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include "ray.h"
#include "ice.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);    
    ~MainWindow();

    void paintEvent(QPaintEvent *);            //绘图，每类的show()都用在这里
    void mousePressEvent(QMouseEvent *event);  //鼠标点击，购塔

    void loadTowerPositions();  //初始化塔位，用在mainwindow的构造函数中
    void loadWayPoints();       //初始化航点，用在mainwindow的构造函数中

    void drawWave(QPainter *painter);  //绘制一波敌人
    void drawHP(QPainter *painter);    //显示城堡血量
    void drawGold(QPainter *painter);  //显示金钱
    void drawBar(QPainter *painter);   //绘制状态栏
    void drawTime(QPainter *painter);  //绘制时间

    bool canBuyTower() const;            //判断该点是否可以买塔
    void getHpDamage(int damage = 1);    //城堡减血
    void awardGold(int gold);            //获得金钱
    void removedBullet(Bullet *bullet);  //移除子弹
    void removeRay(Ray *ray);
    void removeIce(Ice *ice);

    void rayEffect(Ray *ray);
    void iceEffect(Ice *ice);
    void removedEnemy(Enemy *enemy);     //移除敌人
    void addBullet(Bullet *bullet);      //增加子弹
    void addRay(Ray *ray);
    void addIce(Ice *ice);
    bool findEnemy(Enemy *enemy);        //在List中找到该敌人，用于bullet的hitTartget()

    void doGameOver();  //游戏结束，判断胜负

public slots:

    void updateMap();  //定时刷新界面
    bool loadWave();            //加载下一波敌人，数目与间隔，由人预先制定

private:

    Ui::MainWindow *ui;

    QTimer *timer;  //计时器
    QTimer *totalTimer;

    //当前界面上显示内容的管理列表
    QList<TowerPosition> m_towerPositionsList;  //塔位
    QList<Tower *>       m_towersList;          //塔
    QList<WayPoint *>    m_wayPointsList;	    //航点（敌人移动路线）
    QList<Enemy *>       m_enemyList;           //敌人
    QList<Bullet *>      m_bulletList;          //子弹
    QList<Ray *>         m_rayList;
    QList<Ice *>         m_iceList;

    int m_waves;  //已有波数
    int m_gold;   //金钱
    int m_HP;     //城堡生命值

    bool m_gameLose;  //是否失败
    bool m_gameWin;   //是否胜利

};

#endif // MAINWINDOW_H
