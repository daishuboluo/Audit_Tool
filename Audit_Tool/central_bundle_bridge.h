#ifndef CENTRAL_BUNDLE_BRIDGE_H
#define CENTRAL_BUNDLE_BRIDGE_H

#include <QObject>


//中央桥梁
#define RELAY_SIGNALS Central_Bundle_Bridge::instance()



class Central_Bundle_Bridge : public QObject
{
    Q_OBJECT
public:
    explicit Central_Bundle_Bridge(QObject *parent = nullptr);

    static Central_Bundle_Bridge* instance();

signals:
    //主页面信号
    void main_page_signal();
    //设置页面信号
    void settings_page_signal();

    //转发图片
    void image_signal(const QImage &image);

    //转发存在文本
    void text_signal(const QString &text);
    //转发状态bool
    void status_signal(bool status);








private:

    Q_DISABLE_COPY_MOVE(Central_Bundle_Bridge)




};

#endif // CENTRAL_BUNDLE_BRIDGE_H
