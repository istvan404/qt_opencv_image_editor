#include "Adjustment.h"

Adjustment::Adjustment(QString title, int min, int max, int defaultValue)
{
    _title = title;
    _min = min;
    _max = max;
    _defaultValue = defaultValue;

    _labelTitle = new QLabel(_title);
    _labelMin = new QLabel(QString::number(_min));
    _labelMax = new QLabel(QString::number(_max));
    _labelValue = new QLabel(QString::number(_defaultValue));
    _slider = new QSlider(Qt::Horizontal);
    _button = new QPushButton("Apply");

    _slider->setRange(_min, _max);
    _slider->setTickInterval(1);
    _slider->setValue(_defaultValue);
    _slider->setTickPosition(QSlider::TicksAbove);

    this->setSpacing(0);
    this->addWidget(_labelTitle, 0, 0, 1, 4);
    this->addWidget(_labelMin, 1, 0, 1, 1);
    this->addWidget(_labelMax, 1, 2, 1, 1, Qt::AlignRight);
    this->addWidget(_labelValue, 2, 3, 1, 1, Qt::AlignCenter);
    this->addWidget(_slider, 2, 0, 1, 3);
    this->addWidget(_button, 3, 0, 1, 4);

    connect(_slider, &QSlider::valueChanged, this, &Adjustment::onSliderValueChanged);
}

Adjustment::Adjustment(QString title)
{
    _title = title;

    _labelTitle = new QLabel(_title);
    _button = new QPushButton("Apply");

    this->setSpacing(0);
    this->addWidget(_labelTitle, 0, 0, 1, 4);
    this->addWidget(_button, 1, 0, 1, 4);
}

QPushButton* Adjustment::button()
{
    return _button;
}

int Adjustment::value()
{
    return _slider->value();
}

void Adjustment::onSliderValueChanged()
{
    _labelValue->setText( QString::number(_slider->value()) );
}
