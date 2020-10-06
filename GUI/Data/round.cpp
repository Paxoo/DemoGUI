#include "round.h"


Round::Round(QList<QString> &ids)
{
    for(short i=0; i < ids.size(); i++){
        Player player(ids.at(i));
        this->mListPlayer.append(player);
    }
}

void Round::setRoundTicks(ushort startTick, ushort freezeEndTick, ushort endTick, QString reasonRoundEnd)
{
    this->mStartTick = startTick;
    this->mFreezeEndTick = freezeEndTick;
    this->mEndTick = endTick;
    this->mReasonRoundEnd = reasonRoundEnd;
}

void Round::setTstats(ushort startTscore, ushort tmoneySpentTotal, ushort tmoneySpentRound, QString tfreezetimeEndEquipValue)
{
    this->mStartTscore = startTscore;
    this->mTmoneySpentTotal = tmoneySpentTotal;
    this->mTmoneySpentRound = tmoneySpentRound;
    this->mTfreezetimeEndEquipValue = tfreezetimeEndEquipValue;
}

void Round::setCTstats(ushort startCTscore, ushort ctmoneySpentTotal, ushort ctmoneySpentRound, QString ctfreezetimeEndEquipValue)
{
    this->mStartCTscore = startCTscore;
    this->mCTmoneySpentTotal = ctmoneySpentTotal;
    this->mCTmoneySpentRound = ctmoneySpentRound;
    this->mCTfreezetimeEndEquipValue = ctfreezetimeEndEquipValue;
}

ushort Round::getStartTick()
{
    return this->mStartTick;
}

ushort Round::getFreezeEndTick()
{
    return this->mFreezeEndTick;
}

ushort Round::getEndTick()
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

ushort Round::getTmoneySpentTotal()
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

ushort Round::getCTmoneySpentTotal()
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
