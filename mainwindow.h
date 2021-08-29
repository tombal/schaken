//  Student: Thibault Soumoy
//  Rolnummer: 0204602

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SchaakStuk.h"
#include "game.h"
#include <QCloseEvent>

class ChessBoard;
class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void update();

private:
    ChessBoard *scene;
    Game g;
    Game redoG;
    Game undoG;
    bool undoB = false;
    bool redoB = false;
    zw kleur = wit;
    int zetCounter = 0;
    SchaakStuk* s = nullptr;
    static SchaakStuk* shortStringToChessPiece(string shortString);
    void displayThread();
    void displayMovesThread();
    void checkSchaakPad(zw kleur);
    bool againsPc = false;
    zw AIKleur = zwart;

private slots:
    void clicked(int x, int y);
    void newGame();
    void open();
    void save();
    void undo();
    void redo();
    void visualizationChange();
    void AIChange();
    void selectSchaakstuk(int r, int k);
    void zetSchaakstuk(int r, int k);
    void autoPlayer();
    void AIPlayerZwart();
    void AIPlayerWit();
    // De volgende lijnen kunnen genegeerd worden voor de opdracht

    void on_actionExit_triggered();
    void closeEvent (QCloseEvent *event)
    {
        on_actionExit_triggered();
        event->ignore();
    }

private:
    void createActions();
    void createMenus();
    QMenu *fileMenu;
    QMenu *gameMenu;
    QMenu *visualizeMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *exitAct;
    QAction *againstAi;
    QAction *AIZwart;
    QAction *AIWit;

    QAction* display_moves;
    QAction* display_kills;
    QAction* display_threats;
};

#endif // MAINWINDOW_H
