#include "mainwindow.h"
#include <QMessageBox>
#include <QtWidgets>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    /*
     * Start het spelbord en een nieuwe Game
     */
    scene = new ChessBoard;
    QGraphicsView *view = new QGraphicsView(scene);
    setCentralWidget(view);

    connect(scene,SIGNAL(clicked(int,int)),this,SLOT(clicked(int,int)));

    createActions();
    createMenus();

    g.setStartBord();
    update();
}


// Deze functie wordt opgeroepen telkens er op het schaakbord
// geklikt wordt. x,y geeft de positie aan waar er geklikt
// werd; r is de 0-based rij, k de 0-based kolom
void MainWindow::clicked(int r, int k) {
    /*
     * ZetCounter houdt elke click op het veld bij.
     * ZetCounter wordt gebruikt om het verschil tussen
     * (a) selector klik (selectSchaakstuk())
     * (b) schaakstuk zetten klik (zetSchaakstuk())
     */
    if(zetCounter % 2 == 0){
        selectSchaakstuk(r, k);
    }
    else{
        zetSchaakstuk(r, k);
    }
}

void MainWindow::newGame()
{
    /*
     * creert een nieuwe Game:
     * (1) Verwijder alle schaakstukken
     * (2) zet nieuwe stukken op startpositie.
     */

    //verwijder all schaakstukken die momenteel aanwezig zijn
    // door iteratief deletePiece() over alle posities aan te roepen.
    scene->removeAllMarking();
    for (int r=0;r<8;r++) {
        for (int k=0;k<8;k++) {
            g.deletePiece(r, k);
        }
    }

    //zet een nieuw schaakbord klaar
    g.setStartBord();
    kleur = wit;
    zetCounter = 0;
    s = nullptr;
    update();
}

void MainWindow::save() {
    /*
     * functie die het huidige schaakbord omzet naar een bestand
     */
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save game"), "",
                                                    tr("Chess File (*.chs);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QDataStream out(&file);

        //slaagt van elk schaakstuk de ShortString op in volgorde van het schaakbord
        //indien er geen schaakstuk staat word er een '.' geplaatst
        for(int i = 0; i <= 7; i++){
            for(int a = 0; a <= 7; a++){
                if(g.getPiece(i,a) != nullptr){
                    out << QString::fromStdString(g.getPiece(i,a)->toShortString());
                }
                else{
                    out << QString::fromStdString(".");
                }
            }
        }
        //slaag op het einde van het bestand op welke kleur er aan de beurd is
        out << QString::fromStdString(to_string((int)kleur));
        file.close();
    }
}

void MainWindow::open() {
    /*
     * functie die een schaakbestand inlaad
     */
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load game"), "",
                                                    tr("Chess File (*.chs);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),file.errorString());
            return;
        }
        try {
            scene->removeAllMarking();
            QDataStream in(&file);
            QString debugstring;
            //verwijderd elk bestaand schaakstuk en zet de ShortString van het bestand om naar een schaakstuk
            for (int r=0;r<8;r++) {
                for (int k=0;k<8;k++) {
                    QString piece;
                    in >> piece;
                    g.deletePiece(r, k);
                    g.setPiece(r , k, shortStringToChessPiece(piece.toStdString()));

                    if (in.status()!=QDataStream::Ok) {
                        throw QString("Error reading file "+fileName);
                    }
                }
            }
            //het laatste teken is de kleur van de speler die aan de beurd is
            QString color;
            in >> color;

            if(color.toInt() == 1 || color.toInt() == 0){
                kleur = (zw)(color.toInt());
            }
            else{
                throw QString("Error reading file " + fileName);
            }

            update();
            QMessageBox::information(this, tr("Debug"),"spel ingeladen");
        } catch (QString& Q) {
            QMessageBox::information(this, tr("Error reading file"),
                                     Q);
        }
    }
}

SchaakStuk* MainWindow::shortStringToChessPiece(string shortString) {
    /*
     * Functie voor het omzetten van een schortString([letter][(int)kleur]) naar een schaakstuk
     */
    char piece = shortString[0];
    zw color = zwart;
    if(shortString.length() > 1) {
        color = (zw)((int)shortString[1] - 48); // -48 omdat 0 in ASCII 48 is
    }
    switch (piece){
        case 'R':
            return new Toren(color);
        case 'H':
            return new Paard(color);
        case 'B':
            return new Loper(color);
        case 'Q':
            return new Koningin(color);
        case 'K':
            return new Koning(color);
        case 'P':
            return new Pion(color);
        case '.':
            return nullptr;
        default:
            throw QString("Symbool niet herkent: "+ piece);
    }
}

void MainWindow::undo() {
    /*
     * zal de gebruiker zijn zet terug zetten
     */
    if(undoB){
        redoG = g;                      //slaagt de huidige game op in redoG
        scene->removeAllMarking();
        g = undoG;                      //laad in het huidige game de vorige game

        kleur = zw(!bool(kleur));       //zet de kleur naar de andere speler

        zetCounter--;                   //zet de zetCounter 1 terug

        //update allle schaakstukken zodat ze de juiste possitie hebben
        //en zet de zetCounter van de schaakstukken juist
        for (int r=0;r<8;r++) {
            for (int k=0;k<8;k++) {
                if(g.getPiece(r, k) != nullptr) g.getPiece(r, k)->updateLocation(r,k);
                if(redoG.getPiece(r,k) != g.getPiece(r,k) && g.getPiece(r,k) != nullptr && g.getPiece(r,k)->moveCount > 0) g.getPiece(r,k)->moveCount -= 1; //zet het aantal zetten van het schaakstuk teurg

            }
        }
        update();
        undoB = false;
        redoB = true;
    }
    else{
        QMessageBox::information(this, tr("Info"),"U kan niet verder terug gaan");
    }
}

void MainWindow::redo() {
    /*
     * zal de undo terug zetten
     */
    if(redoB ) {
        undoG = g;                      //slaagt de huidige game op in undoG
        scene->removeAllMarking();
        g = redoG;                      //laad in het huidige game de vorige game

        kleur = zw(!bool(kleur));       //zet de kleur naar de andere speler

        zetCounter++;
        for (int r = 0; r < 8; r++) {
            for (int k = 0; k < 8; k++) {
                if (g.getPiece(r, k) != nullptr) g.getPiece(r, k)->updateLocation(r, k);
                if(undoG.getPiece(r,k) != g.getPiece(r,k) && g.getPiece(r,k) != nullptr && g.getPiece(r,k)->moveCount > 0) g.getPiece(r,k)->moveCount += 1; //zet het aantal zetten van het schaakstuk teurg
            }
        }
        update();
        redoB = false;
        undoB = true;
    }
    else{
        QMessageBox::information(this, tr("Info"),"U bent terug bij u laatste zet");
    }
}


void MainWindow::visualizationChange() {
    QMessageBox box;
    QString visstring = QString(display_moves->isChecked()?"T":"F")+(display_kills->isChecked()?"T":"F")+(display_threats->isChecked()?"T":"F");
    box.setText(QString("Visualization changed : ")+visstring);
    box.exec();
}

void MainWindow::update() {
    /*
     * Update de inhoud van de grafische weergave van het schaakbord (scene)
     */
    scene->clearBoard();
    for(int i =0; i <= 7; i++){
        for(int a =0; a <= 7; a++){
           if(g.containsPiece(i, a)){
               scene->setItem(i, a, g.getPiece(i, a)->piece());
           }
        }
    }
}





////////////////






void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Start a new game"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newGame);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Read game from disk"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save game to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Abandon game"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::on_actionExit_triggered);

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo last move"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    redoAct = new QAction(tr("&redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo last undone move"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    againstAi = new QAction(tr("&against computer"), this);
    againstAi->setStatusTip(tr("Play against a computer"));
    againstAi->setCheckable(true);
    againstAi->setChecked(false);
    connect(againstAi, &QAction::triggered, this, &MainWindow::AIChange);

    AIZwart = new QAction(tr("&AI: black"), this);
    AIZwart->setStatusTip(tr("AI player is black"));
    AIZwart->setCheckable(true);
    AIZwart->setChecked(true);
    connect(AIZwart, &QAction::triggered, this, &MainWindow::AIPlayerZwart);

    AIWit = new QAction(tr("&AI: white"), this);
    AIWit->setStatusTip(tr("AI player is white"));
    AIWit->setCheckable(true);
    AIWit->setChecked(false);
    connect(AIWit, &QAction::triggered, this, &MainWindow::AIPlayerWit);

    display_moves= new QAction(tr("&valid moves"), this);
    display_moves->setStatusTip(tr("Show valid moves"));
    display_moves->setCheckable(true);
    display_moves->setChecked(true);
    connect(display_moves, &QAction::triggered, this, &MainWindow::visualizationChange);

    display_kills= new QAction(tr("threathed &enemy"), this);
    display_kills->setStatusTip(tr("Highlight threathened pieces (enemy)"));
    display_kills->setCheckable(true);
    display_kills->setChecked(true);
    connect(display_kills, &QAction::triggered, this, &MainWindow::visualizationChange);

    display_threats= new QAction(tr("threathed &player"), this);
    display_threats->setStatusTip(tr("Highlight threathened pieces (player)"));
    display_threats->setCheckable(true);
    display_threats->setChecked(true);
    connect(display_threats, &QAction::triggered, this, &MainWindow::visualizationChange);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);

    gameMenu = menuBar()->addMenu(tr("&Game"));
    gameMenu->addAction(undoAct);
    gameMenu->addAction(redoAct);
    gameMenu->addAction(againstAi);
    gameMenu->addAction(AIZwart);
    gameMenu->addAction(AIWit);

    visualizeMenu = menuBar()->addMenu(tr("&Visualize"));
    visualizeMenu->addAction(display_moves);
    visualizeMenu->addAction(display_kills);
    visualizeMenu->addAction(display_threats);
}

void MainWindow::on_actionExit_triggered() {
    if (QMessageBox::Yes == QMessageBox::question(this,
                                                  tr("Spel verlaten"),
                                                  tr("Bent u zeker dat u het spel wil verlaten?\nNiet opgeslagen wijzigingen gaan verloren.")))
    {
        QApplication::quit();
    }
}

void MainWindow::selectSchaakstuk(int r, int k) {
    /*
     * Al de gebruiker op een veld klikt (r,k) en er staat een schaakstuk op dan zullen we dit schaakstuk opslaan
     * zodat we deze bij de volgende klik kunnen verplaatsen
     */
    QMessageBox box1;
    s = g.getPiece(r, k);
    if(s == nullptr){
        box1.setText(QString("Selecteer een schaakstuk"));
        box1.exec();
    }
    else if(s->getKleur() == kleur){
        scene->removeAllMarking();
        scene->setTileSelect(r,k,true);
        if(display_moves->isChecked() || display_threats->isChecked()) displayMovesThread();
        zetCounter++;
    }
    else{
        box1.setText(QString("Selecteer je eigen kleur!"));
        box1.exec();
    }
}

void MainWindow::zetSchaakstuk(int r, int k) {
    /*
     * Als de gebruiker op een veld klikt (r,k), en het geselecteerde schaakstuk kan naar daar verplaatsen
     * Dan zal het schaakstuk naar (r,k) verplaatst worden
     */
    undoG = g;
    QMessageBox box1;

    if(g.move(s,pair<int,int>(r,k))){
        //verzet het schaakstuk
        scene->removeAllMarking();
        update();
        kleur = zw(!bool(kleur));
        checkSchaakPad(kleur);
        zetCounter++;
        undoB = true;
        redoB = false;
    }
    else{
        //verzet het schaakstuk niet
        zetCounter--;

        if(g.containsPiece(r, k) && g.getPiece(r, k)->getKleur() == kleur) selectSchaakstuk(r, k);
        else{
            box1.setText(QString("Ongeldige zet! \nprobeer opnieuw"));
            box1.exec();
            scene->removeAllMarking();
        }

    }
    if(display_kills->isChecked()) displayThread();

    //indien Ai player aan staat en het is de beurt van de AI player
    if(kleur == AIKleur && againsPc){
        autoPlayer();
    }
}

void MainWindow::autoPlayer() {
    /*
     * Functie die een willikeurige zet uitvoert voor de AI speler
     * (Niet voledig willekeurig schaakmat > schaak > slagen > random)
     */
    scene->removeAllMarking();
    QMessageBox box1;
    pair<pair<int,int>, pair<int,int>> oldNewLocation = g.AIplayer(kleur);// geeft de zet terug naar waar de AI player zich zal verplaaten

    //indien oldNewLocation = ((-1,-1)(-1,-1) betekend dit dat er geen mogelijke zetten meer zijn voor de AI player
    if(oldNewLocation != pair<pair<int,int>, pair<int,int>>(pair<int,int>(-1,-1),pair<int,int>(-1,-1))){

        //zet de AI player naar de gegeven plaats, indien dit niet mag (zelf schaak) zoekt hij een nieuwe plaats
        while(!g.move(g.getPiece(oldNewLocation.first.first,oldNewLocation.first.second), oldNewLocation.second, true) && !g.schaakmat(kleur)){
           oldNewLocation = g.AIplayer(kleur, false);
        }
        update();
        zetCounter+=2;
        kleur = zw(!bool(kleur));
        checkSchaakPad(kleur);
        if(display_kills->isChecked()) displayThread();
    }
    else if(g.schaak(kleur)){
        box1.setText(QString("U hebt gewonnen!"));
        box1.exec();
    }
    else{
        box1.setText(QString("Gelijke stand (pad)"));
        box1.exec();
    }
}

void MainWindow::displayThread() {
    /*
     * maakt alle schaakstuken die geslagen kunnen worden rood
     */
    for(int i = 0; i <= 7; i++){
        for(int a = 0; a <= 7; a++){
            if(g.getPiece(i,a) != nullptr && g.thread(pair<int, int>(i,a), g.getPiece(i,a)->getKleur())) scene->setPieceThreat(i,a,true);
        }
    }
}

void MainWindow::displayMovesThread() {
    /*
     * r, k (int) de possitie van het te verplaatsen schaakstuk
     * zal voor het meegegeven schaakstuk de plaatsen tonen naar waar het verplaatst mag worden en of er daar gevaar is
     */
    vector<pair<int,int>> v=s->geldige_zetten(&g);
    for(vector<pair<int, int>>::iterator it = v.begin(); it != v.end(); it++){
        int r = s->getLocation().first;
        int k = s->getLocation().second;
        SchaakStuk* prevSchaakstuk = g.getPiece(it->first, it->second);
        g.setPiece(it->first, it->second, s);

        if(display_moves->isChecked()) scene->setTileFocus(it->first,it->second,true);
        if(g.thread(pair<int, int>(it->first,it->second), s->getKleur()) && display_threats->isChecked()) scene->setTileThreat(it->first,it->second,true);

        g.setPiece(r, k, s);
        g.setPiece(it->first , it->second, prevSchaakstuk);
    }
}

void MainWindow::checkSchaakPad(zw kleur) {
    /*
     * Check of een meegegeven kleur schaak, schaakmat, of pad staat
     * indien dit het geval is zal er een melding gegeven worden
     */
    QMessageBox box1;
    if(g.schaak(kleur)){
        box1.setText(QString("Schaak!"));
        if(g.schaakmat(kleur)) box1.setText(QString("Schaakmat!"));
        box1.exec();
    }
    else if(g.pat(kleur)){
        box1.setText(QString("Pad!"));
        box1.exec();
    }
}

void MainWindow::AIChange() {
    /*
     * Functie voor het inschakelen van de AI
     * Extra: Zal Undo en Redo disable
     */
    QMessageBox box1;
    againsPc = !againsPc;
    againstAi->setChecked(againsPc);
    if(againsPc){
        redoAct->setDisabled(true);
        redoAct->setStatusTip(tr("Undo last move (to enable, turn off AI)"));

        undoAct->setDisabled(true);
        undoAct->setStatusTip(tr("Redo last undone (to enable, turn off AI)"));
    }
    else{
        redoAct->setDisabled(false);
        redoAct->setStatusTip(tr("Undo last move"));

        undoAct->setDisabled(false);
        undoAct->setStatusTip(tr("Redo last undone"));
    }

    if(kleur == AIKleur)autoPlayer();
}

void MainWindow::AIPlayerZwart() {
    /*
     * functie voor het veranderen van de AI kleur naar zwart
     */
    AIKleur = zwart;
    AIWit->setChecked(false);
    AIZwart->setChecked(true);
    if(kleur == AIKleur && againsPc)autoPlayer();
}

void MainWindow::AIPlayerWit() {
    /*
     * functie voor het veranderen van de AI kleur naar wit
     */
    AIKleur = wit;
    AIZwart->setChecked(false);
    AIWit->setChecked(true);
    if(kleur == AIKleur && againsPc)autoPlayer();
}
