#include "settings_page.h"

Settings_Page::Settings_Page(QWidget *parent)
    : QWidget{parent}
{

    execute_inference = new Execute_Inference(this); // 创建执行推理对象
    screenshot = new Screenshot(this); // 创建截图对象



    setupUi(this); // 设置UI

    QString Onnx_Path = settings.value("Onnx_Path", QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()).toString();
    QString Yaml_Path = settings.value("Yaml_Path", QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first()).toString();
    if (!Onnx_Path.isEmpty()) onnx_label->setText(Onnx_Path); // 设置ONNX标签文本
    if (!Yaml_Path.isEmpty()) yaml_label->setText(Yaml_Path); // 设置YAML标签文本






    //获取系统cpu是多少核
    int cores = QThread::idealThreadCount();

    //设置comboBox的下拉，键值对1,1
    for (int i = 1; i <= cores; ++i) comboBox->addItem(QString::number(i), i); // 添加选项到comboBox
    //设置comboBox的默认值为5
    comboBox->setCurrentIndex(cores -1); // 设置当前索引为4，即默认选择5个线程




    //设置lineEdit_linedit_3最大长度为2，且只能是数字最大99，最小为50，如果超出范围则回归到相邻值
    lineEdit_linedit_3->setObjectName("lineEdit_linedit_3"); // 设置lineEdit_linedit_3的对象名
    lineEdit_linedit_3->setMaxLength(2); // 设置最大长度为2
    QIntValidator *validator = new QIntValidator(1, 99, this); // 创建整数验证器，范围为50到99
    lineEdit_linedit_3->setValidator(validator); // 设置验证器
    connect(lineEdit_linedit_3, &QLineEdit::textChanged, this, [this](const QString &text) { // 连接文本改变信号到lambda函数
        bool ok;
        int value = text.toInt(&ok); // 将文本转换为整数
        if (ok && (value < 1 || value > 99)) { // 如果转换成功且值不在范围内
            lineEdit_linedit_3->setText(QString::number(qBound(40, value, 99))); // 设置文本为范围内的值
        }
    });
    lineEdit_linedit_3->setText("80"); // 设置默认文本为50



    //设置lineEdit_linedit默认绑定键为X
    lineEdit_linedit->setObjectName("lineEdit_linedit"); // 设置lineEdit_linedit的对象名
    lineEdit_linedit->setText("x"); // 设置默认文本为X
    // lineEdit_linedit->setMaxLength(1); // 设置最大长度为1

    connect(lineEdit_linedit, &QLineEdit::textChanged, this, [this]() { // 连接焦点获取事件到lambda函数
        QString text = lineEdit_linedit->text(); // 获取文本
        if (text.length() > 1) { // 如果文本长度大于1
             // 字符截取最后一个xxxxxx
            lineEdit_linedit->setText(text.right(1)); // 设置文本为最后一个字符
        }
    });




    connect(onnx_but, &QPushButton::clicked, this, &Settings_Page::select_onnx_file); // 连接按钮点击信号到选择ONNX文件槽函数)
    connect(yaml_but, &QPushButton::clicked, this, &Settings_Page::select_yaml_file); // 连接按钮点击信号到选择YAML文件槽函数
    connect(Start_but, &QPushButton::clicked, this, &Settings_Page::confirm_button); // 连接开始按钮点击信号到确认按钮槽函数)
    connect(End_but, &QPushButton::clicked, this, &Settings_Page::cancel_button); // 连接开始按钮点击信号到确认按钮槽函数)


    connect(MOUSE_CAPTURE, &Mouse_Capture::keyPressed, this, &Settings_Page::execute); // 连接结束按钮点击信号到取消按钮槽函数
    connect(this,&Settings_Page::imageCaptured,RELAY_SIGNALS, &Central_Bundle_Bridge::image_signal); // 连接结束按钮点击信号到取消按钮槽函数)
    connect(radioButton,&QRadioButton::toggled,RELAY_SIGNALS,&Central_Bundle_Bridge::status_signal); // 连接结束按钮点击信号到取消按钮槽函数)

}

Settings_Page::~Settings_Page()
{


}


void Settings_Page::select_onnx_file()
{
    //当前目录下是否有onnx_yaml文件夹
    QDir dir(QCoreApplication::applicationDirPath() + "/onnx_yaml");
    if (!dir.exists()) dir.mkpath("."); // 如果不存在则创建目录
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择ONNX文件"), dir.path(), tr("ONNX Files (*.onnx)")); // 打开文件对话框选择ONNX文件

    settings.setValue("Onnx_Path", fileName); //dir.path()文件夹路径 更新最后一次访问的路径


    //设置onnx_label
    onnx_label->setText(fileName); // 设置标签文本为选择的文件名

}

void Settings_Page::select_yaml_file()
{
    //当前目录下是否有onnx_yaml文件夹
    QDir dir(QCoreApplication::applicationDirPath() + "/onnx_yaml");
    if (!dir.exists()) dir.mkpath("."); // 如果不存在则创建目录
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择YAML文件"), dir.path(), tr("YAML Files (*.yaml)")); // 打开文件对话框选择YAML文件

    settings.setValue("Yaml_Path", fileName); //dir.path()文件夹路径 更新最后一次访问的路径

    //设置yaml_label
    yaml_label->setText(fileName); // 设置标签文本为选择的文件名


}

void Settings_Page::confirm_button()
{
    DL_INIT_PARAM init_params; // 创建初始化参数结构体

    //获取onnx_label的文本
    QString onnx_file = onnx_label->text(); // 获取ONNX文件路径
    //转换为std::string




    if (onnx_file.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请先选择ONNX文件")); // 如果未选择ONNX文件，弹出警告对话框
        return;
    }

    //获取yaml_label的文本
    QString yaml_file = yaml_label->text(); // 获取YAML文件路径
    //转换为std::string
    std::string yaml_file_str = yaml_file.toStdString(); // 将QString转换为std::string
    if (yaml_file.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请先选择YAML文件")); // 如果未选择YAML文件，弹出警告对话框
        return;
    }

    //获取comboBox的整数
    int thread_count = comboBox->currentData().toInt(); // 获取线程数
    if (thread_count < 1)
    {
        QMessageBox::warning(this, tr("错误"), tr("线程数必须大于0")); // 如果线程数小于1，弹出警告对话框
        return;
    }

    //获取lineEdit_linedit_3的小数
    float iou_threshold = lineEdit_linedit_3->text().toFloat() / 100.00; // 获取IOU阈值
    init_params.intraOpNumThreads = thread_count; // 设置线程数
    init_params.modelPath = onnx_file.toStdString(); // 设置模型路径
    init_params.rectConfidenceThreshold = iou_threshold; // 设置矩形置信度阈值
    execute_inference->DetectTest(init_params, yaml_file_str); // 读取Coco YAML文件

    _keyCode = qstringToVKey(lineEdit_linedit->text()); // 获取按键码
    qDebug() << "按键码：" << _keyCode; // 打印按键码

    //设置按钮不可点击状态
    Start_but->setEnabled(false); // 禁用开始按钮
}

void Settings_Page::cancel_button()
{
    //按钮可点击状态
    Start_but->setEnabled(true); // 启用开始按钮
}


// 将单个字符转换为虚拟键码
UINT Settings_Page::qstringToVKey(const QString& str)
{
    if (str.isEmpty()) return 0;

    // 只取第一个字符
    WCHAR ch = str.at(0).toUpper().unicode();

    // 使用VkKeyScan函数
    SHORT result = VkKeyScan(ch);
    if (result == -1) return 0;

    return LOBYTE(result);
}


void Settings_Page::execute(DWORD keyCode)
{
    if (Start_but->isEnabled()) return; // 如果按钮已启用，则返回

    if (_keyCode != keyCode) return; // 如果按键码不匹配，则返回


    QImage Image =  screenshot->Capture_DXGI_Image_And_Draw();
    if (Image.isNull())
    {
        qDebug() << "截图失败，无法执行推理。";
        return;
    }

    execute_inference->Draw(Image);

    qDebug() << "推理完成";

    emit imageCaptured(Image);






}











void Settings_Page::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); // 调用基类的resizeEvent函数
    int width = event->size().width(); // 获取新的宽度
    int height = event->size().height(); // 获取新的高度


    widget->setGeometry(QRect(0, 0, width ,height * 0.2 )); // 设置widget的几何形状
    widget_2->setGeometry(QRect(0, height * 0.2, widget_2->width(), height * 0.3)); // 设置widget_2的几何形状
    widget_3->setGeometry(QRect(width * 0.7 , height * 0.5, width * 0.3, height * 0.2)); // 设置widget_3的几何形状
    widget_4->setGeometry(QRect(0, height * 0.5, width * 0.3, height * 0.2)); // 设置widget_4的几何形状








} // setupUi






void Settings_Page::setupUi(QWidget *Widget)
{
    if (Widget->objectName().isEmpty())
        Widget->setObjectName("Widget");
    Widget->resize(1139, 600);
    widget = new QWidget(Widget);
    widget->setObjectName("widget");
    widget->setGeometry(QRect(60, 40, 901, 101));
    verticalLayout = new QVBoxLayout(widget);
    verticalLayout->setObjectName("verticalLayout");
    gridLayout_5 = new QGridLayout();
    gridLayout_5->setObjectName("gridLayout_5");
    gridLayout_3 = new QGridLayout();
    gridLayout_3->setObjectName("gridLayout_3");
    onnx_label = new QLabel(widget);
    onnx_label->setObjectName("onnx_label");
    //设置底色
    onnx_label->setStyleSheet("background-color: rgba(255, 255, 255,11);");

    onnx_label->setMaximumSize(QSize(16777215, 40));

    gridLayout_3->addWidget(onnx_label, 0, 0, 1, 1);

    onnx_but = new QPushButton(widget);
    onnx_but->setObjectName("onnx_but");
    onnx_but->setMaximumSize(QSize(40, 40));

    gridLayout_3->addWidget(onnx_but, 0, 1, 1, 1);


    gridLayout_5->addLayout(gridLayout_3, 0, 0, 1, 1);

    gridLayout_4 = new QGridLayout();
    gridLayout_4->setObjectName("gridLayout_4");
    yaml_label = new QLabel(widget);
    yaml_label->setObjectName("yaml_label");
    //设置底色
    yaml_label->setStyleSheet("background-color: rgba(255, 255, 255,11);");
    yaml_label->setMaximumSize(QSize(16777215, 40));

    gridLayout_4->addWidget(yaml_label, 0, 0, 1, 1);

    yaml_but = new QPushButton(widget);
    yaml_but->setObjectName("yaml_but");
    yaml_but->setMaximumSize(QSize(40, 40));

    gridLayout_4->addWidget(yaml_but, 0, 1, 1, 1);


    gridLayout_5->addLayout(gridLayout_4, 1, 0, 1, 1);


    verticalLayout->addLayout(gridLayout_5);

    widget_2 = new QWidget(Widget);
    widget_2->setObjectName("widget_2");
    widget_2->setGeometry(QRect(60, 160, 231, 181));
    verticalLayout_2 = new QVBoxLayout(widget_2);
    verticalLayout_2->setObjectName("verticalLayout_2");
    gridLayout = new QGridLayout();
    gridLayout->setObjectName("gridLayout");
    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setObjectName("horizontalLayout_4");
    Bind_Key_label = new QLabel(widget_2);
    Bind_Key_label->setObjectName("Bind_Key_label");
    Bind_Key_label->setMaximumSize(QSize(76, 16777215));



    horizontalLayout_4->addWidget(Bind_Key_label);

    lineEdit_linedit = new QLineEdit(widget_2);
    lineEdit_linedit->setObjectName("lineEdit_linedit");
    lineEdit_linedit->setMaximumSize(QSize(125, 40));

    horizontalLayout_4->addWidget(lineEdit_linedit);


    gridLayout->addLayout(horizontalLayout_4, 0, 0, 1, 1);

    horizontalLayout_6 = new QHBoxLayout();
    horizontalLayout_6->setObjectName("horizontalLayout_6");
    Bind_Key_label_2 = new QLabel(widget_2);
    Bind_Key_label_2->setObjectName("Bind_Key_label_2");
    Bind_Key_label_2->setMaximumSize(QSize(76, 16777215));

    horizontalLayout_6->addWidget(Bind_Key_label_2);

    comboBox = new QComboBox(widget_2);
    comboBox->setObjectName("comboBox");
    comboBox->setMaximumSize(QSize(125, 16777215));

    horizontalLayout_6->addWidget(comboBox);


    gridLayout->addLayout(horizontalLayout_6, 1, 0, 1, 1);

    horizontalLayout_7 = new QHBoxLayout();
    horizontalLayout_7->setObjectName("horizontalLayout_7");
    Bind_Key_label_3 = new QLabel(widget_2);
    Bind_Key_label_3->setObjectName("Bind_Key_label_3");
    Bind_Key_label_3->setMaximumSize(QSize(76, 16777215));

    horizontalLayout_7->addWidget(Bind_Key_label_3);

    lineEdit_linedit_3 = new QLineEdit(widget_2);
    lineEdit_linedit_3->setObjectName("lineEdit_linedit_3");
    lineEdit_linedit_3->setMaximumSize(QSize(125, 40));
    lineEdit_linedit_3->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
    lineEdit_linedit_3->setClearButtonEnabled(false);

    horizontalLayout_7->addWidget(lineEdit_linedit_3);


    gridLayout->addLayout(horizontalLayout_7, 2, 0, 1, 1);


    verticalLayout_2->addLayout(gridLayout);

    widget_3 = new QWidget(Widget);
    widget_3->setObjectName("widget_3");
    widget_3->setGeometry(QRect(770, 260, 191, 71));
    horizontalLayout = new QHBoxLayout(widget_3);
    horizontalLayout->setObjectName("horizontalLayout");
    gridLayout_2 = new QGridLayout();
    gridLayout_2->setObjectName("gridLayout_2");
    Start_but = new QPushButton(widget_3);
    Start_but->setObjectName("Start_but");
    Start_but->setMaximumSize(QSize(80, 23));

    gridLayout_2->addWidget(Start_but, 0, 0, 1, 1);

    End_but = new QPushButton(widget_3);
    End_but->setObjectName("End_but");
    End_but->setMaximumSize(QSize(80, 23));

    gridLayout_2->addWidget(End_but, 0, 1, 1, 1);


    horizontalLayout->addLayout(gridLayout_2);

    widget_4 = new QWidget(Widget);
    widget_4->setObjectName("widget_4");
    //设置widget_4最小大小为300*200
    widget_4->setMinimumSize(QSize(230, 80));


    verticalLayout_3 = new QVBoxLayout(widget_4);
    verticalLayout_3->setObjectName("verticalLayout_3");
    radioButton = new QRadioButton(widget_4);
    radioButton->setObjectName("radioButton");
    //设置radioButton默认被选中
    radioButton->setChecked(true); // 设置单选按钮默认选中状态

    verticalLayout_3->addWidget(radioButton);



    retranslateUi(Widget);

    QMetaObject::connectSlotsByName(Widget);
}


void Settings_Page::retranslateUi(QWidget *Widget)
{
    Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
    onnx_label->setText(QString());
    onnx_but->setText(QCoreApplication::translate("Widget", "onnx", nullptr));
    yaml_label->setText(QString());
    yaml_but->setText(QCoreApplication::translate("Widget", "yaml", nullptr));
    Bind_Key_label->setText(QCoreApplication::translate("Widget", "\347\273\221\345\256\232\346\214\211\351\224\256", nullptr));
    Bind_Key_label_2->setText(QCoreApplication::translate("Widget", "\347\272\277\347\250\213\346\225\260", nullptr));
    Bind_Key_label_3->setText(QCoreApplication::translate("Widget", "\344\277\241\344\273\273\345\272\246", nullptr));
    lineEdit_linedit_3->setInputMask(QString());
    lineEdit_linedit_3->setText(QString());
    Start_but->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
    End_but->setText(QCoreApplication::translate("Widget", "\347\273\223\346\235\237", nullptr));
    radioButton->setText(QCoreApplication::translate("Widget", "\346\230\257\345\220\246\345\234\250\345\261\217\345\271\225\344\270\212\346\230\276\347\244\272\344\270\215\345\217\257\347\202\271\345\207\273\347\232\204\346\226\271\346\241\206", nullptr));

} // retranslateUi




