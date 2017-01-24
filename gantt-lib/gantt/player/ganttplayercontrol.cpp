#include "ganttplayercontrol.h"
#include "ui_ganttplayercontrol.h"
#include "ganttplayersettings.h"

#include "gantt-lib_global_values.h"

#include <QTimer>
#include <QAbstractButton>

#include <QDebug>

void GanttPlayerControl::init()
{
    m_timer = std::make_pair<QTimer*,PlayMode>(NULL,PlayMode_count);
    m_speedModifier = 1.0;
    setPlayFrequency(GANTTPLAYER_DEFAULT_FREQUENCY);


    ui->stop->setActiveIcon(QIcon(":/icons/stop_off_64.png"));
    ui->stop->setCheckedIcon(QIcon(":/icons/stop_on_64.png"));

    ui->fragmentBegin->setActiveIcon(QIcon(":/icons/begin_off_64.png"));
    ui->fragmentBegin->setCheckedIcon(QIcon(":/icons/begin_on_64.png"));

    ui->prevEvent->setActiveIcon(QIcon(":/icons/prev_off_64.png"));
    ui->prevEvent->setCheckedIcon(QIcon(":/icons/prev_on_64.png"));

    ui->stepBackward->setActiveIcon(QIcon(":/icons/back_step_off_64.png"));
    ui->stepBackward->setCheckedIcon(QIcon(":/icons/back_step_on_64.png"));

    ui->backward->setActiveIcon(QIcon(":/icons/back_off_64.png"));
    ui->backward->setCheckedIcon(QIcon(":/icons/back_on_64.png"));

    ui->pause->setActiveIcon(QIcon(":/icons/pause_off_64.png"));
    ui->pause->setCheckedIcon(QIcon(":/icons/pause_on_64.png"));

    ui->forward->setActiveIcon(QIcon(":/icons/forward_off_64.png"));
    ui->forward->setCheckedIcon(QIcon(":/icons/forward_on_64.png"));

    ui->stepForward->setActiveIcon(QIcon(":/icons/forward_step_off_64.png"));
    ui->stepForward->setCheckedIcon(QIcon(":/icons/forward_step_on_64.png"));

    ui->nextEvent->setActiveIcon(QIcon(":/icons/next_off_64.png"));
    ui->nextEvent->setCheckedIcon(QIcon(":/icons/next_on_64.png"));

    ui->fragmentEnd->setActiveIcon(QIcon(":/icons/end_off_64.png"));
    ui->fragmentEnd->setCheckedIcon(QIcon(":/icons/end_on_64.png"));

    ui->record->setActiveIcon(QIcon(":/icons/rec_off_64.png"));
    ui->record->setCheckedIcon(QIcon(":/icons/rec_on_64.png"));
}

GanttPlayerControl::GanttPlayerControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GanttPlayerControl)
{
    ui->setupUi(this);

    init();
}

GanttPlayerControl::~GanttPlayerControl()
{
    delete ui;
}

void GanttPlayerControl::makeStepForward()
{
    emit makeStep(_MICROSECONDS_IN_SECOND * m_speedModifier / m_playFrequency);
}

void GanttPlayerControl::makeStepBackward()
{
    emit makeStep(-_MICROSECONDS_IN_SECOND * m_speedModifier / m_playFrequency);
}

void GanttPlayerControl::onSpeedChanged(qreal mult)
{
    setSpeedModifier(mult);
}


void GanttPlayerControl::updateTimers()
{
    if(m_timer.first)
    {
        disconnect(m_timer.first,SIGNAL(timeout()));
        delete m_timer.first;
        m_timer.first = new QTimer();
        connect(m_timer.first,SIGNAL(timeout()),this,(m_timer.second == PlayForward)?(SLOT(makeStepForward()))
                                                                                   :(SLOT(makeStepBackward())));
        m_timer.first->start(msec());

    }
}

int GanttPlayerControl::msec()
{
    return _MILISECONDS_IN_SECOND * 1.0 / m_playFrequency;
}

void GanttPlayerControl::setSpeedModifier(const qreal &speedModifier)
{
    m_speedModifier = speedModifier;
    updateTimers();
}

void GanttPlayerControl::setSettings(GanttPlayerSettings *settings)
{
    if(!settings)
        return;
    setSpeedModifier(settings->currentSpeed());

    connect(settings,SIGNAL(speedChanged(qreal)),this,SLOT(onSpeedChanged(qreal)));
}

qreal GanttPlayerControl::playFrequency() const
{
    return m_playFrequency;
}

void GanttPlayerControl::setPlayFrequency(const qreal &playFrequency)
{
    m_playFrequency = playFrequency;
}


void GanttPlayerControl::on_fragmentBegin_clicked()
{
    emit goToViewStart();
}

void GanttPlayerControl::on_fragmentEnd_clicked()
{
    emit goToViewFinish();
}

void GanttPlayerControl::on_nextEvent_clicked()
{
    emit goToNextEventStart();
}

void GanttPlayerControl::on_prevEvent_clicked()
{
    emit goToPrevEventFinish();
}


void GanttPlayerControl::on_stepBackward_clicked()
{
    makeStepBackward();
}

void GanttPlayerControl::on_stepForward_clicked()
{
    makeStepForward();
}

void GanttPlayerControl::uncheckAll()
{
    foreach(PlayerControlButton* button, m_checkedButtons)
    {
        button->setChecked(false);
        m_checkedButtons.removeOne(button);
    }

    if(m_timer.first)
    {
        m_timer.first->deleteLater();
        m_timer.first = NULL;
    }
}

void GanttPlayerControl::on_forward_clicked(bool checked)
{
    uncheckAll();

    if(checked)
    {
        m_timer.first = new QTimer();
        connect(m_timer.first, SIGNAL(timeout()), this, SLOT(makeStepForward()));
        m_timer.first->start(msec());
        m_timer.second = PlayForward;

        PlayerControlButton *p_button = qobject_cast<PlayerControlButton*>(sender());
        if(p_button)
            m_checkedButtons.append(p_button);
    }
}

void GanttPlayerControl::on_backward_clicked(bool checked)
{
    uncheckAll();

    if(checked)
    {
        m_timer.first = new QTimer();
        connect(m_timer.first, SIGNAL(timeout()), this, SLOT(makeStepBackward()));
        m_timer.first->start(msec());
        m_timer.second = PlayBack;

        PlayerControlButton *p_button = qobject_cast<PlayerControlButton*>(sender());
        if(p_button)
            m_checkedButtons.append(p_button);
    }
}

void GanttPlayerControl::on_pause_clicked()
{
    uncheckAll();
}

void GanttPlayerControl::on_stop_clicked()
{
    uncheckAll();
    emit stop();
}
