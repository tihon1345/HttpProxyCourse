#pragma once

#include "DomainTypes.h"
#include "Serializer.h"
#include <QString>
#include <QList>

/**
 * @brief Менеджер сессии обучения.
 * 
 * Управляет состоянием текущей сессии обучения, включая загрузку курса,
 * навигацию по темам и вопросам, обработку ответов студента.
 */
class SessionManager {
public:
    /**
     * @brief Результат отправки ответа на вопрос.
     */
    enum class SubmitResult {
        Correct,        ///< Правильный ответ
        Wrong,          ///< Неправильный ответ
        FailRelearn,    ///< Слишком много ошибок, нужно повторить тему
        TopicFinished,  ///< Тема завершена успешно
        CourseFinished  ///< Весь курс завершен
    };

    /**
     * @brief Конструктор менеджера сессии.
     */
    SessionManager();

    /**
     * @brief Загружает курс из файла.
     * @param filePath Путь к файлу с данными курса.
     */
    void loadCourse(const QString& filePath);

    /**
     * @brief Проверяет, загружен ли курс.
     * @return true, если курс загружен.
     */
    bool isCourseLoaded() const;

    /**
     * @brief Начинает изучение указанной темы.
     * @param topicIndex Индекс темы для изучения.
     */
    void startTopic(int topicIndex);

    /**
     * @brief Возвращает константную ссылку на курс.
     * @return Ссылка на объект курса.
     */
    const Course& getCourse() const;

    /**
     * @brief Возвращает изменяемую ссылку на курс.
     * @return Изменяемая ссылка на объект курса.
     */
    Course& getMutableCourse();

    /**
     * @brief Возвращает указатель на текущую тему.
     * @return Указатель на текущую тему или nullptr.
     */
    Topic* getCurrentTopic();

    /**
     * @brief Возвращает указатель на текущий вопрос.
     * @return Указатель на текущий вопрос или nullptr.
     */
    Question* getCurrentQuestion();

    /**
     * @brief Обрабатывает ответ студента на текущий вопрос.
     * @param answerIndex Индекс выбранного ответа.
     * @return Результат обработки ответа.
     */
    SubmitResult submitAnswer(int answerIndex);

private:
    Course currentCourse;
    bool m_isLoaded;

    qint32 currentTopicIndex;
    qint32 currentQuestionIndex;
    qint32 errorsInTopic;
};
