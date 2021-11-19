#include <QtWidgets>
#include "values.h"

Values::Values()
{
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *temperature_l = new QLabel(tr("Temperature (°C)"));


    QFont t = temperature_l->font();
    t.setPointSize(200);
    t.setBold(true);
    temperature_l->setFont(t);


    temperature_l->setAlignment(Qt::AlignTop);

    
    temperature_v = new QLabel();


    QFont f = temperature_v->font();
    f.setPointSize(100);
    f.setBold(true);
    temperature_v->setFont(f);
    
    temperature_v->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    

    layout->addWidget(temperature_l);
    layout->addWidget(temperature_v);
    

    setLayout(layout);
}

void Values::handleValueChanged(float temp)
{
    temperature_v->setText(QString::number(temp, 'f', 2));
}