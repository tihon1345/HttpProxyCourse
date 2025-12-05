#pragma once

#include "DomainTypes.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QList>

/**
 * @brief Виджет тестирования знаний.
 * 
 * Отображает вопросы с вариантами ответов и обрабатывает выбор студента.
 */
class TestWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор виджета тестирования.
     * @param parent Родительский виджет.
     */
    explicit TestWidget(QWidget *parent = nullptr);

    /**
     * @brief Отображает вопрос с вариантами ответов.
     * @param question Вопрос для отображения.
     */
    void showQuestion(const Question& question);

    /**
     * @brief Возвращает индекс выбранного варианта ответа.
     * @return Индекс выбранного варианта или -1, если ничего не выбрано.
     */
    int getSelectedVariantIndex();

signals:
    /**
     * @brief Сигнал отправки ответа.
     * @param index Индекс выбранного ответа.
     */
    void answerSubmitted(int index);

private:
    QLabel* questionLabel;
    QWidget* variantsContainer;
    QVBoxLayout* variantsLayout;
    QPushButton* btnAnswer;
    QList<QRadioButton*> radioButtons;
};
