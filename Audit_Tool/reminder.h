#ifndef REMINDER_H
#define REMINDER_H

#include <QWidget>
//动画类
#include <QPropertyAnimation>
#include <QPainter>
#include <QApplication>
#include <QScreen>

//定时器
#include <QTimer>


class Reminder : public QWidget
{
    Q_OBJECT
public:
    explicit Reminder(bool Is_Harmful,QWidget *parent = nullptr);


public slots:
    void closeReminder(); // 关闭提醒窗口的槽函数

    //执行动画
    void executeAnimation(); // 执行动画的槽函数
    //还原动画
    void restoreAnimation(); // 还原动画的槽函数

//绘制
protected:
    void paintEvent(QPaintEvent *event) override; // 重写绘制事件处理函数



private:
    bool Is_Harmful = false; // 是否有害
    QTimer *timer; // 定时器，用于定时关闭提醒窗口
    QSize screenGeometry;


    int countdown = 0; // 倒计时秒数

};

#endif // REMINDER_H
