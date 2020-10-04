#include "playerInfo.h"

PlayerInfo::PlayerInfo(int tick, QPointF playerPosition, float playerXView, ushort money, ushort health, ushort armor, bool helmet, bool kit, QString activeWeapon)
{
    this->mTick = tick;
    this->mPlayerPosition = playerPosition;
    this->mPlayerXView = playerXView;
    this->mMoney = money;
    this->mHealth = health;
    this->mArmor = armor;
    this->mHelmet = helmet;
    this->mKit = kit;
    this->mActiveWeapon = activeWeapon;
}

int PlayerInfo::getTick(){
    return this->mTick;
}

QPointF PlayerInfo::getPlayerPosition(){
    return this->mPlayerPosition;
}

float PlayerInfo::getPlayerXView(){
    return this->mPlayerXView;
}

ushort PlayerInfo::getMoney()
{
    return this->mMoney;
}

ushort PlayerInfo::getHealth()
{
    return this->mHealth;
}

ushort PlayerInfo::getArmor()
{
    return this->mArmor;
}

bool PlayerInfo::getHelmet()
{
    return this->mHelmet;
}

bool PlayerInfo::getKit()
{
    return this->mKit;
}

QString PlayerInfo::getActiveWeapon()
{
    return this->mActiveWeapon;
}

