//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

Game::Game() {setStartBord();}

Game::~Game() {
    if(dead != nullptr){
        delete dead;
    }
    for (int r=0;r<8;r++) {
        for (int k=0;k<8;k++) {
            if(containsPiece(r,k))deletePiece(r, k);
        }
    }
}

// Zet het bord klaar; voeg de stukken op de jusite plaats toe
void Game::setStartBord() {
    /*
     * Zet het bord klaar:
     * Voegt alle schaakstukken toe op de juiste possitie
     */
    for(int i =0; i <= 7; i++){
        for(int a =0; a <= 7; a++){
            setPiece(i , a, nullptr);
        }
    }
    for(int i = 0; i != 14; i+=7) {
        zw kleur = zwart;
        if(i == 7) kleur = wit;
        setPiece(i , 0, new Toren(kleur));
        setPiece(i , 1, new Paard(kleur));
        setPiece(i , 2, new Loper(kleur));
        setPiece(i , 3, new Koningin(kleur));
        setPiece(i , 4, new Koning(kleur));
        setPiece(i , 5, new Loper(kleur));
        setPiece(i , 6, new Paard(kleur));
        setPiece(i , 7, new Toren(kleur));
        for(int a = 0; a <= 7; a++) {
            int pos = 1;
            if(i == 7) pos = 6;
            setPiece(pos , a, new Pion(kleur));
        }
    }
}

// Verplaats stuk s naar positie (r,k)
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk* s,pair<int, int> newLocation, bool AIplayer) {
    vector<pair<int,int>> v=s->geldige_zetten(this);
    if(find(v.begin(), v.end(), newLocation) != v.end()){
        pair<int,int> prevLocation = pair<int,int>(s->getLocation().first , s->getLocation().second);
        SchaakStuk* prevSchaakstuk = getPiece(newLocation.first, newLocation.second);
        setPiece(s->getLocation().first , s->getLocation().second, nullptr);
        setPiece(newLocation.first, newLocation.second, s);
        if(schaak(s->getKleur())){
            //zelf schaak
            //zet de schaakstukken terug op hun juiste plaats
            setPiece(newLocation.first, newLocation.second, prevSchaakstuk);
            setPiece(prevLocation.first, prevLocation.second, s);
            return false;
        }

        if(dead != nullptr) delete dead;    //indien er een schaakstuk is geslagen in vorige zet verwijder deze dan
        dead = prevSchaakstuk;              //indien er een schaakstuk word geslagen slaag deze dan op om in de volgende zet de verwijderen
        s->hasMoved();
        checkBijzondereRegels(prevLocation, newLocation, AIplayer);   //check op "en passant", "rokade", “promotie”
        return true;
    }
    return false;
}

bool Game::schaak(zw kleur) {
    /*
     * Geeft true als kleur schaak staat
     */
    pair<int,int> koningLocation = getkoningLocation(kleur);
    for(int i = 0; i <= 7; i++){
        for(int a = 0; a <= 7; a++){
            if(bord[i][a] != nullptr){
                vector<pair<int,int>> v=bord[i][a]->geldige_zetten(this);
                if(find(v.begin(), v.end(), koningLocation) != v.end()) return true;
            }
        }
    }
    return false;
}

bool Game::thread(pair<int,int> location, zw kleur) {
    /*
     * geeft terug of een schaakstuk op locatie in gevar is voor de meegegeven kleur
     */
    for(int i = 0; i <= 7; i++){
        for(int a = 0; a <= 7; a++){
            if(bord[i][a] != nullptr && bord[i][a]->getKleur() != kleur){
                vector<pair<int,int>> v=bord[i][a]->aanval_zetten(this);
                if(find(v.begin(), v.end(), location) != v.end()) return true;
            }
        }
    }
    return false;
}

bool Game::schaakmat(zw kleur) {
    /*
     * Geeft true als kleur schaakmat staat
     */
    for(int i = 0; i <= 7; i++){
        for(int a = 0; a <= 7; a++){
            if(bord[i][a] != nullptr && bord[i][a]->getKleur() == kleur){
                vector<pair<int,int>> v=bord[i][a]->geldige_zetten(this);
                for(vector<pair<int,int>>::iterator it = v.begin(); it != v.end(); it++){
                    SchaakStuk* prevSchaakstuk = getPiece(it->first, it->second);
                    setPiece(it->first, it->second, bord[i][a]);
                    setPiece(i , a, nullptr);
                    bool mogelijkheid = !schaak(kleur);
                    setPiece(i, a, bord[it->first][it->second]);
                    setPiece(it->first , it->second, prevSchaakstuk);
                    if(mogelijkheid) return false;
                }
            }
        }
    }
    return true;
}

bool Game::pat(zw kleur) {
    /*
     * Geeft true als kleur pat staat
     */
    bool pad = true;
    for(int i = 0; i <= 7; i++){
        for(int a = 0; a <= 7; a++){
            if(bord[i][a] != nullptr && bord[i][a]->getKleur() == kleur){
                vector<pair<int,int>> v=bord[i][a]->geldige_zetten(this);
                for(vector<pair<int,int>>::iterator it = v.begin(); it != v.end(); it++){
                    SchaakStuk* prevSchaakstuk = getPiece(it->first, it->second);
                    setPiece(it->first, it->second, bord[i][a]);
                    setPiece(i , a, nullptr);
                    pad = (pad && schaak(kleur));
                    setPiece(i, a, bord[it->first][it->second]);
                    setPiece(it->first , it->second, prevSchaakstuk);
                }
            }
        }
    }
    return pad;
}

bool Game::containsPiece(int r, int k) const {
    /*
     * checkt of r, k binnen het bord vallen;
     * checkt of er een schaakstuk op r,k staat
     */
    if(r > 7 || k > 7 || k < 0 || r < 0){
        return false;
    }
    return bord[r][k] != nullptr;
}

bool Game::containsAnnemie(int r, int k, const SchaakStuk *s) const{
    /*
     * geeft terug of er een schaakstuk van de andere kleur op deze possitie staat
     */
    if(containsPiece(r, k)){
        return getPiece(r,k)->getKleur() != s->getKleur();
    }
    return false;
}

SchaakStuk *Game::getPiece(int r, int k) const {
    /*
     * geeft het schaakstuk van possitie r, k terug
     */
    if(containsPiece(r, k)){
        return bord[r][k];
    }
    return nullptr;
}

void Game::setPiece(int r, int k, SchaakStuk *s) {
    /*
     * zet schaakstuk s op possitie r,k
     */
    if(s != nullptr){
        s->updateLocation(r, k);
    }
    bord[r][k] = s;
}

bool Game::deletePiece(int r, int k){
    /*
     * verwijderd het schaakstuk op possitie r, k
     */
    if(getPiece(r, k) != nullptr){
        delete getPiece(r, k);
        setPiece(r, k, nullptr);
        return true;
    }
    return false;
}

pair<int, int> Game::getkoningLocation(zw kleur) const{
    /*
     * geeft de locatie van de koning met kleur terug
     */
    for(int i = 0; i < 8; i++){
        for(int a = 0; a < 8; a++){
            if(bord[i][a] != nullptr && bord[i][a]->isKing() && bord[i][a]->getKleur() == kleur) return pair<int,int>(i,a);
        }
    }
    cerr << "Koning niet gevonden!";
    return pair<int,int>(0,0);
}

void Game::checkBijzondereRegels(pair<int, int> prevLocation, pair<int, int> newLocation, bool AIplayer) {
    /*
     * check op de volgende regels:  “en passant”, “rokade” en “promotie”
     */
    rokade(prevLocation, newLocation);
    passand(prevLocation, newLocation);
    promotie(newLocation, AIplayer);
}

void Game::promotie(pair<int, int> newLocation, bool AIplayer) {
    /*
     * indien er een pion aan het einde van het bord geraakt zal de gebruiker een melding krijgen met welk schaakstuk dze vervangen mag worden
     */
    if(getPiece(newLocation.first, newLocation.second)->toShortString()[0] == 'P' && (newLocation.first == 0 || newLocation.first == 7)){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Promotie");
        msgBox.setText(QMessageBox::tr("Kies een schaakstuk voor de pion die de overkant heeft bereikt:"));

        if(AIplayer){
            zw kleur = getPiece(newLocation.first, newLocation.second)->getKleur();
            delete getPiece(newLocation.first, newLocation.second);
            setPiece(newLocation.first, newLocation.second, new Koningin(kleur));
            return;
        }

        QPushButton *toren = msgBox.addButton(QMessageBox::tr("Toren"), QMessageBox::ActionRole);
        QPushButton *paard = msgBox.addButton(QMessageBox::tr("Paard"), QMessageBox::ActionRole);
        QPushButton *loper = msgBox.addButton(QMessageBox::tr("Loper"), QMessageBox::ActionRole);
        QPushButton *koningin = msgBox.addButton(QMessageBox::tr("Koningin"), QMessageBox::ActionRole);

        msgBox.exec();

        if (msgBox.clickedButton() == toren) {
            zw kleur = getPiece(newLocation.first, newLocation.second)->getKleur();
            delete getPiece(newLocation.first, newLocation.second);
            setPiece(newLocation.first, newLocation.second, new Toren(kleur));
        }
        else if(msgBox.clickedButton() == paard){
            zw kleur = getPiece(newLocation.first, newLocation.second)->getKleur();
            delete getPiece(newLocation.first, newLocation.second);
            setPiece(newLocation.first, newLocation.second, new Paard(kleur));
        }
        else if(msgBox.clickedButton() == loper){
            zw kleur = getPiece(newLocation.first, newLocation.second)->getKleur();
            delete getPiece(newLocation.first, newLocation.second);
            setPiece(newLocation.first, newLocation.second, new Loper(kleur));
        }
        else if(msgBox.clickedButton() == koningin){
            zw kleur = getPiece(newLocation.first, newLocation.second)->getKleur();
            delete getPiece(newLocation.first, newLocation.second);
            setPiece(newLocation.first, newLocation.second, new Koningin(kleur));
        }
    }
}

void Game::rokade(pair<int, int> prevLocation, pair<int, int> newLocation){
    /*
     *indien de koning 2 plaatsen verplaatst betekend dit dat de gebruiker een rokade aan het uivoeren is
     * dan zal de toren ook naar de juiste plaats gaan
     */
    if(getPiece(newLocation.first, newLocation.second) != nullptr && getPiece(newLocation.first, newLocation.second)->isKing()){
        if(newLocation.second - 2 == prevLocation.second){
            setPiece(newLocation.first, 5, getPiece(newLocation.first, 7));
            setPiece(newLocation.first , 7, nullptr);
        }
        else if(newLocation.second + 2 == prevLocation.second){
            setPiece(newLocation.first, 3, getPiece(newLocation.first, 7));
            setPiece(newLocation.first , 0, nullptr);
        }
    }
}

void Game::passand(pair<int, int> prevLocation, pair<int, int> newLocation) {

    //verwijder bij elke pion het gevaar om passant gezet te worden (mag enkel direct na de zet uitgevoert worden)
    for(int i = 0; i <= 7; i++){
        for(int a = 0; a <= 7; a++){
            if(bord[i][a] != nullptr && getPiece(i, a)->getKleur() == getPiece(newLocation.first, newLocation.second)->getKleur()){
                getPiece(i, a)->passant = false;
            }
        }
    }

    //Indien het schaakstuk 2 plaatsen vooruit is gegaan is deze in het risico passant gezet te worden
    if((prevLocation.first + 2 == newLocation.first || prevLocation.first - 2 == newLocation.first) && getPiece(newLocation.first, newLocation.second)->toShortString()[0] == 'P') getPiece(newLocation.first, newLocation.second)->passant=true;

    //verwijder schaakstuk indien er een passant word uitgevoert
    if((getPiece(newLocation.first - 1, newLocation.second) != nullptr && getPiece(newLocation.first - 1, newLocation.second)->passant == true && getPiece(newLocation.first, newLocation.second)->getKleur() == zwart) || (getPiece(newLocation.first + 1, newLocation.second) != nullptr && getPiece(newLocation.first + 1, newLocation.second)->passant == true && getPiece(newLocation.first, newLocation.second)->getKleur() == wit)){
        SchaakStuk* s = getPiece(newLocation.first - 1, newLocation.second);
        if(getPiece(newLocation.first, newLocation.second)->getKleur() == wit) s = getPiece(newLocation.first + 1, newLocation.second);

        setPiece(s->getLocation().first, s->getLocation().second, nullptr);
        dead = s;
    }
}

pair<pair<int, int>, pair<int,int>> Game::AIplayer(zw kleur, bool bestMove){
    /*
     * zal een mogelijke zet terug geven voor de AIplayer
     * bestMove (true): Niet voledig willekeurig schaakmat > schaak > slagen > random
     * bestmove (false): voledig willekeurig schaakmat = schaak = slagen = random
     */

    //verzamel alle mogelijke possities in p
    possibleMoves p;
    for(int i = 0; i <= 7; i++){
        for(int a = 0; a <= 7; a++){
            if(getPiece(i,a) != nullptr && getPiece(i,a)->getKleur() == kleur) p.insert(getAttacPossitions(kleur, getPiece(i,a)));
        }
    }

    //indien er geen mogelijke zetten zijn return ((-1,-1),(-1,-1))
    if(p.getMoveCount() == 0){
        return pair<pair<int,int>, pair<int,int>>(pair<int,int>(-1,-1), pair<int,int>(-1,-1));
    }

    //neem een zet uit de mogelijke zetten
    pair<SchaakStuk*, pair<int, int>> randomZet;
    if(bestMove){
        randomZet = p.getRandomBest();
    }
    else{
        randomZet = p.getRandomMove();
    }
    //return deze zet
    return pair<pair<int,int>, pair<int,int>>(randomZet.first->getLocation(), randomZet.second);
}

possibleMoves Game::getAttacPossitions(zw kleur, SchaakStuk* s){
    /*
     * geeft de mogelijke possities van elk schaakstuk
     */
    possibleMoves p;
    if(s->getKleur() == kleur){
        vector<pair<int,int>> geldigeZetten = s->geldige_zetten(this);
        for(vector<pair<int,int>>::iterator it = geldigeZetten.begin(); it != geldigeZetten.end(); it++){
            if((it->first == s->getLocation().first && it->second == s->getLocation().second) || mogelijkSchaak(it->first, it->second, s, s->getKleur())) continue;
            else if(mogelijkSchaakMat(it->first, it->second, s, (zw)(!(bool)(s->getKleur())))) p.insertPossibleSchaakmat(s, pair<int,int>(it->first, it->second));
            else if(mogelijkSchaak(it->first, it->second, s, (zw)(!(bool)(s->getKleur())))) p.insertPossibleSchaak(s, pair<int,int>(it->first, it->second));
            else if(containsAnnemie(it->first, it->second, s)) p.insertPossibleAttacs(s, pair<int,int>(it->first, it->second));
            else p.insertPossibleSchaakStuk(s, pair<int,int>(it->first, it->second));
        }
    }
    return p;
}

bool Game::mogelijkSchaak(int r, int k, SchaakStuk *s, zw kleur){
    /*
     * geeft true indien 'kleur' schaak staat als we s naar r,k verplaatsen
     */
    bool ifschaak = false;
    SchaakStuk* prevSchaakstuk = getPiece(r, k);
    pair<int,int> prevLocation = pair<int,int>(s->getLocation().first , s->getLocation().second);
    setPiece(s->getLocation().first , s->getLocation().second, nullptr);
    setPiece(r, k, s);
    ifschaak = schaak(kleur);
    setPiece(prevLocation.first, prevLocation.second, s);
    setPiece(r, k, prevSchaakstuk);
    return ifschaak;
}

bool Game::mogelijkSchaakMat(int r, int k, SchaakStuk *s, zw kleur){
    /*
     * geeft true indien 'kleur' schaakmat staat als we s naar r,k verplaatsen
     */
    bool ifschaakmat = false;
    SchaakStuk* prevSchaakstuk = getPiece(r, k);
    pair<int,int> prevLocation = pair<int,int>(s->getLocation().first , s->getLocation().second);
    setPiece(s->getLocation().first , s->getLocation().second, nullptr);
    setPiece(r, k, s);
    ifschaakmat = schaakmat(kleur);
    setPiece(prevLocation.first, prevLocation.second, s);
    setPiece(r, k, prevSchaakstuk);
    return ifschaakmat;
}