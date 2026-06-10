#ifndef LEADERBOARD_DATA_H
#define LEADERBOARD_DATA_H

#include <QList> // stores all score entries
#include <QString> // names and win/lose text

struct Leaderboard_Entry // one row shown on the leaderboard
{
    QString Player_Name; // who got the time
    int Time_Ms; // raw time in milliseconds so sorting is easy
    QString Result; // Win or Lose
};

class Leaderboard_Data // little static helper for scores
{
public:
    static void Add_Entry(const QString &playerName, int timeMs, const QString &result); // add new score and sort it
    static QList<Leaderboard_Entry> Get_Entries(); // give screens the current scores
    static QString Format_Time(int timeMs); // turns ms into nice timer text
};

#endif // LEADERBOARD_DATA_H
