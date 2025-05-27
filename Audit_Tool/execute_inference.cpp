#include "execute_inference.h"

Execute_Inference::Execute_Inference(QObject *parent)
    : QObject{parent}
{
    connect(this,&Execute_Inference::Send_Text,RELAY_SIGNALS,&Central_Bundle_Bridge::text_signal); // 连接信号到接收文本槽函数)
    connect(RELAY_SIGNALS,&Central_Bundle_Bridge::status_signal,this,[this](bool status) { // 连接状态信号到lambda函数
        showGlobalReminderBoxes  = status; // 更新全局变量 showGlobalReminderBoxes 的值
    });



}

Execute_Inference::~Execute_Inference()
{

}

/**
 * QPainter绘制函数
 *
 * @param image QImage 类的引用
 */
void Execute_Inference::Draw(QImage& image)
{

    QPainter image_painter(&image);
    image_painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // QImage& image转换为 cv::Mat
    cv::Mat cv_image = cv::Mat(image.height(), image.width(), CV_8UC4, image.bits());

    std::vector<DL_RESULT> res; // 存储检测结果
    yoloDetector->RunSession(cv_image, res); // 运行检测


    QFont  font;
    font.setPixelSize(image.height() / 40);
    font.setBold(true);
    image_painter.setFont(font);

    qDebug() << "1"; // 输出调试信息


    if (res.empty()) // 检测结果为空
    {
        Reminder *reminder = new Reminder(false); // 创建提醒窗口
        reminder->show();
        qDebug() << "检测结果为空"; // 输出调试信息

        emit Send_Text("无害"); // 发送无害文本信号


        return; // 退出函数
    }
    else
    {
        Reminder *reminder = new Reminder(true); // 创建提醒窗口
        reminder->show();

        qDebug() << "检测结果不为空"; // 输出调试信息
    }


    qDebug() << "2"; // 输出调试信息

    for (auto& re : res) // 遍历检测结果
    {
        image_painter.setPen(QPen(yoloDetector->colors[re.classId], 5)); // 设置画笔颜色和宽度
        image_painter.drawRect(re.box.x, re.box.y, re.box.width, re.box.height); // 绘制边界框


        image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 40,QString::fromStdString(yoloDetector->classes[re.classId])); // 绘制边界框
        image_painter.drawText(re.box.x + 10, re.box.y + image.height() / 20,QString::number(re.confidence));

        if (showGlobalReminderBoxes)
        {
            Global_Reminder_Box *globalReminderBox = new Global_Reminder_Box(5, yoloDetector->colors[re.classId]); // 创建全局提醒框
            globalReminderBox->Size_and_Crop(re.box.width, re.box.height);
            globalReminderBox->show(); // 显示全局提醒框
            globalReminderBox->move(re.box.x, re.box.y); // 移动全局提醒框到指定位置
        }



        // 输出检测结果
        qInfo() << " x: " << re.box.x << " y: " << re.box.y
                << " w: " << re.box.width << " h: " << re.box.height;
        qInfo() << "类别  : " << yoloDetector->classes[re.classId];

        qInfo() << "类别 ID: " << re.classId;
        qInfo() << "置信度: " << re.confidence;
        qInfo() << "\n\t"; // 输出换行符
    }
    image_painter.end();

    QString tet = "";
    for (auto& re : res) // 遍历检测结果
    {
        tet += " ( " + QString::fromStdString(yoloDetector->classes[re.classId]) + " )     "; // 拼接文本
    }



    emit Send_Text("存在有害内容" + tet);


}


/**
 * 读取 COCO YAML 文件
 * @param p YOLO_V11 类的指针
 * @return 状态码
 */
int Execute_Inference::ReadCocoYaml(YOLO_V11* p,std::string path)
{
    // 打开 YAML 文件
    std::ifstream file(path);
    if (!file.is_open()) // 检查文件是否成功打开
    {
        qDebug() << "无法打开文件"; // 输出错误信息
        return 1; // 返回错误状态
    }

    // 按行读取文件
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) // 逐行读取
    {
        lines.push_back(line); // 存储每一行
    }

    // 查找名称部分的开始和结束
    std::size_t start = 0;
    std::size_t end = 0;
    for (std::size_t i = 0; i < lines.size(); i++)
    {
        if (lines[i].find("names:") != std::string::npos) // 查找 "names:"
        {
            start = i + 1; // 记录开始行
        }
        else if (start > 0 && lines[i].find(':') == std::string::npos) // 查找结束行
        {
            end = i;
            break;
        }
    }

    // 提取名称
    std::vector<std::string> names;
    QVector<QColor> colors;
    for (std::size_t i = start; i < end; i++)
    {
        // 保存名称
        std::stringstream ss(lines[i]); // 创建字符串流
        std::string name;
        std::getline(ss, name, ':'); // 提取分隔符前的数字
        std::getline(ss, name); // 提取分隔符后的字符串
        name.erase(remove(name.begin(), name.end(), ' '), name.end()); // 去除空格
        names.push_back(name); // 存储名称

        // 设置框颜色
        cv::RNG rng(cv::getTickCount()); // 随机数生成器
        colors.push_back(QColor(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256))); // 设置框颜色
    }

    p->classes = names; // 更新类别名称
    p->colors = colors; // 更新类别颜色

    return 0; // 返回成功状态
}

/**
 * 检测测试函数
 */
void Execute_Inference::DetectTest(DL_INIT_PARAM params,std::string& path)
{
    if (yoloDetector != nullptr) // 检查 yoloDetector 是否已存在
    {
        qDebug() << "yoloDetector 已存在，正在删除"; // 输出调试信息
    }
    yoloDetector = nullptr; // 确保 yoloDetector 被删除并置为 nullptr

    yoloDetector = new YOLO_V11(this); // 创建 YOLO_V11 实例

    if (ReadCocoYaml(yoloDetector,path) == 1) // 读取 COCO YAML 文件
    {
        qDebug() << "读取 COCO YAML 文件失败"; // 输出错误信息
        return; // 退出函数
    }
    else
    {
        qDebug() << "读取 COCO YAML 文件成功"; // 输出成功信息
    }

    params.iouThreshold = 0.5; // 设置 IOU 阈值
    params.imgSize = { 640, 640 }; // 设置图像尺寸

    // CPU 推理
    params.modelType = YOLO_DETECT_V11; // 设置模型类型
    params.cudaEnable = false; // 禁用 CUDA

    yoloDetector->CreateSession(params);




}




