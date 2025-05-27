#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QResizeEvent>

#include "left_column.h"
#include "right_column.h"


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


protected:
    void resizeEvent(QResizeEvent *event) override; // 重写resizeEvent事件处理函数



private:
    Left_Column *left_column;
    Right_Column *right_column;



};
#endif // WIDGET_H
