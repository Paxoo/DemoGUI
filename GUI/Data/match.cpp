#include "match.h"


Match::Match(QList<QString> listPlayerIDs)
{
    this->mListPlayerIDs = listPlayerIDs;
}

void Match::setMap(QString map)
{
    this->mMap = map;
}

void Match::setTeamNameA(QString teamNameA)
{
    this->mTeamNameA = teamNameA;
}

void Match::setTeamNameB(QString teamNameB)
{
    this->mTeamNameB = teamNameB;
}

QString Match::getMap()
{
    return this->mMap;
}

QString Match::getTeamNameA()
{
    return this->mTeamNameA;
}

QString Match::getTeamNameB()
{
    return this->mTeamNameB;
}

void Match::addRound()
{
    Round round(this->mListPlayerIDs);
    this->mListRounds.append(round);
}

QList<Round> Match::getRounds()
{
    return this->mListRounds;
}
