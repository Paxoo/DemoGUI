#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H


class PlayerStats
{
public:
    PlayerStats(int tick, unsigned char kills,  unsigned char assists,  unsigned char deaths);

    int getTick();
    unsigned char getKills();
    unsigned char getAssists();
    unsigned char getDeaths();

private:
    int mTick;
    unsigned char mKills;
    unsigned char mAssists;
    unsigned char mDeaths;
};

#endif // PLAYERSTATS_H
