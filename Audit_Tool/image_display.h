#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <QWidget>
#include <QPainter>

class Image_Display : public QWidget
{
    Q_OBJECT
public:
    explicit Image_Display(QWidget *parent = nullptr);




    //绘制图片
    void drawImage(const QImage &image); // 信号：绘制图片

protected:
    void paintEvent(QPaintEvent *event) override; // 重写绘制事件处理函数




private:
    QImage currentImage; // 当前绘制的图像




};

#endif // IMAGE_DISPLAY_H
