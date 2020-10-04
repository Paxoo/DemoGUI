#ifndef PLAYEREQUIP_H
#define PLAYEREQUIP_H

#include <QMap>

class PlayerEquip
{
public:
    PlayerEquip();

private:
    int mTick;

    bool mBomb;
    bool mHE;
    bool mSmoke;
    bool mMolo;
    unsigned char mFlash;
};

#endif // PLAYEREQUIP_H
