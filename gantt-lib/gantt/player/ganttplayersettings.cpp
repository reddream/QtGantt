#include "ganttplayersettings.h"

#include <QMenu>
#include <QToolButton>

#include <QWidgetAction>
#include <QSlider>
#include <QPointer>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>

#include <QDebug>

GanttPlayerSettings::GanttPlayerSettings(QWidget *parent) :
    QToolBar(parent)
{
    m_precision = 1000;
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setStyleSheet("QToolBar { border: 0px }");

    initActions();
}

GanttPlayerSettings::~GanttPlayerSettings()
{
}

void GanttPlayerSettings::setMultiplies(qreal min, qreal max)
{
    if(m_slider)
    {
        m_slider->setRange((min*m_precision),(max*m_precision));
        if(m_left)
            m_left->setText("x"+QString::number(min));
        if(m_right)
            m_right->setText("x"+QString::number(max));
        if(m_cur!=NULL)
        {
            m_cur->setRange(min,max);
        }
    }
}

void GanttPlayerSettings::initActions()
{
    QAction *p_action;

    p_action = new QAction(QIcon(":/images/Buttons 64x64/reg_speed_on_64.png"),QString::fromUtf8("Выбор скорости воспроизведения"),this);

    QMenu *menu = new QMenu(this);
    QGroupBox* speedSelectionWidget = new QGroupBox(QString::fromUtf8("Скорость воспроизведения"));

    QWidgetAction *wa = new QWidgetAction(this);




    m_slider = new QSlider(menu);

    QVBoxLayout *v_layout = new QVBoxLayout(speedSelectionWidget);
    v_layout->addWidget(m_slider);

    QHBoxLayout *h_lay = new QHBoxLayout;
    m_left = new QLabel();
    m_right = new QLabel();
    m_cur = new QDoubleSpinBox();
    m_cur->setButtonSymbols(QAbstractSpinBox::NoButtons);
    connect(m_cur,SIGNAL(valueChanged(double)),this,SLOT(onCurTextChanged()));
    QSpacerItem *spacer1 = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Minimum),
            *spacer2 = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Minimum);

    h_lay->addWidget(m_left);
    h_lay->addSpacerItem(spacer1);
    h_lay->addWidget(m_cur);
    h_lay->addSpacerItem(spacer2);
    h_lay->addWidget(m_right);

    v_layout->addLayout(h_lay);


    setStyleSheet(
                "QSlider::groove:horizontal {"
                    "border: 1px solid #999999;"
                    "height: 8px;"
                    "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                    "margin: 2px 0;"
                "}"

                "QSlider::handle:horizontal {"
                  "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                  "border: 1px solid #0c457e;"
                  "width: 8px;"
                  "margin: -2px 0;"
                  "border-radius: 2px;"
                  "}");

    m_slider->setOrientation(Qt::Horizontal);
    connect(m_slider,SIGNAL(valueChanged(int)),this,SLOT(onSpeedChanged(int)));
    setMultiplies(0.5,300);
    setCurrentSpeed(1.0);
    if(m_cur)
        m_cur->setValue(m_slider->value()*1.0/m_precision);


    wa->setDefaultWidget(speedSelectionWidget);
    menu->addAction(wa);
    p_action->setMenu(menu);
    addAction(p_action);

    QToolButton *p_button = qobject_cast<QToolButton*>(widgetForAction(p_action));
    if(p_button)
        p_button->setPopupMode(QToolButton::InstantPopup);

//    p_action = new QAction(QIcon(":/images/Buttons 64x64/reg_step_on_64.png"),QString::fromUtf8("выбор шага воспроизведения"),this);
//    addAction(p_action);
}

int GanttPlayerSettings::precision() const
{
    return m_precision;
}

void GanttPlayerSettings::setCurrentSpeed(qreal speed) const
{
    Q_ASSERT(m_slider);
    m_slider->setValue(speed * m_precision);
}

qreal GanttPlayerSettings::currentSpeed() const
{
    Q_ASSERT(m_slider);
    return m_slider->value() * 1.0 / m_precision;
}

void GanttPlayerSettings::onSpeedChanged(int m)
{
    qreal val = m*1.0/m_precision;

    if(m_cur)
    {
        m_cur->blockSignals(true);
        m_cur->setValue(val);
        m_cur->blockSignals(false);
    }

    emit speedChanged(val);
}

void GanttPlayerSettings::onCurTextChanged()
{
    if(!m_cur)
        return;
    m_cur->blockSignals(true);
    setCurrentSpeed(m_cur->value());
    m_cur->blockSignals(false);
}


