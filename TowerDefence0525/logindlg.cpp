#include "logindlg.h"
#include "ui_logindlg.h"
#include <QPainter>

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/pics/background.jpg");
    painter.drawPixmap(0, 0, 800, 600, pix);
}


