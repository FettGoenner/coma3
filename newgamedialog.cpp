#include "newgamedialog.h"
#include "ui_newgamedialog.h"
#include <QDebug>
#include <QRandomGenerator>

NewGameDialog::NewGameDialog(size_t size, QString algoType, QWidget *parent) :
    QDialog(parent)
  , _dim(size)
  , _seed(getRandomValue())
  , _algoType(algoType)
  , ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);
    setWindowTitle("New Game");
    ui->sizeSpinBox->setValue(_dim);
    ui->seedSpinBox->setValue(_seed);
    ui->algoTypeComboBox->setCurrentText(algoType);
    //get and set size
    connect(ui->sizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int size) {
        _dim = size;
    });
    //get and set seed value
    connect(ui->seedSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int seed) {
        _seed = seed;
    });

    connect(ui->getRandomSeedBtn, &QPushButton::clicked, this, [=]() {
        ui->seedSpinBox->setValue(getRandomValue());
    });
    connect(ui->algoTypeComboBox, &QComboBox::currentTextChanged, this, [=]() {
        _algoType = ui->algoTypeComboBox->currentText();
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
    return _dim;
}

size_t NewGameDialog::getSeed()
{
    return _seed;
}

QString NewGameDialog::getAlgoType()
{
    return _algoType;
}
