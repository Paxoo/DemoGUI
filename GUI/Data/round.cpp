#include "round.h"


Round::Round(QList<QString> &ids)
{
    for(short i=0; i < ids.size(); i++){
        Player player(ids.at(i));
        this->mListPlayer.append(player);
    }
}

void Round::setRoundStartStats(uint32_t startTick, ushort startTscore, ushort startCTscore)
{
    this->mStartTick = startTick;
    this->mStartTscore = startTscore;
    this->mStartCTscore = startCTscore;
}

void Round::setFreezeEndTick(uint32_t freezeEndTick)
{
    this->mFreezeEndTick = freezeEndTick;
}

void Round::setEndTick(uint32_t endTick)
{
    this->mEndTick = endTick;
}

void Round::setRoundPurchase(uint32_t tmoneySpentTotal, ushort tmoneySpentRound, QString tfreezetimeEndEquipValue, uint32_t ctmoneySpentTotal, ushort ctmoneySpentRound, QString ctfreezetimeEndEquipValue)
{
    this->mTmoneySpentTotal = tmoneySpentTotal;
    this->mTmoneySpentRound = tmoneySpentRound;
    this->mTfreezetimeEndEquipValue = tfreezetimeEndEquipValue;
    this->mCTmoneySpentTotal = ctmoneySpentTotal;
    this->mCTmoneySpentRound = ctmoneySpentRound;
    this->mCTfreezetimeEndEquipValue = ctfreezetimeEndEquipValue;
}

void Round::setRoundEndStats(QString reasonRoundEnd)
{
    this->mReasonRoundEnd = reasonRoundEnd;
}

uint32_t Round::getStartTick()
{
    return this->mStartTick;
}

uint32_t Round::getFreezeEndTick()
{
    return this->mFreezeEndTick;
}

uint32_t Round::getEndTick()
{
    return this->mEndTick;
}

QString Round::getReasonRoundEnd()
{
    return this->mReasonRoundEnd;
}

ushort Round::getStartTscore()
{
    return this->mStartTscore;
}

ushort Round::getStartCTscore()
{
    return this->mStartCTscore;
}

uint32_t Round::getTmoneySpentTotal()
{
    return this->mTmoneySpentTotal;
}

ushort Round::getTmoneySpentRound()
{
    return this->mTmoneySpentRound;
}

QString Round::getTfreezetimeEndEquipValue()
{
    return this->mTfreezetimeEndEquipValue;
}

uint32_t Round::getCTmoneySpentTotal()
{
    return this->mCTmoneySpentTotal;
}

ushort Round::getCTmoneySpentRound()
{
    return this->mCTmoneySpentRound;
}

QString Round::getCTfreezetimeEndEquipValue()
{
    return this->mCTfreezetimeEndEquipValue;
}

QList<Player> Round::getListPlayer()
{
    return this->mListPlayer;
}
