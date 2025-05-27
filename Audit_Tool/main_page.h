#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <QWidget>
#include <QLabel>
//文本显示区域
#include <QTextEdit>
#include <QResizeEvent>
#include <QPainter>
#include <QDateTime>



#include "central_bundle_bridge.h"
#include "image_display.h"





class Main_Page : public QWidget
{
    Q_OBJECT
public:
    explicit Main_Page(QWidget *parent = nullptr);



public slots:
    //设置图片
    void setImage(const QImage &image);
    //设置文本
    void setText(const QString &text);






protected:
    void resizeEvent(QResizeEvent *event) override; // 重写窗口大小调整事件处理函数
    void paintEvent(QPaintEvent *event) override; // 重写绘制事件处理函数（如果需要自定义绘制）




private:

    Image_Display *image_label = nullptr; // 图像标签
    QTextEdit *text_edit = nullptr; // 文本编辑区域
    QLabel *title_label = nullptr; // 标题标签

    //是否有害QLabel标签
    QLabel *Is_Harmful_label = nullptr; // 可选的标签，如果需要显示其他信息






};

#endif // MAIN_PAGE_H
