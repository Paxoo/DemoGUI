#ifndef MATCH_H
#define MATCH_H

#include <QtCore>
#include "GUI\Data\round.h"

class Match
{
public:
    Match();
    ~Match();

    void setPlayers(QList<QString> listPlayerIDs);
    void setMap(QString map);
    void setTeamNameA(QString teamNameA);
    void setTeamNameB(QString teamNameB);

    QString getMap();
    QString getTeamNameA();
    QString getTeamNameB();

    void addRound();
    QList<QSharedPointer<Round>> getRounds();

private:
    QString mMap = "";
    QString mTeamNameA = "";
    QString mTeamNameB = "";

    QList<QSharedPointer<Round>> mListRounds;
    QList<QString> mListPlayerIDs;
};

#endif // MATCH_H
