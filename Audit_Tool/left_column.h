#ifndef LEFT_COLUMN_H
#define LEFT_COLUMN_H

#include <QWidget>
//按钮
#include <QPushButton>
#include <QResizeEvent>
#include "central_bundle_bridge.h"



class Left_Column : public QWidget
{
    Q_OBJECT
public:
    explicit Left_Column(QWidget *parent = nullptr);





signals:


protected:
    void resizeEvent(QResizeEvent *event) override; // 重写resizeEvent事件处理函数






private:
    //主要显示
    QPushButton *main_show_QPushButton;
    //设置
    QPushButton *settings_QPushButton;





};

#endif // LEFT_COLUMN_H
