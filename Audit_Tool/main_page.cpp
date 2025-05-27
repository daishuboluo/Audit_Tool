#include "main_page.h"

Main_Page::Main_Page(QWidget *parent)
    : QWidget{parent}
{




    image_label = new Image_Display(this); // 创建图像标签
    text_edit = new QTextEdit(this); // 创建文本编辑区域
    title_label = new QLabel("检测结果文本显示——————VVVVVVV", this); // 创建标题标签
    Is_Harmful_label = new QLabel("是否有害", this); // 创建是否有害标签（可选）
    title_label->setAlignment(Qt::AlignCenter); // 设置标题标签文本居中对齐
    //设置Is_Harmful_label背景色

    text_edit->setReadOnly(true); // 设置文本编辑区域为只读

    connect(RELAY_SIGNALS, &Central_Bundle_Bridge::image_signal, this, &Main_Page::setImage); // 连接信号到设置图像槽函数
    connect(RELAY_SIGNALS, &Central_Bundle_Bridge::text_signal, this, &Main_Page::setText); // 连接信号到设置文本槽函数

}

void Main_Page::setImage(const QImage &image)
{
    if (image.isNull())
    {
        qWarning() << "Image is null, cannot set image.";
        return;
    }


    // 设置图像标签的图像
    image_label->drawImage(image);


}

void Main_Page::setText(const QString &text)
{
    if (text.isEmpty())
    {
        qWarning() << "Text is empty, cannot set text.";
        return;
    }


    //判断文本中是否存在"有害"或"无害"
    if (text.contains("有害"))
    {
        Is_Harmful_label->setText("有害");
        // 设置黄色粗体字体
        Is_Harmful_label->setStyleSheet("color: red; font-weight: bold;");

    }
    else if (text.contains("无害"))
    {
        Is_Harmful_label->setText("无害");
        // 设置绿色粗体字体
        Is_Harmful_label->setStyleSheet("color: green; font-weight: bold;");


    }
    else
    {
        Is_Harmful_label->setText("未知");
        Is_Harmful_label->setStyleSheet(""); // 清除样式
    }


    // 设置文本编辑区域的文本
    //获取当前时间

    QDateTime currentTime = QDateTime::currentDateTime(); // 获取当前时间
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss"); // 格式化时间字符串


    text_edit->append("\n" +formattedTime + text );


}

void Main_Page::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // 调用基类的 resizeEvent 方法

    int width = event->size().width(); // 获取新的宽度
    int height = event->size().height(); // 获取新的高度

    image_label->setGeometry(0, 0, width, height * 0.8); // 设置图像标签的几何形状
    text_edit->setGeometry(0, height * 0.85, width , height * 0.15); // 设置文本编辑区域的几何形状
    title_label->setGeometry(0, height * 0.8, width *0.5, height * 0.05); // 设置标题标签的几何形状
    Is_Harmful_label->setFixedWidth(width * 0.1); // 设置是否有害标签的固定宽度
    Is_Harmful_label->setGeometry(width - Is_Harmful_label->width() , height * 0.8, Is_Harmful_label->width() , height * 0.05); // 设置是否有害标签的几何形状



}

void Main_Page::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event); // 调用基类的 paintEvent 方法

    QPainter painter(this);
    //获取系统色彩
    QColor backgroundColor = palette().color(QPalette::Window); // 获取窗口背景色
    painter.setPen(Qt::NoPen); // 不绘制边框
    painter.setBrush(backgroundColor); // 设置画笔颜色为背景色
    painter.drawRect(rect()); // 绘制整个窗口的背景矩形









}
