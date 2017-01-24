#include "playercontrolbutton.h"

#include <QPainter>


PlayerControlButton::PlayerControlButton(QWidget * parent )
    : QPushButton(parent)
{
    connect(this,SIGNAL(toggled(bool)),this,SLOT(onChecked(bool)));

    m_borderPen = QPen(QBrush(QColor::fromRgb(36,110,161)),1,Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    m_hoverStatus = false;
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
}

void PlayerControlButton::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);

    if(m_hoverStatus)
    {
        QPainter painter(this);

        painter.setPen(m_borderPen);
        painter.drawRoundedRect(rect().adjusted(0,0,-1,-1),2,2);
    }


}

void PlayerControlButton::mouseMoveEvent(QMouseEvent *e)
{
    setHoverStatus(true);
    QPushButton::mouseMoveEvent(e);
}

void PlayerControlButton::mousePressEvent(QMouseEvent *e)
{
    QPushButton::mousePressEvent(e);
}

void PlayerControlButton::mouseReleaseEvent(QMouseEvent *e)
{

    QPushButton::mouseReleaseEvent(e);
}

void PlayerControlButton::leaveEvent(QEvent *e)
{
    setHoverStatus(false);
    QPushButton::leaveEvent(e);
}

void PlayerControlButton::setHoverStatus(bool hoverStatus)
{
    if(m_hoverStatus == hoverStatus)
        return;

    m_hoverStatus = hoverStatus;

}

void PlayerControlButton::setCheckedIcon(const QIcon &checkedIcon)
{
    m_checkedIcon = checkedIcon;
}

void PlayerControlButton::onChecked(bool is_check)
{
    if(is_check)
    {
        setIcon(m_checkedIcon);
    }
    else
    {
        setIcon(m_activeIcon);
    }
}

void PlayerControlButton::uncheck()
{
    if(isChecked())
        setChecked(false);
}

void PlayerControlButton::setActiveIcon(const QIcon &activeIcon)
{
    m_activeIcon = activeIcon;

    setIcon(m_activeIcon);
}

