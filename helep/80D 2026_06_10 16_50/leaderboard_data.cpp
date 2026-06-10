#include "leaderboard_data.h"

#include <algorithm>

static QList<Leaderboard_Entry> Leaderboard_Entries;

void Leaderboard_Data::Add_Entry(const QString &playerName, int timeMs, const QString &result)
{
    Leaderboard_Entry Entry;
    Entry.Player_Name = playerName;
    Entry.Time_Ms = timeMs;
    Entry.Result = result;

    Leaderboard_Entries.append(Entry);

    std::sort(Leaderboard_Entries.begin(), Leaderboard_Entries.end(),
              [](const Leaderboard_Entry &a, const Leaderboard_Entry &b)
    {
        if(a.Result == "Win" && b.Result != "Win")
            return true;

        if(a.Result != "Win" && b.Result == "Win")
            return false;

        return a.Time_Ms < b.Time_Ms;
    });
}

QList<Leaderboard_Entry> Leaderboard_Data::Get_Entries()
{
    return Leaderboard_Entries;
}

QString Leaderboard_Data::Format_Time(int timeMs)
{
    int Total_Seconds = timeMs / 1000;
    int Minutes = Total_Seconds / 60;
    int Seconds = Total_Seconds % 60;
    int Hundredths = (timeMs % 1000) / 10;

    return QString("%1:%2.%3")
            .arg(Minutes, 2, 10, QChar('0'))
            .arg(Seconds, 2, 10, QChar('0'))
            .arg(Hundredths, 2, 10, QChar('0'));
}
