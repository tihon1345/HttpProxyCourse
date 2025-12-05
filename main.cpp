#include "mainwindow.h"
#include "Seeder.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Проверяем наличие файла данных курса и создаем его при необходимости
    const QString courseDataFile = "course.dat";
    const QString sourceJsonFile = "source.json";
    
    if (!QFile::exists(courseDataFile)) {
        qDebug() << "Course data file not found. Generating from" << sourceJsonFile;
        try {
            Seeder::generate(sourceJsonFile, courseDataFile);
            qDebug() << "Course data file created successfully!";
        } catch (const std::exception& e) {
            qCritical() << "Failed to generate course data:" << e.what();
            qCritical() << "Application may not work properly without course data.";
        }
    } else {
        qDebug() << "Course data file found:" << courseDataFile;
    }
    
    MainWindow w;
    w.show();
    return a.exec();
}
