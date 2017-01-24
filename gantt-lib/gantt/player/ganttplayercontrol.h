#ifndef GANTTPLAYERCONTROL_H
#define GANTTPLAYERCONTROL_H

#include <QWidget>

namespace Ui {
class GanttPlayerControl;
}

class GanttPlayerSettings;
class PlayerControlButton;

class GanttPlayerControl : public QWidget
{
    Q_OBJECT

    void init();
public:
    explicit GanttPlayerControl(QWidget *parent = 0);
    ~GanttPlayerControl();

    qreal playFrequency() const;
    void setPlayFrequency(const qreal &playFrequency);

    void setSettings(GanttPlayerSettings *settings);

    void setSpeedModifier(const qreal &speedModifier);

signals:
    void makeStep(int stepLengthInMicrosec);
    void goToPrevEventFinish();
    void goToNextEventStart();
    void stop();
    void goToViewStart();
    void goToViewFinish();

private slots:
    void makeStepForward();
    void makeStepBackward();

    void onSpeedChanged(qreal mult);
    void updateTimers();

    void on_fragmentBegin_clicked();
    void on_fragmentEnd_clicked();
    void on_nextEvent_clicked();
    void on_prevEvent_clicked();
    void on_stepBackward_clicked();
    void on_stepForward_clicked();
    void uncheckAll();
    void on_forward_clicked(bool checked);
    void on_backward_clicked(bool checked);
    void on_pause_clicked();
    void on_stop_clicked();

private:
    int msec();

private:
    enum PlayMode
    {
        PlayForward,
        PlayBack,

        PlayMode_count
    };


    Ui::GanttPlayerControl *ui;

    qreal m_playFrequency; ///< Число шагов в секунду
    qreal m_speedModifier;

    QList<PlayerControlButton*> m_checkedButtons;
    std::pair<QTimer*,PlayMode> m_timer;
};

#endif // GANTTPLAYERCONTROL_H
