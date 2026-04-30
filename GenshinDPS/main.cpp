#include <QApplication>
#include "MainWindow.hpp"
#include "DataModel.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 程序退出前保存数据
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        DataModel::instance().saveToJson();
    });

    MainWindow w;
    w.show();
    return app.exec();
}
