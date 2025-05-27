#include "inference.h"
#include <regex>


// 定义宏 min，用于返回两个值中的较小者
#define min(a,b)            (((a) < (b)) ? (a) : (b))

// YOLO_V11 类的构造函数
YOLO_V11::YOLO_V11(QObject* parent)
    : QObject{ parent }
{

}

// YOLO_V11 类的析构函数
YOLO_V11::~YOLO_V11()
{
    delete session; // 删除会话对象
}


/**
 * @brief 从图像创建 Blob
 *
 * @tparam T Blob 的类型
 * @param iImg 输入图像
 * @param iBlob 输出 Blob
 * @return char* 返回状态
 */
template<typename T>
char* BlobFromImage(cv::Mat& iImg, T& iBlob) {
    int channels = iImg.channels(); // 获取图像通道数
    int imgHeight = iImg.rows; // 获取图像高度
    int imgWidth = iImg.cols; // 获取图像宽度

    for (int c = 0; c < channels; c++)
    {
        for (int h = 0; h < imgHeight; h++)
        {
            for (int w = 0; w < imgWidth; w++)
            {
                iBlob[c * imgWidth * imgHeight + h * imgWidth + w] = typename std::remove_pointer<T>::type(
                    (iImg.at<cv::Vec3b>(h, w)[c]) / 255.0f); // 归一化像素值
            }
        }
    }
    return RET_OK; // 返回状态
}

/**
 * @brief 预处理输入图像
 *
 * @param iImg 输入图像
 * @param iImgSize 目标图像大小
 * @param oImg 输出处理后的图像
 * @return char* 返回状态
 */
char* YOLO_V11::PreProcess(cv::Mat& iImg, std::vector<int> iImgSize, cv::Mat& oImg)
{
    // qInfo() << "通道数：" << iImg.channels() ;

    if (iImg.channels() == 3) // 如果输入图像为 RGB
    {
        oImg = iImg.clone(); // 克隆图像
        cv::cvtColor(oImg, oImg, cv::COLOR_BGR2RGB); // 转换颜色空间
    }
    else if (iImg.channels() == 4) // 如果输入图像为灰度图
    {
        oImg = iImg.clone(); // 克隆图像
        cv::cvtColor(iImg, oImg, cv::COLOR_BGRA2RGB); // 转换为 RGB
    }
    else // 如果输入图像为灰度图
    {
        cv::cvtColor(iImg, oImg, cv::COLOR_GRAY2RGB); // 转换为 RGB
    }

    switch (modelType) // 根据模型类型进行处理
    {
    case YOLO_DETECT_V11:
    case YOLO_POSE:
    case YOLO_DETECT_V11_HALF:
    case YOLO_POSE_V11_HALF://LetterBox
    {
        if (iImg.cols >= iImg.rows) // 如果宽度大于高度
        {
            resizeScales = iImg.cols / (float)iImgSize.at(0); // 计算缩放比例
            cv::resize(oImg, oImg, cv::Size(iImgSize.at(0), int(iImg.rows / resizeScales))); // 调整图像大小
        }
        else // 如果高度大于宽度
        {
            resizeScales = iImg.rows / (float)iImgSize.at(0); // 计算缩放比例
            cv::resize(oImg, oImg, cv::Size(int(iImg.cols / resizeScales), iImgSize.at(1))); // 调整图像大小
        }
        cv::Mat tempImg = cv::Mat::zeros(iImgSize.at(0), iImgSize.at(1), CV_8UC3); // 创建零矩阵
        oImg.copyTo(tempImg(cv::Rect(0, 0, oImg.cols, oImg.rows))); // 拷贝到零矩阵
        oImg = tempImg; // 更新输出图像
        break;
    }
    case YOLO_CLS://CenterCrop
    {
        int h = iImg.rows; // 获取图像高度
        int w = iImg.cols; // 获取图像宽度
        int m = min(h, w); // 取高度和宽度的较小值
        int top = (h - m) / 2; // 计算上边界
        int left = (w - m) / 2; // 计算左边界
        cv::resize(oImg(cv::Rect(left, top, m, m)), oImg, cv::Size(iImgSize.at(0), iImgSize.at(1))); // 中心裁剪并调整大小
        break;
    }
    }
    return RET_OK; // 返回状态
}

/**
 * @brief 创建会话
 *
 * @param iParams 初始化参数
 * @return char* 返回状态
 */
void YOLO_V11::CreateSession(DL_INIT_PARAM& iParams)
{
    const char* Ret = RET_OK; // 初始化返回状态
    std::regex pattern("[\u4e00-\u9fa5]"); // 正则表达式，匹配中文字符
    bool result = std::regex_search(iParams.modelPath, pattern); // 检查模型路径中是否包含中文字符
    if (result) // 如果包含中文字符
    {
        qDebug() << "[YOLO_V11]:您的模型路径错误。请将模型路径更改为不包含中文字符的路径。"; // 输出错误信息
    }
    try
    {
        rectConfidenceThreshold = iParams.rectConfidenceThreshold; // 设置置信度阈值
        iouThreshold = iParams.iouThreshold; // 设置 IOU 阈值
        imgSize = iParams.imgSize; // 设置图像大小
        modelType = iParams.modelType; // 设置模型类型
        cudaEnable = iParams.cudaEnable; // 设置 CUDA 启用状态
        env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "Yolo"); // 创建 ONNX 环境
        Ort::SessionOptions sessionOption; // 创建会话选项
        sessionOption.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL); // 设置图形优化级别
        sessionOption.SetIntraOpNumThreads(iParams.intraOpNumThreads); // 设置线程数
        sessionOption.SetLogSeverityLevel(iParams.logSeverityLevel); // 设置日志严重性级别


        int ModelPathSize = MultiByteToWideChar(CP_UTF8, 0, iParams.modelPath.c_str(), static_cast<int>(iParams.modelPath.length()), nullptr, 0); // 获取模型路径大小
        wchar_t* wide_cstr = new wchar_t[ModelPathSize + 1]; // 创建宽字符数组
        MultiByteToWideChar(CP_UTF8, 0, iParams.modelPath.c_str(), static_cast<int>(iParams.modelPath.length()), wide_cstr, ModelPathSize); // 转换为宽字符
        wide_cstr[ModelPathSize] = L'\0'; // 结束字符
        const wchar_t* modelPath = wide_cstr; // 设置模型路径

        session = new Ort::Session(env, modelPath, sessionOption); // 创建会话
        Ort::AllocatorWithDefaultOptions allocator; // 创建分配器
        size_t inputNodesNum = session->GetInputCount(); // 获取输入节点数量
        for (size_t i = 0; i < inputNodesNum; i++)
        {
            Ort::AllocatedStringPtr input_node_name = session->GetInputNameAllocated(i, allocator); // 获取输入节点名称
            char* temp_buf = new char[50]; // 创建临时缓冲区
            strcpy(temp_buf,  input_node_name.get()); // 拷贝输入节点名称
            inputNodeNames.push_back(temp_buf); // 存储输入节点名称
        }
        size_t OutputNodesNum = session->GetOutputCount(); // 获取输出节点数量
        for (size_t i = 0; i < OutputNodesNum; i++)
        {
            Ort::AllocatedStringPtr output_node_name = session->GetOutputNameAllocated(i, allocator); // 获取输出节点名称
            char* temp_buf = new char[10]; // 创建临时缓冲区
            strcpy(temp_buf, output_node_name.get()); // 拷贝输出节点名称
            outputNodeNames.push_back(temp_buf); // 存储输出节点名称
        }
        options = Ort::RunOptions{ nullptr }; // 初始化运行选项
        WarmUpSession(); // 预热会话
        qDebug () << "[YOLO_V11]:创建会话成功。"; // 输出成功信息

    }
    catch (const std::exception& e) // 捕获异常
    {

        const char* str1 = "[YOLO_V11]:";
        const char* str2 = e.what(); // 获取异常信息
        std::string result = std::string(str1) +"  |||||  " + std::string(str2); // 合并字符串
        qDebug() << result.c_str(); // 输出错误信息

        qDebug () << "[YOLO_V11]:创建会话失败。"; // 返回错误信息
    }
}
/**
 * @brief 运行会话并处理输入图像
 *
 * @param iImg 输入图像
 * @param oResult 输出结果
 * @return char* 返回状态
 */
char* YOLO_V11::RunSession(cv::Mat& iImg, std::vector<DL_RESULT>& oResult)
{

    char* Ret = RET_OK; // 初始化返回状态
    cv::Mat processedImg; // 处理后的图像
    PreProcess(iImg, imgSize, processedImg); // 预处理输入图像
    if (modelType < 4) // 如果模型类型为前四种
    {
        float* blob = new float[processedImg.total() * 3]; // 创建浮点数 Blob
        BlobFromImage(processedImg, blob); // 从图像创建 Blob
        std::vector<int64_t> inputNodeDims = { 1, 3, imgSize.at(0), imgSize.at(1) }; // 设置输入节点维度
        TensorProcess(iImg, blob, inputNodeDims, oResult); // 处理张量
    }
    qDebug() << "图片处理完成，开始推理..."; // 输出处理完成信息"

    return Ret; // 返回状态
}








/**
 * @brief 处理张量并运行推理
 *
 * @tparam N 张量数据类型
 * @param starttime_1 开始时间
 * @param iImg 输入图像
 * @param blob 输入 Blob 数据
 * @param inputNodeDims 输入节点维度
 * @param oResult 输出结果
 * @return char* 返回状态
 */
template<typename N>
char* YOLO_V11::TensorProcess(cv::Mat& iImg, N& blob, std::vector<int64_t>& inputNodeDims,
                              std::vector<DL_RESULT>& oResult) {

    // 创建输入张量
    Ort::Value inputTensor = Ort::Value::CreateTensor<typename std::remove_pointer<N>::type>(
        Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU), blob, 3 * imgSize.at(0) * imgSize.at(1),
        inputNodeDims.data(), inputNodeDims.size());



    // 运行推理
    auto outputTensor = session->Run(options, inputNodeNames.data(), &inputTensor, 1, outputNodeNames.data(),
                                     outputNodeNames.size());

    // 获取输出张量的信息
    Ort::TypeInfo typeInfo = outputTensor.front().GetTypeInfo();
    auto tensor_info = typeInfo.GetTensorTypeAndShapeInfo();
    std::vector<int64_t> outputNodeDims = tensor_info.GetShape();
    auto output = outputTensor.front().GetTensorMutableData<typename std::remove_pointer<N>::type>();
    delete[] blob; // 删除输入 Blob

    // 根据模型类型进行处理
    switch (modelType)
    {
    case YOLO_DETECT_V11:
    case YOLO_DETECT_V11_HALF:
    {
        int signalResultNum = outputNodeDims[1]; // 结果数量
        int strideNum = outputNodeDims[2]; // 步幅数量
        std::vector<int> class_ids; // 存储类别 ID
        std::vector<float> confidences; // 存储置信度
        std::vector<cv::Rect> boxes; // 存储边界框
        cv::Mat rawData; // 原始数据

        if (modelType == YOLO_DETECT_V11)
        {
            // FP32
            rawData = cv::Mat(signalResultNum, strideNum, CV_32F, output);
        }
        else
        {
            // FP16
            rawData = cv::Mat(signalResultNum, strideNum, CV_16F, output);
            rawData.convertTo(rawData, CV_32F); // 转换为 FP32
        }

        // 矩阵转置
        rawData = rawData.t();

        float* data = (float*)rawData.data;

        // 解析输出数据
        for (int i = 0; i < strideNum; ++i)
        {
            float* classesScores = data + 4; // 获取类别分数
            cv::Mat scores(1, this->classes.size(), CV_32FC1, classesScores); // 创建分数矩阵
            cv::Point class_id; // 存储最大类别 ID
            double maxClassScore; // 存储最大置信度
            cv::minMaxLoc(scores, 0, &maxClassScore, 0, &class_id); // 查找最大分数

            if (maxClassScore > rectConfidenceThreshold) // 如果置信度超过阈值
            {
                confidences.push_back(maxClassScore); // 存储置信度
                class_ids.push_back(class_id.x); // 存储类别 ID
                float x = data[0]; // 获取中心点 x
                float y = data[1]; // 获取中心点 y
                float w = data[2]; // 获取宽度
                float h = data[3]; // 获取高度

                // 计算边界框
                int left = int((x - 0.5 * w) * resizeScales);
                int top = int((y - 0.5 * h) * resizeScales);
                int width = int(w * resizeScales);
                int height = int(h * resizeScales);

                boxes.push_back(cv::Rect(left, top, width, height)); // 存储边界框
            }
            data += signalResultNum; // 移动到下一个结果
        }

        // 执行非极大值抑制
        std::vector<int> nmsResult;
        cv::dnn::NMSBoxes(boxes, confidences, rectConfidenceThreshold, iouThreshold, nmsResult);
        for (int i = 0; i < nmsResult.size(); ++i)
        {
            int idx = nmsResult[i];
            DL_RESULT result;
            result.classId = class_ids[idx]; // 设置类别 ID
            result.confidence = confidences[idx]; // 设置置信度
            result.box = boxes[idx]; // 设置边界框
            oResult.push_back(result); // 存储结果
        }
        break;
    }
    case YOLO_CLS:
    case YOLO_CLS_HALF:
    {
        cv::Mat rawData; // 原始数据
        if (modelType == YOLO_CLS) {
            // FP32
            rawData = cv::Mat(1, this->classes.size(), CV_32F, output);
        } else {
            // FP16
            rawData = cv::Mat(1, this->classes.size(), CV_16F, output);
            rawData.convertTo(rawData, CV_32F); // 转换为 FP32
        }
        float *data = (float *) rawData.data; // 获取数据指针

        DL_RESULT result;
        for (int i = 0; i < this->classes.size(); i++)
        {
            result.classId = i; // 设置类别 ID
            result.confidence = data[i]; // 设置置信度
            oResult.push_back(result); // 存储结果
        }
        break;
    }
    default:
        std::cout << "[YOLO_V11]: " << "不支持的模型类型。" << std::endl; // 输出错误信息
    }
    return RET_OK; // 返回状态
}
/**
 * @brief 预热会话
 *
 * @return char* 返回状态
 */
char* YOLO_V11::WarmUpSession()
{
    clock_t starttime_1 = clock(); // 记录开始时间
    cv::Mat iImg = cv::Mat(cv::Size(imgSize.at(0), imgSize.at(1)), CV_8UC4); // 创建输入图像
    cv::Mat processedImg; // 存储处理后的图像
    PreProcess(iImg, imgSize, processedImg); // 预处理图像

    if (modelType < 4) // 如果模型类型小于 4
    {
        float* blob = new float[iImg.total() * 3]; // 创建 Blob
        BlobFromImage(processedImg, blob); // 从图像创建 Blob
        std::vector<int64_t> YOLO_input_node_dims = { 1, 3, imgSize.at(0), imgSize.at(1) }; // 输入节点维度
        Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
            Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU), blob, 3 * imgSize.at(0) * imgSize.at(1),
            YOLO_input_node_dims.data(), YOLO_input_node_dims.size()); // 创建输入张量

        // 运行推理
        auto output_tensors = session->Run(options, inputNodeNames.data(), &input_tensor, 1, outputNodeNames.data(),
                                           outputNodeNames.size());
        delete[] blob; // 删除 Blob


    }
    return RET_OK; // 返回状态
}
