#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include <QWidget>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#include <QtGui/QResizeEvent>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QRadioButton>
#include <QStandardPaths>



#include "execute_inference.h"
#include "screenshot.h"
#include "inference.h"   // 引入推理头文件
#include "mouse_capture.h"
#include "central_bundle_bridge.h" // 引入中央桥接头文件


class Settings_Page : public QWidget
{
    Q_OBJECT
public:
    explicit Settings_Page(QWidget *parent = nullptr);
    ~Settings_Page();

    void retranslateUi(QWidget *Widget);
    void setupUi(QWidget *Widget);



public:
    //选择onnx文件
    void select_onnx_file();
    //选择yaml文件
    void select_yaml_file();
    //确定按钮
    void confirm_button();
    //取消按钮
    void cancel_button();


    UINT qstringToVKey(const QString &str);
public slots:
    //执行推理
    void execute(DWORD keyCode);


    //信号
signals:
    void imageCaptured(const QImage& image);




protected:
    // 重写resize事件处理函数
    void resizeEvent(QResizeEvent *event) override;
    //后台监听按键


private:
    UINT _keyCode = 0; // 按键代码
    QSettings settings; //注册表




private:
    Execute_Inference *execute_inference = nullptr; // 执行推理对象
    Screenshot *screenshot = nullptr; // 截图对象

    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_3;
    QLabel *onnx_label;
    QPushButton *onnx_but;
    QGridLayout *gridLayout_4;
    QLabel *yaml_label;
    QPushButton *yaml_but;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *Bind_Key_label;
    QLineEdit *lineEdit_linedit;
    QHBoxLayout *horizontalLayout_6;
    QLabel *Bind_Key_label_2;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_7;
    QLabel *Bind_Key_label_3;
    QLineEdit *lineEdit_linedit_3;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_2;
    QPushButton *Start_but;
    QPushButton *End_but;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *radioButton;

};

#endif // SETTINGS_PAGE_H
