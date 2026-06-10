#ifndef LEADERBOARD_SCREEN_H
#define LEADERBOARD_SCREEN_H

#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class QShowEvent;

class Leaderboard_Screen : public QWidget
{
    Q_OBJECT

public:
    explicit Leaderboard_Screen(QWidget *parent = nullptr);
    QPushButton *Get_Back_Button() const;
    void Refresh_Leaderboard();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    QLabel *Title_Label;
    QTextEdit *Leaderboard_Text;
    QPushButton *Back_Button;
    QVBoxLayout *Main_Layout;

    void Apply_Responsive_Styles();
};

#endif // LEADERBOARD_SCREEN_H
