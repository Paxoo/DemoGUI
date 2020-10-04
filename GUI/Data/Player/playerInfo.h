#ifndef PLAYERINFOCONTAINER_H
#define PLAYERINFOCONTAINER_H

#include <QtCore>

class PlayerInfo
{
public:
    PlayerInfo(int tick, QPointF playerPosition, float playerXView, ushort money, ushort health, ushort armor, bool helmet, bool kit, QString activeWeapon);

    int getTick();
    QPointF getPlayerPosition();
    float getPlayerXView();
    ushort getMoney();
    ushort getHealth();
    ushort getArmor();
    bool getHelmet();
    bool getKit();
    QString getActiveWeapon();

private:
    int mTick;
    QPointF mPlayerPosition;
    float mPlayerXView;
    ushort mMoney;
    ushort mHealth;
    ushort mArmor;
    bool mHelmet;
    bool mKit;
    QString mActiveWeapon;
};

#endif // PLAYERINFOCONTAINER_H
