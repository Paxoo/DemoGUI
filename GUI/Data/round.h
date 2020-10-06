#ifndef ROUND_H
#define ROUND_H

#include <QtCore>
#include <GUI\Data\Player\player.h>

class Round
{
public:
    Round(QList<QString> &ids);

    void setRoundTicks(ushort startTick, ushort freezeEndTick, ushort endTick, QString reasonRoundEnd);
    void setTstats(ushort startTscore, ushort tmoneySpentTotal, ushort tmoneySpentRound, QString tfreezetimeEndEquipValue);
    void setCTstats(ushort startCTscore, ushort ctmoneySpentTotal, ushort ctmoneySpentRound, QString ctfreezetimeEndEquipValue);

    ushort getStartTick();
    ushort getFreezeEndTick();
    ushort getEndTick();
    QString getReasonRoundEnd();
    ushort getStartTscore();
    ushort getStartCTscore();
    ushort getTmoneySpentTotal();
    ushort getTmoneySpentRound();
    QString getTfreezetimeEndEquipValue();
    ushort getCTmoneySpentTotal();
    ushort getCTmoneySpentRound();
    QString getCTfreezetimeEndEquipValue();

    QList<Player> getListPlayer();

private:
    ushort mStartTick;
    ushort mFreezeEndTick;
    ushort mEndTick;
    QString mReasonRoundEnd;

    ushort mStartTscore;
    ushort mStartCTscore;

    ushort mTmoneySpentTotal;
    ushort mTmoneySpentRound;
    QString mTfreezetimeEndEquipValue;
    ushort mCTmoneySpentTotal;
    ushort mCTmoneySpentRound;
    QString mCTfreezetimeEndEquipValue;

    QList<Player> mListPlayer;
};

#endif // ROUND_H
