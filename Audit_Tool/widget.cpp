#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(720, 480); // 设置窗口初始大小



    left_column = new Left_Column(this); // 创建左侧列
    right_column = new Right_Column(this); // 创建右侧列


}

Widget::~Widget() {}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // 调用基类的resizeEvent函数
    int width = event->size().width(); // 获取窗口宽度
    int height = event->size().height(); // 获取窗口高


    left_column->resize(width * 0.1, height); // 设置左侧列的大小
    right_column->resize(width * 0.9, height); // 设置右侧列的大小

    left_column->move(0, 0); // 设置左侧列的位置
    right_column->move(width * 0.1, 0); // 设置右侧列的位置






}
