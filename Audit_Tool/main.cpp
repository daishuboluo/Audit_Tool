#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("Jia_xin_jia_mian"); // 设置组织名称
    QApplication::setApplicationName("Jia_xin_jia_mian_Audit_Tool"); // 设置应用程序名称
    QApplication::setApplicationVersion("0.0.1"); // 设置应用程序版本

    Widget w;
    w.show();
    return a.exec();
}
