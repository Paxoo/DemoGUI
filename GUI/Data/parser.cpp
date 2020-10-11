#include "parser.h"


Parser::Parser(Match &match, float mapRatio)
{
    this->mProcess = new QProcess();

    QObject::connect(this->mProcess,&QProcess::readyRead,this->mProcess,[&](){
        QString result = this->mProcess->readAllStandardOutput();
        QStringList firstList = result.split("\n");
        QStringList items;

        foreach (const QString &str, firstList) {

            if (str.contains("MATCHINFO")){
                items = str.split(", ");
                match.setMap(items[1]);

                QList<QString> playerIDs;
                for(short i = 2; i < (items.size()-1); i++){
                    playerIDs.append(items[i]);
                }
                qDebug() << playerIDs << endl;
                match.setPlayers(playerIDs);

            }

            if (str.contains("ROUND START") or str.contains("MATCH START")){
                match.addRound();
                items = str.split(", ");
                if(match.getRounds().size() == 1){
                    match.getRounds().last()->setRoundStartStats(items[1].toUInt(), 0, 0);
                }else{
                    match.getRounds().last()->setRoundStartStats(items[1].toUInt(), items[3].toUShort(), items[4].toUShort());
                }
            }

            if (str.contains("ROUNDEND") and !str.contains("DRAW") ){
                items = str.split(", ");

                QMap<uint8_t,QString> reason;
                reason[1]="TargetBombed";
                reason[7]="BombDefused";
                reason[8]="CTWin";
                reason[9]="TerroristsWin";
                reason[10]="Draw";
                reason[12]="TargetSaved";

                match.getRounds().last()->setRoundEndStats(reason[items[8].toUShort()]);
                match.setTeamNameA(items[6]);
                match.setTeamNameB(items[7]);
            }

            if (str.contains("FREEZE END")){
                items = str.split(", ");
                match.getRounds().last()->setFreezeEndTick(items[1].toUInt());
            }

            if (str.contains("ROUND END OFFICIAL")){
                items = str.split(", ");
                match.getRounds().last()->setEndTick(items[1].toUInt());
            }

            if (str.contains("ROUND PURCHASE")){
                items = str.split(", ");

                QString tValue = calculateEquipValue(items[6].toUShort(), items[2]);
                QString ctValue = calculateEquipValue(items[10].toUShort(), items[2]);

                match.getRounds().last()->setRoundPurchase(items[4].toUInt(), items[5].toUShort(), tValue, items[8].toUInt(), items[9].toUShort(), ctValue);
            }

            /*if (str.contains("PLAYER INFO")){
                QStringList items = str.split(", ");

                // items[2] = roundnumber -> round 2
                if(items[2] == "5"){
                    // items[6] = playername
                    if(items[6] == "Paxo"){
                        bool helmet = false;
                        bool kit = false;
                        if (items[12] == "true") helmet = true;
                        if (items[13] == "true") kit = true;

                        qDebug() << "ja" << endl;
                        match.getRounds().first().getListPlayer().first().addPlayerInfo(items[1].toInt(), QPointF(items[4].toFloat()/mapRatio, items[5].toFloat()/mapRatio),
                                                items[8].toFloat(), items[9].toUShort(), items[10].toUShort(), items[11].toUShort(), helmet, kit, items[14]);

                    }
                }
            }*/
        }
    });
}

void Parser::runParser(QString fileName)
{
    // parse demofile
    QString cmd = "go run parse_demofile.go -demo " + fileName.toUtf8();
    this->mProcess->start(cmd);
    this->mProcess->waitForFinished(-1); // sets current thread to sleep and waits for pingProcess end

}

QString Parser::calculateEquipValue(ushort value, QString round)
{
    if(round == "1" or round == "2"){
        return "Pistol";
    }

    // Full Eco
    if(value < 2000){
        return "Full Eco";
    }
    // Eco
    if(value >= 2000 and value < 8500){
        return "Eco";
    }

    // Half Buy
    if(value >= 8500 and value < 22000){
        return "Half Buy";
    }

    // Full Buy
    if(value >= 22000){
        return "Half Buy";
    }

    return "NA";
}
