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

void SessionManager::startTopic(int topicIndex) {
    if (!m_isLoaded) {
        throw std::runtime_error("Course not loaded");
    }
    if (topicIndex < 0 || topicIndex >= static_cast<int>(currentCourse.topics.size())) {
        throw std::runtime_error("Invalid topic index");
    }

    currentTopicIndex = topicIndex;
    currentQuestionIndex = 0;
    errorsInTopic = 0;
}

const Course& SessionManager::getCourse() const {
    return currentCourse;
}

Course& SessionManager::getMutableCourse() {
    return currentCourse;
}

Topic* SessionManager::getCurrentTopic() {
    if (!m_isLoaded || currentTopicIndex < 0 || currentTopicIndex >= static_cast<int>(currentCourse.topics.size())) {
        return nullptr;
    }
    return &currentCourse.topics[currentTopicIndex];
}

Question* SessionManager::getCurrentQuestion() {
    Topic* topic = getCurrentTopic();
    if (!topic) return nullptr;

    if (currentQuestionIndex < 0 || currentQuestionIndex >= static_cast<int>(topic->questions.size())) {
        return nullptr;
    }
    return &topic->questions[currentQuestionIndex];
}

SessionManager::SubmitResult SessionManager::submitAnswer(int answerIndex) {
    if (!m_isLoaded) return SubmitResult::Wrong;

    Topic* topic = getCurrentTopic();
    Question* question = getCurrentQuestion();

    if (!topic || !question) return SubmitResult::Wrong;

    // Проверяем границы answerIndex
    if (answerIndex < 0 || answerIndex >= static_cast<int>(question->variants.size())) {
        return SubmitResult::Wrong;
    }

    // Проверяем корректность correctIndex (дополнительная защита)
    if (question->correctIndex < 0 || question->correctIndex >= static_cast<int>(question->variants.size())) {
        return SubmitResult::Wrong;
    }

    if (answerIndex == question->correctIndex) {
        currentQuestionIndex++;

        if (currentQuestionIndex >= static_cast<int>(topic->questions.size())) {
            if (currentTopicIndex >= static_cast<int>(currentCourse.topics.size()) - 1) {
                return SubmitResult::CourseFinished;
            }
            return SubmitResult::TopicFinished;
        }
        return SubmitResult::Correct;
    } else {
        errorsInTopic++;
        if (errorsInTopic >= 3) {
            currentQuestionIndex = 0;
            errorsInTopic = 0;
            return SubmitResult::FailRelearn;
        }
        return SubmitResult::Wrong;
    }
}
