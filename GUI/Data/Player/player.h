#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QObject>
#include <QPainter>

#include "playerInfo.h"

class Player
{
public:
    Player(QString id);

    void setName(QString name);
    void setPlayerSide(QString playerSide);
    void incrementKills();
    void incrementAssists();
    void incrementDeaths();
    void incrementEnemyFlashes();
    void incrementTeamFlahes();
    void incrementUtilityDMG(unsigned short dmg);

    QString getID();
    QString getName();
    QString getPlayerSide();
    ushort getKills();
    ushort getAssists();
    ushort getDeaths();
    ushort getEnemyFlashes();
    ushort getTeamFlahes();
    ushort getUtilityDMG();

    void addPlayerInfo(int tick, QPointF playerPosition, float playerXView, unsigned short money, unsigned char health, unsigned char armor, bool helmet, bool kit, QString activeWeapon);
    QList<PlayerInfo> getListPlayerInfo();

private:
    QString mID = "";
    QString mName= "";
    QString mPlayerSide = "";
    ushort mKills = 0;
    ushort mAssists = 0;
    ushort mDeaths = 0;
    ushort mEnemyFlashes = 0;
    ushort mTeamFlashes = 0;
    ushort mUtilityDMG = 0;

    QList<PlayerInfo> mListPlayerInfo;
};


#endif // PLAYER_H
