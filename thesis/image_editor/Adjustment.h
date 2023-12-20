#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H

#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QSlider>

class AdjustmentBase : public QGridLayout
{
public:
    QPushButton* button() const;
protected:
    QString _title;
    QLabel* _labelTitle;
    QPushButton* _button;
};

class AdjustmentButton : public AdjustmentBase
{
public:
    AdjustmentButton(QString title);
};

class AdjustmentSlider : public AdjustmentBase
{
public:
    AdjustmentSlider(QString title, int min, int max, int value);
    int value() const;
private:
    int _min;
    int _max;
    int _default;
    QLabel* _labelMin;
    QLabel* _labelMax;
    QLabel* _labelValue;
    QSlider* _slider;

private slots:
    void onValueChanged();
};

#endif // ADJUSTMENT_H
