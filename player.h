#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QObject>
#include <QPainter>


class Player : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    Player();
    virtual ~Player();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

    void set_color(QColor color);
    void set_name(QString name);

private:
    QColor m_color;
    QString m_name;
};


#endif // PLAYER_H
