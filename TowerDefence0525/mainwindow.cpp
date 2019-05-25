#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmath.h"
#include "QTime"
#include "QTimer"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置计时器
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));  //关联时间与updateMap()函数
    timer->start(10);  //每多少毫秒传递一次时间信号 //帧数

    totalTimer = new QTimer(this);
    totalTimer->start(5*60*1000);

    m_waves = 0;           //初始时已有波数为零
    m_gameLose = false;
    m_gameWin = false;
    m_HP = 10;
    m_gold = 600;

    loadTowerPositions();  //初始化塔位
    loadWayPoints();       //初始化航点
    QTimer::singleShot( 6 * 1000 , this , SLOT(loadWave()));//加载第一波敌人

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //判断游戏结束
    if (m_gameLose || m_gameWin)
    {
        QString text = m_gameWin ? "YOU WIN!!!" : "YOU LOST!!!";
        QPainter painter;
        painter.begin(this);
        QFont font = painter.font();
        font.setPixelSize(50);
        painter.setFont(font);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        painter.end();
        return;
    }

    QPainter *pt = new QPainter();
    pt->begin(this);  //就是一定要加painter的begin()和end()，我也不知道为什么

    //画地图
    pt->drawPixmap(0, 0, QPixmap(":/pics/RUCmap.jpg"));
    //画塔位
    int i;
    for(i=0;i<29;i++) { m_towerPositionsList[i].draw(pt); }  //有29个塔位
    //画塔、航点、敌人
    foreach (Tower *tower, m_towersList) { tower->draw(pt); } //把draw的参数都改成指针了，否则报错。网上说什么深拷贝浅拷贝，不懂。
    foreach (WayPoint *waypoint, m_wayPointsList) { waypoint->draw(pt); }
    foreach (Enemy *enemy, m_enemyList) { enemy->draw(pt); }
    foreach (Bullet *bullet, m_bulletList) { bullet->draw(pt);}
    foreach (Ray *ray, m_rayList) { ray->draw(pt);}
    foreach (Ice *ice, m_iceList) { ice->draw(pt);}
    //显示已有波数、城堡血量、金钱以及状态栏
    //设置字体大小
    QFont font = pt->font();
    font.setPixelSize(25);
    pt->setFont(font);
    drawBar(pt);
    drawWave(pt);
    drawHP(pt);
    drawGold(pt);
    drawTime(pt);

    pt->end();
    delete pt;
}

void MainWindow::loadTowerPositions(){
    QPoint pos[] =
        {
        //共29个点
        QPoint(0, 0),
        QPoint(200, 0),
        QPoint(400, 0),
        QPoint(600, 0),
        QPoint(800, 0),

        QPoint(200, 200),
        QPoint(400, 200),
        QPoint(600, 200),
        QPoint(800, 200),

        QPoint(100, 300),
        QPoint(100, 500),
        QPoint(100, 700),

        QPoint(300, 400),
        QPoint(300, 600),
        QPoint(300, 800),

        QPoint(500, 300),
        QPoint(500, 500),
        QPoint(500, 700),

        QPoint(700, 400),
        QPoint(700, 600),
        QPoint(700, 800),

        QPoint(900, 300),
        QPoint(900, 500),
        QPoint(900, 700),

        QPoint(0, 900),
        QPoint(200, 900),
        QPoint(400, 900),
        QPoint(600, 900),
        QPoint(800, 900),
        };

        int len	= sizeof(pos) / sizeof(pos[0]);

        for (int i = 0; i < len; ++i){
            m_towerPositionsList.push_back(pos[i]);
        }
}

static const int TowerCost = 300;

void MainWindow::mousePressEvent(QMouseEvent *event){
    //点击位置
    QPoint pressPos = event->pos();
    auto it = m_towerPositionsList.begin();
    while (it != m_towerPositionsList.end())
        //end():Returns an iterator pointing to the imaginary item after the last item in the list.
        {
            //如果金额足够，点击位置在范围中，未有塔
            if (canBuyTower()
                    && it->containPoint(pressPos)
                    && ! it->hasTower())
            {
                m_gold -= TowerCost;
                it->setHasTower(); //将塔位状态修改为已有塔
                //Tower *tower = new Tower(it->centerPos(), this); //生成新塔对象
                //TowerMagic *towermagic = new TowerMagic(it->centerPos(), this);
                TowerFreeze *towerfreeze = new TowerFreeze(it->centerPos(), this);

                //m_towersList.push_back(tower); //将新塔添入塔列表中
                //m_towersList.push_back(towermagic);
                m_towersList.push_back(towerfreeze);

                update(); //更新mainwindow
                break;
            }
            it++;
        }
}


bool MainWindow::canBuyTower() const
{
    if (m_gold >= TowerCost)
    { return true; }
    return false;
}

void MainWindow::addBullet(Bullet *bullet){
    m_bulletList.push_back(bullet);

}

void MainWindow::addRay(Ray *ray)
{
    m_rayList.push_back(ray);
}

void MainWindow::addIce(Ice *ice)
{
    m_iceList.push_back(ice);
}

void MainWindow::loadWayPoints()
{
    WayPoint *wayPoint1 = new WayPoint(QPoint(100, 100));
    m_wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(100, 900));
    m_wayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    WayPoint *wayPoint3 = new WayPoint(QPoint(900, 900));
    m_wayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);
}

void MainWindow::getHpDamage(int damage/* = 1*/)
{
    //m_audioPlayer->playSound(LifeLoseSound);  //音乐，暂未使用
    m_HP -= damage;
    if (m_HP <= 0)
    { doGameOver(); }
}

void MainWindow::removedBullet(Bullet *bullet)
{
    m_bulletList.removeOne(bullet);
    delete bullet;
}

void MainWindow::removeRay(Ray *ray)
{
    m_rayList.removeOne(ray);
    delete ray;
}

void MainWindow::removeIce(Ice *ice)
{
    m_iceList.removeOne(ice);
    delete ice;
}

void MainWindow::removedEnemy(Enemy *enemy)
{
    m_enemyList.removeOne(enemy);
    delete enemy;
    //如果屏幕上没有敌人
    if (m_enemyList.empty())
    {
        ++this->m_waves; // 当前波数加1
        // 然后，继续读取下一波
        if (!loadWave())
        {
            // 当没有下一波时，游戏胜利
            // 设置游戏胜利标志为true
            m_gameWin = true;
        }
    }
}

bool MainWindow::loadWave()
{
    //一共6波，每波6个敌人
    if (m_waves >= 6){return false;}
    WayPoint *startWayPoint = m_wayPointsList.back(); // 这里是个逆序的，末项是起点
    int enemyStartInterval = 1000;  //敌人出现间隔
    for (int i = 0; i < 6; i++)
    {
        Enemy *enemy = new Enemy(startWayPoint, this, QPixmap(":/pics/enemy.png"));
        m_enemyList.push_back(enemy);
        QTimer::singleShot( i * enemyStartInterval, enemy, SLOT(doActivate()));
            //singleShot相当于定时器，好处在于不需使用timer，在设置时间到后自动触发SLOT中的函数
    }
    return true;
}

//判断是否在射程内，内容与碰撞函数一模一样
bool ifInRange(QPoint point1, int radius1, QPoint point2, int radius2)
{
    const int xdif = point1.x() - point2.x();
    const int ydif = point1.y() - point2.y();
    const int distance = qSqrt(xdif * xdif + ydif * ydif);
    if (distance <= radius1 + radius2)
        return true;
    return false;
}

void MainWindow::updateMap()  //更新界面：移动敌人，判断tower与enemy的距离
{
    //1.移动敌人
    foreach (Enemy *enemy, m_enemyList)
    {
        enemy->move();
    }

    //2.判断tower与enemy的距离
    foreach (Tower *tower, m_towersList)
    {
        //如果tower没有攻击
        if(tower->getTargetEnemy() == NULL)
        {
            foreach  (Enemy *enemy, m_enemyList)
            {
                if(ifInRange(tower->getPos(), tower->getAttackRange(), enemy->getPos(), 1))
                {
                    tower->beginAttackEnemy(enemy);
                    break;
                }
            }
        }
        //如果tower正在攻击
        else
        {
            if( !ifInRange(tower->getPos(), tower->getAttackRange(), tower->getTargetEnemy()->getPos(), 1))
            {
                tower->loseEnemy();
            }
        }
    }
    update();
}

void MainWindow::doGameOver()
{
    if (!m_gameLose)
    {
        m_gameLose = true;
    }
}

void MainWindow::awardGold(int gold)
{
    m_gold += gold;
    update();
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(400, 5, 200, 25), QString("WAVE : %1").arg(m_waves + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(0, 5, 200, 25), QString("HP : %1").arg(m_HP));
}

void MainWindow::drawGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(m_gold));
}

void MainWindow::drawTime(QPainter *painter)
{
    painter->setPen(QPen(Qt::red));
    int msec = 5*60*1000 - totalTimer->remainingTime();
    int muni = msec/(60*1000);
    int sec = (msec - muni*(60*1000))/1000;
    painter->drawText(QRect(600, 5, 200, 25), QString("TIME : %1").arg(muni)+QString(":%1").arg(sec));
}

void MainWindow::drawBar(QPainter *painter)
{
    painter->setPen(QPen(Qt::green));
    painter->drawRect(QRect(0, 0, 1000, 35));
}

bool MainWindow::findEnemy(Enemy *enemy)
{
    if(m_enemyList.indexOf(enemy) != -1)
        //indexOf()查找，未找到则返回-1
        return true;
    return false;
}

void MainWindow::rayEffect(Ray *ray)
{
    foreach(Enemy *enemy, m_enemyList)
    {
        if(ifInRange(enemy->getPos(),1,ray->getPos(),ray->getTargetRadius()))
        {
            enemy->getDamaged(ray->getDamage());
        }

    }
    removeRay(ray);
}

void MainWindow::iceEffect(Ice *ice)
{
    foreach(Enemy *enemy, m_enemyList)
    {
        if(ifInRange(enemy->getPos(),1,ice->getPos(),ice->getTargetRadius())
                && ! enemy->ifDecelerated())
        {
            enemy->getDecelerated(ice->getDeceleration());
        }
    }
    removeIce(ice);
}
