#include "player.h"

Player::Player()
{

}

Player::~Player()
{

}

QRectF Player::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-16 - adjust, -16 - adjust, 50 + adjust , 25 + adjust);
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setBrush(m_color);
    painter->drawEllipse(0, 0, 10, 10);

    // Name
    painter->setPen(Qt::white);
    painter->drawText(QPoint(-6, -4), m_name);

    // Viewdirection
    //painter->setBrush(color);
    //painter->drawLine(QPoint(0,0),QPoint());
}


void Player::set_color(QColor color){
    this->m_color = color;
}

void Player::set_name(QString name){
   this->m_name = name;
}
