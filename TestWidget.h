#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QList>
#include "DomainTypes.h"

class TestWidget : public QWidget {
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = nullptr);

    void showQuestion(const Question& question);
    int getSelectedVariantIndex();

signals:
    void answerSubmitted(int index);

private:
    QLabel* questionLabel;
    QWidget* variantsContainer;
    QVBoxLayout* variantsLayout;
    QPushButton* btnAnswer;

    // Список указателей на текущие радиокнопки для быстрого поиска
    QList<QRadioButton*> radioButtons;
};
