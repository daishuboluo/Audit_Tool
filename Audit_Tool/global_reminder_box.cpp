#include "global_reminder_box.h"

Global_Reminder_Box::Global_Reminder_Box(int borderWidth,QColor backgroundColor,QWidget *parent)
    : QWidget{parent}
{

    //无边框
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool); // 设置窗口为无边框且总在最上层
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint); // 设置窗口总在最上层



    this->borderWidth = borderWidth; // 设置边框粗细为4像素
    this->backgroundColor = backgroundColor; // 设置背景颜色为半透明白色

    timer = new QTimer(this); // 创建定时器对象
    connect(timer, &QTimer::timeout, this,&Global_Reminder_Box::closeReminder); // 连接定时器的超时信号到更新遮罩槽函数
    timer->start(2000); // 设置定时器每5秒触发一次







}




void Global_Reminder_Box::Size_and_Crop(int width, int height)
{
    // 设置窗口大小并更新遮罩
    setFixedSize(width,height); // 设置窗口的固定大小
    updateMask(); // 更新遮罩区域
    update(); // 触发重绘事件以应用新的遮罩




}


void Global_Reminder_Box::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event); // 调用基类的 paintEvent 方法

    QPainter painter(this); // 创建绘图对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿

    painter.setPen(Qt::NoPen); // 不使用边框
    painter.setBrush(backgroundColor); // 设置半透明白色背景
    painter.drawRect(rect()); // 绘制矩形覆盖整个窗口区域



}






void Global_Reminder_Box::updateMask() //更新遮罩
{
    QPainterPath path;
    path.addRect(borderWidth, borderWidth, width()-borderWidth-borderWidth, height()-borderWidth-borderWidth);
    path.closeSubpath(); // 闭合路径


    QPainterPath path2;
    path.addRect(0, 0, width(), height());
    path.closeSubpath(); // 闭合路径


    QPainterPath path3;



    path3 = path.subtracted(path2);




    QRegion region = QRegion(path3.toFillPolygon().toPolygon());
    setMask(region); // 设置遮罩区域
}

void Global_Reminder_Box::closeReminder()
{
    timer->stop(); // 停止定时器
    this->close(); // 关闭窗口

}






