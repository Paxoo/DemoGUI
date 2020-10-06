#include "player.h"

Player::Player(QString id)
{
    this->mID = id;
}

void Player::setName(QString name)
{
    this->mName = name;
}

void Player::setPlayerSide(QString playerSide){
    this->mPlayerSide = playerSide;
}

void Player::incrementKills()
{
    this->mKills = this->mKills + 1;
}

void Player::incrementAssists()
{
    this->mAssists = this->mAssists + 1;
}

void Player::incrementDeaths()
{
    this->mDeaths = this->mDeaths + 1;
}

void Player::incrementEnemyFlashes()
{
    this->mEnemyFlashes = this->mEnemyFlashes + 1;
}

void Player::incrementTeamFlahes()
{
    this->mTeamFlashes = this->mTeamFlashes + 1;
}

void Player::incrementUtilityDMG(unsigned short dmg)
{
    this->mUtilityDMG = this->mUtilityDMG + dmg;
}

QString Player::getID()
{
    return this->mID;
}

QString Player::getName()
{
    return this->mName;
}

QString Player::getPlayerSide()
{
    return this->mPlayerSide;
}

ushort Player::getKills()
{
    return this->mKills;
}

ushort Player::getAssists()
{
    return this->mAssists;
}

ushort Player::getDeaths()
{
    return this->mDeaths;
}

ushort Player::getEnemyFlashes()
{
    return this->mEnemyFlashes;
}

ushort Player::getTeamFlahes()
{
    return this->mTeamFlashes;
}

ushort Player::getUtilityDMG()
{
    return this->mUtilityDMG;
}



void Player::addPlayerInfo(int tick, QPointF playerPosition, float playerXView, unsigned short money, unsigned char health, unsigned char armor, bool helmet, bool kit, QString activeWeapon){
    PlayerInfo info(tick, playerPosition, playerXView, money, health, armor, helmet, kit, activeWeapon);
    this->mListPlayerInfo.append(info);
}

QList<PlayerInfo> Player::getListPlayerInfo(){
    return this->mListPlayerInfo;
}
