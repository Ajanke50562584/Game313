#ifndef LEADERBOARD_DATA_H
#define LEADERBOARD_DATA_H

#include <QList>
#include <QString>

struct Leaderboard_Entry
{
    QString Player_Name;
    int Time_Ms;
    QString Result;
};

class Leaderboard_Data
{
public:
    static void Add_Entry(const QString &playerName, int timeMs, const QString &result);
    static QList<Leaderboard_Entry> Get_Entries();
    static QString Format_Time(int timeMs);
};

#endif // LEADERBOARD_DATA_H
