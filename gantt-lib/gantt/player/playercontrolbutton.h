#ifndef PLAYERCONTROLBUTTON_H
#define PLAYERCONTROLBUTTON_H

#include <QPushButton>
#include <QIcon>
#include <QPen>

class PlayerControlButton : public QPushButton
{
    Q_OBJECT

public:
    PlayerControlButton(QWidget * parent = 0);

    void setActiveIcon(const QIcon &activeIcon);

    void setCheckedIcon(const QIcon &checkedIcon);

public slots:
    void onChecked(bool is_check);
    void uncheck();

protected:
    void paintEvent(QPaintEvent *e);

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void leaveEvent(QEvent *);

private:
    void setHoverStatus(bool hoverStatus);

    QIcon m_activeIcon,
            m_checkedIcon;

    bool m_hoverStatus;
    QPen m_borderPen;
};

#endif // PLAYERCONTROLBUTTON_H
