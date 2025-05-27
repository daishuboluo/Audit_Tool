#ifndef GLOBAL_REMINDER_BOX_H
#define GLOBAL_REMINDER_BOX_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>



class Global_Reminder_Box : public QWidget
{
    Q_OBJECT
public:
    explicit Global_Reminder_Box(int borderWidth,QColor backgroundColor,QWidget *parent = nullptr);


    //设置边框粗细

    //大小和裁剪
    void Size_and_Crop(int width, int height); // 设置大小和裁剪);

    void updateMask();

    //关闭的函数

public slots:
    void closeReminder();



protected:
    void paintEvent(QPaintEvent *event) override;


private:
    //色彩
    QColor backgroundColor; // 半透明白色背景
    //边框粗细
    int borderWidth = 4; // 默认边框粗细为2像素

    QTimer *timer; // 定时器



};

#endif // GLOBAL_REMINDER_BOX_H
