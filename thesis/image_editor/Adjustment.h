#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H

#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QSlider>

class Adjustment : public QGridLayout
{
public:
    Adjustment(QString title, int min, int max, int defaultValue);
    Adjustment(QString title);

    QPushButton* button();
    int value();
private:
    QString _title;
    int _min;
    int _max;
    int _defaultValue;

    QLabel* _labelTitle;
    QLabel* _labelMin;
    QLabel* _labelMax;
    QLabel* _labelValue;
    QSlider* _slider;
    QPushButton* _button;

private slots:
    void onSliderValueChanged();
};

#endif // ADJUSTMENT_H
