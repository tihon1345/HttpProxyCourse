#include "TestWidget.h"
#include <QMessageBox>

TestWidget::TestWidget(QWidget *parent) : QWidget(parent) {
    auto mainLayout = new QVBoxLayout(this);

    questionLabel = new QLabel("Загрузка...", this);
    questionLabel->setWordWrap(true);
    QFont f = questionLabel->font();
    f.setPointSize(12);
    f.setBold(true);
    questionLabel->setFont(f);

    variantsContainer = new QWidget(this);
    variantsLayout = new QVBoxLayout(variantsContainer);
    btnAnswer = new QPushButton("Ответить", this);

    mainLayout->addWidget(questionLabel);
    mainLayout->addWidget(variantsContainer);
    mainLayout->addWidget(btnAnswer);
    mainLayout->addStretch();

    connect(btnAnswer, &QPushButton::clicked, this, [this]() {
        int idx = getSelectedVariantIndex();
        if (idx == -1) {
            QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите вариант ответа.");
            return;
        }
        emit answerSubmitted(idx);
    });
}

void TestWidget::showQuestion(const Question& question) {
    // Безопасная очистка layout с использованием deleteLater()
    QLayoutItem* item;
    while ((item = variantsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    radioButtons.clear();

    questionLabel->setText(question.text);

    for (const QString& variant : question.variants) {
        auto rb = new QRadioButton(variant, variantsContainer);
        variantsLayout->addWidget(rb);
        radioButtons.append(rb);
    }
}

int TestWidget::getSelectedVariantIndex() {
    for (int i = 0; i < radioButtons.size(); ++i) {
        if (radioButtons[i]->isChecked()) {
            return i;
        }
    }
    return -1;
}
