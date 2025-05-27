#ifndef RIGHT_COLUMN_H
#define RIGHT_COLUMN_H

#include <QWidget>
#include <QResizeEvent>
#include "main_page.h"
#include "settings_page.h"
#include "central_bundle_bridge.h"


class Right_Column : public QWidget
{
    Q_OBJECT
public:
    explicit Right_Column(QWidget *parent = nullptr);

signals:





protected:
    void resizeEvent(QResizeEvent *event) override; // 重写resizeEvent事件处理函数


//槽函数
private slots:
    //显示主页面
    void show_main_page();
    //显示设置页面
    void show_settings_page();


private:
    //主页面
    Main_Page *main_page;
    //设置页面
    Settings_Page *settings_page;











};

#endif // RIGHT_COLUMN_H
