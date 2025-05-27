#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QObject>

#include <QThread>
#include <QImage>
#include <QScreen>
#include <dxgi.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>  // 需要这个头文件来获取 IDXGIOutput1
#include <QPainter>
#include <dwmapi.h>   // 需要链接 Dwmapi.lib
#include <QTimer>






class Screenshot : public QObject
{
    Q_OBJECT
public:
    explicit Screenshot(QObject *parent = nullptr);
    ~Screenshot();


    QImage Capture_DXGI_Image_And_Draw(); // 捕获 DXGI 图像并绘制
    bool initDXGI();                     // 初始化 DXGI

//信号
signals:



private:

    DXGI_OUTDUPL_FRAME_INFO frameInfo;   // 框架信息
    IDXGIResource* resource = nullptr;    // DXGI 资源
    ID3D11Texture2D* texture = nullptr;   // D3D11 纹理
    D3D11_TEXTURE2D_DESC desc;            // 纹理描述
    ID3D11Texture2D* stagingTexture = nullptr; // 临时纹理
    D3D11_MAPPED_SUBRESOURCE mapped;      // 映射的子资源







};

#endif // SCREENSHOT_H
