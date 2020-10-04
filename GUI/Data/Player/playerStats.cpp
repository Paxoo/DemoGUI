#include "playerStats.h"


PlayerStats::PlayerStats(int tick, unsigned char kills, unsigned char assists, unsigned char deaths)
{
    this->mTick = tick;
    this->mKills = kills;
    this->mAssists = assists;
    this->mDeaths = deaths;
}

int PlayerStats::getTick()
{
    return this->mTick;
}

unsigned char PlayerStats::getKills()
{
    return this->mKills;
}

unsigned char PlayerStats::getAssists()
{
    return this->mAssists;
}

unsigned char PlayerStats::getDeaths()
{
    return this->mDeaths;
}


