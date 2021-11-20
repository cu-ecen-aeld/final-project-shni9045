#include <QtWidgets>
#include "values.h"

Values::Values()
{
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *app_name = new QLabel(tr("AESD SECURE ATTENDANCE"));

    QFont s = app_name->font();
    s.setPointSize(100);
    s.setBold(true);
    s.setItalic(true);
    app_name->setFont(s);


    app_name->setAlignment(Qt::AlignLeft|Qt::AlignTop);



    QLabel *temperature_l = new QLabel(tr("USER BODY TEMP.(°C)"));


    QFont t = temperature_l->font();
    t.setPointSize(80);
    t.setBold(true);
    temperature_l->setFont(t);


    temperature_l->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    
    temperature_v = new QLabel();


    QFont f = temperature_v->font();
    f.setPointSize(100);
    f.setBold(true);
    temperature_v->setFont(f);
    
    temperature_v->setAlignment(Qt::AlignRight|Qt::AlignVCenter);


    QLabel *fingerprint = new QLabel(tr("FINGERPRINT STATUS"));

    QFont u = fingerprint->font();
    u.setPointSize(80);
    u.setBold(true);
    fingerprint->setFont(u);

    fingerprint->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
    
    layout->addWidget(app_name);
    layout->addWidget(temperature_l);
    layout->addWidget(temperature_v);
    layout->addWidget(fingerprint);

    setLayout(layout);
}

void Values::handleValueChanged(float temp)
{
    temperature_v->setText(QString::number(temp, 'f', 2));
}