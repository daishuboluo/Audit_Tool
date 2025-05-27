#include "left_column.h"

Left_Column::Left_Column(QWidget *parent)
    : QWidget{parent}
{
    main_show_QPushButton = new QPushButton("主页", this); // 创建主要显示按钮
    settings_QPushButton = new QPushButton("设置", this); // 创建设置按钮

    connect(main_show_QPushButton, &QPushButton::clicked, RELAY_SIGNALS, &Central_Bundle_Bridge::main_page_signal); // 连接主要显示按钮的点击信号
    connect(settings_QPushButton, &QPushButton::clicked, RELAY_SIGNALS, &Central_Bundle_Bridge::settings_page_signal); // 连接设置按钮的点击信号)

}

void Left_Column::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // 调用基类的resizeEvent函数
    int width = event->size().width(); // 获取窗口宽度
    int height = event->size().height(); // 获取窗口高度

    main_show_QPushButton->resize(width * 0.8, height * 0.1); // 设置主要显示按钮的大小
    settings_QPushButton->resize(width * 0.8, height * 0.1); // 设置设置按钮的大小

    main_show_QPushButton->move(width * 0.1, 0); // 设置主要显示按钮的位置
    settings_QPushButton->move(width * 0.1, height * 0.1); // 设置设置按钮的位置



}
