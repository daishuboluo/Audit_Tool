#include "central_bundle_bridge.h"


Q_GLOBAL_STATIC(Central_Bundle_Bridge, fileRelatedInstance) // 创建File_Related类的全局静态单例实例


Central_Bundle_Bridge::Central_Bundle_Bridge(QObject *parent)
    : QObject{parent}
{

}



Central_Bundle_Bridge* Central_Bundle_Bridge::instance() // 获取File_Related的单例实例
{
    return fileRelatedInstance; // 返回全局静态实例
}











