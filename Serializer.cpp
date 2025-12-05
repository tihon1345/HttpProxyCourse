#include "Serializer.h"
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <stdexcept>

void Serializer::save(const Course& course, const QString& filePath) {
    // 1. Сериализация данных в память (QByteArray)
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    // Фиксация версии для совместимости (Qt 5.15)
    stream.setVersion(QDataStream::Qt_5_15);

    stream << course;

    // 2. Шифрование XOR
    // Применяем маску 0x5A к каждому байту сериализованных данных
    for (int i = 0; i < data.size(); ++i) {
        data[i] = data[i] ^ XOR_KEY;
    }

    // 3. Запись в файл
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Serializer::save - Cannot open file for writing: " + filePath.toStdString());
    }

    file.write(data);
    file.close();
}

Course Serializer::load(const QString& filePath) {
    // 1. Чтение файла
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Serializer::load - Cannot open file for reading: " + filePath.toStdString());
    }

    QByteArray data = file.readAll();
    file.close();

    // Проверка на корректность содержимого
    if (data.isEmpty()) {
        throw std::runtime_error("Serializer::load - File is empty: " + filePath.toStdString());
    }

    // 2. Расшифровка XOR
    // Операция XOR обратима: (A ^ Key) ^ Key = A
    for (int i = 0; i < data.size(); ++i) {
        data[i] = data[i] ^ XOR_KEY;
    }

    // 3. Десериализация
    QDataStream stream(&data, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_5_15);

    Course course;
    stream >> course;

    // Проверка целостности потока после чтения
    if (stream.status() != QDataStream::Ok) {
        throw std::runtime_error("Serializer::load - Data corruption or version mismatch in: " + filePath.toStdString());
    }

    return course;
}
