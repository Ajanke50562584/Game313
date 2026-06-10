#ifndef LEADERBOARD_SCREEN_H
#define LEADERBOARD_SCREEN_H

#include <QLabel> // title label
#include <QPushButton> // back button
#include <QResizeEvent> // resize the screen nicely
#include <QTextEdit> // score list display
#include <QVBoxLayout> // vertical layout
#include <QWidget> // screen base

class QShowEvent;

class Leaderboard_Screen : public QWidget // screen that shows all saved times
{
    Q_OBJECT // Signals and slots

public:
    explicit Leaderboard_Screen(QWidget *parent = nullptr); // construct the leaderboard screen
    QPushButton *Get_Back_Button() const; // used by menu manager to go back
    void Refresh_Leaderboard(); // reload scores into the text box

protected:
    void resizeEvent(QResizeEvent *event) override; // keeps text readable when window changes
    void showEvent(QShowEvent *event) override; // refresh when this screen opens

private:
    QLabel *Title_Label; // big heading
    QTextEdit *Leaderboard_Text; // actual highscore text
    QPushButton *Back_Button; // return to main menu
    QVBoxLayout *Main_Layout; // stacks all widgets

    void Apply_Responsive_Styles(); // changes fonts and style for current size
};

#endif // LEADERBOARD_SCREEN_H
