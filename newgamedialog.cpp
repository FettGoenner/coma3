#include "newgamedialog.h"
#include "ui_newgamedialog.h"
#include <QDebug>
#include <QRandomGenerator>

NewGameDialog::NewGameDialog(size_t size, QString algoType, QWidget *parent) :
    QDialog(parent)
    , _DIM(size)
    , seed(this->getRandomValue())
    , algoType(algoType)
    , ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("New Game");
    ui->sizeSpinBox->setValue(this->_DIM);
    ui->seedSpinBox->setValue(this->seed);
    ui->algoTypeComboBox->setCurrentText(this->algoType);
    //get and set size
    connect(ui->sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int size) {
        this->_DIM = size;
    });
    //get and set seed value
    connect(ui->seedSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int seed) {
        this->seed = seed;
    });

    connect(ui->getRandomSeedBtn, &QPushButton::clicked, this, [=]() {
        ui->seedSpinBox->setValue(this->getRandomValue());
    });
    connect(ui->algoTypeComboBox, &QComboBox::currentTextChanged, this, [=]() {
        this->algoType = ui->algoTypeComboBox->currentText();
    });

}

int NewGameDialog::getRandomValue()
{
    return QRandomGenerator::global()->bounded(0, INT_MAX);
}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}

size_t NewGameDialog::getSize()
{
    return this->_DIM;
}

size_t NewGameDialog::getSeed()
{
    return this->seed;
}

QString NewGameDialog::getAlgoType()
{
    return this->algoType;
}
