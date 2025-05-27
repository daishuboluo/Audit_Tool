#include "mouse_capture.h"



Q_GLOBAL_STATIC(Mouse_Capture, fileRelatedInstance) // 创建File_Related类的全局静态单例实例



Mouse_Capture::Mouse_Capture(QObject *parent)
    : QObject{parent}
{
    // 安装低级键盘钩子
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    if (hHook == NULL)
    {
        qDebug() << "Failed to install hook!";
    } else {
        qDebug() << "Hook installed successfully!";
    }



}

Mouse_Capture::~Mouse_Capture()
{
    UnhookWindowsHookEx(hHook);

}




Mouse_Capture* Mouse_Capture::instance() // 获取File_Related的单例实例
{
    return fileRelatedInstance; // 返回全局静态实例
}

// 钩子回调函数
LRESULT CALLBACK Mouse_Capture::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    if (nCode >= 0)
    {
        KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;

        // 检查是否是按键按下事件
        if (wParam == WM_KEYDOWN)
        {
            qDebug() << "Key pressed:" << kb->vkCode; // 打印按键的虚拟键码
            emit MOUSE_CAPTURE->keyPressed(kb->vkCode); // 发射按键信号

        }



    }
    return CallNextHookEx(NULL, nCode, wParam, lParam); // 调用下一个钩子
}
