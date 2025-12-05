#pragma once

#include <QString>
#include <QList>
#include "DomainTypes.h"
#include "Serializer.h"

class SessionManager {
public:
    enum class SubmitResult {
        Correct,
        Wrong,
        FailRelearn,
        TopicFinished,
        CourseFinished
    };

    SessionManager();

    void loadCourse(const QString& filePath);
    bool isCourseLoaded() const;

    // --- ЧАСТЬ 1: Новый метод ---
    void startTopic(int topicIndex);

    // Геттеры
    const Course& getCourse() const; // Добавлен для совместимости с MainWindow
    Course& getMutableCourse();
    Topic* getCurrentTopic();
    Question* getCurrentQuestion();

    SubmitResult submitAnswer(int answerIndex);

private:
    Course currentCourse;
    bool m_isLoaded;

    qint32 currentTopicIndex;
    qint32 currentQuestionIndex;
    qint32 errorsInTopic;
};
