#include "image_display.h"

Image_Display::Image_Display(QWidget *parent)
    : QWidget{parent}
{


}

void Image_Display::drawImage(const QImage &image)
{
    if (image.isNull()) // 检查图像是否有效
    {
        qWarning() << "Image is null, cannot draw image."; // 输出警告信息
        return; // 如果图像无效，直接返回
    }
    this->currentImage = image; // 更新当前图像

    update(); // 更新窗口以触发绘制事件



}

void Image_Display::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event); // 调用基类的 paintEvent 方法
    QPainter painter(this); // 创建绘图设备
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); // 设置渲染提示

    painter.setPen(Qt::NoPen); // 设置无边框绘制
    painter.setBrush(QColor(255,255,255,11)); // 设置背景色为窗口颜色
    painter.drawRect(rect()); // 绘制整个窗口的矩形区域
    //设置文章大小
    QFont font;
    font.setPixelSize(rect().height() / 10); // 设置字体大小为窗口高度的1/20
    font.setBold(true); // 设置字体为粗体
    painter.setFont(font); // 应用字体设置
    painter.setPen(Qt::lightGray); // 设置绘图颜色为黑色



    //绘制
    painter.drawText(rect(), Qt::AlignCenter, "可视结果显示区域"); // 在窗口中心绘制文字



    //居中等比例绘制图片
    if (!currentImage.isNull()) // 检查当前图像是否有效
    {
        QRect targetRect = rect(); // 获取目标矩形区域
        QRect sourceRect = currentImage.rect(); // 获取源图像的矩形区域

        // 计算缩放比例
        qreal scaleX = static_cast<qreal>(targetRect.width()) / sourceRect.width();
        qreal scaleY = static_cast<qreal>(targetRect.height()) / sourceRect.height();
        qreal scale = qMin(scaleX, scaleY); // 取最小缩放比例

        // 计算绘制位置
        int x = (targetRect.width() - sourceRect.width() * scale) / 2;
        int y = (targetRect.height() - sourceRect.height() * scale) / 2;

        QRectF scaledRect(x, y, sourceRect.width() * scale, sourceRect.height() * scale); // 缩放后的矩形区域

        painter.drawImage(scaledRect, currentImage); // 绘制图像
    }








}
