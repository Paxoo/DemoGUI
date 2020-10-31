#include "parser.h"


Parser::Parser(Match &match, float mapRatio)
{
    this->mProcess = new QProcess();
    static bool gTeamname = true;

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
                    // standard T CT
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

                match.getRounds().last()->setRoundEndStats(items[5], reason[items[8].toUShort()]);

                if(gTeamname == true){
                    match.setTeamNameA(items[6]);
                    match.setTeamNameB(items[7]);
                    gTeamname = false;
                }

                match.getRounds().last()->setEndTick(items[1].toUInt());

                // there is a demo bug, where the first round is actually the warmup. teamname has [READY] -> remove round from list
                if(match.getTeamNameA().contains("[READY]") == true){
                    match.removeRound(0);
                    gTeamname = true;
                }
            }

            if (str.contains("FREEZE END")){
                items = str.split(", ");
                match.getRounds().last()->setFreezeEndTick(items[1].toUInt());
            }

            if (str.contains("ROUND PURCHASE")){
                items = str.split(", ");

                QString tValue = calculateEquipValue(items[6].toUShort(), items[2]);
                QString ctValue = calculateEquipValue(items[10].toUShort(), items[2]);

                match.getRounds().last()->setRoundPurchase(items[4].toUInt(), items[5].toUShort(), tValue, items[8].toUInt(), items[9].toUShort(), ctValue);
            }

            if (str.contains("PLAYER INFO")){
                QStringList items = str.split(", ");

                for(int i=0; i<match.getRounds().last()->getListPlayer().size(); i++){
                    if(match.getRounds().last()->getListPlayer().at(i)->getID() == items[3]){
                        match.getRounds().last()->getListPlayer().at(i)->setName(items[6]);
                        match.getRounds().last()->getListPlayer().at(i)->setPlayerSide(items[7]);

                        bool helmet = false;
                        bool kit = false;
                        if (items[12] == "true") helmet = true;
                        if (items[13] == "true") kit = true;
                        match.getRounds().last()->getListPlayer().at(i)->addPlayerInfo(items[1].toInt(),
                                QPointF(items[4].toFloat()/mapRatio, items[5].toFloat()/mapRatio),
                                items[8].toFloat(), items[9].toUShort(), items[10].toUShort(), items[11].toUShort(), helmet, kit, items[14]);
                    }
                }
            }

            if (str.contains("TEAMFLASHED")){
                items = str.split(", ");

                for(int i=0; i<match.getRounds().last()->getListPlayer().size(); i++){
                    if(match.getRounds().last()->getListPlayer().at(i)->getID() == items[3]){
                        match.getRounds().last()->getListPlayer().at(i)->incrementTeamFlahes();
                    }
                }
            }

            if (str.contains("ENEMIESFLASHED")){
                items = str.split(", ");

                for(int i=0; i<match.getRounds().last()->getListPlayer().size(); i++){
                    if(match.getRounds().last()->getListPlayer().at(i)->getID() == items[3]){
                        match.getRounds().last()->getListPlayer().at(i)->incrementEnemyFlashes();
                    }
                }
            }

            if (str.contains("KILL")){
                items = str.split(", ");

                for(int i=0; i<match.getRounds().last()->getListPlayer().size(); i++){
                    // Attacker
                    if(match.getRounds().last()->getListPlayer().at(i)->getID() == items[9] and items[3] != items[9]){
                        match.getRounds().last()->getListPlayer().at(i)->incrementKills();

                        // playerstats is for animation purpose
                        match.getRounds().last()->getListPlayer().at(i)->addPlayerStats(items[1].toUInt(),
                                match.getRounds().last()->getListPlayer().at(i)->getKills(),
                                match.getRounds().last()->getListPlayer().at(i)->getAssists(),
                                match.getRounds().last()->getListPlayer().at(i)->getDeaths());
                    }
                    // Victim
                    if(match.getRounds().last()->getListPlayer().at(i)->getID() == items[3]){
                        match.getRounds().last()->getListPlayer().at(i)->incrementDeaths();

                        // playerstats is for animation purpose
                        match.getRounds().last()->getListPlayer().at(i)->addPlayerStats(items[1].toUInt(),
                                match.getRounds().last()->getListPlayer().at(i)->getKills(),
                                match.getRounds().last()->getListPlayer().at(i)->getAssists(),
                                match.getRounds().last()->getListPlayer().at(i)->getDeaths());
                    }
                    // Assist
                    if(match.getRounds().last()->getListPlayer().at(i)->getID() == items[15]){
                        match.getRounds().last()->getListPlayer().at(i)->incrementAssists();

                        // playerstats is for animation purpose
                        match.getRounds().last()->getListPlayer().at(i)->addPlayerStats(items[1].toUInt(),
                                match.getRounds().last()->getListPlayer().at(i)->getKills(),
                                match.getRounds().last()->getListPlayer().at(i)->getAssists(),
                                match.getRounds().last()->getListPlayer().at(i)->getDeaths());
                    }
                }
            }

            if (str.contains("DAMAGE")){
                items = str.split(", ");

                for(int i=0; i<match.getRounds().last()->getListPlayer().size(); i++){
                    if(match.getRounds().last()->getListPlayer().at(i)->getID() == items[9]){ //attacker
                        match.getRounds().last()->getListPlayer().at(i)->incrementDMGdone(getWeaponType(items[17]), items[15].toInt());
                    }
                }
            }
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
    if(round == "1" or round == "16"){
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
        return "Full Buy";
    }

    return "NA";
}

QString Parser::getWeaponType(QString type)
{
    QMap<QString, QString> weaponMap{
        {"0", "Unknown"},
        {"1", "P2000"},
        {"2", "Glock"},
        {"3", "P250"},
        {"4", "Deagle"},
        {"5", "FiveSeven"},
        {"6", "DualBerettas"},
        {"7", "Tec9"},
        {"8", "CZ"},
        {"9", "USP"},
        {"10", "Revolver"},
        {"101", "MP7"},
        {"102", "MP9"},
        {"103", "Bizon"},
        {"104", "Mac10"},
        {"105", "UMP"},
        {"106", "P90"},
        {"107", "MP5"},
        {"201", "SawedOff"},
        {"202", "Nova"},
        {"203", "Swag7"},
        {"204", "XM1014"},
        {"205", "M249"},
        {"206", "Negev"},
        {"301", "Galil"},
        {"302", "Famas"},
        {"303", "AK47"},
        {"304", "M4A4"},
        {"305", "M4A1"},
        {"306", "Scout"},
        {"307", "SG556"},
        {"308", "AUG"},
        {"309", "AWP"},
        {"310", "Scar20"},
        {"311", "G3SG1"},
        {"401", "Zeus"},
        {"402", "Kevlar"},
        {"403", "Helmet"},
        {"404", "Bomb"},
        {"405", "Knife"},
        {"406", "DefuseKit"},
        {"407", "World"},
        {"501", "Decoy"},
        {"502", "Molotov"},
        {"503", "Incendiary"},
        {"504", "Flash"},
        {"505", "Smoke"},
        {"506", "HE"},
    };
    return weaponMap[type];
}
