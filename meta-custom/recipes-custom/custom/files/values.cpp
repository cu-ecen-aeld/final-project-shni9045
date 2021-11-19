#include <QtWidgets>
#include "values.h"

Values::Values()
{
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *temperature_l = new QLabel(tr("Temperature (°C)"));
    
    temperature_v = new QLabel();


    QFont f = temperature_v->font();
    f.setPointSize(20);
    f.setBold(true);
    temperature_v->setFont(f);
    
    temperature_v->setAlignment(Qt::AlignVCenter);
    

    layout->addWidget(temperature_l);
    layout->addWidget(temperature_v);
    

    setLayout(layout);
}

void Values::handleValueChanged(float temp)
{
    temperature_v->setText(QString::number(temp, 'f', 2));
}