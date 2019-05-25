#include "mainwindow.h"
#include <QApplication>

#include "logindlg.h"
/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDlg dlg;
    w.show();

    return a.exec();
}
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDlg dlg;                        // 建立自己新建的LoginDlg类的实例dlg
    if(dlg.exec() == QDialog::Accepted) // 利用Accepted返回值判断按钮是否被按下
    {
        w.show();                      // 如果被按下，显示主窗口
        return a.exec();              // 程序一直执行，直到主窗口关闭
    }
    else return 0;          //如果没有被按下，则不会进入主窗口，整个程序结束运行
}

/*
备注：
1.素材图片大小：
bullet 6 * 6
enemy 60 * 60
RUCmap 1000 *1000
tower 80 * 80
towerposition 100 * 100
用自带画图更改大小最简便
*/
