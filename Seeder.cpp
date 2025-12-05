#include "Seeder.h"
#include "DomainTypes.h"
#include "Serializer.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QVariant> // Для безопасной конвертации типов

void Seeder::generate(const QString& jsonPath, const QString& binPath) {
    // 1. Открытие файла JSON
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[Seeder] Error: Could not open JSON file at:" << jsonPath;
        return;
    }

    // 2. Чтение и парсинг документа
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "[Seeder] JSON Parse Error:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qDebug() << "[Seeder] Error: Root JSON element is not an Object.";
        return;
    }

    // 3. Формирование объекта Course
    Course course;
    QJsonObject root = doc.object();

    // Безопасное чтение массива тем
    if (root.contains("topics") && root["topics"].isArray()) {
        QJsonArray topicsArray = root["topics"].toArray();

        for (const QJsonValue& topicVal : topicsArray) {
            if (!topicVal.isObject()) continue;

            QJsonObject topicObj = topicVal.toObject();
            Topic topic;

            // Заполнение полей темы с дефолтными значениями
            topic.title = topicObj.value("title").toString("Untitled Topic");
            topic.htmlContent = topicObj.value("htmlContent").toString("<p>No content</p>");

            // Обработка вопросов внутри темы
            if (topicObj.contains("questions") && topicObj["questions"].isArray()) {
                QJsonArray questionsArray = topicObj["questions"].toArray();

                for (const QJsonValue& qVal : questionsArray) {
                    if (!qVal.isObject()) continue;

                    QJsonObject qObj = qVal.toObject();
                    Question question;

                    // Заполнение полей вопроса
                    question.text = qObj.value("text").toString("Empty Question");
                    question.correctIndex = qObj.value("correctIndex").toInt(0);

                    // Обработка вариантов ответов
                    if (qObj.contains("variants") && qObj["variants"].isArray()) {
                        QJsonArray variantsArray = qObj["variants"].toArray();
                        for (const QJsonValue& vVal : variantsArray) {
                            question.variants.append(vVal.toString("Empty Variant"));
                        }
                    }

                    // Валидация: если вариантов нет, вопрос может вызвать сбой в UI,
                    // добавим заглушку, если список пуст (опциональная защита)
                    if (question.variants.isEmpty()) {
                        question.variants << "Yes" << "No";
                    }

                    topic.questions.append(question);
                }
            }

            course.topics.append(topic);
        }
    } else {
        qDebug() << "[Seeder] Warning: No 'topics' array found in JSON.";
    }

    // 4. Сохранение через Serializer
    try {
        Serializer::save(course, binPath);
        qDebug() << "[Seeder] Success: Course generated and saved to" << binPath;
    } catch (const std::exception& e) {
        qDebug() << "[Seeder] Critical Error during serialization:" << e.what();
    }
}
