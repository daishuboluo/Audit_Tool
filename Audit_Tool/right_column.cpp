#include "right_column.h"

Right_Column::Right_Column(QWidget *parent)
    : QWidget{parent}
{

    main_page = new Main_Page(this); // 创建主页面
    settings_page = new Settings_Page(this); // 创建设置页面
    settings_page->hide();

    connect(RELAY_SIGNALS, &Central_Bundle_Bridge::main_page_signal, this, &Right_Column::show_main_page); // 连接信号和槽
    connect(RELAY_SIGNALS, &Central_Bundle_Bridge::settings_page_signal, this, &Right_Column::show_settings_page); // 连接信号和槽


}

void Right_Column::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // 调用基类的resizeEvent函数
    int width = event->size().width(); // 获取窗口宽度
    int height = event->size().height(); // 获取窗口高度

    main_page->resize(width, height); // 设置主页面的大小
    settings_page->resize(width, height); // 设置设置页面的大小

    main_page->move(0, 0); // 设置主页面的位置
    settings_page->move(0, 0); // 设置设置页面的位置



}

void Right_Column::show_main_page()
{
    main_page->show(); // 显示主页面
    settings_page->hide(); // 隐藏设置页面


}

void Right_Column::show_settings_page()
{
    main_page->hide(); // 隐藏主页面
    settings_page->show(); // 显示设置页面

}
