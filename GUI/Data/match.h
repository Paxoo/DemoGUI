#ifndef MATCH_H
#define MATCH_H

#include <QtCore>
#include "GUI\Data\round.h"

class Match
{
public:
    Match(QList<QString> listPlayerIDs);

    void setMap(QString map);
    void setTeamNameA(QString teamNameA);
    void setTeamNameB(QString teamNameB);

    QString getMap();
    QString getTeamNameA();
    QString getTeamNameB();

    void addRound();
    QList<Round> getRounds();

private:
    QString mMap;
    QString mTeamNameA;
    QString mTeamNameB;

    QList<Round> mListRounds;
    QList<QString> mListPlayerIDs;
};

#endif // MATCH_H
