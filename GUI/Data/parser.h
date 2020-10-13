#ifndef PARSER_H
#define PARSER_H

#include <GUI\Data\match.h>

class Parser
{
public:
    Parser(Match &match, float mapRatio);
    void runParser(QString fileName);

private:
    QProcess* mProcess;
    QString calculateEquipValue(ushort value, QString round);
    QString getWeaponType(QString type);
};

#endif // PARSER_H
