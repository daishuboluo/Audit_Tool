#ifndef EXECUTE_INFERENCE_H
#define EXECUTE_INFERENCE_H

#include <QObject>
#include <iostream>      // 引入输入输出流库
#include <iomanip>       // 引入输入输出格式库
#include <filesystem>    // 引入文件系统库
#include <fstream>       // 引入文件流库
#include <random>        // 引入随机数库

#include <QPainter>
#include <QDebug>

#include "inference.h"   // 引入推理头文件
#include "reminder.h"
#include "central_bundle_bridge.h"
#include "global_reminder_box.h"



class Execute_Inference : public QObject
{
    Q_OBJECT

public:
    explicit Execute_Inference(QObject *parent = nullptr);
    ~Execute_Inference();

    int ReadCocoYaml(YOLO_V11 *p,std::string path);     // 读取coco.yaml文件函数
    void DetectTest(DL_INIT_PARAM params,std::string& path);                  // 检测测试函数




    void Draw(QImage& image);          // QPainter绘制检测函数


signals:
    //发送文本
    void Send_Text(const QString &text);







private:
    YOLO_V11* yoloDetector = nullptr;             // YOLO检测器
    //Global_Reminder_Box的列表
    QList<Global_Reminder_Box*> globalReminderBoxes; // 全局提醒框列表

    //是否在屏幕上显示不可点击的方框
    bool showGlobalReminderBoxes = true;          // 是否在屏幕上显示不可点击的方框


};

#endif // EXECUTE_INFERENCE_H
