#ifndef MOUSE_CAPTURE_H
#define MOUSE_CAPTURE_H

#include <QObject>
#include <windows.h>
#include <QDebug>



#define MOUSE_CAPTURE Mouse_Capture::instance()




class Mouse_Capture : public QObject
{
    Q_OBJECT
public:
    explicit Mouse_Capture(QObject *parent = nullptr);
    ~Mouse_Capture();

    static Mouse_Capture *instance();




    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    //发射当前按键信号
signals:
    void keyPressed(DWORD keyCode); // 发射按键信号)



private:
    HHOOK hHook;





    Q_DISABLE_COPY_MOVE(Mouse_Capture)


};

#endif // MOUSE_CAPTURE_H
