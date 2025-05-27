#include "screenshot.h"

// 在widget.cpp中
static ID3D11Device* device = nullptr; // D3D设备
static ID3D11DeviceContext* contexts = nullptr; // D3D设备上下文
static IDXGIOutputDuplication* duplication = nullptr; // DXGI输出复制对象

Screenshot::Screenshot(QObject *parent)
    : QObject{parent}
{
    if (!initDXGI())
    {
        qDebug() << "初始化DXGI失败";
        return;
    }
    qDebug() << "初始化DXGI成功";


}

Screenshot::~Screenshot()
{
    duplication->Release();
    contexts->Release();
    device->Release();
}



QImage Screenshot::Capture_DXGI_Image_And_Draw()
{
    QImage image;
    // DXGI_OUTDUPL_FRAME_INFO frameInfo; // 框架信息
    // IDXGIResource* resource = nullptr;
    HRESULT hr = duplication->AcquireNextFrame(0, &frameInfo, &resource); // 获取下一帧
    if (FAILED(hr))
    {
        return image; // 如果获取失败，直接返回
    }

    // ID3D11Texture2D* texture = nullptr; // 声明纹理接口
    hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture); // 查询纹理接口
    resource->Release(); // 释放资源
    if (FAILED(hr)) {
        duplication->ReleaseFrame(); // 释放帧资源
        return image; // 如果查询失败，返回
    }

    // D3D11_TEXTURE2D_DESC desc; // 纹理描述
    texture->GetDesc(&desc); // 获取纹理描述

    // 创建暂存纹理
    desc.Usage = D3D11_USAGE_STAGING; // 设置用法为暂存
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // 设置CPU访问权限
    desc.BindFlags = 0; // 不绑定
    desc.MiscFlags = 0; // 没有额外的标志

    // ID3D11Texture2D* stagingTexture = nullptr;
    hr = device->CreateTexture2D(&desc, nullptr, &stagingTexture); // 创建暂存纹理
    if (FAILED(hr)) {
        texture->Release(); // 释放纹理资源
        duplication->ReleaseFrame(); // 释放帧资源
        return image; // 创建失败返回
    }

    contexts->CopyResource(stagingTexture, texture); // 复制资源到暂存纹理
    texture->Release(); // 释放纹理资源
    duplication->ReleaseFrame(); // 释放帧资源

    // 映射纹理数据
    // D3D11_MAPPED_SUBRESOURCE mapped;
    hr = contexts->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mapped); // 映射暂存纹理
    if (FAILED(hr)) {
        stagingTexture->Release(); // 释放暂存纹理
        return image; // 映射失败返回
    }

    // 创建QImage

    image = QImage((const uchar*)mapped.pData, desc.Width, desc.Height, mapped.RowPitch, QImage::Format_ARGB32); // 使用映射的数据创建QImage

    contexts->Unmap(stagingTexture, 0); // 取消映射
    stagingTexture->Release(); // 释放暂存纹理

    qDebug() << "已捕获图片"; // 打印捕获的图像大小

    // 发射捕获的图像信号
    return image; // 返回捕获的图像

}

bool Screenshot::initDXGI()
{
    // 创建D3D设备
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                   nullptr, 0, D3D11_SDK_VERSION, &device,
                                   &featureLevel, &contexts);
    if (FAILED(hr)) return false; // 创建失败返回 false

    // 获取DXGI设备
    IDXGIDevice* dxgiDevice = nullptr;
    hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    if (FAILED(hr)) return false; // 查询失败返回 false

    IDXGIAdapter* adapter = nullptr;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter);
    dxgiDevice->Release();
    if (FAILED(hr)) return false; // 获取适配器失败返回 false

    IDXGIOutput* output = nullptr;
    hr = adapter->EnumOutputs(0, &output);
    adapter->Release();
    if (FAILED(hr)) return false; // 枚举输出失败返回 false

    IDXGIOutput1* output1 = nullptr;
    hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&output1);
    output->Release();
    if (FAILED(hr)) return false; // 查询输出接口失败返回 false

    hr = output1->DuplicateOutput(device, &duplication);
    output1->Release();
    if (FAILED(hr)) return false; // 复制输出失败返回 false

    return true; // 初始化成功返回 true

}



