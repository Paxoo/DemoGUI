#ifndef ROUND_H
#define ROUND_H

#include <QtCore>
#include <GUI\Data\Player\player.h>

class Round
{
public:
    Round(QList<QString> &ids);
    ~Round();

    void setRoundStartStats(uint32_t startTick, ushort startTscore, ushort startCTscore);
    void setFreezeEndTick(uint32_t freezeEndTick);
    void setEndTick(uint32_t endTick);
    void setRoundPurchase(uint32_t tmoneySpentTotal, ushort tmoneySpentRound, QString tfreezetimeEndEquipValue, uint32_t ctmoneySpentTotal, ushort ctmoneySpentRound, QString ctfreezetimeEndEquipValue);
    void setRoundEndStats(QString winner, QString reasonRoundEnd);

    uint32_t getStartTick();
    uint32_t getFreezeEndTick();
    uint32_t getEndTick();
    QString getRoundWinner();
    QString getReasonRoundEnd();
    ushort getStartTscore();
    ushort getStartCTscore();
    uint32_t getTmoneySpentTotal();
    ushort getTmoneySpentRound();
    QString getTfreezetimeEndEquipValue();
    uint32_t getCTmoneySpentTotal();
    ushort getCTmoneySpentRound();
    QString getCTfreezetimeEndEquipValue();

    QList<QSharedPointer<Player>> getListPlayer();

private:
    uint32_t mStartTick = 0;
    uint32_t mFreezeEndTick = 0;
    uint32_t mEndTick = 0;
    QString mReasonRoundEnd = "";

    ushort mStartTscore = 0;
    ushort mStartCTscore = 0;
    QString mRoundWinner = "";

    uint32_t mTmoneySpentTotal = 0;
    ushort mTmoneySpentRound = 0;
    QString mTfreezetimeEndEquipValue = "";
    uint32_t mCTmoneySpentTotal = 0;
    ushort mCTmoneySpentRound = 0;
    QString mCTfreezetimeEndEquipValue = "";

    QList<QSharedPointer<Player>> mListPlayer;
};

#endif // ROUND_H
