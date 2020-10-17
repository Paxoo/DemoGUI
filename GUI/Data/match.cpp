#include "match.h"


Match::Match()
{

}

Match::~Match()
{
    if(!this->mListRounds.empty()){
        this->mListRounds.clear();
    }
    if(!this->mListPlayerIDs.empty()){
        this->mListPlayerIDs.clear();
    }
}

void Match::setPlayers(QList<QString> listPlayerIDs)
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
    this->mListRounds.append(QSharedPointer<Round>(new Round(this->mListPlayerIDs)));
}

QList<QSharedPointer<Round>> Match::getRounds()
{
    return this->mListRounds;
}

QList<QString> Match::getListPlayerIDs()
{
    return this->mListPlayerIDs;
}
