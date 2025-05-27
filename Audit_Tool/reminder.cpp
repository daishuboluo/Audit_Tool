#include "reminder.h"

Reminder::Reminder(bool Is_Harmful,QWidget *parent)
    : QWidget{parent}
{
    //窗口类型枚举
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool); // 设置窗口为无边框且
    //总在最上层
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint); // 设置窗口总在最上层



    //设置固定大小
    setFixedSize(200, 150); // 设置窗口的固定大小为300x100
    this->Is_Harmful = Is_Harmful; // 设置是否有害的状态
    update();



    this->timer = new QTimer(this); // 创建定时器对象


    //获取屏幕大小
    screenGeometry = QGuiApplication::primaryScreen()->size(); // 获取屏幕几何信息
    qDebug() << "Screen size:" << screenGeometry; // 输出屏幕大小到调试信息

    this->move(screenGeometry.width(), screenGeometry.height());
    //动画xxxxxxxxx




    connect(timer, &QTimer::timeout, this, [this]{
        if (countdown == 0)
        {
            executeAnimation(); // 执行动画

        }
        else if (countdown == 20)
        {

            restoreAnimation();
        }
        else if (countdown > 26)
        {
            closeReminder();
        }




        countdown++; // 倒计时减1

     });
    timer->start(100); // 每秒触发一次定时器










}




void Reminder::closeReminder()
{


    this->close(); // 关闭窗口



}

void Reminder::executeAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos"); // 创建动画对象
    animation->setDuration(160); // 设置动画持续时间为1000毫秒
    animation->setStartValue(QPoint(screenGeometry.width(), screenGeometry.height() - height() -70)); // 设置动画起始位置
    animation->setEndValue(QPoint(screenGeometry.width() - width(), screenGeometry.height() - height() -70)); // 设置动画结束位置
    animation->setEasingCurve(QEasingCurve::OutBounce); // 设置动画缓动曲线为OutBounce
    animation->start(); // 启动动画
}

void Reminder::restoreAnimation()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos"); // 创建动画对象
    animation->setDuration(160); // 设置动画持续时间为1000毫秒
    animation->setStartValue(this->pos()); // 设置动画起始位置
    animation->setEndValue(QPoint(this->pos().x() +this->width(),this->pos().y())); // 设置动画结束位置
    animation->setEasingCurve(QEasingCurve::OutBounce); // 设置动画缓动曲线为OutBounce
    animation->start(); // 启动动画
}

void Reminder::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event); // 调用基类的 paintEvent 方法

    QPainter painter(this);
    //抗锯齿
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // 设置抗锯齿和光滑变换

    if (Is_Harmful)
    {
        painter.setPen(Qt::red); // 设置画笔颜色为红色
        painter.setBrush(Qt::red); // 设置填充颜色为红色

        painter.setPen(Qt::NoPen); // 设置画笔颜色为绿色
        painter.drawRect(0, 0, width(), height()); // 绘制一个红色圆形

        QFont font;
        font.setPixelSize(rect().height() / 3); // 设置字体大小为窗口高度的1/20
        font.setBold(true); // 设置字体为粗体
        painter.setFont(font); // 应用字体设置
        painter.setPen(Qt::yellow); // 设置绘图颜色为黑色        painter.drawText(rect(), Qt::AlignCenter, "有害"); // 在圆形内绘制文本“有害”
        painter.drawText(rect(), Qt::AlignCenter, "存在有害"); // 在圆形内绘制文本“无害”


    }
    else
    {
        painter.setPen(Qt::green); // 设置画笔颜色为绿色
        painter.setBrush(Qt::green); // 设置填充颜色为绿色

        painter.setPen(Qt::NoPen); // 设置画笔颜色为绿色
        painter.drawRect(0, 0, width(), height()); // 绘制一个绿色圆形

        QFont font;
        font.setPixelSize(rect().height() / 3); // 设置字体大小为窗口高度的1/20
        font.setBold(true); // 设置字体为粗体
        painter.setFont(font); // 应用字体设置
        painter.setPen(Qt::blue); // 设置绘图颜色为黑色

        painter.drawText(rect(), Qt::AlignCenter, "无害存在"); // 在圆形内绘制文本“无害”


    }







}
