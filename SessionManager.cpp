#include "SessionManager.h"
#include <stdexcept>

SessionManager::SessionManager()
    : m_isLoaded(false)
    , currentTopicIndex(-1)
    , currentQuestionIndex(-1)
    , errorsInTopic(0)
{
}

void SessionManager::loadCourse(const QString& filePath) {
    try {
        currentCourse = Serializer::load(filePath);
        m_isLoaded = true;
        // Сброс индексов, пока тема не выбрана
        currentTopicIndex = -1;
        currentQuestionIndex = -1;
        errorsInTopic = 0;
    } catch (const std::exception& e) {
        m_isLoaded = false;
        throw std::runtime_error(std::string("Failed to load course: ") + e.what());
    }
}

bool SessionManager::isCourseLoaded() const {
    return m_isLoaded;
}

// --- ЧАСТЬ 1: Реализация startTopic ---
void SessionManager::startTopic(int topicIndex) {
    if (!m_isLoaded) {
        throw std::runtime_error("Course not loaded");
    }
    if (topicIndex < 0 || topicIndex >= currentCourse.topics.size()) {
        throw std::runtime_error("Invalid topic index");
    }

    currentTopicIndex = topicIndex;
    currentQuestionIndex = 0; // Начинаем с первого вопроса
    errorsInTopic = 0;        // Сбрасываем ошибки
}

const Course& SessionManager::getCourse() const {
    return currentCourse;
}

// // Возвращает ссылку без const, позволяя менять поля Course
// Course& SessionManager::getMutableCourse() {
//     return m_course;
// }

Topic* SessionManager::getCurrentTopic() {
    if (!m_isLoaded || currentTopicIndex < 0 || currentTopicIndex >= currentCourse.topics.size()) {
        return nullptr;
    }
    return &currentCourse.topics[currentTopicIndex];
}

Question* SessionManager::getCurrentQuestion() {
    Topic* topic = getCurrentTopic();
    if (!topic) return nullptr;

    if (currentQuestionIndex < 0 || currentQuestionIndex >= topic->questions.size()) {
        return nullptr;
    }
    return &topic->questions[currentQuestionIndex];
}

SessionManager::SubmitResult SessionManager::submitAnswer(int answerIndex) {
    if (!m_isLoaded) return SubmitResult::Wrong;

    Topic* topic = getCurrentTopic();
    Question* question = getCurrentQuestion();

    if (!topic || !question) return SubmitResult::Wrong;

    if (answerIndex == question->correctIndex) {
        // Верно
        currentQuestionIndex++;

        // Проверка конца темы
        if (currentQuestionIndex >= topic->questions.size()) {
            // Тема пройдена.
            // ВАЖНО: Мы не переключаем currentTopicIndex автоматически здесь,
            // чтобы UI мог показать "Тема завершена" в контексте текущей темы.
            // Переключение произойдет, когда пользователь выберет следующую тему в меню.

            // Однако, для проверки CourseFinished нужно знать, последняя ли это тема.
            if (currentTopicIndex >= currentCourse.topics.size() - 1) {
                return SubmitResult::CourseFinished;
            }
            return SubmitResult::TopicFinished;
        }
        return SubmitResult::Correct;
    } else {
        // Ошибка
        errorsInTopic++;
        if (errorsInTopic >= 3) {
            // Сброс прогресса
            currentQuestionIndex = 0;
            errorsInTopic = 0;
            return SubmitResult::FailRelearn;
        }
        return SubmitResult::Wrong;
    }
}
